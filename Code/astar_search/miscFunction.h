/*
 * miscFunction.h
 *
 *  Created on: Mar 30, 2017
 *      Author: kingo
 */

#ifndef MISCFUNCTION_H_
#define MISCFUNCTION_H_

#include <iostream>
#include "miscType.h"

std::ostream& operator<<(std::ostream& os, const Location& loc);

bool operator==(const Location& lhs, const Location& rhs);

bool operator!=(const Location& lhs, const Location& rhs);

bool operator>(const priorityElement& lhs, const priorityElement& rhs);

#endif /* MISCFUNCTION_H_ */
