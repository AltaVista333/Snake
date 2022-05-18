
#include <malloc.h>
#include "point.h"




Point *get_vec2(Point p1, Point p2){
    Point* p = malloc(sizeof (Point));
    p->x = p1.x - p2.x;
    p->y = p1.y - p2.y;
    return p;
}