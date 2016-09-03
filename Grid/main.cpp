#include <iostream>
#include <windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "globals.h"
#include "helpers.h"
#include "uiBar.h"

// Variable/constant Declares
/*
const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 981; // Min GRID_HEIGHT + 1 to allow for bottom line
const int GRID_WIDTH = 1200;
const int GRID_HEIGHT = 980;
const int GRID_SIZE = 10;
const Uint32 UPDATE_DELAY = 200; //in ms
*/
int BORDER_VAL = 0;
int TIME_UNLIMIT = 1;
int gridSizeX = GRID_WIDTH / GRID_SIZE;
int gridSizeY = GRID_HEIGHT / GRID_SIZE;


// Function Declares
SDL_Texture* drawBackgroundGrid(SDL_Renderer*);
void drawGrid(SDL_Renderer*, int**, SDL_Texture*);
void printArray(int**, int, int);
void update(int***, int***);
int getGridVal(int**, int, int);
int getNeighbours(int**, int, int);
void drawText(SDL_Renderer*, SDL_Texture*);
SDL_Texture* drawTextBox(SDL_Renderer*, TTF_Font*);

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


	//
	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
	//	"Starting",
	//	"Starting - Test",
	//	window);
	//

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

	//int grid[gridSizeX][gridSizeY] = { 0 };
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
	int mouseX, mouseY, quit = 0, running = 0;
	Uint32 lastTime = 0, time;

	
	/*
	std::cout << "making test text" << std::endl;
	//SDL_Texture* testText = renderText("test", black, 22, renderer, font);
	SDL_Texture* testText = renderText("test", "../res/sample.ttf", black, 22, renderer);
	if (testText == nullptr){ std::cout << "Text make not successful" << std::endl; return 1; }
	// SDL_Texture* titleText = renderText("Game of Life!", black, 22, renderer, font);
	textureBlit(renderer, textBox, testText, 100, 200, 1);
	*/
	
	//drawText(renderer, textBox);

	SDL_Event e;
	while (!quit)
	{
		// Running=True enables updates
		if (running)
		{	// Check if it has been 1*UPDATE_DELAY since last update, if so, update
			time = SDL_GetTicks();
			if ((TIME_UNLIMIT) || (time > lastTime + UPDATE_DELAY))
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
							case 0: // stop run
								running = 0;
								std::cout << "Running: " << running << std::endl;
								break;
								break;
							case 1: // start run
								running = 1;
								std::cout << "Running: " << running << std::endl;
								break;
								break;
							case 2: // stop onestep - nothing
								break;
							case 3:
								update(activeGrid, inactiveGrid);
								drawGrid(renderer, *activeGrid, texGrid);
								uiBar->Click(SDL_Point{ mouseX, mouseY });
								uiBar->Step();
								break;
							case 4:
								break;
							case 5:
								quit = 1;
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
						std::cout << "Running: " << running << std::endl;
						break;
					case SDLK_SPACE: // Space Key, draw grid in console
						for (int rowY = 0; rowY < gridSizeY; rowY++)
						{
							for (int rowX = 0; rowX < gridSizeX; rowX++)
							{
								std::cout << (*activeGrid)[rowX][rowY] << " ";
							}
							std::cout << std::endl;
						}
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
	/*
	*activeGrid = iGrid;
	*inactiveGrid = aGrid;
	
	printArray(aGrid, gridSizeX, gridSizeY);
	std::cout << std::endl;
	
	printArray(*activeGrid, gridSizeX, gridSizeY);
	*/
	int** tmpPtr;
	tmpPtr = *activeGrid;
	*activeGrid = *inactiveGrid;
	*inactiveGrid = tmpPtr; 
	/*
	std::cout << "Active Grid [0][1]: " << activeGrid[0][1] << std::endl;
	std::cout << "Inactive Grid [0][1]: " << inactiveGrid[0][1] << std::endl; */
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
	if (x == -1 || x == gridSizeX || y == -1 || y == gridSizeY)
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


SDL_Texture* drawTextBox(SDL_Renderer* renderer, TTF_Font* font)
{
	std::cout << "DEPRECATED CALL TO drawTextBox. wuttt??" << std::endl;
	int centreX = /*GRID_WIDTH + */((SCREEN_WIDTH - GRID_WIDTH) / 2),
		centreY = SCREEN_HEIGHT / 2;
	//std::cout << ""
	SDL_Texture *textBox = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH - GRID_WIDTH-1, SCREEN_HEIGHT);
	SDL_SetRenderTarget(renderer, textBox); // draw to our sidebox texture
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_Texture* titleText = renderText("Game of Life!", black, 22, renderer, font);
	if (titleText == nullptr){ std::cout << "Text make not successful" << std::endl;}
	int tW, tH, x, y;
	SDL_QueryTexture(titleText, NULL, NULL, &tW, &tH);
	x = centreX - tW / 2;
	y = 60 - tH / 2;
	renderTexture(titleText, renderer, x, y);



	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderTarget(renderer, NULL); // back to default, not texGrid anymore
	return textBox;
}