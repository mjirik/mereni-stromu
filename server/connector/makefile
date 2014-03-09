# Toto je makefile k programu pro komunikaci se zařízením pro měření objemu
# dřeva na pile v Němčicích


CC = gcc # jmeno prekladace
# -ggdb3
CFLAGS = -ggdb3 -o pila_connector -Wall -DMODUL_NOTALL -DWIN32
LFLAGS = -c -Wall

#napojení na databázi
DATABASE = Off
#Sériová komunikace
COMMUNICATION = On
# kofigurace hardware - asi zastaralá
HARDWARECFG = Off


# seznam .c souboru bez hlavniho
CFILES = conf.c  data.c log.c

# seznam .o souborů bez hlavního
OFILES = conf.o hardware_config.o data.o log.o


# přepínání jednotlivých modulů
ifeq ($(DATABASE),On)
#echo "databáze"
CFLAGS +=  -DMODUL_DATABASE
CFILES +=  mysql_connect.c
OFILES +=  mysql_connect.o
endif

ifeq ($(COMMUNICATION),On)
#echo "komunikace"
CFLAGS+=  -DMODUL_COMMUNICATION
CFILES+=  serial.c
OFILES+=  serial.o
endif

ifeq ($(HARDWARECFG),On)
CFLAGS+=  -DMODUL_HARDWARE
CFILES+=  hardware_config.c
OFILES+=  hardware_config.o
endif

# jméno hlavního .c souboru
CMAIN = main.c





#cesta k mysql_config (Mandrake, Red Hat)
CFGI = /usr/bin/mysql_config
CFGL = /usr/bin/mysql_config

#CFG = /usr/local/mysql/bin/mysql_config

# windows
CFGI = "C:\Program Files\MySQL\MySQL_Connector_C_6.0.2\include"
CFGL = "C:\Program Files\MySQL\MySQL_Connector_C_6.0.2\lib"

all: pila

#	$(CC) $(OFILES) $(CMAIN) $(CFLAGS)

pila: $(OFILES) 
	sh -c "$(CC) $(CFLAGS) `$(CFGI) --cflags` $(OFILES) $(CMAIN) `$(CFGL) --libs`"
	#mv -f ./pila_connector ../bin/pila_connector
	#rm *.o

.o: $(CFILES)
	$(CC) $(LFLAGS) -I$(CFGI) *.c

#conf.o: conf.c
#	$(CC) $(LFLAGS) conf.c

#mysql_connect.o: mysql_connect.c
#	$(CC) $(LFLAGS) -I$(CFGI) mysql_connect.c