#! /bin/sh
# skript slouzi pro startovani systemu pro naslouchani seriovemu portu a zapisovani
# dat do MySQL
# echo "novy start" >> /root/tmp/file.txt

stty -F /dev/ttyS0 9600

stty -F /dev/ttyS0 clocal cread -crtscts cs8 cstopb hup -parenb parodd \
  -brkint -icrnl ignbrk -igncr ignpar imaxbel -inlcr inpck -istrip \
	-iuclc -ixany ixoff -ixon bs0 cr0 ff0 nl0 -ocrnl -ofdel -ofill -olcuc \
	-onlcr -onlret onocr -opost tab0 vt0 -crterase crtkill -ctlecho -echo \
	-echok -echonl -echoprt -icanon -iexten -isig -noflsh -tostop -xcase \
	time 5 min 1


/usr/local/bin/pila_connector&

#echo "konec startu" >> /root/tmp/file.txt
