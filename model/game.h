#pragma once
#ifndef SNAKE_2_GAME_H
#define SNAKE_2_GAME_H
#include "../game_settings.h"
#include "board.h"
#include "point.h"

typedef enum {
    RUNNING,
    PAUSED,
    MENU,
    GAME_OVER
} Game_state;


typedef struct {
    char* title;
    int windowWidth;
    int windowHeight;
    int windowPosX;
    int windowPosY;
    int speed;
    Game_state game_state;
    Board* board;
} Game;

void init_game();
void start_game();
void end_game();
void set_game_over();
Game_state get_game_state();
void pause_game();
void sh_menu();
Point* point_arr();
int get_snake_size();
#endif //SNAKE_2_GAME_H
