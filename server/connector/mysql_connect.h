void sql_params(char * this_user, char * this_psswd, char * this_db_name);
int sql_open(void);
int sql_query(char * s);
void insertZakaznik(int id, char * jmeno);
void insertMereni(int idzak, float delka, float prumer, int druh_dreva, int manipulace, int dalsi_zpracovani, int dvakrat );
void dataLoad(void);
void insertTimemark(void);
void sql_close(void);
