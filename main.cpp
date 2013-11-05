#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <time.h>
#include <string>

using namespace std;

#define WIDTH 800
#define HEIGHT 600

#define MAP_WIDTH 2000
#define MAP_HEIGHT 2000

#define FRAMES_PER_SECOND 60

bool ENDGAME=false;

SDL_Texture *level_texture = NULL;
SDL_Texture *player_texture = NULL;

SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *debug_texture = NULL;
TTF_Font *debug_font = NULL;
SDL_Color debug_color = { 255, 255, 255 };
SDL_Surface *debug_message = NULL;

int player_x = 300, player_y = 150;
SDL_Rect texr, player_rect, camera;
	
bool q1=false, q2=false, q3=false, q4=false;
bool jumping = false;	// состояние прыжка
int jump_powah = 20;		// мощность прыжка
int jump_powah_temp = jump_powah;

/*
//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
SDL_PixelFormat *fmt;
SDL_Surface *load_image( std::string filename )
{
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = SDL_LoadBMP( filename.c_str() );

    //If nothing went wrong in loading the image
    if( loadedImage != NULL )
    {
        //Create an optimized image
		optimizedImage = SDL_ConvertSurface(loadedImage,fmt,0);

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    //Return the optimized image
    return optimizedImage;
}
*/


void ShowSprite(SDL_Rect &onscreen, SDL_Texture *img, int x, int y, int h, int w)
{
	SDL_Rect rect;
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	SDL_RenderCopy(renderer, img, &onscreen, &rect);
}


void key_state_check(SDL_Event &e, SDL_Keycode key, bool &q)
{
	if (e.type == SDL_KEYUP && e.key.keysym.sym == key)
	{
		q=false;
	}
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == key)
	{
		q=true;
	}
}

void CallUpdate()
{

		// event handling
		SDL_Event e;
		if ( SDL_PollEvent(&e) ) {
			if (e.type == SDL_QUIT)
				ENDGAME=true;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				ENDGAME=true;

			key_state_check(e, SDLK_LEFT, q1);
			key_state_check(e, SDLK_RIGHT, q2);
			key_state_check(e, SDLK_UP, q3);
			key_state_check(e, SDLK_DOWN, q4);
			
		}
		if(q1) // LEFT
		{
			if(player_rect.x > 400)
			{
				player_rect.x -= 5;
				player_x-=5;
			}
			else
			{
				player_x-=5;
				camera.x-=5; 
							
				if(camera.x < 0) 
				{ 
					camera.x = 0; 
					player_rect.x -= 5;
					if(player_rect.x < 32)
					{
						player_rect.x = 32;
						player_x += 5;
					}
				}
			}
		}
		if(q2) // RIGHT
		{ 
			if(player_rect.x < 400)
			{
				player_rect.x += 5;
				player_x+=5;
			}
			else
			{
				player_x+=5;
				camera.x+=5; 

				if(camera.x > 1000) 
				{
					camera.x = 1000;
					player_rect.x += 5;
					if(player_rect.x > 750)
					{
						player_rect.x = 750;
						player_x -= 5;
					}
				}
			}
		}
		if(q3) if(!jumping) jumping = true;


		if(jumping)
		{
			jump_powah_temp--;
			player_rect.y -= jump_powah_temp;
			if(player_rect.y + player_rect.h >= texr.y)
			{
				player_rect.y = texr.y - player_rect.h;
				jumping = false;
				jump_powah_temp = jump_powah;
			}
		}
}

