#include <iostream>
#include <windows.h>
#include <SDL.h>
//#include <SDL_ttf.h>
#include "globals.h"
#include "helpers.h"
#include "uiBar.h"
#include "presetFile.h"


// Function Declares
SDL_Texture* drawBackgroundGrid(SDL_Renderer*);
void drawGrid(SDL_Renderer*, int**, SDL_Texture*);
void printArray(int**, int, int);
void resetGrids(int**, int**);
void update(int***, int***);
int getGridVal(int**, int, int);
int getNeighbours(int**, int, int);
void drawText(SDL_Renderer*, SDL_Texture*);

SDL_Color black = { 0, 0, 0, 255 };

int main(int argc, char **argv){
	//Init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){ logSDLError(std::cout, "SDL_Init"); return 1; }
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){ logSDLError(std::cout, "IMG_Init"); return 1; }
	if (TTF_Init() != 0){ logSDLError(std::cout, "TTF_Init"); return 1;}


	// Debug output setup
	FILE* consoleOut;
	AllocConsole();
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);

	// get window
	SDL_Window *window = SDL_CreateWindow("Grid - Game of Life", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){ logSDLError(std::cout, "SDL_CreateWindow"); return 1; }
	
	// get renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){ logSDLError(std::cout, "SDL_CreateRenderer"); return 1; }

	// This texture gets rendered to as our grid in drawGrid
	SDL_Texture *texGrid = drawBackgroundGrid(renderer);


	// Window for presets loading
	int msgBoxButtonID;
	const SDL_MessageBoxButtonData loadMsgBox_Buttons[] = {
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Load (1)" },
		{ 0, 1, "Load (2)" },
		{ 0, 2, "Load (3)" },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 3, "Cancel" },
	};
	const SDL_MessageBoxButtonData saveMsgBox_Buttons[] = {
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Save (1)" },
		{ 0, 1, "Save (2)" },
		{ 0, 2, "Save (3)" },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 3, "Cancel" },
	};
	std::string filenameMappings[] = {
		"../resource/gridPreset1.ssv",
		"../resource/gridPreset2.ssv",
		"../resource/gridPreset3.ssv",
	};

	const SDL_MessageBoxColorScheme loadMSgBox_ColourScheme = {
		{ /* .colors (.r, .g, .b) */
		  /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
			{ 255,   0,   0 },
			/* [SDL_MESSAGEBOX_COLOR_TEXT] */
			{ 0, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
			{ 255, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
			{ 0,   0, 255 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
			{ 255,   0, 255 }
		}
	};
	const SDL_MessageBoxData loadMsgBox_Data = {
		SDL_MESSAGEBOX_INFORMATION, /* .flags */
		NULL, /* .window */
		"Select Preset File", /* .title */
		"Select a slot to load from:", /* .message */
		SDL_arraysize(loadMsgBox_Buttons), /* .numbuttons */
		loadMsgBox_Buttons, /* .buttons */
		&loadMSgBox_ColourScheme /* .colorScheme */
	};
	const SDL_MessageBoxData saveMsgBox_Data = {
		SDL_MESSAGEBOX_INFORMATION, /* .flags */
		NULL, /* .window */
		"Select Preset File", /* .title */
		"Select a slot to save to:", /* .message */
		SDL_arraysize(saveMsgBox_Buttons), /* .numbuttons */
		saveMsgBox_Buttons, /* .buttons */
		&loadMSgBox_ColourScheme /* .colorScheme */
	};




	// Load image and set it as our window icon
	SDL_Surface *windowIcon;
	windowIcon = IMG_Load("../resource/windowIcon.png");
	if (windowIcon) {
		SDL_SetWindowIcon(window, windowIcon);
		SDL_FreeSurface(windowIcon);
	} else {
		std::cout << "Could not load window icon" << std::endl;
	}
	
	// Load ui bar
	SDL_Rect uiBarRect = { SCREEN_WIDTH - 199, 0, 200, SCREEN_HEIGHT };
	UIBar* uiBar = new UIBar(uiBarRect, renderer);

	// boolean switch for unlimiting the update time
	int timeUnlimited = 0;


	//int initalise the grid pair
	int **grid1;
	grid1 = (int **)calloc(gridSizeX, sizeof(int));
	for (int i = 0; i < gridSizeX; i++)
		grid1[i] = (int*)calloc(gridSizeY, sizeof(int));
	int **grid2;
	grid2 = (int **)calloc(gridSizeX, sizeof(int));
	for (int i = 0; i < gridSizeX; i++)
		grid2[i] = (int*)calloc(gridSizeY, sizeof(int));
	int*** activeGrid = &grid1;
	int*** inactiveGrid = &grid2;
	drawGrid(renderer, *activeGrid, texGrid);


	//loop vars
	int mouseX, mouseY, quit = 0, running = 0;
	Uint32 lastTime = 0, time;
	SDL_Event e;


	while (!quit)
	{
		// Running=True enables updates
		if (running)
		{	// Check if it has been 1*UPDATE_DELAY since last update, if so, update
			time = SDL_GetTicks();
			if ((timeUnlimited) || (time > lastTime + UPDATE_DELAY))
			{ // UPDATE_DELAY ms has passed, update, and set last time to this time
				std::cout << "Updating" << std::endl;
				update(activeGrid, inactiveGrid);
				drawGrid(renderer, *activeGrid, texGrid);
				lastTime = time;
				uiBar->Step();
			}
		}

		// Event Handling
		while (SDL_PollEvent(&e)){
			//If user closes the window
			if (e.type == SDL_QUIT)
				quit = 1;

			//If user clicks the mouse
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&mouseX, &mouseY);
				std::cout << mouseX << ", " << mouseY << std::endl;
				std::cout << mouseX / GRID_SIZE << ", " << mouseY / GRID_SIZE << std::endl;
				if (mouseX < GRID_WIDTH)
				{
					(*activeGrid)[mouseX / GRID_SIZE][mouseY / GRID_SIZE] = !(*activeGrid)[mouseX / GRID_SIZE][mouseY / GRID_SIZE];
					drawGrid(renderer, *activeGrid, texGrid);
				} else {
					int response = uiBar->Click(SDL_Point{ mouseX, mouseY });
					if (response >= 0) { // response is: 0 - button0_off, 1 - button0_on, 2 - button1_on, 3 - button1_off, ...
						switch (response) {
							// 0: Run Button
							case 0: // stop run
								running = 0;
								std::cout << "Running: " << running << std::endl;
								uiBar->OneStepButton->Activate();
								break;
							case 1: // start run
								running = 1;
								std::cout << "Running: " << running << std::endl;
								uiBar->OneStepButton->Deactivate();
								break;
							// 1: One Step Button
							case 2: // stop onestep - nothing
								break;
							case 3:
								update(activeGrid, inactiveGrid);
								drawGrid(renderer, *activeGrid, texGrid);
								uiBar->Click(SDL_Point{ mouseX, mouseY });
								uiBar->Step();
								break;
							// 2: Quit Button
							case 4:
								// "quit" off - pass;
								break;
							case 5:
								quit = 1;
								break;
							// 3: Unlimit Button
							case 6:
								timeUnlimited = 0;
								break;
							case 7:
								timeUnlimited = 1;
								break;
							// 4: Load Button
							case 8:
								// "load" off - pass;
								break;
							case 9:
								if (SDL_ShowMessageBox(&loadMsgBox_Data, &msgBoxButtonID) < 0) {
									SDL_Log("error displaying message box");
									return 1;
								}
								if (msgBoxButtonID == -1) {
									SDL_Log("no selection");
								}
								else {
									std::string filename = filenameMappings[msgBoxButtonID];
									loadPreset(filename, grid1, grid2);
									drawGrid(renderer, *activeGrid, texGrid);
								}
								uiBar->Click(SDL_Point{ mouseX, mouseY });
								break;
							// 3: Save Button
							case 10:
								break;
							case 11:
								if (SDL_ShowMessageBox(&saveMsgBox_Data, &msgBoxButtonID) < 0) {
									SDL_Log("error displaying message box");
								} else {
									if (msgBoxButtonID == -1) {
										SDL_Log("no selection");
									} else {
										std::string filename = filenameMappings[msgBoxButtonID];
										savePreset(filename, *activeGrid);
									}
									uiBar->Click(SDL_Point{ mouseX, mouseY });
								}
								break;
								// 4: Reset Button
							case 12:
								break;
							case 13:
								resetGrids(grid1, grid2);
								drawGrid(renderer, *activeGrid, texGrid);
								uiBar->ResetStep();
								uiBar->Click(SDL_Point{ mouseX, mouseY });
								break;
						}
					}
				}
			}
			if (e.type == SDL_KEYDOWN)
			{
				SDL_Keycode keySymbol = e.key.keysym.sym;
				switch (keySymbol)
				{
					case SDLK_RETURN: // Enter Key, start/stop running the simulation!
						running = !running;
						uiBar->forcePlayButtonState(running);
						std::cout << "Running: " << running << std::endl;
						break;
					case SDLK_SPACE: // Space Key, draw grid in console
						update(activeGrid, inactiveGrid);
						drawGrid(renderer, *activeGrid, texGrid);
						uiBar->Click(SDL_Point{ mouseX, mouseY });
						uiBar->Step();
						break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				//On mouse move, if new location is in UI, tell UI so it can check if buttons are hovered over
				SDL_Point newPos = { e.motion.x, e.motion.y };
				//std::cout << "MousePos: ( " + std::to_string(newPos.x) + ", " + std::to_string(newPos.y) + " );" << std::endl;
				if (collidepoint(newPos, uiBar->GetRect())) uiBar->Hover(newPos);
			}
		}
		uiBar->Draw(renderer);

		SDL_RenderPresent(renderer);


	}
	for (int i = 0; i < gridSizeX; i++)
	{
		free(grid1[i]);
		free(grid2[i]);
	}
	free(grid1);
	free(grid2);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
	return 0;

}


void update(int*** activeGrid, int*** inactiveGrid)
{
	//int** aGrid = *activeGrid;
	//int** iGrid = *inactiveGrid;
	//apply logic on activeGrid, storing in inactiveGrid. Then flip and return
	int neighbourNum;
	for (int y=0; y<gridSizeY; y++)
	{
		for (int x=0; x<gridSizeX; x++)
		{
			neighbourNum = getNeighbours((*activeGrid), x, y);
			if ((*activeGrid)[x][y])
			{
				//std::cout << x << "," << y << "  n:" << neighbourNum << std::endl;
				if (neighbourNum < 2 || neighbourNum > 3) (*inactiveGrid)[x][y] = 0;
				else { (*inactiveGrid)[x][y] = 1; }
			} else {
				if (neighbourNum == 3) (*inactiveGrid)[x][y] = 1;
				else (*inactiveGrid)[x][y] = 0;
			}
		}
	}
	// Flip pointers
	int** tmpPtr;
	tmpPtr = *activeGrid;
	*activeGrid = *inactiveGrid;
	*inactiveGrid = tmpPtr; 
}

int getNeighbours(int** grid, int x, int y)
{
	int n = 0;

	n += getGridVal(grid, x - 1, y - 1);
	n += getGridVal(grid, x, y - 1);
	n += getGridVal(grid, x + 1, y - 1);
	n += getGridVal(grid, x - 1, y);
	n += getGridVal(grid, x + 1, y);
	n += getGridVal(grid, x - 1, y + 1);
	n += getGridVal(grid, x, y + 1);
	n += getGridVal(grid, x + 1, y + 1);

	return n;
}

int getGridVal(int** grid, int x, int y)
{
	if (x <= -1 || x >= gridSizeX || y <= -1 || y >= gridSizeY)
		return BORDER_VAL;
	else
		return grid[x][y];
}

void printArray(int** arr,int x,int y)
{
	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++)
		{
			std::cout << arr[i][j];
		}
		std::cout << std::endl;
	}
}

