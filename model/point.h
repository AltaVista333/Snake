#ifndef SNAKE_2_POINT_H
#define SNAKE_2_POINT_H

#include "cell.h"

typedef struct {
    int x;
    int y;
} Point;
Point* get_vec2(Point p1, Point p2);
#endif //SNAKE_2_POINT_H
