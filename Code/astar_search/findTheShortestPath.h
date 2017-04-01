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
	std::map<int, int> hummer_usage_table;
	//map<int, Location> came_from;
	//map<int, double> cost_so_far;
public:
	void resetContainer(gridHandler& graph);
	void aStarSearch(gridHandler, Location , Location, const int maxHummerNum);
	std::vector<pathProp> mergePathProp(const std::vector<pathProp> path_prop_1, const std::vector<pathProp> path_prop_2);
	std::vector<Location> reconstructPath(gridHandler &graph, Location start, Location goal);
	double heuristic(Location, Location);

};


#endif /* FINDTHESHORTESTPATH_H_ */
