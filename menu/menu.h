#include "parser.h"
#ifndef _MENU_H_
#define _MENU_H_


void SystemOpen();
void show_menu();
void SystemUpdate ( void (*callback_with_res)(char* res),void (*callback_when_done)());
void SystemInitialize(char* path);
void keyboard_fun(int keycode, int x, int y);
void process_normal_keys(unsigned char key, int x, int y);
#endif//_MENU_H_
