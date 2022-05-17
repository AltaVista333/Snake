#include <windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <stb_easy_font.h>
#include "menu.h"
#define MAX_NAME_LENGTH 20

typedef enum {
  READY_FOR_PARSE,
  READY_FOR_SHOW,
  PENDING,
  DONE,
  SYSTEM_CLOSE
} MENU_STATE;



void move_focus(int i);
int find_btn_in_focus();
int menu_add_button(char* name, float x, float y, float width, float height, float text_scale, Color color,Color hglt_color);
int menu_add_window(float x, float y, float width, float height,Color color);
MENU_STATE get_state();


typedef struct {
  char name[MAX_NAME_LENGTH];
  char is_highlight;
  float vert[8];
  Color color;
  Color highlight_color;
  float buffer[50 * MAX_NAME_LENGTH];
  int num_quads;
  float text_pos_x, text_pos_y, text_scale;
} Button_draw;

typedef struct {
  float vert[8];
  Color color;
} Window_draw;

MENU_STATE st;
char* result;
Button_draw* btn = 0;
int btn_cnt = 0;
Window_draw* wndw = 0;
int wndw_cnt = 0;

void init_menu(Menu* menu){
  for (int i = 0; i < menu->windows_cnt; ++i) {
	menu_add_window (menu->windows[i].coord.x,
					 menu->windows[i].coord.y,
					 menu->windows[i].width,
					 menu->windows[i].height,
					 menu->windows[i].color
	);
  }
  for (int i = 0; i < menu->buttons_cnt; ++i)
  {
	menu_add_button (menu->buttons[i].name,
					 menu->buttons[i].coord.x,
					 menu->buttons[i].coord.y,
					 menu->buttons[i].width,
					 menu->buttons[i].height,
					 2,
					 menu->buttons[i].color,
					 menu->buttons[i].high_light_color
	);
  }
}


void show_window(int window_id){
  Window_draw* wndw1 = wndw + window_id;
  glVertexPointer (2, GL_FLOAT, 0, wndw1->vert);
  glEnableClientState (GL_VERTEX_ARRAY);
  glColor3ub(wndw1->color.r,wndw1->color.g,wndw1->color.b);
  glDrawArrays (GL_TRIANGLE_FAN, 0, 4);
  glColor3f (1,1,1);
  glLineWidth (2);
  glDrawArrays (GL_LINE_LOOP,0,4);
  glDisableClientState (GL_VERTEX_ARRAY);
}

void show_button(int button_id){
  Button_draw* btn1 = btn + button_id;
  glVertexPointer (2, GL_FLOAT, 0, btn1->vert);
  glEnableClientState (GL_VERTEX_ARRAY);
  if(btn1->is_highlight){
	glColor3ub (btn1->highlight_color.r, btn1->highlight_color.g,btn1->highlight_color.b);
  }
  else{
	glColor3ub(btn1->color.r,btn1->color.g,btn1->color.b);
  }
  glDrawArrays (GL_TRIANGLE_FAN, 0, 4);
  glColor3ub (0,0,255);
  glLineWidth (1);
  glDrawArrays (GL_LINE_LOOP,0,4);
  glDisableClientState (GL_VERTEX_ARRAY);

  glPushMatrix();
  glColor3ub (0,0,0);
  glTranslatef(btn1->text_pos_x,btn1->text_pos_y,0);
  glScalef (btn1->text_scale, btn1->text_scale,1);
  glEnableClientState (GL_VERTEX_ARRAY);
  glVertexPointer (2, GL_FLOAT, 16, btn1->buffer);
  glDrawArrays (GL_QUADS, 0 ,btn1->num_quads * 4);
  glDisableClientState (GL_VERTEX_ARRAY);
  glPopMatrix();
}

