#pragma once
#include <SDL.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include "globals.h"


struct FilenameMapping {
	int buttonId;
	std::string filename;
};
std::vector<SDL_Point> parseFile(std::string);

void loadPreset(std::string, int**, int**);

void savePreset(std::string, int**);