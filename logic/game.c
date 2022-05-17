#include <sys/time.h>
#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../util/logger.h"

Game* game = NULL;
clock_t start,speed_timer,delay_timer;

int is_changed = 0;

Board* init_board();
Snake* init_snake();
Node* create_node_struct();
void add_snake_tail_on_init(Snake* snake,float x, float y);
void check_out_of_board_end(int x, int y);
void print_cells_state();
void add_item(CellState item);
void remove_from_snake_tail();
Cell* get_board_cell(int row, int col);
void move(Cell *next);
void move_and_add(Cell *next);

void init_game(){
    game = malloc(sizeof (Game));
    if (game == NULL){
        logger("ERROR", "No memory from os for game struct");
        exit(1);
    }
    logger("INFO", "Game struct created");
    game->title = GAME_NAME;
    game->windowWidth  = WINDOW_WIDTH;
    game->windowHeight = WINDOW_HEIGHT;
    game->windowPosX   = WINDOW_POSITION_X;
    game->windowPosY   = WINDOW_POSITION_Y;
    game->speed = DEFAULT_SPEED_DELAY;
    game->board = init_board();
    logger("INFO", "Game inited");
    game->board->snake = init_snake();
    add_item(FROG);
    add_item(STONE);
    game->game_state = MENU;
    //начинаем через 1 секунду после инициализации
    speed_timer = clock() + 1000;
    delay_timer = 0;
}

