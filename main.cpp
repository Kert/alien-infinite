#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

// window
#define WIDTH 800
#define HEIGHT 600

#define MAP_WIDTH 2000
#define MAP_HEIGHT 2000

#define GRAVITY 2800
#define JUMP_FORCE 1000

#define FRAMES_PER_SECOND 60

// blocks data (air or solid)
bool tiles[1000][1000];

// ends gameloop
bool ENDGAME=false;

SDL_Texture *level_texture = NULL;
SDL_Texture *player_texture = NULL;

SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *debug_texture = NULL;
TTF_Font *debug_font = NULL;
SDL_Color debug_color = { 255, 255, 255 };
SDL_Surface *debug_message = NULL;

SDL_Rect camera = {0, 0, WIDTH, HEIGHT};
	
bool q1=false, q2=false, q3=false, q4=false;
// array that stores key states (pressed or not)
#define HOLD_RIGHT 0
#define HOLD_LEFT 1
#define HOLD_DOWN 2
#define HOLD_UP 3

bool holdkeys[4] = {false};

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
/*
class Tile
{
	private:
	int x;
	int y;
	bool solid;

	public:
	Tile();

	bool is_solid();
};

Tile::Tile()
{
	x = 0;
	y = 0;
	solid = false;
}

bool Tile::is_solid()
{
	return solid;
}
*/

class Player
{
    private:
    float x, y;
	float xVel, yVel;
	bool direction;
	int player_vel;
	int player_width;
	int player_height;
	SDL_Rect rect;

    public:
    //Initializes the variables
    Player();

    //Takes key presses and adjusts the player's velocity
	void handle_input(SDL_Event event);

    void update_pos( Uint32 deltaTicks );

	float get_xvelocity();
	float get_yvelocity();
	bool get_direction();
	void set_direction(bool direction);
	void get_pos(int &x, int &y);
	void set_pos(int x, int y);
	void get_pos_rect(int &x, int &y);
	void set_camera();

    void show();
};

Player player;
Player::Player()
{
	player_vel = 380;
	player_width = 30;
	player_height = 26;

    //Initialize the offsets
    x = 0;
    y = 0;

    //Initialize the velocity
    xVel = 0;
    yVel = 0;

	rect.h = 36; rect.w = 30;
}

float Player::get_xvelocity()
{
	return xVel;
}

float Player::get_yvelocity()
{
	return yVel;
}

bool Player::get_direction()
{
	return direction;
}

void Player::set_direction(bool direction)
{
	this->direction = direction;
}

void Player::get_pos(int &x, int &y)
{
	x = (int)this->x;
	y = (int)this->y;
}

void Player::set_pos(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Player::get_pos_rect(int &x, int &y)
{
	x = rect.x;
	y = rect.y;
}

//bool released = true;
void Player::handle_input(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		switch( event.key.keysym.sym )
        {
			case SDLK_RIGHT:
			{
				holdkeys[HOLD_RIGHT] = true;
				break;
			}
			case SDLK_LEFT:
			{
				holdkeys[HOLD_LEFT] = true;
				break;
			}
			case SDLK_z:
			{
				if(yVel == 0)
				{
					yVel = -JUMP_FORCE;	
				}	
				break;
			}
			case SDLK_t: // teleport key
			{
				y = 32;
				x = 32;
				break;
			}
		}
	}

	if (event.type == SDL_KEYUP)
	{
		switch( event.key.keysym.sym )
        {
			case SDLK_RIGHT:
			{
				holdkeys[HOLD_RIGHT] = false;
				break;
			}
			case SDLK_LEFT:
			{
				holdkeys[HOLD_LEFT] = false;
				break;
			}
		}
	}
	
	if(holdkeys[HOLD_LEFT])
	{
		direction = 0;
		xVel = -player_vel;
	}

	if(holdkeys[HOLD_RIGHT])
	{
		direction = 1;
		xVel = player_vel;
	}
	
	// stop if keys aren't pressed anymore
	else if(!holdkeys[HOLD_RIGHT] && !holdkeys[HOLD_LEFT])
	{
		xVel = 0;
	}
}

