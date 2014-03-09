# Nastavení sériového portu 2 stop bity, žádná parita

stty -F /dev/ttyS1 clocal cread -crtscts cs8 cstopb hup -parenb parodd
-brkint -icrnl ignbrk -igncr ignpar imaxbel -inlcr inpck -istrip
-iuclc -ixany ixoff -ixon bs0 cr0 ff0 nl0 -ocrnl -ofdel -ofill -olcuc
-onlcr -onlret onocr -opost tab0 vt0 -crterase crtkill -ctlecho -echo
-echok -echonl -echoprt -icanon -iexten -isig -noflsh -tostop -xcase
time 5 min 1    

#nastavení rychlosti komunikace
stty -F /dev/ttyS1 9600
