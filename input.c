#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "main.h"

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            move(UP);
            break;
        case GLUT_KEY_DOWN:
            move(Down);
            break;
        case GLUT_KEY_LEFT:
            move(Left);
            break;
        case GLUT_KEY_RIGHT:
            move(Right);
            break;
    }
}

void mouse(int button, int state, int x, int y) {

}

void keyboard(unsigned char key, int x, int y) {
    //printf("%d", key);
    switch (key) {
        case 112:
            pause();
            break;
        case 97:
            break;
        case 100:
            break;
        case 27: // escape
            printf("Game closed");
            exit(0);
            break;
            
    }
}