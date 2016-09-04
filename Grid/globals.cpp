#include "globals.h"

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 981; // Min GRID_HEIGHT + 1 to allow for bottom line
const int GRID_WIDTH = 1200;
const int GRID_HEIGHT = 980;
const int GRID_SIZE = 10;
const Uint32 UPDATE_DELAY = 200; //in ms
int BORDER_VAL = 0;
int gridSizeX = GRID_WIDTH / GRID_SIZE;
int gridSizeY = GRID_HEIGHT / GRID_SIZE;
TextureManager textureManager = TextureManager();
