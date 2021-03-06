#define NAME_LENGTH 10


typedef struct{
  int id;
  int id_mysql;
  char *jmeno;

}ZAKAZNIK;

typedef struct{
  int id;
  int id_mysql;
  float prumer;
  float delka;
  int druh_drev;
  int operace;
  int manipulace;
  int dvakrat;
}ZAZNAM;

void del_zakaznici(void);
void clr_zakaznik (int i);
char add_zakaznik(int id, int id_mysql, char*jmeno);
char del_zakaznik(int id);
ZAKAZNIK get_zakaznik(int id);
void print_zakaznici(void);

void clr_zaznam (int i);


#define MAX_ZAKAZNIK 50
#define MAX_ZAZNAM 300

ZAKAZNIK zakaznici[MAX_ZAKAZNIK];
ZAZNAM zaznamy[MAX_ZAZNAM];
int max_zakaznik;
int max_zaznam;
