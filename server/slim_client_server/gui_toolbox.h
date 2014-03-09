// gui_toolbox.c
#include "lcd.h"
typedef struct menu_item{
  unsigned int x;
  unsigned int y;
  unsigned int len;
  char *(*polozky);
  unsigned int val;
}MENU_ITEM;


extern void read_line_init(int y, int x, char *s);
extern void read_line_init_edefault(int y, int x, char *s,char len);

extern char *read_line (void);
extern void read_line_kill(void);


extern MENU_ITEM *new_menu_item(void);
extern void menu_draw(MENU_ITEM *menu[], unsigned int len, char i);
//void menu_draw(MENU_ITEM menu, unsigned int len);

extern char menu_next_item(MENU_ITEM *menu[], char i, char max_i);
extern char menu_prev_item(MENU_ITEM *menu[], char i, char max_i);
extern void menu_next_value(MENU_ITEM *[], char i);

#define symbol_kurzoru '\377'

