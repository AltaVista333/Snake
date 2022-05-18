#include <malloc.h>
#include "board.h"
#include "game.h"
#include <time.h>

Board* board = NULL;

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

    while (!is_added){
        srand(time(NULL));
        random_x = rand() % BOARD_COLS_COUNT;
        random_y = rand() % BOARD_ROWS_COUNT;
        CellState* state = &board->cells[random_x][random_y].state;
        if (*state == FREE){
            *state = item;
            is_added = 1;
        }
    }
}

Cell* get_board_cell(int row, int col){
    // printf("get_boadrd_cell row : %d, col : %d\n", row, col);
    return &board->cells[row][col];
}

void check_out_of_board_end(int x, int y) {

    if(x < 0 || x >= BOARD_COLS_COUNT ||
       y < 0 || y >= BOARD_ROWS_COUNT ){
        set_game_over();
        logger("INFO", "Game Over!");
    }
}
//TODO: change
void set_cell_free(Cell* cell){
    cell->state = FREE;
}

void merge_cell_state(Cell* cell) {

    check_out_of_board_end(cell->row,cell->col);
    if(get_game_state() != RUNNING){
        return;
    }
    //CellState current_state = game->board->cells[cell->row][cell->col].state;
    switch (cell->state) {
        case SNAKE_PART:

            logger("INFO", "Cannibalism");
           set_game_over();


            break;
        case SNAKE_HEAD:
            logger("INFO", "Changed head");
            break;
        case FROG:
            logger("INFO", "Nice! Tasty frog");
            //game->board->cells[cell->row][cell->col].state = SNAKE_HEAD;
            move_and_add(cell);
            add_item(FROG);
            break;
        case STONE:
            logger("INFO", "Rolling Stones");
            move(cell);
            remove_from_snake_tail();
//            remove_from_snake_tail();
            add_item(STONE);
            break;
        case WALL:
            logger("INFO", "Another brick in the wall");
            set_game_over();
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

