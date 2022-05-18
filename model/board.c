#include <malloc.h>
#include "board.h"
#include "game.h"
#include "../util/timer.h"
#include <time.h>

Board* board = NULL;
clock_t frog_timer;
CellState* state;
void add_timer(int i, CellState *state);

Board* init_board(){
    board = malloc(sizeof (Board));
    if(board == NULL){
        logger("Error", "No memory from os for board struct");
        exit(1);
    }
    logger("INFO", "Board struct created");
    board->rows_count = BOARD_ROWS_COUNT;
    board->cols_count = BOARD_COLS_COUNT;
    board->cell_width = (float)WINDOW_WIDTH / (float)board->cols_count;
    board->cell_height = (float)WINDOW_HEIGHT / (float)board->rows_count;
    board->cells = (Cell**) malloc(sizeof(Cell*) * board->rows_count);
    for (int i = 0; i < board->rows_count; ++i) {
        board->cells[i] = malloc(sizeof (Cell) * board->cols_count);
    }
    for (int i = 0; i < board->rows_count; ++i) {
        for (int j = 0; j < board->cols_count; ++j) {
            board->cells[i][j].row = j;
            board->cells[i][j].col = i;
            board->cells[i][j].state = FREE;
        }
    }
    logger("INFO", "Board struct inited");
    return board;
}

void add_item(CellState item){
    int is_added = 0;
    int random_x = 0;
    int random_y = 0;
    CellState* state;
    while (!is_added){
        srand(time(NULL));
        random_x = rand() % BOARD_COLS_COUNT;
        random_y = rand() % BOARD_ROWS_COUNT;
        state = &board->cells[random_x][random_y].state;
        if (*state == FREE){
            *state = item;
            is_added = 1;
        }
    }
    if(*state == FROG){
        add_timer(FROG_MAX_TIME, state);
    }
}

void add_timer(int i, CellState *st) {
    frog_timer = clock() + FROG_MAX_TIME;
    state = st;

}

void check_timer(){
    if(is_time(frog_timer) == 1){
        *state = FREE;
        add_item(FROG);
    }
}

Cell* get_board_cell(int row, int col){
    return &board->cells[row][col];
}

void check_out_of_board(int x, int y) {

    if(x < 0 || x >= BOARD_COLS_COUNT || y < 0 || y >= BOARD_ROWS_COUNT ){
        set_game_over();
        logger("INFO", "Game Over!");
    }
}
//TODO: change
void set_cell_free(Cell* cell){
    cell->state = FREE;
}

void merge_cell_state(Cell* cell) {
    if(get_game_state() != RUNNING){
        return;
    }
    check_out_of_board(cell->row, cell->col);
    check_timer();
    switch (cell->state) {
        case SNAKE_PART:
            logger("INFO", "Cannibalism");
           set_game_over();
            break;
        case FROG:
            logger("INFO", "Nice! Tasty frog");
            move_and_add(cell);
            add_item(FROG);
            add_score();
            break;
        case STONE:
            logger("INFO", "Rolling Stones");
            move(cell);
            remove_from_snake_tail();
            add_item(STONE);
            break;
        case FREE:
            move(cell);
            logger("INFO", "Cell state updated");
            break;
        default:
            logger("ERROR", "Wrong state");
            exit(1);
    }
}