Board* init_board(){
    Board* board = malloc(sizeof (Board));
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

Snake* init_snake() {
    Snake* snake = malloc(sizeof(Snake));
    if (snake == NULL) {
        logger("ERROR", "No memory from os for snake struct");
        exit(1);
    }
    logger("INFO", "Snake struct created");
    snake->size = INITIAL_SNAKE_SIZE;
    snake->direction = INITIAL_SNAKE_DIRECTION;
    //todo: re-write
    Node* n = create_node_struct();
    snake->head = n;
    //game->board->cells[INIT_SNAKE_X][INIT_SNAKE_Y].state = SNAKE_HEAD;
    snake->head->cell = get_board_cell(INIT_SNAKE_X, INIT_SNAKE_Y);
    snake->head->cell->state = SNAKE_HEAD;
    snake->head->next = NULL;

    add_snake_tail_on_init(snake,4,5);
    add_snake_tail_on_init(snake,3,5);
//    add_snake_tail_on_init(snake,2,5);
//    add_snake_tail_on_init(snake,1,5);
//    add_snake_tail_on_init(snake,0,5);
//    add_snake_tail_on_init(snake,0,4);
//    add_snake_tail_on_init(snake,0,3);
    logger("INFO", "Snake inited");
    return snake;
}

Cell* get_board_cell(int row, int col){
   // printf("get_boadrd_cell row : %d, col : %d\n", row, col);
    return &game->board->cells[row][col];
}

void start_game(){
 game->game_state = RUNNING;
}
void end_game(){
    exit(0);
}

void add_item(CellState item){
   int is_added = 0;
   int random_x = 0;
   int random_y = 0;

   while (!is_added){
       srand(time(NULL));
       random_x = rand() % BOARD_COLS_COUNT;
       random_y = rand() % BOARD_ROWS_COUNT;
       CellState* state = &game->board->cells[random_x][random_y].state;
        if (*state == FREE){
            *state = item;
            is_added = 1;
        }
    }
}

Game_state get_game_state(){
    return game->game_state;
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
                game->game_state = GAME_OVER;


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
            game->game_state = GAME_OVER;
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

void pause_game(){
    switch (get_game_state()) {
        case RUNNING:
            game->game_state = PAUSED;
            logger("INFO", "Game paused");
            break;
        case PAUSED:
            game->game_state = RUNNING;
            logger("INFO", "Game running");
            break;
        case GAME_OVER:
            //do nothing
            break;
    }
}

void sh_menu(){
    game->game_state = MENU;
}


Snake* get_snake(){
    return game->board->snake;
}

Node* create_node_struct(){
    Node* n = malloc(sizeof(Node));
    if(n == NULL){
        logger("ERROR", "No memory from os for node struct");
        exit(1);
    }
    return n;
}

//Node* node_add_new_head(Node* node, float x, float y){
//    Node* new = create_node_struct();
//    new->next = node;
//    new->p.x = x;
//    new->p.y = y;
//    return new;
//}

//void snake_new_head(Direction direction){
//    //print_cells_state();
//
//    merge_cell_state(x, y, SNAKE_HEAD);
//    int xx = (int)get_snake()->head->p.x;
//    int yy = (int)get_snake()->head->p.y;
//    game->board->cells[xx][yy].state = SNAKE_PART;
//
//    game->board->snake->head = node_add_new_head(get_snake()->head, x, y);
//    char log[100];
//    sprintf(log, "New head cell column : %d, row %d", (int)x, (int)y);
//    logger("INFO", log);
//
//}


Node* node_get_node_before_tail(Node* node){
    Node* tmp = node;
    while(tmp->next != NULL && tmp->next->next != NULL){
        tmp = (Node *) tmp->next;
    }
    return tmp;
}

Node* node_get_tail(Node* node){
    Node* tmp = node;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    return tmp;
}

void node_remove_tail(Node* node){
    Node* tmp = node_get_node_before_tail(node);
    Node* tmp_to_free = tmp->next;
    tmp_to_free->cell->state = FREE;
    tmp->next = NULL;
    free(tmp_to_free);
    logger("INFO", "Node tail successfully removed");
}

void check_out_of_board_end(int x, int y) {

    if(x < 0 || x >= BOARD_COLS_COUNT ||
       y < 0 || y >= BOARD_ROWS_COUNT ){
        game->game_state = GAME_OVER;
        logger("INFO", "Game Over!");
    }
}

int is_time(clock_t time){
    start = clock();
    return start > time ? 1 : 0;
}

void add_speed_delay(){
    speed_timer = clock() + DEFAULT_SPEED_DELAY;
}

void change_direction(Direction d){
    if(get_game_state() != RUNNING){
        return;
    }
    Direction current_direction = get_snake()->direction;
    //Значения противоположных направлений различаются знаком
    if((current_direction + d) == 0){
        return;
    }
//    if(!is_time(delay_timer)){
//        return;
//    }

    if(is_changed == 1){
        return;
    }

//Задержка на изменение направления
//    delay_timer = clock() + 100;
    switch (d) {

        case UP:
            get_snake()->direction = UP;
            break;
        case RIGHT:
            get_snake()->direction = RIGHT;
            break;
        case DOWN:
            get_snake()->direction = DOWN;
            break;
        case LEFT:
            get_snake()->direction = LEFT;
            break;
    }

    is_changed = 1;
}

Cell* board_get_cell(Cell* cell, Direction d){
    Cell* res;
    switch (d) {
        case UP:
            res =  get_board_cell(cell->col, cell->row + 1);
            break;
        case DOWN:
            res =  get_board_cell(cell->col, cell->row - 1);
            break;
        case LEFT:
            res =  get_board_cell(cell->col -1, cell->row);
            break;
        case RIGHT:
            res =  get_board_cell(cell->col + 1, cell->row);
            break;

    }
    return res;
}


void move_and_add(Cell* next){
    Node* tmp = get_snake()->head;
    Node* new = create_node_struct();
    new->cell = next;
    new->cell->state = tmp->cell->state;
    tmp->cell->state = SNAKE_PART;
    new->next = tmp;
    game->board->snake->head = new;
}

void move_snake() {
    if(get_game_state() != RUNNING){
        return;
    }
    if(!is_time(speed_timer)){
        return;
    }
    //print_cells_state();
    Node *head = get_snake()->head;
    Direction direction = get_snake()->direction;
    Cell* currentCell = get_snake()->head->cell;
    Cell* nextCell = board_get_cell(currentCell, direction);
    merge_cell_state(nextCell);
    //move_and_add(head, nextCell);

    add_speed_delay();
    is_changed = 0;
}

void add_new_to_tail(Node* node, float x, float y){
    Node* new = create_node_struct();
    Node* tmp = node_get_tail(node);
    new->cell = get_board_cell(x,y);
    new->next = NULL;
    tmp->next = new;
}
//
//void add_snake_tail(float x, float y){
//    add_new_to_tail(get_snake()->head, x,y);
//    get_snake()->size++;
//}

void add_snake_tail_on_init(Snake* snake,float x, float y){
    game->board->cells[(int)x][(int)y].state = SNAKE_PART;
    add_new_to_tail(snake->head, x,y);
    snake->size++;
}

void move(Cell *next) {
    Node* node = get_snake()->head;
    Cell* tmp;
    while (node != NULL){
        tmp = node->cell;
        next->state = tmp->state;
        node->cell = next;
        node = node->next;
        next = tmp;
    }
    tmp->state = FREE;
}


void remove_from_snake_tail(){
    if(get_game_state() != RUNNING){
        return;
    }
    Cell* cell = node_get_tail(get_snake()->head)->cell;
    node_remove_tail(get_snake()->head);
    game->board->cells[cell->row][cell->col].state = FREE;
    get_snake()->size--;
}

Cell* get_cell(int cols, int rows){
    return &game->board->cells[cols][rows];
}