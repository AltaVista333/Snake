#include <malloc.h>
#include "snake_fragments.h"
#include "../model/game.h"

Fragments snake_head_fragment();

Snake_fragment_to_draw* get_fragments(){
    Point *ptr = point_arr();
    Snake_fragment_to_draw* fragments = malloc(sizeof (Snake_fragment_to_draw) * get_snake_size());
    fragments[0].point = &ptr[0];
    fragments[0].fragment = snake_head_fragment();
    for (int i = 1; i < get_snake_size() - 1; ++i) {
        fragments[i].point = &ptr[i];
        Point* next_vec = get_vec2(ptr[i - 1], ptr[i]);
        Point* prev_vec = get_vec2(ptr[i + 1], ptr[i]);
        if(prev_vec->x == -1 && next_vec->y == -1 || prev_vec->y == -1 && next_vec->x == -1){
            fragments[i].fragment =  LEFT_UP;
            continue;
        }
        if(prev_vec->x == -1 && next_vec->y == 1 || prev_vec->y == 1 && next_vec->x == -1){
            fragments[i].fragment =  DOWN_LEFT;
            continue;
        }
        if(prev_vec->x == 1 && next_vec->y == -1 || prev_vec->y == -1 && next_vec->x == 1){
            fragments[i].fragment =  UP_RIGHT;
            continue;
        }
        if(prev_vec->x == 1 && next_vec->y == 1 || prev_vec->y == 1 && next_vec->x == 1){
            fragments[i].fragment =  RIGHT_DOWN;
            continue;
        }
        if(prev_vec->y == 0 && next_vec->y == 0){
            fragments[i].fragment = BODY_HORYZONTAL;
            continue;
        }

        if(prev_vec->x == 0 && next_vec->x == 0){
            fragments[i].fragment = BODY_VERTICAL;
            continue;
        }
    }
    fragments[get_snake_size() - 1].point = &ptr[get_snake_size() - 1];
    Point* next_vec = get_vec2(ptr[get_snake_size() - 2], ptr[get_snake_size() - 1]);
    if(next_vec->x == 1 && next_vec->y == 0){
        fragments[get_snake_size() - 1].fragment = TAIL_RIGHT;
    }
    if(next_vec->x == -1 && next_vec->y == 0){
        fragments[get_snake_size() - 1].fragment = TAIL_LEFT;

    }
    if(next_vec->y == 1 && next_vec->x == 0){
        fragments[get_snake_size() - 1].fragment = TAIL_DOWN;

    }
    if(next_vec->y == -1 && next_vec->x == 0){
        fragments[get_snake_size() - 1].fragment = TAIL_UP;

    }



    return fragments;
}

Fragments snake_head_fragment(){
    Fragments f = 0;
    Direction d = get_snake()->direction;
    switch (d) {

        case UP:
            f = HEAD_UP;
            break;
        case RIGHT:
            f = HEAD_RIGHT;
            break;
        case DOWN:
            f = HEAD_DOWN;
            break;
        case LEFT:
            f = HEAD_LEFT;
            break;
    }
    return f;
}