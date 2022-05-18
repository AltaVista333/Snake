#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"


#define DELIMETR_SIZE 1
#define BTN_MENU_CNT 9
#define WINDOW_MENU_CNT 7
#define MAX_ITEMS_COUNT 50
#define MAX_LINE_LENGTH 200
#define MAX_LINE_COUNT 200

typedef enum {
  BTN_POS_X = 1,
  BTN_POS_Y,
  BTN_CLR,
  BTN_HGHL_CLR,
  BTN_NAME,
  BTN_WIDTH,
  BTN_HEIGHT
} Btn;

typedef enum {
  WINDOW_POS_X = 1,
  WINDOW_POS_Y,
  WINDOW_CLR,
  WINDOW_WIDTH,
  WINDOW_HEIGHT
} Wndw;

char* buttons[] = {"ButtonBegin", "Position_x", "Position_y", "Color", "HighlightColor", "Name","Width","Height", "ButtonEnd"};
char* windows[] = {"WindowBegin", "Position_x", "Position_y","Color","Width","Height", "WindowEnd"};
int check_menu_item(char** to_check,int pos, char** checker_arr, int arr_size);
void check_begin_and_end (char **arr, int cnt);
int check_windows_and_buttons (char **arr, int cnt, int*items_pos);
void parse_items_to_struct (char **items_arr, int cnt, int* item_pos, Menu *menu);
void add_window (Menu *menu, char **items_arr, int i);
void add_button (Menu *menu, char **items_arr, int i);


void convert_hex_to_color_struct (char* arr,Color* color){
  //string example #ffaaff
  if(strlen (arr) != 7){
	printf("Wrong color format");
	return;
  }
  	char buff[2] = {0};
	color->r = hex2int (memcpy(buff, &arr[1],2));
	color->g = hex2int (memcpy(buff, &arr[3],2));
	color->b = hex2int (memcpy(buff, &arr[5],2));
}

int read_file(char* path, char** res) {
  FILE *file = fopen(path, "r");
  char tmp[MAX_LINE_LENGTH] = {0};
  int counter = 0;
  if (!file) {
	printf ("Wrong path");
	exit(1);
  }
  while (fgets(tmp, MAX_LINE_LENGTH, file)) {
	size_t line_cnt = strlen(tmp) - 1;
	char line[line_cnt];
	remove_spaces (line, tmp);
	res[counter] = malloc(line_cnt * sizeof (char));
	strcpy(res[counter++], line);

  }

  if (fclose(file)) {
	printf ("Can't close file");
	exit (1);
  }
  return counter;
}

int check_windows_and_buttons (char **arr, int cnt, int* items_pos)
{
  int pos = 1;
  int item_counter = 0;
  while (pos < cnt - 1) {
	if(strcmp (arr[pos], windows[0]) == 0){
	  items_pos[item_counter++] = pos;
	  pos += check_menu_item(arr, pos, windows, WINDOW_MENU_CNT);
	  continue;
	}
	else if(strcmp(arr[pos], buttons[0]) == 0){
	  items_pos[item_counter++] = pos;
	  pos += check_menu_item(arr, pos, buttons, BTN_MENU_CNT);
	  continue;
	}
	else if(pos != cnt - 1){
	  return -1;
	}
  }
  return item_counter;
}

int check_menu_item(char** to_check,int pos, char** checker_arr, int arr_size){
  for (int i = 0; i < arr_size; ++i){
	if(strncmp (to_check[pos + i], checker_arr[i], strlen(checker_arr[i])) != 0){
	  return -1;
	}
  }
  return arr_size;
}
void check_begin_and_end (char **arr, int cnt)
{
  if(!strcmp (arr[0], "MenuBegin") && !strcmp(arr[cnt - 1], "MenuEnd")){
	return;
  }
  else{
	printf("Wrong format! MenuBegin or MenuEnd missing");
	exit(1);
  }
}

void menu_parser(char* path, Menu* menu){
  char** arr = (char**)malloc (sizeof(char*) * MAX_LINE_COUNT);
  int line_cnt = read_file (path, arr);
  check_begin_and_end(arr, line_cnt);
  int item_pos[MAX_ITEMS_COUNT];
  int items_cnt = check_windows_and_buttons(arr, line_cnt, item_pos);
  if(items_cnt == -1){
	printf("wrong format");
	exit (1);
  }
  menu->buttons = malloc (sizeof(Button) * items_cnt);
  menu->buttons_cnt = 0;
  menu->windows = malloc (sizeof(Window) * items_cnt);
  menu->windows_cnt = 0;
  parse_items_to_struct(arr, items_cnt, item_pos, menu);
}

void parse_items_to_struct (char **items_arr, int cnt, int* item_pos, Menu *menu){
  for (int i = 0; i < cnt; ++i){
	if(strcmp ((items_arr[item_pos[i]]), windows[0]) == 0){
	  add_window(menu, items_arr, item_pos[i]);
	}
	if(strcmp ((items_arr[item_pos[i]]), buttons[0]) == 0){
	  add_button(menu, items_arr, item_pos[i]);
	}
  }
}
void add_button (Menu *menu, char **items_arr, int i)
{
  Button* btn = malloc (sizeof (Button));
  btn->name = malloc (sizeof (char) * 15);
  btn->coord.x = strtol (items_arr[i + BTN_POS_X] + strlen(buttons[BTN_POS_X]) + DELIMETR_SIZE, NULL, 10);
  btn->coord.y = strtol (items_arr[i + BTN_POS_Y] + strlen(buttons[BTN_POS_Y]) + DELIMETR_SIZE, NULL, 10);
  convert_hex_to_color_struct (items_arr[i + BTN_CLR] + strlen (buttons[BTN_CLR]) + DELIMETR_SIZE, &btn->color);
  convert_hex_to_color_struct (items_arr[i + BTN_HGHL_CLR] + strlen (buttons[BTN_HGHL_CLR]) + DELIMETR_SIZE, &btn->high_light_color);
  strcpy (btn->name, items_arr[i + BTN_NAME] + strlen(buttons[BTN_NAME]) + DELIMETR_SIZE);
  btn->width = strtol (items_arr[i + BTN_WIDTH] + strlen(buttons[BTN_WIDTH]) + DELIMETR_SIZE, NULL, 10);
  btn->height = strtol (items_arr[i + BTN_HEIGHT] + strlen(buttons[BTN_HEIGHT]) + DELIMETR_SIZE, NULL, 10);
  menu->buttons[menu->buttons_cnt++] = *btn;

}
void add_window (Menu *menu, char **items_arr, int i)
{
  Window* window = malloc (sizeof (Window));
  window->coord.x = strtol (items_arr[i + WINDOW_POS_X] + strlen(windows[WINDOW_POS_X]) + DELIMETR_SIZE, NULL, 10);
  window->coord.y = strtol (items_arr[i + WINDOW_POS_Y] + strlen(windows[WINDOW_POS_Y]) + DELIMETR_SIZE, NULL, 10);
  convert_hex_to_color_struct (items_arr[i + WINDOW_CLR] + strlen (windows[WINDOW_CLR]) + DELIMETR_SIZE, &window->color);
  window->width = strtol (items_arr[i + WINDOW_WIDTH] + strlen(windows[WINDOW_WIDTH]) + DELIMETR_SIZE, NULL, 10);
  window->height = strtol (items_arr[i + WINDOW_HEIGHT] + strlen(windows[WINDOW_HEIGHT]) + DELIMETR_SIZE, NULL, 10);
  menu->windows[menu->windows_cnt++] = *window;
}
