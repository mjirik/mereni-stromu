#! /bin/sh
# Skript je provádìn po úspìšném naètení dat ze sériového portu do databáze.
# Je urèen pro zálohování dat.

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

