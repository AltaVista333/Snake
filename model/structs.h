#ifndef SNAKE_2_STRUCTS_H
#define SNAKE_2_STRUCTS_H

typedef enum {
    UP=-2,
    RIGHT=-1,
    DOWN=2,
    LEFT=1
} Direction;

typedef enum {
    HEAD,
    BODY,
    TAIL
} Snake_part;

typedef enum {
    SNAKE_HEAD,
    SNAKE_PART,
    SNAKE_TAIL,
    FROG,
    STONE,
    WALL,
    FREE
} CellState;

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    int row;
    int col;
    CellState state;
} Cell;

typedef struct Node{
    Cell* cell;
    struct Node* next;
} Node;

typedef struct {
    int size;
    Direction direction;
    Node* head;
} Snake;

typedef struct {
    char* name;
} Item;



typedef struct {
    Cell** cells;
    int rows_count;
    int cols_count;
    float cell_width;
    float cell_height;
    Snake* snake;
} Board;

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

#endif //SNAKE_2_STRUCTS_H
