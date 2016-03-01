/*  ONE Card number: 1461696
 Computing Science UNIX ID: jinzhu
 lecture section: A2
 lab section: D01
 TA's name: Chenyang Huang  */

#include <assert.h>
#include <curses.h>
#include "set_curses.h"

#define _XOPEN_SOURCE 700

void init_ncurse()
{
    int r;
    initscr();
    r=cbreak(); assert(r != ERR);
    
    r=noecho(); assert(r != ERR);
    
    r=nonl(); assert(r != ERR);
    
    r=curs_set(0); assert(r != ERR);
    
    r=leaveok(stdscr, TRUE); assert(r != ERR);
    
    r=intrflush(stdscr, FALSE); assert(r != ERR);
    
    r=keypad(stdscr, TRUE); assert(r != ERR);

}

void shutdown_ncurse()
{
    endwin();
}



