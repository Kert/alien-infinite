#include "camera.h"

SDL_Rect camera = {0, 0, WIDTH, HEIGHT};

void SetCamera(DynamicEntity &p)
{
	//Center the camera over the player
    camera.x = ( p.x + p.width / 2 ) - WIDTH / 2;
    camera.y = ( p.y + p.height / 2 ) - HEIGHT / 2;

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