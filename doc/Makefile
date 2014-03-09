# Makefile generuje automatickou dokumentaci pro projekt mìøení objemu stromù.
all:  main

./web/web.tag: web.conf
	doxygen web.conf

./vozik/vozik.tag: vozik.conf
	doxygen vozik.conf

./katr/katr.tag: katr.conf
	doxygen katr.conf

main: ./web/web.tag ./vozik/vozik.tag ./katr/katr.tag main.conf
	doxygen main.conf
