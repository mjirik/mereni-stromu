# Toto je makefile k programu pro komunikaci se zařízením pro měření objemu
# dřeva na pile v Němčicích


CC = gcc # jmeno prekladace
CFLAGS = -ggdb3 -o pila_connector -Wall
LFLAGS = -c -Wall

# jméno hlavního .c souboru
CMAIN = main.c

# seznam .c souboru bez hlavniho
CFILES = conf.c serial.c mysql.c hardware_config.c data.c log.c

# seznam .o souborů bez hlavního
OFILES = conf.o mysql.o serial.o hardware_config.o data.o log.o

#cesta k mysql_config (Mandrake, Red Hat)
CFG = /usr/bin/mysql_config
#CFG = /usr/local/mysql/bin/mysql_config

all: pila

#	$(CC) $(OFILES) $(CMAIN) $(CFLAGS)

pila: $(OFILES) 
	sh -c "$(CC) $(CFLAGS) `$(CFG) --cflags` $(OFILES) $(CMAIN) `$(CFG) --libs`"
	mv -f ./pila_connector ../bin/pila_connector
	rm *.o

conf.o: conf.c
	$(CC) $(LFLAGS) conf.c

mysql.o: mysql.c
	$(CC) $(LFLAGS) mysql.c