#ifndef SNAKE_2_MAIN_H
#define SNAKE_2_MAIN_H
typedef enum {
    UP,
    Right,
    Down,
    Left
} Direction;


void move(Direction d);
void pause();
#endif //SNAKE_2_MAIN_H