void show_menu(){

  glClearColor(0, 0, 0, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glPushMatrix();
  glOrtho (0, 300,300,0,1,-1);
  for (int i = 0; i < wndw_cnt; ++i){
	show_window(i);
  }
  for (int i = 0; i < btn_cnt; ++i){
	show_button (i);
  }
  glPopMatrix();
}
void process_normal_keys(unsigned char key, int x, int y)
{
  switch (key)
  {
	  //enter
	case 13:
	  result = btn[find_btn_in_focus()].name;
	  st = PENDING;
	  break;
	default:
	  break;
  }
}
void keyboard_fun(int keycode, int x, int y)
{
  switch (keycode)
  {
	case GLUT_KEY_UP:
	  move_focus(-1);
	  break;
	case GLUT_KEY_DOWN:
	  move_focus(1);
	  break;
	default:
	  break;
  }
}
int find_btn_in_focus(){
  for (int i = 0; i < btn_cnt; ++i) {
	if(btn[i].is_highlight){
	  return i;
	}
  }
}
void move_focus (int i)
{
  int current = find_btn_in_focus();
  btn[current].is_highlight = 0;
  if((current + i) < 0){
	btn[btn_cnt - 1].is_highlight = 1;
  }
  else if((current + i) > btn_cnt - 1){
	btn[0].is_highlight = 1;
  }
  else{
	btn[current + i].is_highlight = 1;
  }
}
int menu_add_button (char *name, float x, float y, float width, float height, float text_scale, Color color, Color hglt_color)
{
  btn_cnt++;
  btn = realloc (btn, sizeof (Button_draw) * btn_cnt);
  snprintf (btn[btn_cnt - 1].name, MAX_NAME_LENGTH, "%s", name);
  float* vert = btn[btn_cnt - 1].vert;
  vert[0] = vert[6] = x;
  vert[2] = vert[4] = x + width;
  vert[1] = vert[3] = y;
  vert[5] = vert[7] = y + height;
  btn[btn_cnt - 1].color = color;
  btn[btn_cnt - 1].highlight_color = hglt_color;
  btn[btn_cnt - 1].is_highlight = btn_cnt == 1 ? 1 : 0;

  Button_draw *btns = btn + btn_cnt - 1;
  btns->num_quads = stb_easy_font_print (0,0, name,0, btns->buffer, sizeof(btns->buffer));
  btns->text_pos_x = x + (width - stb_easy_font_width(name) * text_scale) / 2.0;
  btns->text_pos_y = y + (height - stb_easy_font_height(name) * text_scale) / 2.0;
  btns->text_pos_y += text_scale * 2;
  btns->text_scale = text_scale;
  return btn_cnt - 1;
}
int menu_add_window (float x, float y, float width, float height, Color color)
{
  wndw_cnt++;
  wndw = realloc (wndw, sizeof (Window_draw ) * wndw_cnt);
  float* vert = wndw[wndw_cnt - 1].vert;
  vert[0] = vert[6] = x;
  vert[2] = vert[4] = x + width;
  vert[1] = vert[3] = y;
  vert[5] = vert[7] = y + height;
  wndw[wndw_cnt - 1].color = color;
  return wndw_cnt - 1;
}
MENU_STATE get_state(){
  return st;
}

char* get_result(){
  st = DONE;
  return result;
}

void SystemRelease(){
  free(result);
  free(btn);
  free(wndw);
  st = READY_FOR_PARSE;
}

void SystemOpen(){
  if(st == SYSTEM_CLOSE){
	printf("The system was closed.");
  }
  printf ("System Open");
  st = READY_FOR_PARSE;
}

void SystemClose(){
  SystemRelease();
  printf("The system was closed");

}

void SystemInitialize(char* path){
  printf ("Initializing");
  if(st != READY_FOR_PARSE){
	printf("Open or Release system, before init");
	return;
  }

  Menu* menu = malloc (sizeof(Menu));
  menu_parser (path, menu);
  init_menu (menu);
  st = READY_FOR_SHOW;
}

void SystemUpdate ( void (*callback_with_res)(char* res),void (*callback_when_done)()){
  if(st == SYSTEM_CLOSE){
	printf("The system was closed");
	return;
  }
  if(st == READY_FOR_SHOW){
	show_menu();
  }  else if(get_state() == PENDING) {
	if(callback_with_res == NULL){
	  printf("Wrong callback");
	  exit(1);
	}
	callback_with_res (get_result());
	st = DONE;
  } else if(st == DONE){
	if(callback_when_done == NULL){
	  printf("Wrong callback");
	  exit(1);
	}
	callback_when_done();
  }

}