void CallDraw()
{
	char debug_str[128];
	SDL_Rect player_sprite_coords;
	player_sprite_coords.h = 36;
	player_sprite_coords.w = 30;
	player_sprite_coords.x = 0;
	player_sprite_coords.y = 0;

	// clear the screen
	SDL_RenderClear(renderer);
	// copy the texture to the rendering context
	texr.x = 0; texr.y = 270; texr.w = 800; texr.h = 60;
	SDL_RenderCopy(renderer, level_texture, &camera, NULL);
	SDL_RenderCopy(renderer, player_texture, &player_sprite_coords, &player_rect);

	SDL_FreeSurface(debug_message);
	sprintf_s(debug_str,"PlayerX = %d. PlayerY = %d PlayerRectX = %d PlayerRectY = %d", player_x, player_y, player_rect.x, player_rect.y);

	debug_message = TTF_RenderText_Solid( debug_font, debug_str, debug_color );
	//SDL_UpdateTexture(debug_texture, NULL, debug_message->pixels, debug_message->pitch);
	SDL_DestroyTexture(debug_texture);
	debug_texture = SDL_CreateTextureFromSurface(renderer, debug_message);

	SDL_Rect temp;
	SDL_GetClipRect(debug_message, &temp);
	//temp.h = 16;
	//temp.w = 200;
	temp.x = 0;
	temp.y = HEIGHT-32;
	SDL_RenderCopy(renderer, debug_texture, NULL, &temp);
	
	// flip the backbuffer
	// this means that everything that we prepared behind the screens is actually shown
	SDL_RenderPresent(renderer);
}

void LoadLevel(SDL_Surface * surf_map, SDL_Surface * surf_textures)
{
	SDL_Rect rect;
	SDL_Rect rect2;
	rect.x=66; rect.y=0; rect.w=32; rect.h=32;
	rect2.w=32; rect2.h=32;
	for(int i = 0; i < 20; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			rect2.x = i * 32;
			rect2.y = j * 32;
			SDL_BlitSurface(surf_textures,&rect,surf_map,&rect2);
		}
	}

	rect.x=99; rect.y=0; rect.w=32; rect.h=32;
	for(int i = 0; i < 20; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			rect2.x = i * 32 + 640;
			rect2.y = j * 32;
			SDL_BlitSurface(surf_textures,&rect,surf_map,&rect2);
		}
	}

}

int main( int argc, char* argv[] )
{
	
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;
	
	if( TTF_Init() == -1 )
    {
        return false;    
    }
	
	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	
	debug_font = TTF_OpenFont( "verdana.ttf", 12 );

	/*SDL_Surface *surface_screen = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);*/

	SDL_Surface *surface_map = SDL_CreateRGBSurface(0, 2000, 600, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);

	SDL_Surface *surface_textures = SDL_LoadBMP("images/blocks.bmp");

	level_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 2000, 600);
	//SDL_QueryTexture(img, NULL, NULL, &w, &h);	

	camera.x = 0;
	camera.y = 0;
	camera.w = 800;
	camera.h = 600;

	player_rect.x = 300; player_rect.y = 170;
	player_rect.h = 36; player_rect.w = 30;

	player_texture = IMG_LoadTexture(renderer, "images/player.png");

	LoadLevel(surface_map, surface_textures);

	// load our image
	SDL_UpdateTexture(level_texture, NULL, surface_map->pixels, surface_map->pitch);

	//debug_message = TTF_RenderText_Solid( debug_font, "What happens", debug_color );
	//debug_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB555, SDL_TEXTUREACCESS_STREAMING, 93, 16);
	debug_message = TTF_RenderText_Solid( debug_font, "What happens", debug_color );
	//SDL_UpdateTexture(debug_texture, NULL, debug_message->pixels, debug_message->pitch);

	debug_texture = SDL_CreateTextureFromSurface(renderer, debug_message);
	
	//SDL_UpdateTexture(debug_texture, NULL, debug_message->pixels, debug_message->pitch);

	//debug_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 300, 100);
	//SDL_QueryTexture(img, NULL, NULL, &w, &h);	

	// main loop
	while (!ENDGAME) {
		
		CallUpdate();
		CallDraw();
		SDL_Delay(16);
	}
	
	SDL_FreeSurface(surface_map);
	SDL_FreeSurface(surface_textures);
	//SDL_FreeSurface(debug_message);

	SDL_DestroyTexture(level_texture);
	//SDL_DestroyTexture(player_texture);
	//SDL_DestroyTexture(debug_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	TTF_CloseFont(debug_font);
    TTF_Quit();
	SDL_Quit();

  return 0;
}