#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

#define FRAMES_PER_SECOND 60

bool ENDGAME=false;

SDL_Texture *img = NULL;
SDL_Texture *player_texture = NULL;

SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;

int player_x = 30, player_y = 150;
SDL_Rect texr, player_rect;
	
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
		if(q1) player_rect.x-=5;
		if(q2) player_rect.x+=5;
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
	SDL_Rect player_sprite_coords;
	player_sprite_coords.h = 36;
	player_sprite_coords.w = 30;
	player_sprite_coords.x = 0;
	player_sprite_coords.y = 0;

		// clear the screen
		SDL_RenderClear(renderer);
		// copy the texture to the rendering context
		texr.x = 0; texr.y = 270; texr.w = 800; texr.h = 60;
		SDL_RenderCopy(renderer, img, NULL, &texr);
		SDL_RenderCopy(renderer, player_texture, &player_sprite_coords, &player_rect);
		// flip the backbuffer
		// this means that everything that we prepared behind the screens is actually shown
		SDL_RenderPresent(renderer);
}

int main( int argc, char* argv[] )
{

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;
	
	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	
	// load our image
	img = IMG_LoadTexture(renderer, "images/hello.png");
	//SDL_QueryTexture(img, NULL, NULL, &w, &h);	

	player_rect.x = 30; player_rect.y = 170;
	player_rect.h = 36; player_rect.w = 30;

	player_texture = IMG_LoadTexture(renderer, "images/player.png");

	// main loop
	while (!ENDGAME) {
		
		CallUpdate();
		CallDraw();
		SDL_Delay(16);
	}
	
	SDL_DestroyTexture(img);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

  return 0;
}