#pragma once
#ifndef SNAKE_2_BOARD_H
#define SNAKE_2_BOARD_H
#include "snake.h"
#include "cell.h"




typedef struct {
    Cell** cells;
    int rows_count;
    int cols_count;
    float cell_width;
    float cell_height;
    Snake* snake;
} Board;

Board* init_board();
void add_item(CellState item);
void set_cell_free(Cell* cell);
void merge_cell_state(Cell* cell);
Cell* get_board_cell(int row, int col);
#endif //SNAKE_2_BOARD_H
