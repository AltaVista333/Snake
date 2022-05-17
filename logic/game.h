#ifndef SNAKE_2_GAME_H
#define SNAKE_2_GAME_H
#include "../game_settings.h"
#include "../model/structs.h"

void init_game();
Cell* get_cell(int cols, int rows);
Snake* get_snake();
void move_snake();
Game_state get_game_state();
void sh_menu();
void pause_game();
void start_game();
void end_game();
void change_direction(Direction d);
#endif //SNAKE_2_GAME_H
