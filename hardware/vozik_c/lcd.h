// lcd.c
#ifndef __LCD_H__
#define __LCD_H__

#define LCD_X_MAX 40
#define LCD_Y_MAX 4
#define DVA_E

extern void lcd_init(void);
extern void lcd_control(void);
extern void printxy(char (*screen)[LCD_X_MAX], int x, int y,char * s);
extern void printc(char c);
extern void print(char * s);
extern void clear_screen(char (*screen)[LCD_X_MAX]);
extern char lcd_new_frame(void);
extern void printcxyd(int x, int y, char c);
/// Vytiskne n znaků do defaultscreenu na souřadnici x,y.
extern void printnxyd(int y, int x, char * s, int n);
extern void printxyd(int y, int x, char * s);
extern void clear_dscreen(void);
extern void printxyd_P(int y,int x,void * s);
extern void printnt_P(char * s);
extern void lcd_w_inst(char c);
extern void printnt(char *s);

extern char (*p_akt_screen)[LCD_X_MAX];
// Je to sice trochu divný, ale funguje to.
// Představuje to ukazatel na pole 20x4 nebo spíš 20xN
// Je to proto, že ten ukazatel ukazuje na pole ukazatelů na dvacetice znaků char. viz Herout str 218.

extern char default_screen[LCD_Y_MAX][LCD_X_MAX];

#endif
