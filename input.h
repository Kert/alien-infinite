#ifndef _input_h_
#define _input_h_ 

#include "entities.h"

// array that stores key states (pressed or not)
#define HOLD_RIGHT 0
#define HOLD_LEFT 1
#define HOLD_DOWN 2
#define HOLD_UP 3
#define HOLD_JUMP 4

void ProcessInput(Player &p);

#endif