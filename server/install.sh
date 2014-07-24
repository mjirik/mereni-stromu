#!/bin/sh

# Zajistit spouštìcí práva k tomuto skriptu:
# chmod +x install.sh

#web
echo "Kopiovani webove casti"
cp -f -R web/* /var/www/html

#db
echo "Instalace databaze"
# mysql --user=root --password=moje.heslo < ./db/start.sql
mysql -u root -p < ./db/start.sql
echo "Heslo MySQL bylo zmeneno"

#connector
echo "Nastaveni spoustecich prav k prislusnym souborum"
chmod +x ./init/pila_connector.sh
chmod +x ./connector/bin/pila_connector
chmod +x-wr ./connector/bin/pila_connector.pst_ld_scr.sh
echo "Kopirovani binarek pila_connector"
cp -f ./connector/bin/pila_connector /usr/local/bin/pila_connector
echo "Kopirovani zalohovaciho skriptu"
cp -f ./connector/bin/pila_connector.pst_ld_scr.sh /usr/local/bin/pila_connector.pst_ld_scr.sh
echo "Kopirovani konfiguracniho souboru connectoru"
cp -f ./connector/etc/pila_connector /usr/local/etc/pila_connector

#start
echo "Zajisteni automatickeho spousteni pri startu"
cp ./init/pila_connector.sh /etc/init.d/pila_connector.sh
ln -s -f /etc/init.d/pila_connector.sh /etc/rc5.d/S98pila_connector.sh
