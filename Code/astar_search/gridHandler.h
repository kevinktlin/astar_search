/*
 * gridHandler.h
 *
 *  Created on: Mar 30, 2017
 *      Author: kingo
 */

#ifndef GRIDHANDLER_H_
#define GRIDHANDLER_H_

#include <vector>
#include <cstdlib>
#include "miscType.h"

class gridHandler {
private:
	std::vector< std::vector<char> > grid;
	int height;
	int width;
	std::vector<Location> directions;
public:
	gridHandler(void);
	void setGrid(std::vector< std::vector<char> > gridInput, int widthInput, int heightInput) {
		grid = gridInput;
		width = widthInput;
		height = heightInput;
	}
	std::vector<Location> neighbors(Location loc, int hummerUsage, const int maxHummerNum);
	bool inBound(Location loc) {
		if(loc.x <= width - 1 && loc.x >= 0 && loc.y <= height - 1 && loc.y >= 0)	return true;
		else	return false;
	}

	bool goable(Location loc) {
		if(grid[loc.y][loc.x] == '1')	return true;
		else	return false;
	}


	double cost(Location start, Location end) {
		if(abs(start.x - end.x) == 1 &&	abs(start.y - end.y) == 1)	return 1.414; // diagnal
		else return 1;
	}


	void drawGrid(void);
	void drawPath(std::vector<Location> travelPath, Location start, Location terminal);
	void changeGrid(Location loc, char ch) {
		grid[loc.y][loc.x] = ch;
	}

	int getWidth(void)	{	return width;	}
	int getHeight(void)	{	return height;	}
};




#endif /* GRIDHANDLER_H_ */
