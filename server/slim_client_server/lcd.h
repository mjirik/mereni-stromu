// lcd.c
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
extern void printxyd(int y, int x, char * s);
extern void clear_dscreen(void);
extern void printxyd_P(int y,int x,void * s);
//void printnt_P(char * s);
extern void lcd_w_inst(char c);
extern unsigned int zmena_na_lcd;

extern char (*p_akt_screen)[LCD_X_MAX];
// Je to sice trochu divn�, ale funguje to.
// P�edstavuje to ukazatel na pole 20x4 nebo sp� 20xN
// Je to proto, �e ten ukazatel ukazuje na pole ukazatel� na dvacetice znak� char. viz Herout str 218.

extern char default_screen[LCD_Y_MAX][LCD_X_MAX];

