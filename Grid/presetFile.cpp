#include "presetFile.h"


//http://stackoverflow.com/questions/7868936/read-file-line-by-line

std::vector<SDL_Point> parseFile(std::string filename) {
	std::ifstream inFile(filename);

	std::vector<SDL_Point> v;

	if (inFile) {
		std::copy(
			std::istream_iterator<SDL_Point>(inFile),
			std::istream_iterator<SDL_Point>(),
			std::back_inserter(v)
		);
	}
	else {
		std::cout << "Couldn't open " << filename << " for reading\n";
	}
	return v;
}

void writeFile(std::string filename, std::vector<SDL_Point> inputCoords) {
	std::ofstream outFile(filename);


	if (outFile) {
		for (SDL_Point coord : inputCoords) {
			outFile << coord.x << " " << coord.y << std::endl;
		}
	} else {
		std::cout << "Couldn't open " << filename << " for writing\n";
	}
}

std::istream &operator >> (std::istream& is, SDL_Point& coordinates)
{
	is >> coordinates.x >> coordinates.y;

	return is;
}


void loadPreset(std::string filename, int** grid1, int** grid2) {
	std::vector<SDL_Point> presetFileCoords = parseFile(filename);
	if (!presetFileCoords.empty()) {
		// clear existing grid:
		for (int j = 0; j < gridSizeY; j++) {
			for (int i = 0; i < gridSizeX; i++) {
				grid1[i][j] = 0;
				grid2[i][j] = 0;
			}
		}

		//set grid from coords read file
		for (SDL_Point coord : presetFileCoords) {
			if ((-1 < coord.x) && (coord.x < gridSizeX) && (-1 < coord.y) && (coord.y < gridSizeY)) {
				// valid
				grid1[coord.x][coord.y] = 1;
				grid2[coord.x][coord.y] = 1;
				std::cout << "setting coord: " << coord.x << ", " << coord.y << std::endl;
			}
			else {
				std::cout << "Invalid Coordinate in file: " << coord.x << ", " << coord.y << std::endl;
			}
		}
	} else {
		std::cout << "Grid not set from preset. File blank or invalid\n";
	}
}

void savePreset(std::string filename, int** grid) {
	// generate a vector of of the coords for 1s
	std::vector<SDL_Point> setCoords;
	for (int j = 0; j < gridSizeY; j++) {
		for (int i = 0; i < gridSizeX; i++) {
			if (grid[i][j] == 1) {
				setCoords.push_back(SDL_Point{ i, j });
			}
		}
	}
	writeFile(filename, setCoords);
}