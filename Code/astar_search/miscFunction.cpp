/*
 * miscFunction.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kingo
 */

#include <ostream>
#include "miscFunction.h"
#include "miscType.h"

std::ostream& operator<<(std::ostream& os, const Location& loc) {
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
