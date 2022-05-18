
#ifndef SNAKE_2_SNAKE_FRAGMENTS_H
#define SNAKE_2_SNAKE_FRAGMENTS_H

#include "../model/point.h"

typedef enum {
    HEAD_UP,
    HEAD_LEFT,
    HEAD_RIGHT,
    HEAD_DOWN,
    BODY_VERTICAL,
    BODY_HORYZONTAL,
    UP_RIGHT,
    RIGHT_DOWN,
    DOWN_LEFT,
    LEFT_UP,
    TAIL_UP,
    TAIL_DOWN,
    TAIL_LEFT,
    TAIL_RIGHT
} Fragments;

typedef struct {
    Point* point;
    Fragments fragment;
} Snake_fragment_to_draw;

Snake_fragment_to_draw* get_fragments();
#endif //SNAKE_2_SNAKE_FRAGMENTS_H
