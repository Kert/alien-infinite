#include "main.h"

bool ENDGAME=false;
Timer delta;
extern Player player;

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
	GraphicsUpdate();
}

int main( int argc, char* argv[] )
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;
		
	GraphicsSetup();

	LoadLevel();

	player.set_pos(50,50);
	player.set_direction(true);

	// start delta timer
	// used to change velocity of objects according to time (not fps)
	delta.start();
	// main loop
	while (!ENDGAME)
	{
		CallUpdate();
		CallDraw();
	}
	
	// clean up
	GraphicsExit();

	// close SDL
	SDL_Quit();

  return 0;
}