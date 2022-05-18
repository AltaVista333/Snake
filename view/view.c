#include <windows.h>


#include <GL/glut.h>
#include <GL/glu.h>

#include "view.h"
#include "snake_fragments.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



void render_board();
void render_object(CellState state);
void render_background_texture();
void render_snake();
void draw_game_over();

unsigned int frog_texture;
unsigned int tombstone_texture;
unsigned int background_texture;
unsigned int game_over_texture;
struct Snake_textures{
    unsigned int head_up;
    unsigned int head_down;
    unsigned int head_left;
    unsigned int head_right;
    unsigned int body_bl;
    unsigned int body_br;
    unsigned int body_tl;
    unsigned int body_tr;
    unsigned int body_vertical;
    unsigned int body_horizontal;
    unsigned int tail_up;
    unsigned int tail_down;
    unsigned int tail_left;
    unsigned int tail_right;

} Snake_textures;


float texCoord[] = {1,0,0,0,0,1,1,1};
float texCoord2[] = {1,-1,-1,-1,-1,1,1,1};

//float texCoord[] = {-1,-1,-1,0,0,0,0,-1};

//float vertex[] = {-1, -1,0,0,-1,0,0,0,0,-1,0,0};
float vertex[] = {0, 1,0,1,1,0,1,0,0,0,0,0};
//float vertex2[] = {-1, -1,0,-1,1,0,0.6f,1,0,0.6f,-1,0};
float vertex2[] = {-1,-1 ,0,-1,1,0,1,1,0,1,-1,0};
float vertex5[] = {-1, -1,0,1,-1,0,1,1,0,-1,1,0};
float texCoord5[] = {0,1,1,1,1,0,0,0};

unsigned int* snake_head_text_with_direction();
unsigned int* snake_body_text_with_direction();
void draw_item_new(unsigned int tex, float* vert, float* text);


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



void draw_game(){
    glClearColor(1,1,1,1);
    switch (get_game_state()) {
        case RUNNING:
            render_background_texture();
            render_board();
            render_snake();
            move_snake();
            break;
        case PAUSED:
            render_background_texture();
            render_board();
            render_snake();
            break;
        case MENU:
            display_function();
            break;
        case GAME_OVER:
            draw_game_over();
            break;
    }
}



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
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_game();
    glutSwapBuffers();
}

void render_board_objects(Cell* cell){
    if(cell->state != FREE && cell->state != SNAKE_HEAD && cell->state != SNAKE_PART){
        render_object(cell->state);
    }
   
}


void render_snake(){
    Snake_fragment_to_draw *fragments = get_fragments();
    int size = get_snake_size();
    glLoadIdentity();
    glTranslatef(-1,-1,0);
    // glScalef(1.6 / BOARD_COLS_COUNT,2.0 / BOARD_ROWS_COUNT,1); // 0.16 : 0.2
    glScalef(2.0 / BOARD_COLS_COUNT,2.0 / BOARD_ROWS_COUNT,1); // 0.16 : 0.2
    for (int i = 0; i < size; ++i) {
        glPushMatrix();
        glTranslatef(fragments[i].point->x, fragments[i].point->y, 0);
        unsigned int *sn_part = snake_body_text_with_direction(fragments[i].fragment);
        draw_item_new(*sn_part,vertex,texCoord);
        glPopMatrix();
    }
}

void render_board() {
    glLoadIdentity();

    glTranslatef(-1,-1,0);
   // glScalef(1.6 / BOARD_COLS_COUNT,2.0 / BOARD_ROWS_COUNT,1); // 0.16 : 0.2
    glScalef(2.0 / BOARD_COLS_COUNT,2.0 / BOARD_ROWS_COUNT,1); // 0.16 : 0.2
    
    for (int i = 0; i < BOARD_ROWS_COUNT; ++i) {
        for (int j = 0; j < BOARD_COLS_COUNT; ++j) {
            glPushMatrix();
            glTranslatef(i,j,0);
//            printf("I : %d, J : %d\n",i, j);
            render_board_objects(get_board_cell(i, j));
            glPopMatrix();
        }
    }
}

