#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <map>
// Handles the loading and subsequest storing of textures for the vast majority of objects
// When an image load is requested, it checks if it exists, and returns pointer to if so
// if not, loads , stores, then returns pointer
class TextureManager
{
	std::map<std::string, SDL_Texture*> textures;
public:
	TextureManager();
	~TextureManager();
	SDL_Texture* loadImage(SDL_Renderer* ren, std::string);
};

