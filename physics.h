#ifndef _physics_h_
#define _physics_h_ 

#include <SDL.h>
#include <math.h>
#include "entities.h"
#include "level.h"
#include "utils.h"

#define PHYSICS_SPEED 1000.f
#define GRAVITY 2500
#define AIR_RESISTANCE 3000
#define FRICTION 9999
#define JUMP_FORCE 500

void ApplyPhysics(DynamicEntity &p, Uint32 deltaTicks )
{
	int tempx, tempy, tempx2, tempy2;
	float tx, ty;

	if(!p.onground)
	{

#ifdef AIR_RESISTANCE_ENABLED

		if(p.xVel > 0)
		{
			p.xVel -= AIR_RESISTANCE * ( deltaTicks / PHYSICS_SPEED );
			if(p.xVel < 0) p.xVel = 0;
		}
		if(p.xVel < 0)
		{
			p.xVel += AIR_RESISTANCE * ( deltaTicks / PHYSICS_SPEED );
			if(p.xVel > 0) p.xVel = 0;
		}
#endif

	}
	else
	{
		if(p.xVel > 0)
		{
			p.xVel -= FRICTION * ( deltaTicks / PHYSICS_SPEED );
			if(p.xVel < 0) p.xVel = 0;
		}
		if(p.xVel < 0)
		{
			p.xVel += FRICTION * ( deltaTicks / PHYSICS_SPEED );
			if(p.xVel > 0) p.xVel = 0;
		}
	}

	//Move the player up or down
	ty = p.y;
	
	if(p.jumping) p.yVel -= p.accel * deltaTicks;

	// Add some gravity
	p.yVel += GRAVITY * ( deltaTicks / PHYSICS_SPEED );
	if(p.yVel > GRAVITY)
	{
		p.yVel = GRAVITY;
	}
	
    p.y += p.yVel * ( deltaTicks / PHYSICS_SPEED );
	
    //If the player went too far up
    if(p.y < 0 )
    {
        //Move back
        p.y = 0;
    }

    //or down
    else if( p.y + p.height > MAP_HEIGHT )
    {
        //Move back
        p.y = MAP_HEIGHT - p.height;
    }
		
	tempx = ceilf(p.x / 32)-0.5;
	tempy = ceilf(p.y / 32)-0.5;
	tempx2 = ceilf((p.x+p.width) / 32)-0.5;
	tempy2 = ceilf((p.y+p.height) / 32)-0.5;

	for(int i = tempx; i <= tempx2; i++)
	{
		if(tiles[i][tempy] == true)
		{
			p.y = ty;
			p.yVel = 0;
		}
	}
	
	p.onground = false;
	for(int i = tempx; i <= tempx2; i++)
	{
		if(tiles[i][tempy2] == true)
		{
			if(p.yVel > 0)
			{
				p.y = ty;
				//y = tempy * 32;
				//y = tempy2 * 32 - p.player_height;
				p.yVel = 0;
				p.onground = true;
			}
		}
	}


    //Move the player left or right
	tx = p.x;
    p.x += p.xVel * ( deltaTicks / PHYSICS_SPEED );
	
	//If the player went too far to the left
    if( p.x < 0 )
    {
        //Move back
        p.x = 0;
    }

    //or the right
    else if( p.x + p.width > MAP_WIDTH )
    {
        //Move back
        p.x = MAP_WIDTH - p.width;
    }

	tempx = ceilf(p.x / 32)-0.5;
	tempy = ceilf(p.y / 32)-0.5;
	tempx2 = ceilf((p.x+p.width) / 32)-0.5;
	tempy2 = ceilf((p.y+p.height) / 32)-0.5;

	for(int j = tempy; j <= tempy2; j++)
	{
		if(tiles[tempx][j] == true)
		{
			p.x = tx;
			break;
		}
		if(tiles[tempx2][j] == true)
		{
			p.x = tx;
			break;
		}
	}
}

#endif