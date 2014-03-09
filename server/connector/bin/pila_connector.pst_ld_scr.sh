#! /bin/sh
# Skript je prov�d�n po �sp�n�m na�ten� dat ze s�riov�ho portu do datab�ze.
# Je ur�en pro z�lohov�n� dat.

mount /dev/sda /mnt/usbflash
if [ \( $? == 0 \) -o \( $? == 32 \) ]; 
    then
    mysqldump -u root -pmoje.heslo Pila > /mnt/usbflash/dbbackup.sql
    cd /mnt/usbflash
    zip -q -P rm5hj4hf /mnt/usbflash/dbbackup /mnt/usbflash/dbbackup.sql 
    rm -f /mnt/usbflash/dbbackup.sql 
    cd /
#    sleep 1s
    umount /mnt/usbflash/
fi

