/*
 * miscType.h
 *
 *  Created on: Mar 30, 2017
 *      Author: kingo
 */

#ifndef MISCTYPE_H_
#define MISCTYPE_H_

struct Location {
	int x;
	int y;
};

struct pathProp {
	Location came_from;
	double cost_so_far;
};

struct priorityElement {
	Location loc;
	int priority;
	int tableID;
};

#endif /* MISCTYPE_H_ */
