#ifndef _input_h_
#define _input_h_ 

#include <SDL.h>
#include "entities.h"
#include "physics.h"


// ends gameloop
bool ENDGAME=false;


// array that stores key states (pressed or not)
#define HOLD_RIGHT 0
#define HOLD_LEFT 1
#define HOLD_DOWN 2
#define HOLD_UP 3

bool holdkeys[4] = {false};

void ProcessInput(DynamicEntity &p)
{
	if(holdkeys[HOLD_LEFT])
	{
		p.direction = 0;
		p.xVel = -p.move_vel;
	}

	if(holdkeys[HOLD_RIGHT])
	{
		p.direction = 1;
		p.xVel = p.move_vel;
	}

	SDL_Event event;
	while( SDL_PollEvent( &event ) )
    {
        //Handle events for the player
		if (event.type == SDL_KEYDOWN)
		{
			if(!event.key.repeat)
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
						if(p.onground)
						{
							p.yVel = -JUMP_FORCE;	
						}	
						break;
					}
					case SDLK_t: // teleport key
					{
						p.y = 32;
						p.x = 32;
						break;
					}
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
			
        //If the user has Xed out the window
        if( event.type == SDL_QUIT )
        {
            //Quit the program
            ENDGAME = true;
        }
    }
}

#endif