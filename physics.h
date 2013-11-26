#ifndef _physics_h_
#define _physics_h_ 

#include <SDL.h>
#include <math.h>
#include "level.h"
#include "entities.h"

#define PHYSICS_SPEED 1000.f
#define GRAVITY 2500
#define AIR_RESISTANCE 3000
#define FRICTION 9999
#define JUMP_FORCE 500

void ApplyPhysics(DynamicEntity &p, Uint32 deltaTicks);

#endif