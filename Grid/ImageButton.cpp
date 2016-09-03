#include "ImageButton.h"
#include "paths.h"
#include "helpers.h"

/*******************************************************************************************************************/

// ImageButton

/*******************************************************************************************************************/


ImageButton::ImageButton(SDL_Point topleft, SDL_Renderer* ren, std::string imagepath, SDL_Rect buttonSize, int inOnValue, int inOffValue)
{
	onValue = inOnValue;
	offValue = inOffValue;
	rect = { topleft.x, topleft.y, buttonSize.w, buttonSize.h};
	srcRect = { 0, 0, buttonSize.w, buttonSize.h };
	state = 0;
	image = textureManager.loadImage(ren, imagepath);
}
ImageButton::ImageButton(SDL_Point topleft, SDL_Renderer* ren, std::string imagepath, SDL_Rect buttonSize, int inOnValue)
{
	onValue = inOnValue;
	offValue = 0;
	rect = { topleft.x, topleft.y, buttonSize.w, buttonSize.h };
	srcRect = { 0, 0, buttonSize.w, buttonSize.h };
	state = 0;
	image = textureManager.loadImage(ren, imagepath);
}



ImageButton::~ImageButton()
{
}

void ImageButton::Draw(SDL_Renderer* ren)
{
	//std::cout << "D ";
	SDL_RenderCopy(ren, image, &srcRect, &rect);
}

int ImageButton::Click()
{
	int ret;
	std::cout << "Button clicked! " << ImageButton::state << std::endl;
	if (state == 0 || state == 1) // inactive or hover
	{
		std::cout << "button was in inactive or hover state" << std::endl;
		state = 2;
		ret = onValue;
	}
	else
	{
		std::cout << "button was in clicked state" << std::endl;
		state = 0;
		ret = offValue;
	}
	srcRect.x = state * rect.w;
	std::cout << srcRect.x << ", " << srcRect.y << ", " << srcRect.w << ", " << srcRect.h << std::endl;
	std::cout << ImageButton::state << std::endl;
	return ret;
}

void ImageButton::HoverOn()
{
	if (state==0)
	{
		state = 1;
		srcRect.x = state * rect.w;
	}
}
void ImageButton::HoverOff()
{
	if (state == 1)
	{
		state = 0;
		srcRect.x = state * rect.w;
	}
}

/*******************************************************************************************************************/

// GangedImageButtons

/*******************************************************************************************************************/
//#include "Paths.h"
//#include "SDLhelpers.h"
/*

GangedImageButton::GangedImageButton(SDL_Point topleft, std::string basepath, std::string imagenames[], int values[], int buttonNum, SDL_Renderer* ren)
{

	rect = { topleft.x, topleft.y, 40, (50 * buttonNum) - 10};
	for (int i = 0; i < buttonNum; i++)
	{
		buttons.push_back(new ImageButton(topleft, ren, pathAppend(basepath, imagenames[i]), values[i]));
		topleft.y += 50;
	}
	activeButton = buttons[0];
	activeButton->Click();


}

GangedImageButton::~GangedImageButton()
{

}

int GangedImageButton::Click(SDL_Point pos)
{
	for (ImageButton* button : buttons)
	{
		if (collidepoint(pos, button->GetRect()))
		{
			int response = button->Click();
			if (response != 0) //Button clicked on (not off)
			{
				activeButton->Unclick();
				activeButton = button;
			}
			value = response;
		}
	}
	return value;
}

int GangedImageButton::GetValue()
{
	return value;
}

void GangedImageButton::Draw(SDL_Renderer* ren)
{
	for (ImageButton* b : buttons)
	{
		b->Draw(ren);
	}
}

void GangedImageButton::Hover(SDL_Point pos)
{
	for (ImageButton* b : buttons)
	{
		if (collidepoint(pos, b->GetRect()))
		{
			b->HoverOn();
		}
		else
		{
			b->HoverOff();
		}
	}
}
*/