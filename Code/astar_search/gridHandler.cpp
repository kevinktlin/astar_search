/*
 * gridHandler.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kingo
 */


#include <iostream>
#include <vector>
#include "gridHandler.h"
#include "miscType.h"

struct Location;

gridHandler::gridHandler(void) {
	height = 0;
	width = 0;

	directions.resize(8);
	directions[0].x = -1;
	directions[0].y = -1;

	directions[1].x = -1;
	directions[1].y = 0;

	directions[2].x = -1;
	directions[2].y = 1;

	directions[3].x = 0;
	directions[3].y = -1;

	directions[4].x = 0;
	directions[4].y = 1;

	directions[5].x = 1;
	directions[5].y = -1;

	directions[6].x = 1;
	directions[6].y = 0;

	directions[7].x = 1;
	directions[7].y = 1;
}

std::vector<Location> gridHandler::neighbors(Location loc, int hummerUsage, const int maxHummerNum) {
	std::vector<Location> neighbors;
	Location nextMove;

#if defined DEBUG
	//cout << "neighbors vector size: " << directions.size() << endl;
#endif

	for (auto dir : directions) {
		nextMove.x = loc.x + ((Location)dir).x;
		nextMove.y = loc.y + ((Location)dir).y;

#if defined DEBUG
//		cout  << "next move:" << nextMove << endl;
#endif

		if (inBound(nextMove)) {
#if defined DEBUG
//			cout  << "entering move:" << nextMove << endl;
#endif
			if(hummerUsage < maxHummerNum)
				neighbors.push_back(nextMove);
			else{
				if(goable(nextMove))	neighbors.push_back(nextMove);
			}
		}
	}

#if defined DEBUG
/*	for(int i = 0 ; i < neighbors.size(); i++)
		cout << "neighbors " << i <<':'<< neighbors[i] << endl;*/
#endif


	return neighbors;
}

void gridHandler::drawGrid(void) {
	std::cout << "Grid: " << std::endl;
	for(int i =0 ; i < grid.size(); i++) {
		for(int j = 0 ; j < grid[i].size(); j++)
			std::cout << grid[i][j] << ' ';
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void gridHandler::drawPath(std::vector<Location> travelPath, Location start, Location terminal) {

	if(!travelPath.size())	{
		std::cout << "no valid path" <<std::endl;
		return;
	}

	for(int i = 0 ; i < travelPath.size(); i ++) {
		Location tmp = travelPath[i];
		changeGrid(tmp, '*');
	}

	changeGrid(start, 'S');
	changeGrid(terminal, 'T');

	drawGrid();

	return;
}
