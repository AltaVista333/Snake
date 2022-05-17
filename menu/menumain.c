#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "parser.h"
#include "../logic/game.h"

void timer(int value_ms) {
  glutPostRedisplay();
  glutTimerFunc(value_ms, timer, 0);
}

void do_something_with_result (char* result)
{
    printf("%s", result);
    if(strcmp(result, "Start") == 0){
        start_game();
    } else{
        end_game();
    }
}

void do_something_when_done(){
  glClearColor(1, 1, 0, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
}

void display_function (){

  SystemUpdate (do_something_with_result, do_something_when_done);

}



