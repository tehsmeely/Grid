#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "globals.h"

class ImageButton
{
protected:
	SDL_Texture* image;
	int onValue, offValue;
public:
	SDL_Rect rect, srcRect;
	int state; // 0-inactive, 1-hover, 2-clicked
	ImageButton(SDL_Point, SDL_Renderer*, std::string, SDL_Rect, int);
	ImageButton(SDL_Point, SDL_Renderer*, std::string, SDL_Rect, int, int);
	~ImageButton();
	void Draw(SDL_Renderer*);
	
	int Click();
	void HoverOn();
	void HoverOff();
	void Unclick() { state = 0; srcRect.x = 0; }

	SDL_Rect GetRect() { return rect; }
};



class GangedImageButton
{
protected:
	std::vector<ImageButton*> buttons;
	ImageButton* activeButton;
	SDL_Rect rect;
	int value;
public:
	GangedImageButton(SDL_Point, std::string, std::string[], int[], int, SDL_Renderer*);
	~GangedImageButton();
	int Click(SDL_Point);
	int GetValue();
	void Draw(SDL_Renderer*);
	SDL_Rect GetRect() { return rect; }
	void Hover(SDL_Point);
};
