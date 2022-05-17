#include <GL/glut.h>
#include <stdlib.h>
#include "input_controller.h"
#include "../logic/game.h"
#include "../util/logger.h"
#include "../menu/menu.h"

void specialKeys(int key, int x, int y) {
    if(get_game_state() == MENU){
        keyboard_fun(key,x,y);
        return;
    }
    switch (key) {
        case GLUT_KEY_UP:
            logger("INFO", "Key: UP");
            change_direction(UP);
            break;
        case GLUT_KEY_DOWN:
            logger("INFO", "Key: DOWN");
            change_direction(DOWN);
            break;
        case GLUT_KEY_LEFT:
            logger("INFO", "Key: LEFT");
            change_direction(LEFT);
            break;
        case GLUT_KEY_RIGHT:
            logger("INFO", "Key: RIGHT");
            change_direction(RIGHT);
            break;
    }
}

void mouse(int button, int state, int x, int y) {

}

void keyboard(unsigned char key, int x, int y) {
    if(get_game_state() == MENU){
        process_normal_keys(key,x,y);
        return;
    }

    switch (key) {
        case 112:
            logger("INFO", "KEY: p");
            pause_game();
            break;
        case 97:
            sh_menu();
            break;
        case 100:
            break;
        case 27: // escape
            logger("INFO", "Game closed");
            exit(0);
        default:
            break;
    }
}