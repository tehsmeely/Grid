#include "uiBar.h"
#include <iostream>

/*******************************************************************************************************************/

// UIBar

/*******************************************************************************************************************/


UIBar::UIBar(SDL_Rect irect, SDL_Renderer* ren)
{
	rect = irect;
	stepNumber = 0;

	int centreX = rect.w / 2,
		centreY = rect.h / 2;

	background = SDL_CreateTexture(ren, 0, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);


	SDL_Rect buttonRects = { 0, 0, 100, 40 };
	SDL_Rect resetButtonRect = { 0, 0, 80, 30 };
	PlayButton = new ImageButton(
		SDL_Point{ centreX - buttonRects.w / 2, 70 }, ren, "../resource/runButton.png", buttonRects, 1, 0
	);
	OneStepButton = new ImageButton(
		SDL_Point{ centreX - buttonRects.w / 2, 130 }, ren, "../resource/oneStepButton.png", buttonRects, 3, 2
	);
	QuitButton = new ImageButton(
		SDL_Point{ centreX - buttonRects.w / 2, rect.h - 50 }, ren, "../resource/quitButton.png", buttonRects, 5, 4
	);
	UnlimitStepsButton = new ImageButton(
		SDL_Point{ centreX - buttonRects.w / 2, rect.h - 110 }, ren, "../resource/unLimitButton.png", buttonRects, 7, 6
	);
	LoadButton = new ImageButton(
		SDL_Point{ centreX - buttonRects.w / 2, rect.h /2 + 20 }, ren, "../resource/loadButton.png", buttonRects, 9, 8
	);
	SaveButton = new ImageButton(
		SDL_Point{ centreX - buttonRects.w / 2, rect.h / 2 - (buttonRects.h + 20) }, ren, "../resource/saveButton.png", buttonRects, 11, 10
	);
	ResetButton = new ImageButton(
		SDL_Point{ centreX - resetButtonRect.w / 2, 210 }, ren, "../resource/resetButton.png", resetButtonRect, 13, 12
	);

	font = TTF_OpenFont("../resource/sample.ttf", 22);
	if (font == nullptr) {
		logSDLError(std::cout, "TTF_OpenFont");
		std::cout << "UIBar Font load Failed!" << std::endl;
	}

	buttons.push_back(PlayButton);
	buttons.push_back(OneStepButton);
	buttons.push_back(QuitButton);
	buttons.push_back(UnlimitStepsButton);
	buttons.push_back(LoadButton);
	buttons.push_back(SaveButton);
	buttons.push_back(ResetButton);
}

UIBar::~UIBar() {
	buttons.clear();

}


void UIBar::Draw(SDL_Renderer* ren)
{
	int centreX = rect.w / 2,
		centreY = rect.h / 2;
	SDL_Color black = { 0, 0, 0, 255 };
	//Clear 
	SDL_SetRenderTarget(ren, background); // draw to our background texture
	SDL_SetRenderDrawColor(ren, 205, 205, 205, 255);
	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

	//generate texts and draw
	int tW, tH, x, y;
	SDL_Texture* titleText = renderText("Game of Life!", black, 22, ren, font);	
	if (titleText == nullptr) { 
		std::cout << "Text make not successful" << std::endl;
	} else {
		SDL_QueryTexture(titleText, NULL, NULL, &tW, &tH);
		x = centreX - tW / 2;
		y = 20;
		renderTexture(titleText, ren, x, y);
	}
	SDL_Texture* stepNumberText = renderText("Step: "+ std::to_string(stepNumber), black, 22, ren, font);
	if (stepNumberText == nullptr) {
		std::cout << "Text make not successful" << std::endl;
	} else {
		SDL_QueryTexture(stepNumberText, NULL, NULL, &tW, &tH);
		x = centreX - tW / 2;
		y = 180;
		renderTexture(stepNumberText, ren, x, y);
	}
	SDL_DestroyTexture(titleText);
	SDL_DestroyTexture(stepNumberText);

	//draw buttons
	for (ImageButton* b : buttons)
	{
		b->Draw(ren);
	}



	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_SetRenderTarget(ren, NULL); // back to default, not texGrid anymore


	SDL_RenderCopy(ren, background, NULL, &rect);
}

int UIBar::Click(SDL_Point pos)
{
	SDL_Point localPos = { pos.x - rect.x, pos.y - rect.y };
	int responseVal = -1;
	for (ImageButton* b : buttons)
	{
		if (collidepoint(localPos, b->GetRect()))
		{
			responseVal = b->Click();
		}
	}
	return responseVal;
}

void UIBar::Hover(SDL_Point pos)
{
	SDL_Point localPos = { pos.x - rect.x, pos.y - rect.y };

	for (ImageButton* b : buttons)
	{
		if (collidepoint(localPos, b->GetRect()))
		{
			b->HoverOn();
		}
		else
		{
			b->HoverOff();
		}
	}
}

void UIBar::forcePlayButtonState(int running)
{
	if ((running && PlayButton->state < 2) || (!running && PlayButton->state == 2)) {
		//button is inactive/hovered but its running
		PlayButton->Click();
	}
}