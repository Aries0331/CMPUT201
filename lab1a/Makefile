#name: Jinzhu Li
#ONE Card number:1461696
#Computing Science UNIX ID: jinzhu
#lecture section: A2
#lab section: D01
#TA's name: Chenyang Huang

CFLAGS = -Wall -std=c99

draw1a : draw1a.o
	gcc $(CFLAGES) -o draw1a draw1a.o -lm
	fold -w 80 -b draw1a.c | diff - draw1a.c

draw1a.o : draw1a.c
	gcc $(CFLAGES) -c draw1a.c -g

clean : 
	-rm -f *.o draw1a core
