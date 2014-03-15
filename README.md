mereni-stromu
=============

Mereni reziva pred vstupem do katru





## Testování funkčnosti


Zapojení:

 * Katr
 * Kabel RS422 (s napájením pro převodník ve dvou vodičích)
 * Převodník RS422/RS232
 * Křížený kabel RS232
 * Převodník RS232/USB



Nastavení sériového portu 2 stop bity, žádná parita

    stty -F /dev/ttyUSB0 clocal cread -crtscts cs8 cstopb hup -parenb parodd -brkint -icrnl ignbrk -igncr ignpar imaxbel -inlcr inpck -istrip -iuclc -ixany ixoff -ixon bs0 cr0 ff0 nl0 -ocrnl -ofdel -ofill -olcuc -onlcr -onlret onocr -opost tab0 vt0 -crterase crtkill -ctlecho -echo -echok -echonl -echoprt -icanon -iexten -isig -noflsh -tostop -xcase time 5 min 1    

Nastavení rychlosti komunikace

    stty -F /dev/ttyUSB0 9600
    
Čtení z portu

    cat /dev/ttyUSB0 
    
Při pootočení inkrementálního čidla by měla přicházet data
