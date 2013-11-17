#include "utils.h"

#ifndef _main_h_
#define _main_h_ 
#endif

// window
#define WIDTH 800
#define HEIGHT 600

#define MAP_WIDTH 2000
#define MAP_HEIGHT 2000

// ends gameloop
bool ENDGAME=false;

SDL_Texture *level_texture = NULL;
SDL_Texture *player_texture = NULL;
SDL_Surface *player_surface = NULL;

SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *debug_texture = NULL;
TTF_Font *debug_font = NULL;
SDL_Color debug_color = { 255, 255, 255 };
SDL_Surface *debug_message = NULL;

SDL_Rect camera = {0, 0, WIDTH, HEIGHT};

#define GRAVITY 2800
#define AIR_RESISTANCE 3000
#define FRICTION 9999
#define JUMP_FORCE 1000

// blocks data (air or solid)
bool tiles[1000][1000];

// array that stores key states (pressed or not)
#define HOLD_RIGHT 0
#define HOLD_LEFT 1
#define HOLD_DOWN 2
#define HOLD_UP 3

bool holdkeys[4] = {false};