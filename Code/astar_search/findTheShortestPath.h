/*
 * findTheShortestPath.h
 *
 *  Created on: Mar 30, 2017
 *      Author: kingo
 */

#ifndef FINDTHESHORTESTPATH_H_
#define FINDTHESHORTESTPATH_H_

#include <vector>
#include <map>

#include "gridHandler.h"

class findTheShortestPath {
	std::vector<Location> optimal_path;
	std::map<int, std::vector<pathProp> > path_prop_table;
	std::map<int, std::vector<Location> > came_from_table;
	std::map<int, std::vector<double> > cost_so_far_table;
	std::map<int, int> hummer_usage_table;
	//map<int, Location> came_from;
	//map<int, double> cost_so_far;
public:
	void resetContainer(gridHandler& graph);
	void aStarSearch(gridHandler, Location , Location, const int maxHummerNum);
	void printCameFrom(void);
	void printCost(void);
	std::vector<Location> reconstructPath(gridHandler &graph, Location start, Location goal);
	double heuristic(Location, Location);

};


#endif /* FINDTHESHORTESTPATH_H_ */
