#ifndef _graphics_h_
#define _graphics_h_ 

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "entities.h"
#include "camera.h"
#include "input.h"
#include "level.h"

SDL_Renderer *renderer = NULL;
SDL_Texture *player_texture = NULL;
SDL_Texture *level_texture = NULL;
SDL_Surface *player_surface = NULL;

SDL_Window *win = NULL;

SDL_Texture *debug_texture = NULL;
TTF_Font *debug_font = NULL;
SDL_Color debug_color = { 255, 255, 255 };
SDL_Surface *debug_message = NULL;


void Render(DynamicEntity &p)
{
	SDL_Rect player_sprite_coords;
	player_sprite_coords.h = 32;
	player_sprite_coords.w = 24;
	player_sprite_coords.x = 0;
	player_sprite_coords.y = 0;

	p.rect.x = (int)p.x - camera.x;
	p.rect.y = (int)p.y - camera.y;
	//SDL_RenderCopy(renderer, player_texture, &player_sprite_coords, &p.rect);
	if(!p.direction) SDL_RenderCopyEx(renderer, player_texture, &player_sprite_coords, &p.rect, NULL, NULL, SDL_FLIP_HORIZONTAL);
	else SDL_RenderCopyEx(renderer, player_texture, &player_sprite_coords, &p.rect, NULL, NULL, SDL_FLIP_NONE);
}


void ShowDebugInfo()
{
	char debug_str[128];
	int x, y, rectx, recty;
	player.get_pos(x, y);
	player.get_pos_rect(rectx, recty);
	SDL_FreeSurface(debug_message);
	sprintf_s(debug_str,"PlayerX = %d. PlayerY = %d PlayerRectX = %d PlayerRectY = %d VelX: %d VelY: %d Direction: %d HOLD_RIGHT: %d HOLD_LEFT: %d",
		x, y, rectx, recty, (int)player.get_xvelocity(), (int)player.get_yvelocity(), player.get_direction() , holdkeys[HOLD_RIGHT], holdkeys[HOLD_LEFT]);
	debug_message = TTF_RenderText_Solid( debug_font, debug_str, debug_color );
	SDL_DestroyTexture(debug_texture);
	debug_texture = SDL_CreateTextureFromSurface(renderer, debug_message);
	SDL_Rect temp;
	SDL_GetClipRect(debug_message, &temp);
	temp.x = 0;
	temp.y = HEIGHT-32;
	SDL_RenderCopy(renderer, debug_texture, NULL, &temp);
}

#endif