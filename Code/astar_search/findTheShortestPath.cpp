/*
 * findTheShortestPath.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kingo
 */

#include <iostream>
#include <queue>
#include <algorithm>

#include "findTheShortestPath.h"
#include "miscType.h"
#include "miscFunction.h"
#include "gridHandler.h"

struct Location;
struct priorityElement;

#define DEBUG

void findTheShortestPath::resetContainer(gridHandler& graph) {

	Location defaultLoc;
	defaultLoc.x = -1;
	defaultLoc.y = -1;

	path_prop_table[0].resize(graph.getHeight()*graph.getWidth());
	came_from_table[0].resize(graph.getHeight()*graph.getWidth());
	cost_so_far_table[0].resize(graph.getHeight()*graph.getWidth());

	// set all cost to invalid
	for(int i=0; i < path_prop_table[0].size(); i++ ) {
		path_prop_table[0][i].came_from = -100;
		path_prop_table[0][i].came_from_table = defaultLoc;
		//cout <<"cost_so_far "<< i << ':' << cost_so_far_table[0][i] << endl;
	}

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
	int start_index = start.x+start.y*graph.getWidth();

	path_prop_table[0][start_index].came_from = start;
	path_prop_table[0][start_index].cost_so_far = 0;

	came_from_table[0][start_index] = start;
	cost_so_far_table[0][start_index] = 0;

	while(!frontEdge.empty()) {
		priorityElement currentNode = frontEdge.top();
		frontEdge.pop();
		Location currentLoc = currentNode.loc;

		if (currentLoc == goal) {
			break;
		}

		std::vector<Location> neighborList = graph.neighbors(currentLoc, hummer_usage_table[currentNode.tableID], maxHummerNum);

#if defined DEBUG
		std::cout << "current location:" << currentLoc << std::endl;
		std::cout << "next move:";
		for(std::vector<Location>::iterator next = neighborList.begin(); next!=neighborList.end(); next++ ) {
			std::cout << *next ;
		}
		std::cout << std::endl ;
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
			else if(hummer_usage_table.find(currentNode.tableID) != hummer_usage_table.end() &&
					hummer_usage_table[currentNode.tableID]< maxHummerNum){
				if(cost_so_far_table.find(indexNext) == cost_so_far_table.end() &&
						came_from_table.find(indexNext) == came_from_table.end()) {
					cost_so_far_table[indexNext] = cost_so_far_table[currentNode.tableID];
					came_from_table[indexNext]= came_from_table[currentNode.tableID];
				}
				else {
					// get the optimal table for different break time, but same break wall
				}

				double new_cost = cost_so_far_table[indexNext][indexCurr] + graph.cost(currentLoc, nextLoc);
				hummer_usage_table[indexNext] = 1;

#if defined DEBUG
				std::cout << nextLoc << ", new cost:" << new_cost << ", cost_so_far:" << cost_so_far_table[currentNode.tableID][indexNext] << std::endl;
				std::cout << "new table id:" << indexNext << ", Hummer usage:" << hummer_usage_table[indexNext] << std::endl;
#endif

				if (cost_so_far_table[indexNext][indexNext] < 0 || new_cost < cost_so_far_table[indexNext][indexNext]) {
					cost_so_far_table[indexNext][indexNext] = new_cost;
					double priority = new_cost + heuristic(nextLoc, goal);
					tmp.loc = nextLoc;
					tmp.priority = priority;
					tmp.tableID = indexNext;
					frontEdge. push(tmp);
					came_from_table[indexNext][indexNext] = currentLoc;
				}
			}
			else {

			}
		}
	}
}


std::vector<Location> findTheShortestPath::reconstructPath(gridHandler &graph, Location start, Location goal) {
	std::vector<Location> path;
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
	std::cout << "min cost table id:" << minCostTableID << std::endl;
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
		std::cout << std::endl;
	std::cout << std::endl;
}

void findTheShortestPath::printCost(void) {
	for(int i=0; i < cost_so_far_table.size(); i++)
		for(int j=0; j < cost_so_far_table[i].size(); j++)
			std::cout << cost_so_far_table[i][j] << ' ';
	std::cout << std::endl;
}
