#pragma once
#ifndef SNAKE_2_SNAKE_H
#define SNAKE_2_SNAKE_H
#include "../util/logger.h"
#include "../game_settings.h"
#include "cell.h"

//typedef enum {
//    HEAD_UP,
//    HEAD_LEFT,
//    HEAD_RIGHT,
//    HEAD_DOWN,
//    BODY_VERTICAL,
//    BODY_HORYZONTAL,
//    UP_RIGHT,
//    RIGHT_DOWN,
//    DOWN_LEFT,
//    LEFT_UP
//} ImgType;

typedef struct Node{
  //  ImgType type;
    Cell* cell;
    struct Node* next;
} Node;

typedef enum {
    UP=-2,
    RIGHT=-1,
    DOWN=2,
    LEFT=1
} Direction;





typedef struct{
    int size;
    Direction direction;
    Node* head;
} Snake;
//ImgType get_part();
void move_snake();
void move_and_add(Cell* next);
Snake* get_snake();
Snake* init_snake();
void move(Cell *next);
void remove_from_snake_tail();
void change_direction(Direction d);
void add_new_to_tail(Node* node, int x, int y);
#endif //SNAKE_2_SNAKE_H
