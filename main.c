#include <windows.h>
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include "input.h"
#include "main.h"

typedef struct {
    float x;
    float y;
} Point;

struct Node{
    Point p;
    struct Node* next;
} ;

typedef struct Node Node;


typedef struct {
    int size;
    Node* head;
} Snake;

typedef struct {
    char* name;
} Item;

typedef struct {
    int row;
    int col;
    Item* item;
    int has_item;
} Cell;

typedef struct {
    Cell** cells;
    int rows_count;
    int cols_count;
    float cell_width;
    float cell_height;
} Board;



char title[] = "Snake game";
int windowWidth  = 600;
int windowHeight = 600;
int windowPosX   = 50;
int windowPosY   = 50;

int refreshMillis = 30;

int cell_size = 40;
int snakeX = 0;
int snakeY = 0;
float snakeSpeed = 0.03;
int tempSpeed = 0;
Direction snake_direction = Right;

void add_new_to_tail(Node* node, float x, float y);
Node* add_new_head(Node* node, float x, float y);
Snake* create_new_snake(float x, float y);
Board* board = NULL;
Snake* snake = NULL;

GLfloat ballX = 0.0f;        // Ball's center (x, y) position
GLfloat ballY = 0.0f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin;

GLfloat angl = 0;
typedef struct {
    int r,g,b;
} Color;

void add_snake_tail(float x, float y);
void init_snake();

void init(){
    board = malloc(sizeof (Board));
    board->rows_count = 12;
    board->cols_count = 12;
    board->cell_width = (float)windowWidth / (float)board->cols_count;
    board->cell_height = (float)windowHeight / (float)board->rows_count;
    //board->cells = malloc(sizeof (Cell*) * board->rows_count);
    board->cells = (Cell**) malloc(sizeof(Cell*) * board->rows_count);
    for (int i = 0; i < board->rows_count; ++i) {
        board->cells[i] = malloc(sizeof (Cell) * board->cols_count);
    }
    for (int i = 0; i < board->rows_count; ++i) {
        for (int j = 0; j < board->cols_count; ++j) {
            board->cells[i][j].row = j;
            board->cells[i][j].col = i;
            board->cells[i][j].has_item = 0;
            board->cells[i][j].item = NULL;
        }
    }
    board->cells[7][7].has_item = 1;
    board->cells[7][7].item = malloc(sizeof(Item));
    init_snake();
}

void init_snake() {
    snake = create_new_snake(3,3);
    add_snake_tail(3,4);
    add_snake_tail(3,5);
    add_snake_tail(4,5);
    add_snake_tail(5,5);
    add_snake_tail(5,4);
    add_snake_tail(4,4);
}

Snake* create_new_snake(float x, float y){
   Snake* s = malloc(sizeof (Snake));
    s->size = 1;
    Node* n = malloc(sizeof(Node));
    n->next = NULL;
    n->p.x = x;
    n->p.y = y;
    s->head = n;
    return s;

}

Node* add_new_head(Node* node, float x, float y){
    Node* new = malloc(sizeof(Node));
    new->next = (struct Node *) node;
    new->p.x = x;
    new->p.y = y;
    return new;
}

Node* get_node_before_tail(Node* node){
    Node* tmp = node;
    while(tmp->next != NULL && tmp->next->next != NULL){
        tmp = (Node *) tmp->next;
    }
    return tmp;
}

Node* get_node_tail(Node* node){
    Node* tmp = node;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    return tmp;
}

void remove_tail(Node* node){
    Node* tmp = get_node_before_tail(node);
    Node* tmp_to_free = tmp->next;
    tmp->next = NULL;
    free(tmp_to_free);
}

void add_new_to_tail(Node* node, float x, float y){
    Node* new = malloc(sizeof(Node));
    Node* tmp = get_node_tail(node);
    new->p.x = x;
    new->p.y = y;
    new->next = NULL;
    tmp->next = new;
}

void add_snake_tail(float x, float y){
    add_new_to_tail(snake->head, x,y);
    snake->size++;
}

void remove_from_snake_tail(){
    remove_tail(snake->head);
    snake->size--;
}


void draw_snake_cell(Point* p);
void drawSnake();
void drawBoard();

void draw_item(Item *ptr);

