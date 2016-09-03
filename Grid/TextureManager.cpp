#include "TextureManager.h"
#include <iostream>
#include <string>
TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	std::cout << "TextureManager Destructor, " << std::endl;
	for (auto iter : textures)
	{
		//std::cout << iter.first << ", " << iter.second << std::endl;
		SDL_DestroyTexture(iter.second);
	}
}

SDL_Texture* TextureManager::loadImage(SDL_Renderer* ren, std::string path)
{
	std::cout << "Texture manager receiving request for: " << path << std::endl;
	if (textures.count(path)) // texture already exists
	{
		std::cout << "Texture already loaded!" << std::endl << std::endl;
		return textures[path];
	}
	else // make new one and store
	{
		std::cout << "Texture not loaded, loading now!" << std::endl;
		SDL_Texture *texture = IMG_LoadTexture(ren, path.c_str());
		if (texture == nullptr)
			std::cout << ">>ERROR<< Issue loading texture at: " << path << std::endl;
		textures[path] = texture;
		return texture;
	}
}