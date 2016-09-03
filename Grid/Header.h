#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef HELPERS_H
#define HELPERS_H

void renderTexture(SDL_Texture*, SDL_Renderer*, int, int, int, int);
void renderTexture(SDL_Texture*, SDL_Renderer*, int, int);
SDL_Texture* loadTexture(const std::string&, SDL_Renderer*);
void logSDLError(std::ostream&, const std::string&);
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer);
SDL_Texture* renderText(const std::string &message,
	SDL_Color color, int fontSize, SDL_Renderer *renderer, TTF_Font *font);

#endif
