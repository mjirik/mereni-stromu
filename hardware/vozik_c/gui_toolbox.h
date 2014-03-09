// gui_toolbox.c
#include "lcd.h"
void read_line_init(int y, int x, char *s);
void read_line_init_edefault(int y, int x, char *s,char len);

char *read_line (void);
void read_line_kill(void);

typedef struct menu_item{
  uint8_t x;
  uint8_t y;
  uint8_t len;
  char *(*polozky);
  uint8_t val;
}MENU_ITEM;


MENU_ITEM *new_menu_item(void);
void menu_draw(MENU_ITEM *menu[], uint8_t len, char i);
//void menu_draw(MENU_ITEM menu, uint8_t len);

char menu_next_item(MENU_ITEM *menu[], char i, char max_i);
char menu_prev_item(MENU_ITEM *menu[], char i, char max_i);
void menu_next_value(MENU_ITEM *[], char i);