void render_background_texture(){
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, background_texture);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT,0, vertex2);
    glTexCoordPointer(2, GL_FLOAT,0,texCoord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void draw_item_new(unsigned int tex, float* vert, float* text){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor3f(1,1,1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT,0, vert);
    glTexCoordPointer(2, GL_FLOAT,0,text);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void render_object(CellState state) {

    switch (state) {
        case FROG:
            draw_item_new(frog_texture, vertex, texCoord);
            break;
        case STONE:
            draw_item_new(tombstone_texture, vertex, texCoord);
            break;
//        case SNAKE_HEAD:
//            draw_item_new(*snake_head_text_with_direction(), vertex, texCoord);
//            break;
//        case SNAKE_PART:
//            draw_item_new(*snake_body_text_with_direction(), vertex, texCoord);
//            break;
        case WALL:
            break;
        case FREE:
            break;
    }
}

unsigned int* snake_body_text_with_direction(Fragments type){
    
    unsigned int* snake_body_text;
    switch (type) {

        case BODY_VERTICAL:
            snake_body_text = &Snake_textures.body_vertical;
            break;
        case BODY_HORYZONTAL:
            snake_body_text = &Snake_textures.body_horizontal;
            break;
        case UP_RIGHT:
            snake_body_text = &Snake_textures.body_bl;
            break;
        case RIGHT_DOWN:
            snake_body_text = &Snake_textures.body_tl;
            break;
        case DOWN_LEFT:
            snake_body_text = &Snake_textures.body_tr;
            break;
        case LEFT_UP:
            snake_body_text = &Snake_textures.body_br;
            break;
        case HEAD_UP:
            snake_body_text = &Snake_textures.head_up;
            break;
        case HEAD_LEFT:
            snake_body_text = &Snake_textures.head_right;
            break;
        case HEAD_RIGHT:
            snake_body_text = &Snake_textures.head_left;
            break;
        case HEAD_DOWN:
            snake_body_text = &Snake_textures.head_down;
            break;
        case TAIL_RIGHT:
            snake_body_text = &Snake_textures.tail_right;
            break;
        case TAIL_DOWN:
            snake_body_text = &Snake_textures.tail_down;
            break;
        case TAIL_LEFT:
            snake_body_text = &Snake_textures.tail_left;
            break;
        case TAIL_UP:
            snake_body_text = &Snake_textures.tail_up;
            break;
    }
    return snake_body_text;
}

//unsigned int* snake_head_text_with_direction(){
//    unsigned int* snake_head_text;
//    Direction d = get_snake()->direction;
//    switch (d) {
//
//        case UP:
//
//            break;
//        case RIGHT:
//            snake_head_text = &Snake_textures.head_left;
//            break;
//        case DOWN:
//            snake_head_text = &Snake_textures.head_down;
//            break;
//        case LEFT:
//            snake_head_text = &Snake_textures.head_right;
//            break;
//    }
//    return snake_head_text;
//}

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
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);
    glutCreateWindow(GAME_NAME);

    load_img("../img/frog.png", &frog_texture);
    load_img("../img/tombstone.png", &tombstone_texture);
    load_img("../img/g2.png", &background_texture);
    load_img("../img/gameover2.png", &game_over_texture);
    load_img("../img/snake/head_up.png", &Snake_textures.head_up);
    load_img("../img/snake/head_left.png", &Snake_textures.head_left);
    load_img("../img/snake/head_right.png", &Snake_textures.head_right);
    load_img("../img/snake/head_down.png", &Snake_textures.head_down);
    load_img("../img/snake/body_vertical.png", &Snake_textures.body_vertical);
    load_img("../img/snake/body_horizontal.png", &Snake_textures.body_horizontal);
    load_img("../img/snake/body_bl.png", &Snake_textures.body_bl);
    load_img("../img/snake/body_br.png", &Snake_textures.body_br);
    load_img("../img/snake/body_tl.png", &Snake_textures.body_tl);
    load_img("../img/snake/body_tr.png", &Snake_textures.body_tr);
    load_img("../img/snake/tail_up.png", &Snake_textures.tail_up);
    load_img("../img/snake/tail_down.png", &Snake_textures.tail_down);
    load_img("../img/snake/tail_left.png", &Snake_textures.tail_left);
    load_img("../img/snake/tail_right.png", &Snake_textures.tail_right);

    glutDisplayFunc(display);

    glutTimerFunc(0, Timer, 0);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    init_m();

    logger("INFO", "Game started!");
    glutMainLoop();
}