void resetGrids(int **grid1, int **grid2) {
	for (int rowY = 0; rowY < gridSizeY; rowY++) {
		for (int rowX = 0; rowX < gridSizeX; rowX++) {
			grid1[rowX][rowY] = 0;
			grid2[rowX][rowY] = 0;
		}
	}
}


void drawGrid(SDL_Renderer* renderer, int** grid, SDL_Texture* texGrid)
{
	
	//cover with blank grid, then draw filled rects
	renderTexture(texGrid, renderer, 0, 0);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Draw colour set to black
	SDL_Rect gridRect;
	gridRect.w = GRID_SIZE;
	gridRect.h = GRID_SIZE;

	for (int rowY = 0; rowY < gridSizeY; rowY++)
	{ 
		for (int rowX = 0; rowX < gridSizeX; rowX++)
		{
			if (grid[rowX][rowY])
			{
				gridRect.x = rowX * GRID_SIZE;
				gridRect.y = rowY * GRID_SIZE;
				SDL_RenderFillRect(renderer, &gridRect);
			}
		}
	}
	//SDL_RenderPresent(renderer);
}

void drawText(SDL_Renderer* renderer, SDL_Texture* textBox)
{
	renderTexture(textBox, renderer, GRID_WIDTH+1, 0);
	SDL_RenderPresent(renderer);
}

