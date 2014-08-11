#!/bin/sh

# Zajistit spouštìcí práva k tomuto skriptu:
# chmod +x install.sh

#web
echo "Kopiovani webove casti"
sudo ln -s ~/mereni-stromu/server/web/ /var/www/mereni-stomu 
#sudo cp -f -R web/* /var/www/html
sudo rm /var/www/html/index.html

#db
echo "Instalace databaze"
# mysql --user=root --password=moje.heslo < ./db/start.sql
mysql -u root -p < ./db/start.sql
echo "Heslo MySQL bylo zmeneno, pokud bylo zadano spravne"

#connector
echo "Nastaveni spoustecich prav k prislusnym souborum"
chmod +x ./init/pila_connector.sh
chmod +x ./connector/bin/pila_connector
chmod +xr ./connector/bin/pila_connector.pst_ld_scr.sh
echo "Kopirovani binarek pila_connector"
sudo cp -f ./connector/bin/pila_connector /usr/local/bin/pila_connector
echo "Kopirovani zalohovaciho skriptu"
sudo cp -f ./connector/bin/pila_connector.pst_ld_scr.sh /usr/local/bin/pila_connector.pst_ld_scr.sh
echo "Kopirovani konfiguracniho souboru connectoru"
sudo cp -f ./connector/etc/pila_connector /usr/local/etc/pila_connector

#start
echo "Zajisteni automatickeho spousteni pri startu"
sudo cp ./init/pila_connector.sh /etc/init.d/pila_connector.sh
sudo ln -s -f /etc/init.d/pila_connector.sh /etc/rc5.d/S98pila_connector.sh
