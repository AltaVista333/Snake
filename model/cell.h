#pragma once
#ifndef SNAKE_2_CELL_H
#define SNAKE_2_CELL_H
typedef enum {
    SNAKE_HEAD,
    SNAKE_PART,
    FROG,
    STONE,
    WALL,
    FREE
} CellState;

typedef struct {
    int row;
    int col;
    CellState state;
} Cell;


#endif //SNAKE_2_CELL_H
