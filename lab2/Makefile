#name: Jinzhu Li
#ONE Card number:1461696
#Computing Science UNIX ID: jinzhu
#lecture section: A2
#lab section: D01
#TA's name: Chenyang Huang

CFLAGS = -Wall -std=c99 -g

all: draw2


# main program, uses ll_draw2 module
main : main.o ll_draw2.o
	gcc $(CFLAGES) main.o ll_draw2.o -lm -o main

main.o: main.c ll_draw2.h
	gcc $(CFLAGES) -c main.c 

# test program for checking correctness of ll_draw2 module
draw2: draw2.o ll_draw2.o
	gcc $(CFLAGS) draw2.o ll_draw2.o -lm -o draw2

draw2.o: draw2.c draw2.h
	gcc $(CFLAGS) -c draw2.c -D_BSD_SOURCE


# ll_draw2 module
ll_draw2.o: ll_draw2.c draw2.h
	gcc $(CFLAGS) -c ll_draw2.c -D_BSD_SOURCE


# remove any compiled files, and any core files
clean : 
	-rm -f *.o draw2 core

