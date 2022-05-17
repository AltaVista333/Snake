#include <windows.h>

#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "../controller/input_controller.h"
#include "view.h"
#include "../logic/game.h"
#include "../util/logger.h"
#include "../menu/menu.h"
#include "../menu/menumain.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void draw_snake_cell(Point p);
void drawSnake();
void drawBoard();
void draw_item(CellState state);
void game_show();

void draw_game_over();

unsigned int frog_texture;
unsigned int tombstone_texture;
unsigned int background_texture;
unsigned int game_over_texture;
float texCoord[] = {1,0,0,0,0,1,1,1};
float texCoord2[] = {1,-1,-1,-1,-1,1,1,1};

//float texCoord[] = {-1,-1,-1,0,0,0,0,-1};

//float vertex[] = {-1, -1,0,0,-1,0,0,0,0,-1,0,0};
float vertex[] = {0, 1,0,1,1,0,1,0,0,0,0,0};
float vertex2[] = {-1,0.7 ,0,0.7,0.7,0,0.7,-1,0,-1,-1,0};

void load_img(char* path, unsigned int* dest){
    int width, height, cnt;
    unsigned char* data = stbi_load(path,&width, &height,&cnt, 0);
    glGenTextures(1, dest);
    glBindTexture(GL_TEXTURE_2D, *dest);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width, height,0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}
void text_init(){
    int width, height, cnt;
    unsigned char* data = stbi_load("../img/frog.png",&width, &height,&cnt, 0);

    glGenTextures(1, &frog_texture);
    glBindTexture(GL_TEXTURE_2D, frog_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width, height,0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}


void text_init2(){
    int width, height, cnt;
    unsigned char* data = stbi_load("../img/tombstone.png",&width, &height,&cnt, 0);

    glGenTextures(1, &tombstone_texture);
    glBindTexture(GL_TEXTURE_2D, tombstone_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width, height,0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}


void text_init3(){
    int width, height, cnt;
    unsigned char* data = stbi_load("../img/g2.png",&width, &height,&cnt, 0);

    glGenTextures(1, &background_texture);
    glBindTexture(GL_TEXTURE_2D, background_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width, height,0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void text_init4(){
    int width, height, cnt;
    unsigned char* data = stbi_load("../img/gameover2.png",&width, &height,&cnt, 0);

    glGenTextures(1, &game_over_texture);
    glBindTexture(GL_TEXTURE_2D, game_over_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width, height,0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void initGL() {
   // glClearColor(1.0, 1.0, 1.0, 1.0); // Set background (clear) color to black
}

void reshape(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void draw_when_pause(){
   // glClearColor(0.0, 1.0, 0.0, 1.0);
}

void draw_game(){
    glClearColor(1,1,1,1);
    switch (get_game_state()) {
        case RUNNING:
            game_show();
            drawBoard();
            move_snake();
            break;
        case PAUSED:
            drawBoard();
            game_show();
            break;
        case MENU:
            display_function();
            break;
        case GAME_OVER:
            draw_game_over();
            break;
    }
}

float vertex5[] = {-1, -1,0,1,-1,0,1,1,0,-1,1,0};
float texCoord5[] = {0,1,1,1,1,0,0,0};

void draw_game_over() {
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, game_over_texture);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor4f(1,1,1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT,0, vertex5);
    glTexCoordPointer(2, GL_FLOAT,0,texCoord5);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void display(){
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_game();
    glutSwapBuffers();
}

void draw_cell3(Cell* cell){

    if(cell->state != FREE){
        draw_item(cell->state);
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
        draw_snake_cell(p);
    }

}

void drawSnake(){
    Node* snake_node = get_snake()->head;
    while (snake_node != NULL){
        draw_snake_cell(snake_node->p);

        snake_node = snake_node->next;
    }


}

void draw_snake_cell(Point p){
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
    glLoadIdentity();
    glScalef(2.0 / BOARD_COLS_COUNT, 2.0 / BOARD_ROWS_COUNT, 1);

    //ставим круг по центру клеточки
    glTranslatef(-0.5 * (BOARD_COLS_COUNT - 1), -0.5 * (BOARD_ROWS_COUNT - 1), 0);
    glTranslatef(p.x , p.y , 0.0f);
    //glTranslatef(0.5 , 0.5 , 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 0, 0);
    glVertex2f(0.0f, 0.0f);
    int segments = 100;
    GLfloat angle;
    float circle_radius = 0.3f;
    for (int i = 0; i <= segments; i++) {
        angle = i * 2.0f * M_PI / segments;
        glVertex2f(cos(angle) * circle_radius, sin(angle) * circle_radius);
    }
    glEnd();
    glPopMatrix();
}

void drawBoard() {
    glLoadIdentity();
    //game_show();

    // 2.0 это ширина и длина единичной матрицы.
    glScalef(2.0 / BOARD_COLS_COUNT, 2.0 / BOARD_ROWS_COUNT, 1);
    glTranslatef(-0.5 * BOARD_COLS_COUNT, -0.5 * BOARD_ROWS_COUNT, 0);
    for (int i = 0; i < BOARD_ROWS_COUNT; ++i) {
        for (int j = 0; j < BOARD_COLS_COUNT; ++j) {

            glPushMatrix();
            glTranslatef(i,j,0);
            //Cell* cell = board->cells[i] + j;
            draw_cell3(get_cell(i,j));
            glPopMatrix();
        }
    }
    //drawSnake();
}

void game_show(){
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, background_texture);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    //glColor4f(1,1,1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT,0, vertex2);
    glTexCoordPointer(2, GL_FLOAT,0,texCoord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   // glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    drawSnake();
}

void draw_item_new(unsigned int tex, float* vert, float* text){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor4f(1,1,1,1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT,0, vert);
    glTexCoordPointer(2, GL_FLOAT,0,text);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_BLEND);
}

void draw_item(CellState state) {

    switch (state) {
        case FROG:
            draw_item_new(frog_texture, vertex, texCoord);
            break;
        case STONE:
            draw_item_new(tombstone_texture, vertex, texCoord);
            break;
    }
}

void Timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(REFRESH_TIME, Timer, 0);
}

void init_m() {
    SystemOpen();
    char* menu_file = "../menu.txt";
    SystemInitialize(menu_file);
}

void glut_init(int argc, char** argv){
    logger("INFO", "Loading game...");

    init_game();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH + 200, WINDOW_HEIGHT + 200);
    glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);
    glutCreateWindow(GAME_NAME);

    load_img("../img/frog.png", &frog_texture);
    load_img("../img/tombstone.png", &tombstone_texture);
    load_img("../img/g2.png", &background_texture);
    load_img("../img/gameover2.png", &game_over_texture);
//    text_init();
//    text_init2();
//    text_init3();
//
//    text_init4();

//    glEnable(GL_ALPHA_TEST);
//    glAlphaFunc(GL_GREATER, 0.99);
    glutDisplayFunc(display);

    glutTimerFunc(0, Timer, 0);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    init_m();

    logger("INFO", "Game started!");
    initGL();
    glutMainLoop();
}



