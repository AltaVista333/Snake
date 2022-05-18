#include "snake.h"
#include "board.h"
#include "game.h"
#include "point.h"
#include <malloc.h>
#include <time.h>
#include <stdio.h>


Snake* snake = NULL;
int is_changed = 0;
clock_t frog_timer;

clock_t speed_timer;
Node* create_node_struct();
int is_time(clock_t time);
Node* node_get_tail(Node* node);
Snake* init_snake() {
     snake = malloc(sizeof(Snake));
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
    snake->head->cell = get_board_cell(INIT_SNAKE_X, INIT_SNAKE_Y);
    snake->head->cell->state = SNAKE_HEAD;
    snake->head->next = NULL;
    logger("INFO", "Snake inited");
    return snake;
}

Node* create_node_struct(){
    Node* n = malloc(sizeof(Node));
    if(n == NULL){
        logger("ERROR", "No memory from os for node struct");
        exit(1);
    }
    return n;
}

Snake* get_snake(){
    return snake;
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

Node* node_get_node_before_tail(Node* node){
    Node* tmp = node;
    while(tmp->next != NULL && tmp->next->next != NULL){
        tmp = (Node *) tmp->next;
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

void remove_from_snake_tail(){
    if(get_game_state() != RUNNING){
        return;
    }
    Cell* cell = node_get_tail(get_snake()->head)->cell;
    node_remove_tail(get_snake()->head);
    set_cell_free(cell);
    get_snake()->size--;

}
Node* node_get_tail(Node* node){
    Node* tmp = node;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    return tmp;
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

    if(is_changed == 1){
        return;
    }
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
            check_out_of_board(cell->col, cell->row + 1);
            res =  get_board_cell(cell->col, cell->row + 1);
            break;
        case DOWN:
            check_out_of_board(cell->col, cell->row - 1);
            res =  get_board_cell(cell->col, cell->row - 1);
            break;
        case LEFT:
            check_out_of_board(cell->col - 1, cell->row);
            res =  get_board_cell(cell->col -1, cell->row);
            break;
        case RIGHT:
            check_out_of_board(cell->col + 1, cell->row);
            res =  get_board_cell(cell->col + 1, cell->row);
            break;

    }
    return res;
}



void move_and_add(Cell* next){
    Node* tail = node_get_tail(snake->head);
    Node* node = get_snake()->head;
    Cell* tmp;
    while (node != NULL){
        tmp = node->cell;
        next->state = tmp->state;
        node->cell = next;
        node = node->next;
        next = tmp;
    }
    tail->next = create_node_struct();
    tail->next->cell = tmp;
    tail->next->next = NULL;
    snake->size++;
}

void add_speed_delay(){
    speed_timer = clock() + DEFAULT_SPEED_DELAY;
}

void move_snake() {
    if(get_game_state() != RUNNING){
        return;
    }
    if(!is_time(speed_timer)){

        return;
    }
    is_changed = 0;
    Direction direction = get_snake()->direction;
    Cell* currentCell = get_snake()->head->cell;
    Cell* nextCell = board_get_cell(currentCell, direction);
    merge_cell_state(nextCell);

    add_speed_delay();
}

void add_new_to_tail(Node* node, int x, int y){
    Node* new = create_node_struct();
    Node* tmp = node_get_tail(node);
    new->cell = get_board_cell(x,y);
    new->next = NULL;
    tmp->next = new;
}



