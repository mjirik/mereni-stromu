// lcd.c
#define LCD_X_MAX 40
#define LCD_Y_MAX 4
#define DVA_E
void lcd_init(void);
void lcd_control(void);
void printxy(char (*screen)[LCD_X_MAX], int x, int y,char * s);
void printc(char c);
void print(char * s);
void clear_screen(char (*screen)[LCD_X_MAX]);
char lcd_new_frame(void);
void printcxyd(int x, int y, char c);
void printxyd(int y, int x, char * s);
void clear_dscreen(void);
void printxyd_P(int y,int x,void * s);
void printnt_P(char * s);
void lcd_w_inst(char c);

char (*p_akt_screen)[LCD_X_MAX];
// Je to sice trochu divný, ale funguje to.
// Pøedstavuje to ukazatel na pole 20x4 nebo spíš 20xN
// Je to proto, že ten ukazatel ukazuje na pole ukazatelù na dvacetice znakù char. viz Herout str 218.

char default_screen[LCD_Y_MAX][LCD_X_MAX];

