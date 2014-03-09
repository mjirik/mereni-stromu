# Instalator serverove casti projektu mereni dreva pro firmu Hranola
# 
# Zastaralý. Zatím však zachován. Novìjší je skript install.sh
install: startup connector db web

web:
	echo "Kopiovani webove casti"
	@cp -f -R web /var/www/html
db:
	@echo "Instalace databaze"
	@mysql --user=root --password=moje.heslo < ./db/start.sql
connector:
	@echo "Nastaveni spoustecich prav k prislusnym souborum"
	@chmod +x ./init/pila_connector.sh
	@chmod +x ./connector/bin/pila_connector
	@chmod +x ./connector/bin/pila_connector.pst_ld_src.sh
	@echo "Kopirovani binarek pila_connector"
	@cp -f ./connector/bin/pila_connector /usr/local/bin/pila_connector
	@echo "Kopirovani zalohovaciho skriptu"
	@cp -f ./connector/bin/pila_connector.pst_ld_src.sh /usr/local/bin/pila_connector .pst_ld_src.sh
	@echo "Kopirovani konfiguracniho souboru connectoru"
	@cp -f ./connector/etc/pila_connector /usr/local/etc/pila_connector
startup:
	@echo "Zajisteni automatickeho spousteni pri startu"
	@cp ./init/pila_connector.sh /etc/init.d/pila_connector.sh
	@ln -s -f /etc/init.d/pila_connector.sh /etc/rc5.d/S98pila_connector.sh