void Player::update_pos( Uint32 deltaTicks )
{
	int tempx, tempy, tempx2, tempy2;

    //Move the player left or right
    x += xVel * ( deltaTicks / 1000.f );
	tempx = x / 32;
	tempy = y / 32;
	tempx2 = (x+player_width) / 32;
	tempy2 = (y+player_height) / 32;

	//check collision right
	if(tiles[tempx+1][tempy] == true || tiles[tempx+1][tempy2] == true)
	{
		x = tempx * 32;
	}
	
	tempx = x / 32;
	tempy = y / 32;
	tempx2 = (x+player_width) / 32;
	tempy2 = (y+player_height) / 32;

	//check collision left
	if(tiles[tempx][tempy] == true || tiles[tempx][tempy2] == true)
	{
		x = tempx2 * 32;
	}

    //If the player went too far to the left
    if( x < 0 )
    {
        //Move back
        x = 0;
    }
    //or the right
    else if( x + player_width > MAP_WIDTH )
    {
        //Move back
        x = MAP_WIDTH - player_width;
    }

	
    //Move the player up or down
    y += yVel * ( deltaTicks / 1000.f );
	yVel += GRAVITY * ( deltaTicks / 1000.f );
	
	if(yVel > GRAVITY)
	{
		yVel = GRAVITY;
	}

    //If the player went too far up
    if( y < 0 )
    {
        //Move back
        y = 0;
    }
    //or down
    else if( y + player_height > MAP_HEIGHT )
    {
        //Move back
        y = MAP_HEIGHT - player_height;
    }
		
	tempx = x / 32;
	tempx2 = (x+player_width) / 32;
	tempy = y / 32;	
	tempy2 = (y+player_height) / 32;

	//check collision above
	if(tiles[tempx][tempy] == true || tiles[tempx2][tempy] == true)
	{
		y = (tempy+1) * 32;
		yVel = 0;
	}

	tempx = x / 32;
	tempx2 = (x+player_width) / 32;
	tempy = y / 32;	
	tempy2 = (y+player_height) / 32;

	//check collision below
	if(tiles[tempx][tempy+1] == true || tiles[tempx2][tempy+1] == true)
	{
		if(yVel > 0)
		{
			y = tempy * 32;
			yVel = 0;
		}
	}
}

void Player::set_camera()
{
    //Center the camera over the player
    camera.x = ( x + player_width / 2 ) - WIDTH / 2;
    camera.y = ( y + player_height / 2 ) - HEIGHT / 2;

    //Keep the camera in bounds.
    if( camera.x < 0 )
    {
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > MAP_WIDTH - camera.w )
    {
        camera.x = MAP_WIDTH - camera.w;
    }
    if( camera.y > MAP_HEIGHT - camera.h )
    {
        camera.y = MAP_HEIGHT - camera.h;
    }
}

void Player::show()
{
	SDL_Rect player_sprite_coords;
	player_sprite_coords.h = 36;
	player_sprite_coords.w = 30;
	player_sprite_coords.x = 0;
	player_sprite_coords.y = 0;

	rect.x = (int)x - camera.x;
	rect.y = (int)y - camera.y;
	SDL_RenderCopy(renderer, player_texture, &player_sprite_coords, &rect);
}

class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
}; 

Timer delta;
Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

/*
void ShowSprite(SDL_Rect &onscreen, SDL_Texture *img, int x, int y, int h, int w)
{
	SDL_Rect rect;
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	SDL_RenderCopy(renderer, img, &onscreen, &rect);
}
*/

void CallUpdate()
{
	SDL_Event e;
	
	while( SDL_PollEvent( &e ) )
    {
        //Handle events for the player
		player.handle_input(e);

        //If the user has Xed out the window
        if( e.type == SDL_QUIT )
        {
            //Quit the program
            ENDGAME = true;
        }
    }

		//Move the player
    player.update_pos( delta.get_ticks() );

	player.set_camera();

    //Restart delta timer
    delta.start();
}

void ShowDebugInfo()
{
	char debug_str[128];
	int x, y, rectx, recty;
	player.get_pos(x, y);
	player.get_pos_rect(rectx, recty);
	SDL_FreeSurface(debug_message);
	sprintf_s(debug_str,"PlayerX = %d. PlayerY = %d PlayerRectX = %d PlayerRectY = %d VelX: %d VelY: %d Direction: %d HOLD_RIGHT: %d HOLD_LEFT: %d",
		x, y, rectx, recty, (int)player.get_xvelocity(), (int)player.get_yvelocity(), player.get_direction() , holdkeys[HOLD_RIGHT], holdkeys[HOLD_LEFT]);
	debug_message = TTF_RenderText_Solid( debug_font, debug_str, debug_color );
	SDL_DestroyTexture(debug_texture);
	debug_texture = SDL_CreateTextureFromSurface(renderer, debug_message);
	SDL_Rect temp;
	SDL_GetClipRect(debug_message, &temp);
	temp.x = 0;
	temp.y = HEIGHT-32;
	SDL_RenderCopy(renderer, debug_texture, NULL, &temp);
}

