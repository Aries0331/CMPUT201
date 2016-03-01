#ONE Card number: 1461696
#Computing Science UNIX ID: jinzhu
#lecture section: A2
#lab section: D01
#TA's name: Chenyang Huang  

CFLAGS = -Wall -std=c99 -g

draw_stl: set_curses.o lander.o
	  gcc $(CFLAGS) -o lander set_curses.o lander.o -lm -lcurses 

set_curses.o: set_curses.c set_curses.h
	      gcc $(CFLAGS) -c set_curses.c 

draw_stl.o: lander.c lander.h set_curses.h
	    gcc $(FLAGS) -c lander.c 

clean : 
	-rm -f *.o draw_stl core
