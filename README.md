# Grid
A Game of Life clone. Using SDL2 for the graphical display / window management



## Game of Life
You know Game of Life, a frankly fascinating simulation on a grid using the "B3/S23" system to replicate/destroy cells on the grid.

https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life



## About This
The graphical representation is handled using SDL2 (https://www.libsdl.org/)
with support from [SDL_Image](https://www.libsdl.org/projects/SDL_image/) and [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)

The game of life is implemented using the "standard" algorithm - two full-sized 2D int grids are maintained, with 0 and 1 to represents death and life in a cell
To implement a step/phase/cycle, one grid is read to and the other written two. Each step these swap over.
The edges of the grid are hardcoded to death.
There is also an arbitrary pause of 20ms between frames, to keep steps ticking along at a good rate for the user - but with buttons on the GUI to eitrher manually step or to run unlimited