void CallDraw()
{
	// clear the screen
	SDL_RenderClear(renderer);

	// show level map
	SDL_RenderCopy(renderer, level_texture, &camera, NULL);

	// show player sprite
	player.show();

	ShowDebugInfo();
	
	// flip the backbuffer
	// this means that everything that we prepared behind the screens is actually shown
	SDL_RenderPresent(renderer);
}

void LoadLevel(SDL_Surface * surf_map, SDL_Surface * surf_textures)
{
	SDL_Rect rect;
	SDL_Rect rect2;

	//tile size on level map surface
	rect2.w=32; rect2.h=32;

	// Load background tiles

	// tile coords in texture
	rect.x=99; rect.y=64; rect.w=32; rect.h=32;
	
	for(int i = 0; i <= (MAP_HEIGHT / 32); i++)
	{
		for(int j = 0; j < (MAP_WIDTH / 32); j++)
		{
			rect2.x = i * 32;
			rect2.y = j * 32;
			SDL_BlitSurface(surf_textures,&rect,surf_map,&rect2);
		}
	}

	// Load solid tiles

	// tile coords in texture
	rect.x=33; rect.y=0; rect.w=32; rect.h=32;

	for(int i = 0; i <= (MAP_WIDTH / 32); i++)
	{
		for(int j = 13; j < 14; j++)
		{
			rect2.x = i * 32;
			rect2.y = j * 32;
			tiles[i][j] = true;
			SDL_BlitSurface(surf_textures,&rect,surf_map,&rect2);
		}
	}

	for(int i = 12; i <= 12; i++)
	{
		for(int j = 4; j < 12; j++)
		{
			rect2.x = i * 32;
			rect2.y = j * 32;
			tiles[i][j] = true;
			SDL_BlitSurface(surf_textures,&rect,surf_map,&rect2);
		}
	}

	// some other solid blocks
	rect2.x = 10 * 32;
	rect2.y = 10 * 32;
	SDL_BlitSurface(surf_textures,&rect,surf_map,&rect2);
	tiles[10][10] = true;

	rect2.x = 11 * 32;
	rect2.y = 9 * 32;
	SDL_BlitSurface(surf_textures,&rect,surf_map,&rect2);
	tiles[11][9] = true;

	rect2.x = 8 * 32;
	rect2.y = 8 * 32;
	SDL_BlitSurface(surf_textures,&rect,surf_map,&rect2);
	tiles[8][8] = true;

	// transfer pixedata from surface to texture
	SDL_UpdateTexture(level_texture, NULL, surf_map->pixels, surf_map->pitch);
}

int main( int argc, char* argv[] )
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;
	
	// Initialize SDL_TTF for font rendering
	if( TTF_Init() == -1 )
    {
        return false;    
    }
	
	debug_font = TTF_OpenFont( "verdana.ttf", 12 );
	
	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	
	// used for texture transparency, but I don't really know how to use it
	// it doesn't work
	//SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_Surface *surface_map = SDL_CreateRGBSurface(0, MAP_WIDTH, MAP_HEIGHT, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);

	SDL_Surface *surface_textures = SDL_LoadBMP("images/blocks.bmp");

	level_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, MAP_WIDTH, MAP_HEIGHT);
	// can be used to return dimensions of texture
	//SDL_QueryTexture(img, NULL, NULL, &w, &h);

	LoadLevel(surface_map, surface_textures);

	player_texture = IMG_LoadTexture(renderer, "images/player.png");
	player.set_pos(10,20);
	
	// start delta timer
	// used to change velocity of objects according to time (not fps)
	delta.start();
	// main loop
	while (!ENDGAME) {
		
		CallUpdate();
		CallDraw();
	}
	
	// clean up
	SDL_FreeSurface(surface_map);
	SDL_FreeSurface(surface_textures);
	SDL_FreeSurface(debug_message);

	SDL_DestroyTexture(level_texture);
	SDL_DestroyTexture(player_texture);
	SDL_DestroyTexture(debug_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	TTF_CloseFont(debug_font);
    TTF_Quit();
	SDL_Quit();

  return 0;
}