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

	// set all cost to invalid
	for(int i=0; i < path_prop_table[0].size(); i++ ) {
		path_prop_table[0][i].cost_so_far = -100;
		path_prop_table[0][i].came_from = defaultLoc;
		//cout <<"cost_so_far "<< i << ':' << cost_so_far_table[0][i] << endl;
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
				double new_cost = path_prop_table[currentNode.tableID][indexCurr].cost_so_far + graph.cost(currentLoc, nextLoc);

#if defined DEBUG
				//std::cout << nextLoc << ", new cost:" << new_cost << ", cost_so_far:" << cost_so_far[indexNext] << std::endl;
#endif

				if (path_prop_table[currentNode.tableID][indexNext].cost_so_far < 0 || new_cost < path_prop_table[currentNode.tableID][indexNext].cost_so_far) {
					path_prop_table[currentNode.tableID][indexNext].cost_so_far = new_cost;
					double priority = new_cost + heuristic(nextLoc, goal);
					tmp.loc = nextLoc;
					tmp.priority = priority;
					tmp.tableID = currentNode.tableID;
					frontEdge.push(tmp);
					path_prop_table[currentNode.tableID][indexNext].came_from = currentLoc;
				}
			}
			else if(hummer_usage_table.find(currentNode.tableID) != hummer_usage_table.end() &&
					hummer_usage_table[currentNode.tableID]< maxHummerNum){
				double new_cost;
				if(path_prop_table.find(indexNext) == path_prop_table.end() &&
						path_prop_table.find(indexNext) == path_prop_table.end()) {
					path_prop_table[indexNext] = path_prop_table[currentNode.tableID];

					new_cost = path_prop_table[indexNext][indexCurr].cost_so_far + graph.cost(currentLoc, nextLoc);
					hummer_usage_table[indexNext] = 1;

					if (path_prop_table[indexNext][indexNext].cost_so_far < 0 || new_cost < path_prop_table[indexNext][indexNext].cost_so_far) {
						path_prop_table[indexNext][indexNext].cost_so_far = new_cost;
						double priority = new_cost + heuristic(nextLoc, goal);
						tmp.loc = nextLoc;
						tmp.priority = priority;
						tmp.tableID = indexNext;
						frontEdge. push(tmp);
						path_prop_table[indexNext][indexNext].came_from = currentLoc;
					}
				}
				else if(path_prop_table.find(indexNext) != path_prop_table.end()) {

					new_cost = path_prop_table[indexNext][indexCurr].cost_so_far + graph.cost(currentLoc, nextLoc);
					hummer_usage_table[indexNext] = 1;

					// get the optimal table for different break time, but same break wall
					path_prop_table[indexNext] = mergePathProp(path_prop_table[indexNext], path_prop_table[currentNode.tableID]);


					if (path_prop_table[indexNext][indexNext].cost_so_far < 0 || new_cost < path_prop_table[indexNext][indexNext].cost_so_far) {
						path_prop_table[indexNext][indexNext].cost_so_far = new_cost;
						double priority = new_cost + heuristic(nextLoc, goal);
						tmp.loc = nextLoc;
						tmp.priority = priority;
						tmp.tableID = indexNext;
						frontEdge. push(tmp);
						path_prop_table[indexNext][indexNext].came_from = currentLoc;
					}
				}
				else {

				}
/*

				double new_cost = path_prop_table[indexNext][indexCurr].cost_so_far + graph.cost(currentLoc, nextLoc);
				hummer_usage_table[indexNext] = 1;
*/

#if defined DEBUG
				std::cout << nextLoc << ", new cost:" << new_cost << ", cost_so_far:" << path_prop_table[currentNode.tableID][indexNext].cost_so_far << std::endl;
				std::cout << "new table id:" << indexNext << ", Hummer usage:" << hummer_usage_table[indexNext] << std::endl;
#endif
/*

				if (path_prop_table[indexNext][indexNext].cost_so_far < 0 || new_cost < path_prop_table[indexNext][indexNext].cost_so_far) {
					path_prop_table[indexNext][indexNext].cost_so_far = new_cost;
					double priority = new_cost + heuristic(nextLoc, goal);
					tmp.loc = nextLoc;
					tmp.priority = priority;
					tmp.tableID = indexNext;
					frontEdge. push(tmp);
					path_prop_table[indexNext][indexNext].came_from = currentLoc;
				}

*/
			}
			else {

			}
		}
	}
}


std::vector<pathProp> findTheShortestPath::mergePathProp(const std::vector<pathProp> path_prop_1, const std::vector<pathProp> path_prop_2) {
	std::vector<pathProp> path_prop_optimal;
	if(path_prop_1.size() != path_prop_2.size())	return path_prop_optimal;
	for(int i=0; i < path_prop_1.size(); i++) {
		if(path_prop_1[i].cost_so_far >= path_prop_2[i].cost_so_far && path_prop_2[i].cost_so_far > 0)
			path_prop_optimal.push_back(path_prop_2[i]);
		else if(path_prop_1[i].cost_so_far < path_prop_2[i].cost_so_far && path_prop_1[i].cost_so_far > 0)
			path_prop_optimal.push_back(path_prop_1[i]);
		else
			path_prop_optimal.push_back(path_prop_1[i]);
	}
	return path_prop_optimal;
}

std::vector<Location> findTheShortestPath::reconstructPath(gridHandler &graph, Location start, Location goal) {
	std::vector<Location> path;
	Location current = goal;
	int currentIndex = current.x + current.y*graph.getWidth();

	int minCostTableID = 0;
	double minCost = 100000;
	// find the best solution of all cost_so_far
	for(auto const &path_prop : path_prop_table) {
		if(path_prop.second[goal.x + goal.y*graph.getWidth()].cost_so_far < minCost &&
				path_prop.second[goal.x + goal.y*graph.getWidth()].cost_so_far > 0) {
			minCost = path_prop.second[goal.x + goal.y*graph.getWidth()].cost_so_far;
			minCostTableID = path_prop.first;
		}
	}

	if(minCost == 100000)	return path;
	path.push_back(current);

#if defined DEBUG
	std::cout << "min cost table id:" << minCostTableID << std::endl;
#endif

	if(path_prop_table[minCostTableID][currentIndex].came_from.x < 0 || path_prop_table[minCostTableID][currentIndex].came_from.y < 0)	return path;

	while (current != start) {
		//cout << "path index:" << currentIndex << endl;
		current = path_prop_table[minCostTableID][currentIndex].came_from;
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

