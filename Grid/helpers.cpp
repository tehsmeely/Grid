#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

void logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " error: " << SDL_GetError() << std::endl;

}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr)
		logSDLError(std::cout, "LoadTexture");
	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}



/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @param font The open font file, use if running this funct often
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* renderText(const std::string &message,
	SDL_Color color, int fontSize, SDL_Renderer *renderer, TTF_Font *font)
{
	if (font == NULL) { std::cout << "Font is NULL" << std::endl; return nullptr; }
	if (font == nullptr) { std::cout << "Font is nullptr" << std::endl; return nullptr; }
	//std::cout << message << std::endl;
	//const char* cmessage = message.c_str();
	//std::cout << cmessage << std::endl;
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	//TTF_CloseFont(font);
	return texture;
}


/**
* Render the message we want to display to a texture for drawing
* @param renderer Renderer
* @param masterTex Master texture, or 'background' texture
* @param blitTex Texture to be blitted
* @param x x pos
* @param y ypos
* @param middle int(boolean) true for middle (x,y) false for topleft (x, y)
* @return An SDL_Texture containing the master texture with the blit texture blitted on
*/
/*SDL_Texture*/ void textureBlit(SDL_Renderer* renderer, SDL_Texture *masterTex, SDL_Texture *blitTex, int x, int y, int middle)
{
	SDL_SetRenderTarget(renderer, masterTex); // draw to our sidebox texture
	if (middle)
	{
		int iW, iH;
		SDL_QueryTexture(blitTex, NULL, NULL, &iW, &iH);
		x -= iW / 2;
		y -= iH / 2;
	}
	renderTexture(blitTex, renderer, x, y);
	SDL_SetRenderTarget(renderer, NULL); // back to default target
	//return 
}



// Rect operations

/**
* Returns true if point is in rect, false if outside
* @param point
* @param rect
*/
bool collidepoint(SDL_Point point, SDL_Rect rect)
{
	return (rect.x < point.x && point.x < rect.x + rect.w
		&& rect.y < point.y && point.y < rect.y + rect.h);
}