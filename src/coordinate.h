#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <stdio.h>

#include "random.h"

typedef struct coordinate {
	int row;
	int column;
} Coordinate;

Coordinate generatePosition(int direction, int length);
Coordinate getTarget(void);

#endif
