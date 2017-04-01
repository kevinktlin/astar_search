/*
 * main.cpp
 *
 *  Created on: Mar 10, 2017
 *      Author: Kotung Lin
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>
#include <string>
#include <sstream>
#include <limits>

#include "miscType.h"
#include "miscFunction.h"
#include "gridHandler.h"
#include "findTheShortestPath.h"

using namespace std;

static vector< vector<char> > grid;


int main(int argc, char *argv[]) {
	fstream inputFile;

	gridHandler gridManager;
	findTheShortestPath pathSolver;

	if(argc == 3) {
		string filePath(argv[1]);
		string str;
		vector<Location> travelPath;

#if 0
		inputFile.open("K:\\Dropbox\\Job_Interview\\autoX\\astar_search_hummer\\astar_search_hummer\\Default\\grid_5_1.txt");
#else
		inputFile.open(argv[1]);
#endif
		// form start and grid based on files

		if (!inputFile.is_open()) {
			cerr << "Error opening file" << endl;
			return 0;
		}

		getline(inputFile, str);

		size_t deliPos = str.find(' ');
		string i1 = str.substr(0, deliPos);
		string i2 = str.substr(deliPos+1, str.size()-1);
		//cout << i1 << endl << i2 << endl;

		int height = atoi(i1.c_str());
		int width = atoi(i2.c_str());
		grid.resize(height);

		int currHeight = 0;
		Location startLoc, terminalLoc, defaultLoc;
		defaultLoc.x = -1;
		defaultLoc.y = -1;
		startLoc = terminalLoc = defaultLoc;

		while(getline(inputFile, str)) {
			stringstream ss(str);
			string element;
			for(int i =0 ; i < width; i ++) {
				ss >> element;
				char tmp = element[0];
				if(tmp != 'S' && tmp != 'T')
					grid[currHeight].push_back(tmp);
				else if(tmp == 'S') {
					startLoc.x = i;
					startLoc.y = currHeight;
					grid[currHeight].push_back('1');
				}
				else if(tmp == 'T') {
					terminalLoc.x = i;
					terminalLoc.y = currHeight;
					grid[currHeight].push_back('1');
				}
				else {
					// do nothing
				}
			}
			currHeight++;
		}
		inputFile.close();
		// put the grid into graph
		gridManager.setGrid(grid, width, height);
		gridManager.drawGrid();

		if(startLoc != defaultLoc && terminalLoc != defaultLoc) {
			cout << "Start: "<< startLoc << endl;
			cout << "Goal: "<< terminalLoc << endl;
			// invoke the path finder

#if defined DEBUG
			cout << "run a star search!" << endl;
#endif
			pathSolver.aStarSearch(gridManager, startLoc, terminalLoc, atoi(argv[2]));

#if defined DEBUG
			cout << "reconstruct path!" << endl;
#endif
			travelPath = pathSolver.reconstructPath(gridManager, startLoc, terminalLoc);

#if defined DEBUG
			cout << "print path:";
			for(int i = 0 ; i < travelPath.size(); i ++)
				cout << travelPath[i];
			cout << endl;

			cout << "draw path:" << endl;
#endif
			gridManager.drawPath(travelPath, startLoc, terminalLoc);

#if defined DEBUG
			cout << "done" << endl;
#endif
		}
		else cout << "invalid start and terminal" << endl;
	}
	else	cout << "input variable is not right " << endl;

	return 0;
}