SDL_Texture* drawBackgroundGrid(SDL_Renderer* renderer)
{
	SDL_Texture *texGrid = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, GRID_WIDTH+1, GRID_HEIGHT);
	SDL_SetRenderTarget(renderer, texGrid); // draw to our grid texture
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 0; i < GRID_WIDTH / GRID_SIZE + 1; i++)
	{
		int x = i * GRID_SIZE;
		int output = SDL_RenderDrawLine(renderer, x, 0, x, GRID_HEIGHT);
		//std::cout << output << std::endl;
	}
	for (int j = 0; j < GRID_HEIGHT / GRID_SIZE+1; j++)
	{
		int y = j * GRID_SIZE;
		SDL_RenderDrawLine(renderer, 0, y, GRID_WIDTH, y);
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderTarget(renderer, NULL); // back to default, not texGrid anymore
	return texGrid;
}


//SDL_Texture* drawTextBox(SDL_Renderer* renderer, TTF_Font* font)
//{
//	std::cout << "DEPRECATED CALL TO drawTextBox. wuttt??" << std::endl;
//	int centreX = /*GRID_WIDTH + */((SCREEN_WIDTH - GRID_WIDTH) / 2),
//		centreY = SCREEN_HEIGHT / 2;
//	//std::cout << ""
//	SDL_Texture *textBox = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH - GRID_WIDTH-1, SCREEN_HEIGHT);
//	SDL_SetRenderTarget(renderer, textBox); // draw to our sidebox texture
//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//	SDL_RenderClear(renderer);
//	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//	SDL_Texture* titleText = renderText("Game of Life!", black, 22, renderer, font);
//	if (titleText == nullptr){ std::cout << "Text make not successful" << std::endl;}
//	int tW, tH, x, y;
//	SDL_QueryTexture(titleText, NULL, NULL, &tW, &tH);
//	x = centreX - tW / 2;
//	y = 60 - tH / 2;
//	renderTexture(titleText, renderer, x, y);
//
//
//
//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//	SDL_SetRenderTarget(renderer, NULL); // back to default, not texGrid anymore
//	return textBox;
//}

