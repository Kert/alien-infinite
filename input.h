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
#define HOLD_JUMP 4

bool holdkeys[5] = {false};

void ProcessInput(Player &p)
{
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
							holdkeys[HOLD_JUMP] = true;
							p.jumping = true;
							if(p.xVel > 0) p.xVel = p.in_air_vel;
							else if(p.xVel < 0) p.xVel = -p.in_air_vel;
							p.jumptime = SDL_GetTicks();
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
				case SDLK_z:
				{	
					holdkeys[HOLD_JUMP] = false;
					p.jumping = false;
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

	if(holdkeys[HOLD_LEFT])
	{
		p.direction = 0;
		if(p.onground) p.xVel = -p.move_vel;
		else p.xVel = -p.in_air_vel;
	}

	if(holdkeys[HOLD_RIGHT])
	{
		p.direction = 1;
		if(p.onground) p.xVel = p.move_vel;
		else p.xVel = p.in_air_vel;
	}

	if(holdkeys[HOLD_JUMP])
	{
		if(!p.onground)
		{
			if(SDL_GetTicks() - p.jumptime > 200) p.jumping = false;
		}
	}
}

#endif