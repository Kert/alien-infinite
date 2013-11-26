#include "graphics.h"


extern bool holdkeys[5];
extern Animation anim_player;
extern Player player;
extern bool tiles[1000][1000];
extern SDL_Rect camera;

SDL_Renderer *renderer = NULL;
SDL_Texture *player_texture = NULL;
SDL_Texture *level_texture = NULL;
SDL_Surface *player_surface = NULL;
SDL_Surface *surface_map = NULL;
SDL_Surface *surface_textures = NULL;

SDL_Window *win = NULL;

SDL_Texture *debug_texture = NULL;
TTF_Font *debug_font = NULL;
SDL_Color debug_color = { 255, 255, 255 };
SDL_Surface *debug_message = NULL;

int GraphicsSetup()
{
	// Initialize SDL_TTF for font rendering
	if( TTF_Init() == -1 )
    {
        return 0;    
    }

	debug_font = TTF_OpenFont( "verdana.ttf", 12 );
	
	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	
	// used for texture transparency, but I don't really know how to use it
	// it doesn't work
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	surface_map = SDL_CreateRGBSurface(0, MAP_WIDTH, MAP_HEIGHT, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);

	surface_textures = SDL_LoadBMP("images/blocks.bmp");

	// can be used to return dimensions of texture
	//SDL_QueryTexture(img, NULL, NULL, &w, &h);

	player_surface = IMG_Load("images/player.png");
	SDL_SetColorKey(player_surface, 1, SDL_MapRGB(player_surface->format, 239, 239, 239));
	player_texture = SDL_CreateTextureFromSurface(renderer, player_surface);

	anim_player.SetFrameRate(125);
	anim_player.MaxFrames = 4;
	return 1;
}

void GraphicsUpdate()
{
	// clear the screen
	SDL_RenderClear(renderer);

	// show level map
	SDL_RenderCopy(renderer, level_texture, &camera, NULL);
		
	// show player sprite
	Render(player);

	ShowDebugInfo();
	
	// flip the backbuffer
	// this means that everything that we prepared behind the screens is actually shown
	SDL_RenderPresent(renderer);
}

void LoadLevel()
{
	level_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, MAP_WIDTH, MAP_HEIGHT);

	SDL_Rect rect;
	SDL_Rect rect2;

	//tile size on level map surface
	rect2.w=32; rect2.h=32;

	// Load background tiles

	// tile coords in texture
	rect.x=99; rect.y=64; rect.w=TILESIZE; rect.h=TILESIZE;
	
	for(int i = 0; i <= (MAP_HEIGHT / TILESIZE); i++)
	{
		for(int j = 0; j < (MAP_WIDTH / TILESIZE); j++)
		{
			rect2.x = i * TILESIZE;
			rect2.y = j * TILESIZE;
			SDL_BlitSurface(surface_textures,&rect,surface_map,&rect2);
		}
	}

	// Load solid tiles

	// tile coords in texture
	rect.x=33; rect.y=0; rect.w=TILESIZE; rect.h=TILESIZE;

	for(int i = 0; i <= (MAP_WIDTH / TILESIZE); i++)
	{
		for(int j = 13; j < 14; j++)
		{
			rect2.x = i * TILESIZE;
			rect2.y = j * TILESIZE;
			tiles[i][j] = true;
			SDL_BlitSurface(surface_textures,&rect,surface_map,&rect2);
		}
	}

	for(int i = 12; i <= 12; i++)
	{
		for(int j = 4; j < 12; j++)
		{
			rect2.x = i * TILESIZE;
			rect2.y = j * TILESIZE;
			tiles[i][j] = true;
			SDL_BlitSurface(surface_textures,&rect,surface_map,&rect2);
		}
	}

	// some other solid blocks
	rect2.x = 10 * TILESIZE;
	rect2.y = 10 * TILESIZE;
	SDL_BlitSurface(surface_textures,&rect,surface_map,&rect2);
	tiles[10][10] = true;

	rect2.x = 11 * TILESIZE;
	rect2.y = 9 * TILESIZE;
	SDL_BlitSurface(surface_textures,&rect,surface_map,&rect2);
	tiles[11][9] = true;

	rect2.x = 8 * TILESIZE;
	rect2.y = 8 * TILESIZE;
	SDL_BlitSurface(surface_textures,&rect,surface_map,&rect2);
	tiles[8][8] = true;

	// transfer pixedata from surface to texture
	SDL_UpdateTexture(level_texture, NULL, surface_map->pixels, surface_map->pitch);
}

void GraphicsExit()
{
	SDL_FreeSurface(player_surface);
	SDL_FreeSurface(surface_map);
	SDL_FreeSurface(surface_textures);
	SDL_FreeSurface(debug_message);

	SDL_DestroyTexture(level_texture);
	SDL_DestroyTexture(player_texture);
	SDL_DestroyTexture(debug_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
		
	// close SDL_ttf
	TTF_CloseFont(debug_font);
    TTF_Quit();
}

void Render(DynamicEntity &p)
{
	SDL_Rect player_sprite_coords;
	player_sprite_coords.h = 32;
	player_sprite_coords.w = 24;

	anim_player.Animate();

	if(p.xVel == 0)
	{
		player_sprite_coords.x = 24;
		player_sprite_coords.y = 0;
		anim_player.SetCurrentFrame(0);
	}
	else
	{
		player_sprite_coords.x = (2+anim_player.GetCurrentFrame()) * 24;
		player_sprite_coords.y = 0;
	}
	if(!p.onground)
	{
		if(p.yVel < 0) player_sprite_coords.x = (7 * 24);
		else player_sprite_coords.x = (8 * 24);
	}
	p.rect.x = (int)p.x - camera.x;
	p.rect.y = (int)p.y - camera.y;
	if(!p.direction) SDL_RenderCopyEx(renderer, player_texture, &player_sprite_coords, &p.rect, NULL, NULL, SDL_FLIP_HORIZONTAL);
	else SDL_RenderCopyEx(renderer, player_texture, &player_sprite_coords, &p.rect, NULL, NULL, SDL_FLIP_NONE);
}


void ShowDebugInfo()
{
	char debug_str[128];
	int x, y, rectx, recty;
	player.get_pos(x, y);
	player.get_pos_rect(rectx, recty);
	SDL_FreeSurface(debug_message);
	sprintf_s(debug_str,"PlayerX = %d. PlayerY = %d VelX: %d VelY: %d Direction: %d Onground: %d HOLD_RIGHT: %d HOLD_LEFT: %d",
		x, y, (int)player.get_xvelocity(), (int)player.get_yvelocity(), player.get_direction() , player.onground, holdkeys[HOLD_RIGHT], holdkeys[HOLD_LEFT]);
	debug_message = TTF_RenderText_Solid( debug_font, debug_str, debug_color );
	SDL_DestroyTexture(debug_texture);
	debug_texture = SDL_CreateTextureFromSurface(renderer, debug_message);
	SDL_Rect temp;
	SDL_GetClipRect(debug_message, &temp);
	temp.x = 0;
	temp.y = HEIGHT-32;
	SDL_RenderCopy(renderer, debug_texture, NULL, &temp);
}