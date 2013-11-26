#ifndef _graphics_h_
#define _graphics_h_ 

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "animation.h"
#include "camera.h"
#include "input.h"
#include "level.h"

int GraphicsSetup();
void LoadLevel();
void GraphicsUpdate();
void GraphicsExit();
void Render(DynamicEntity &p);
void ShowDebugInfo();

#endif