#pragma once

#ifndef SNAKE_2_INPUT_CONTROLLER_H
#define SNAKE_2_INPUT_CONTROLLER_H
#include "../util/logger.h"
#include "../menu/menu.h"
#include "../model/game.h"
void specialKeys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
#endif //SNAKE_2_INPUT_CONTROLLER_H