void move(Direction d){
    switch (d) {
        case UP:
            snake->head = add_new_head(snake->head, snake->head->p.x,snake->head->p.y + 1);
            remove_from_snake_tail();
            break;
        case Down:
            snake->head = add_new_head(snake->head, snake->head->p.x,snake->head->p.y - 1);
            remove_from_snake_tail();
            break;
        case Right:
            snake->head = add_new_head(snake->head, snake->head->p.x + 1,snake->head->p.y);
            remove_from_snake_tail();
            break;
        case Left:
            snake->head = add_new_head(snake->head, snake->head->p.x - 1,snake->head->p.y);
            remove_from_snake_tail();
            break;
    }
}

void pause(){
    if(tempSpeed == 0){
        tempSpeed = snakeSpeed;
        snakeSpeed = 0;
    } else {
        snakeSpeed = tempSpeed;
        tempSpeed = 0;
    }
}

void initGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}

void reshape(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawBoard();
    glutSwapBuffers();
}

// 170 215 81 - light green
// 162 208 73 - darker green
void draw_cell2(Cell* cell){
    glBegin(GL_TRIANGLE_STRIP);
    if((cell->row + cell->col) % 2 == 0){
        glColor3ub(134,61,197);
    } else {
        glColor3ub(225,125,216);
    }
    glVertex2f(0, 1);
    glVertex2f(1, 1);
    glVertex2f(0, 0);
    glVertex2f(1, 0);
    glEnd();
    if(cell->has_item){
        draw_item(cell->item);
    }
}

void draw_snake_from_to(float x, float x1, float y, float y1){
    float xx = (x - x1);
    float yy = (y - y1);
    int max = fabs(xx) > fabs(yy) ? 1 : 0;
    float len = max ? xx : yy;
    Point p  = {x, y};
    for (int i = 1; i < 11; ++i) {
        if (max) {
            p.x += (-xx / 10.0f);
        } else {
            p.y += (-yy / 10.0f);
        }
        draw_snake_cell(&p);
    }

}

void drawSnake(){
    Node* snake_node = snake->head;
    int cnt = 0;
    while (snake_node != NULL){
        draw_snake_cell(&snake_node->p);
        if( snake_node->next != NULL){
            draw_snake_from_to(snake_node->next->p.x,
                               snake_node->p.x,
                               snake_node->next->p.y,
                               snake_node->p.y
            );
        }
        snake_node = snake_node->next;
    }


}

void draw_snake_cell(Point* p){
    glPushMatrix();
    //glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
    glLoadIdentity();
    glScalef(2.0 / board->cols_count, 2.0 / board->rows_count, 1);

    //ставим круг по центру клеточки
    glTranslatef(-0.5 * (board->cols_count - 1), -0.5 * (board->rows_count - 1), 0);
    glTranslatef(p->x , p->y , 0.0f);
    //glTranslatef(0.5 , 0.5 , 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(60, 185, 252);
    glVertex2f(0.0f, 0.0f);       // Center of circle
    int numSegments = 100;
    GLfloat angle;
    float ballRadius = 0.3f;
    for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
        angle = i * 2.0f * M_PI / numSegments;  // 360 deg for all segments
        glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
    }
    glEnd();
    glPopMatrix();
}

void drawBoard() {
    glLoadIdentity();
    // 2.0 это ширина и длина единичной матрицы.
    glScalef(2.0 / board->cols_count, 2.0 / board->rows_count, 1);
    glTranslatef(-0.5 * board->cols_count, -0.5 * board->rows_count, 0);
    for (int i = 0; i < board->rows_count; ++i) {
        for (int j = 0; j < board->cols_count; ++j) {
            glPushMatrix();
            glTranslatef(i,j,0);
            Cell* cell = board->cells[i] + j;
            draw_cell2(cell);
            glPopMatrix();
        }
    }
    drawSnake();
}

void draw_item(Item *ptr) {
    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(255,255,255);
    glVertex2f(0.25f, 0.75f);
    glVertex2f(0.75f, 0.75f);
    glVertex2f(0.25f, 0.25f);
    glVertex2f(0.75f, 0.25f);
    glEnd();
}

void Timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(refreshMillis, Timer, 0);
}

int main(int argc, char** argv) {
    printf("Game Started\n");
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow(title);

    glutDisplayFunc(display);
   // glutReshapeFunc(reshape);
    glutTimerFunc(0, Timer, 0);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    //glutFullScreen();
    glutMouseFunc(mouse);
    initGL();
    glutMainLoop();
    return 0;
}

