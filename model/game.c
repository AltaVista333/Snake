#include "game.h"
#include "point.h"
#include <malloc.h>
#include <time.h>


Game* game = NULL;

clock_t start,speed_timer,delay_timer;

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

void add_snake_tail_on_init(Snake* snake,int x, int y){
    game->board->cells[x][y].state = SNAKE_PART;
    add_new_to_tail(snake->head, x,y);
    snake->size++;
}

void start_game(){
    add_snake_tail_on_init(game->board->snake,4,5);
    add_snake_tail_on_init(game->board->snake,3,5);
    game->game_state = RUNNING;

}

void set_game_over(){
    game->game_state = GAME_OVER;
}

void end_game(){
    exit(0);
}

Game_state get_game_state(){
    return game->game_state;
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

Point* point_arr(){
    Point* p = malloc(sizeof(Point) * game->board->snake->size);
    Node* node = game->board->snake->head;
    for (int i = 0; i < game->board->snake->size; ++i) {
        p[i].x = node->cell->col;
        p[i].y = node->cell->row;
        node = node->next;
    }
    return p;
}

int get_snake_size(){
    return game->board->snake->size;
}

