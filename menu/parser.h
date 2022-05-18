#pragma once

#ifndef _PARSER_H_
#define _PARSER_H_
#include "menu.h"
#include "util.h"
typedef struct {
  int x;
  int y;
} MenuPoint;

typedef struct {
  int r;
  int g;
  int b;
} Color;

typedef struct {
  MenuPoint coord;
  Color color;
  int width;
  int height;
} Window;

typedef struct {
  MenuPoint coord;
  Color color;
  Color high_light_color;
  char* name;
  int width;
  int height;
} Button;

typedef struct {
  int windows_cnt;
  int buttons_cnt;
  Window* windows;
  Button* buttons;
} Menu;

void menu_parser(char* path, Menu* menu);
#endif//_PARSER_H_
