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

using namespace std;

static vector< vector<char> > grid;

struct Location {
	int x;
	int y;
};


struct priorityElement {
	Location loc;
	int priority;
	int tableID;
};

ostream& operator<<(ostream& os, const Location& loc) {
	return os << '(' << loc.x << ',' << loc.y << ')';
}

bool operator==(const Location& lhs, const Location& rhs) {
  return (lhs.x == rhs.x && lhs.y == rhs.y);
}

bool operator!=(const Location& lhs, const Location& rhs) {
  return (lhs.x != rhs.x || lhs.y != rhs.y);
}

bool operator>(const priorityElement& lhs, const priorityElement& rhs) {
  return lhs.priority > rhs.priority;
}


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
	void drawPath(vector<Location> travelPath, Location start, Location terminal);
	void changeGrid(Location loc, char ch) {
		grid[loc.y][loc.x] = ch;
	}

	int getWidth(void)	{	return width;	}
	int getHeight(void)	{	return height;	}
};

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
		nextMove.x = loc.x + dir.x;
		nextMove.y = loc.y + dir.y;

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

void gridHandler::drawPath(vector<Location> travelPath, Location start, Location terminal) {

	if(!travelPath.size())	{
		cout << "no valid path" <<endl;
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

class findTheShortestPath {
	std::vector<Location> optimal_path;
	vector< vector<Location> > came_from_table;
	vector< vector<double> > cost_so_far_table;
	vector<int> hummer_usage_table;
	//map<int, Location> came_from;
	//map<int, double> cost_so_far;
public:
	void resetContainer(gridHandler& graph);
	void aStarSearch(gridHandler, Location , Location, const int maxHummerNum);
	void printCameFrom(void);
	void printCost(void);
	vector<Location> reconstructPath(gridHandler &graph, Location start, Location goal);
	double heuristic(Location, Location);

};

void findTheShortestPath::resetContainer(gridHandler& graph) {

	Location defaultLoc;
	defaultLoc.x = -1;
	defaultLoc.y = -1;

	came_from_table.resize(1);
	cost_so_far_table.resize(1);
	hummer_usage_table.resize(1);

	came_from_table[0].resize(graph.getHeight()*graph.getWidth());
	cost_so_far_table[0].resize(graph.getHeight()*graph.getWidth());

	// set all cost to invalid
	for(int i=0; i < cost_so_far_table[0].size(); i++ ) {
		cost_so_far_table[0][i] = -100;
		//cout <<"cost_so_far "<< i << ':' << cost_so_far_table[0][i] << endl;
	}

	for(int i=0; i < came_from_table[0].size(); i++ ) {
		came_from_table[0][i] = defaultLoc;
		//cout <<"came_from_table "<< i << ':' << came_from_table[0][i] << endl;
	}

	hummer_usage_table[0] = 0;
}

void findTheShortestPath::aStarSearch
	(   gridHandler graph,
		Location start,
		Location goal,
		const int maxHummerNum)
{
	std::priority_queue<priorityElement, std::vector<priorityElement>, std::greater<priorityElement> > frontEdge;

	priorityElement tmp;
	tmp.loc = start;
	tmp.priority = 0;
	tmp.tableID = 0;
	frontEdge.push(tmp);

	resetContainer(graph);

	came_from_table[0][start.x+start.y*4] = start;
	cost_so_far_table[0][start.x+start.y*4] = 0;

	while(!frontEdge.empty()) {
		priorityElement currentNode = frontEdge.top();
		frontEdge.pop();
		Location currentLoc = currentNode.loc;

		if (currentLoc == goal) {
			break;
		}

		std::vector<Location> neighborList = graph.neighbors(currentLoc, hummer_usage_table[currentNode.tableID], maxHummerNum);

#if defined DEBUG
		cout << "current location:" << currentLoc << endl;
		cout << "next move:";
		for(std::vector<Location>::iterator next = neighborList.begin(); next!=neighborList.end(); next++ ) {
			cout << *next ;
		}
		cout << endl ;
#endif


		for(std::vector<Location>::iterator next = neighborList.begin(); next!=neighborList.end(); next++ ) {

			Location nextLoc = *next;
			int indexCurr = currentLoc.x+currentLoc.y*graph.getWidth();
			int indexNext = nextLoc.x+nextLoc.y*graph.getWidth();
			if(graph.goable(nextLoc)) {
				double new_cost = cost_so_far_table[currentNode.tableID][indexCurr] + graph.cost(currentLoc, nextLoc);

#if defined DEBUG
				//std::cout << nextLoc << ", new cost:" << new_cost << ", cost_so_far:" << cost_so_far[indexNext] << std::endl;
#endif

				if (cost_so_far_table[currentNode.tableID][indexNext] < 0 || new_cost < cost_so_far_table[currentNode.tableID][indexNext]) {
					cost_so_far_table[currentNode.tableID][indexNext] = new_cost;
					double priority = new_cost + heuristic(nextLoc, goal);
					tmp.loc = nextLoc;
					tmp.priority = priority;
					tmp.tableID = currentNode.tableID;
					frontEdge.push(tmp);
					came_from_table[currentNode.tableID][indexNext] = currentLoc;
				}
			}
			else if(hummer_usage_table[currentNode.tableID] < maxHummerNum){
				cost_so_far_table.push_back(cost_so_far_table[currentNode.tableID]);
				came_from_table.push_back(came_from_table[currentNode.tableID]);
				int new_table_id =  cost_so_far_table.size()-1;
				double new_cost = cost_so_far_table[new_table_id][indexCurr] + graph.cost(currentLoc, nextLoc);
				hummer_usage_table[new_table_id] = 1;

#if defined DEBUG
//				std::cout << nextLoc << ", new cost:" << new_cost << ", cost_so_far:" << cost_so_far_table[currentNode.tableID][indexNext] << std::endl;
				cout << "new table id:" << new_table_id << ", Hummer usage:" << hummer_usage_table[new_table_id] << endl;
#endif

				if (cost_so_far_table[new_table_id][indexNext] < 0 || new_cost < cost_so_far_table[new_table_id][indexNext]) {
					cost_so_far_table[new_table_id][indexNext] = new_cost;
					double priority = new_cost + heuristic(nextLoc, goal);
					tmp.loc = nextLoc;
					tmp.priority = priority;
					tmp.tableID = new_table_id;
					frontEdge.push(tmp);
					came_from_table[new_table_id][indexNext] = currentLoc;
				}
			}
			else {

			}
		}
	}
}


vector<Location> findTheShortestPath::reconstructPath(gridHandler &graph, Location start, Location goal) {
	vector<Location> path;
	Location current = goal;
	int currentIndex = current.x + current.y*graph.getWidth();

	int minCostTableID = cost_so_far_table.size();
	double minCost = 100000;
	// find the best solution of all cost_so_far
	for(int i=0; i < cost_so_far_table.size(); i++) {
		if(cost_so_far_table[i][goal.x + goal.y*graph.getWidth()] < minCost &&
				cost_so_far_table[i][goal.x + goal.y*graph.getWidth()] > 0) {
			minCost = cost_so_far_table[i][goal.x + goal.y*graph.getWidth()];
			minCostTableID = i;
		}
	}

	if(minCost == 100000)	return path;
	path.push_back(current);

#if defined DEBUG
	cout << "min cost table id:" << minCostTableID << endl;
#endif

	if(came_from_table[minCostTableID][currentIndex].x < 0 || came_from_table[minCostTableID][currentIndex].y < 0)	return path;

	while (current != start) {
		//cout << "path index:" << currentIndex << endl;
		current = came_from_table[minCostTableID][currentIndex];
		path.push_back(current);
		currentIndex = current.x + current.y*graph.getWidth();
	}
	std::reverse(path.begin(), path.end());
	return path;
}

double findTheShortestPath::heuristic(Location currLoc, Location goal) {
	int delta_x = abs(currLoc.x - goal.x);
	int delta_y = abs(currLoc.y - goal.y);

	if(currLoc.x == goal.x) return delta_y;
	else if(currLoc.y == goal.y) return delta_x;
	else	return std::min(delta_x, delta_y)*1.414 + abs(delta_x - delta_y);
}

void findTheShortestPath::printCameFrom(void) {
	for(int i = 0; i < came_from_table.size(); i++)
		for(int j = 0; j < came_from_table[i].size(); j++)
			std::cout << came_from_table[i][j] << ' ';
		cout << endl;
	std::cout << std::endl;
}

void findTheShortestPath::printCost(void) {
	for(int i=0; i < cost_so_far_table.size(); i++)
		for(int j=0; j < cost_so_far_table[i].size(); j++)
			std::cout << cost_so_far_table[i][j] << ' ';
	std::cout << std::endl;
}


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
