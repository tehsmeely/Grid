#ifndef GLOBALS_H
#define GLOBALS_H
#include <SDL.h>
#include "TextureManager.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT; // Min GRID_HEIGHT + 1 to allow for bottom line
extern const int GRID_WIDTH;
extern const int GRID_HEIGHT;
extern const int GRID_SIZE;
extern const Uint32 UPDATE_DELAY; //in ms
extern TextureManager textureManager;

#endif