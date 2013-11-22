#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <math.h>
#include "main.h"

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

void CallUpdate()
{
	ProcessInput(player);
	
	ApplyPhysics(player, delta.get_ticks());

	SetCamera(player);

    //Restart delta timer
    delta.start();
}

void CallDraw()
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
	
	GraphicsSetup();

	LoadLevel(surface_map, surface_textures);

	player.set_pos(10,20);
	player.set_direction(true);

	// start delta timer
	// used to change velocity of objects according to time (not fps)
	delta.start();

	// main loop
	while (!ENDGAME) {
		
		CallUpdate();
		CallDraw();
	}
	
	// clean up
	GraphicsCleanup();

	// close SDL_ttf
	TTF_CloseFont(debug_font);
    TTF_Quit();

	// close SDL
	SDL_Quit();

  return 0;
}