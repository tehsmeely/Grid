#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "globals.h"
#include "ImageButton.h"
#include "helpers.h"

class UIBar
{
protected:
	SDL_Texture* background;
	TTF_Font* font;
	std::vector<ImageButton*> buttons;
	
public:
	int stepNumber;
	SDL_Rect rect;
	ImageButton *PlayButton, *OneStepButton, *QuitButton;

	UIBar(SDL_Rect irect, SDL_Renderer* ren);
	~UIBar();
	void Draw(SDL_Renderer*);

	int Click(SDL_Point);
	void Hover(SDL_Point);
	void Step() { stepNumber++; }

	SDL_Rect GetRect() { return rect; }
};