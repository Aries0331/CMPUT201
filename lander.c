/*Computing Science UNIX ID: jinzhu
 lecture section: A2
 lab section: D01
 TA's name: Chenyang Huang  */

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "lander.h"
#include "set_curses.h"

int main(int argc, char *argv[])
{
    comm_line_argu(argc, argv);
    lander = popen("java -jar Sketchpad.jar", "w");
    if(input == NULL || Gravity == 0 || Thrust == 0) {
        printf("Need flags -g gravity,-t thrust -f landscape.txt\n");
        exit(EXIT_FAILURE);
    }
    else {
        ship_coordinates[0].x = 320;
        ship_coordinates[0].y = 1;
        ship_coordinates[1].x = 310;
        ship_coordinates[1].y = 5;
        ship_coordinates[2].x = 310;
        ship_coordinates[2].y = 29;
        ship_coordinates[3].x = 320;
        ship_coordinates[3].y = 19;
        ship_coordinates[4].x = 330;
        ship_coordinates[4].y = 29;
        ship_coordinates[5].x = 330;
        ship_coordinates[5].y = 5;
        
        thrust_coordinates[0].x = 310;
        thrust_coordinates[0].y = 38;
        thrust_coordinates[1].x = 320;
        thrust_coordinates[1].y = 29;
        thrust_coordinates[2].x = 330;
        thrust_coordinates[2].y = 38;
        
        center_x = 320;
        center_y = 15;
    
        new_velocity_x = new_velocity_y = 0;
        yA = gravity;
        angle_degrees = 90;
        angle_radians = angle_degrees * M_PI / 180.0;
    
        init_ncurse();
    
        mvprintw(5,10,"Press any key to start.");
        refresh();
        c = getch();  /* get the first character, the game starts */

        nodelay(stdscr,TRUE);
        
        erase();
        mvprintw(5,10,"(Press arrow keys to rotate,space for thrust,'q' to quit.)");
        refresh();
        
        drawLandscape(input, lander);
        draw_ship(lander);
        if(Improve)
            draw_fuel();
        
        // set up signal handler
        sigemptyset(&block_mask);
        sigaddset(&block_mask, SIGALRM);
        struct sigaction handler;
        handler.sa_handler = handle_timeout;
        sigemptyset(&handler.sa_mask);
        handler.sa_flags = 0;
        if(sigaction(SIGALRM, &handler, NULL) < 0) {
            exit(EXIT_FAILURE);
        }
        // set up alarm time
        struct itimerval timer;
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 50000;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 50000;
        if(setitimer(ITIMER_REAL, &timer, NULL) < 0) {
            exit(EXIT_FAILURE);
        }
        
        running = 1;
        do{
            c = getch();
            if(!running) {
                while(1) {
                    if((c = getch()) == 'q')
                        break;
                }
            }
            if(c == ERR)
                continue;
            erase();
            mvprintw(5,10,"left arrow key to rotate counter-closkwise,");
            mvprintw(6,10,"right arrow key to rotate clockwise, space for thrust, 'q' to qiut.");
            switch(c){
                case KEY_LEFT:      /* left arrow to rotate counter clockwise */
                    rotate_left();
                    break;
                case KEY_RIGHT:     /* right arow to rotate clockwise */
                    rotate_right();
                    break;
                case 'q':           /* 'q' to quit */
                    running = 0;
                    break;
                case ' ':           /* press space to activate thrust */
                    erase_thrust(lander);
                    active_thrust(lander);
                    break;
                default:
                    break;
            }
            refresh();
        }while(running);
        
        
        shutdown_ncurse();
        
        fprintf(lander, "end \n");
        fflush(lander);
        
        pclose(lander);
        fclose(input);
        
        return 0;
    }
    
}
/**************************************************
 * read from a text file containing the landscape *
 * information and draw the landscape as a series *
 * of line segments from the first to the second  *
 * point,the second to the third and so on.       *
 **************************************************/
void drawLandscape(FILE * input, FILE * lander)
{
    num = 0;
    int j;
    double x1,y1,x2,y2;
    line = malloc(sizeof(char)*100);

    while(fgets(line, 100, input) != NULL) {
        sscanf(line, "%lf %lf", &points[num].x, &points[num].y);
        num++;
    }
    for(j=0; j<num-1; j++){
        x1 = points[j].x;
        y1 = points[j].y;
        x2 = points[j+1].x;
        y2 = points[j+1].y;
        fprintf(lander, "drawSegment %ld %ld %ld %ld \n", lround(x1), lround(y1), lround(x2), lround(y2));
        fflush(lander);
    }
    fflush(lander);
    free(line);
}

/******************************************
*             draw the spacechip          *
*******************************************/
void draw_ship(FILE * lander)
{
    int m;
    for(m=0; m<5; m++) {
        fprintf(lander,"setColor 0 0 255\n");
        fprintf(lander, "drawSegment %ld %ld %ld %ld\n", lround(ship_coordinates[m].x), lround(ship_coordinates[m].y), lround(ship_coordinates[m+1].x), lround(ship_coordinates[m+1].y));
        fflush(lander);
    }
    fprintf(lander, "drawSegment %ld %ld %ld %ld\n", lround(ship_coordinates[5].x), lround(ship_coordinates[5].y), lround(ship_coordinates[0].x), lround(ship_coordinates[0].y));
    fflush(lander);
}

/******************************************
*             erase the spacechip         *
*******************************************/
void erase_ship(FILE * lander)
{
    int m;
    for(m=0; m<5; m++) {
        fprintf(lander, "eraseSegment %ld %ld %ld %ld\n", lround(ship_coordinates[m].x), lround(ship_coordinates[m].y), lround(ship_coordinates[m+1].x), lround(ship_coordinates[m+1].y));
        fflush(lander);
    }
    fprintf(lander, "eraseSegment %ld %ld %ld %ld\n", lround(ship_coordinates[5].x), lround(ship_coordinates[5].y), lround(ship_coordinates[0].x), lround(ship_coordinates[0].y));
    
    erase_thrust(lander);
    fflush(lander);
}

/******************************************
*             draw the thrust             *
*******************************************/
void draw_thrust(FILE * lander)
{
    int i;
    for(i=0;i<3;i++) {
        fprintf(lander,"setColor 255 0 0\n");
        fprintf(lander, "drawSegment %ld %ld %ld %ld\n", lround(ship_coordinates[2+i].x), lround(ship_coordinates[2+i].y), lround(thrust_coordinates[i].x), lround(thrust_coordinates[i].y));
        fflush(lander);
    }
}


/******************************************
*            erase the thrust             *
*******************************************/
void erase_thrust(FILE * lander)
{
    int i;
    for(i=0;i<3;i++) {
        fprintf(lander, "eraseSegment %ld %ld %ld %ld\n", lround(ship_coordinates[2+i].x), lround(ship_coordinates[2+i].y), lround(thrust_coordinates[i].x), lround(thrust_coordinates[i].y));
     fflush(lander);
    }
}

double rotate_radius_R = 10 * M_PI / 180.0;
double rotate_radius_L = 0 - 10 * M_PI / 180.0;

/*******************************************
* when the left error key pressed,the ship *
* rotate counter-clockwise                 *
********************************************/
void rotate_left()
{
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGALRM);
    if(sigprocmask(SIG_BLOCK, &block_mask, &old_mask) < 0) {
            exit(EXIT_FAILURE);
    }
    erase_ship(lander);
    angle_degrees -= 10;
    angle_radians = angle_degrees * M_PI / 180.0;
    double x_curr, y_curr;
    int i,j;
    
    for(i=0; i<6; i++) {
        x_curr = ship_coordinates[i].x - center_x;
        y_curr = ship_coordinates[i].y - center_y;
        ship_coordinates[i].x = x_curr * cos(rotate_radius_L) - y_curr * sin(rotate_radius_L);
        ship_coordinates[i].x += center_x;
        ship_coordinates[i].y = x_curr * sin(rotate_radius_L) + y_curr * cos(rotate_radius_L);
        ship_coordinates[i].y += center_y;
    }
    draw_ship(lander);
    
    for(j=0;j<3;j++) {
        x_curr = thrust_coordinates[j].x - center_x;
        y_curr = thrust_coordinates[j].y - center_y;
        thrust_coordinates[j].x = x_curr * cos(rotate_radius_L) - y_curr * sin(rotate_radius_L);
        thrust_coordinates[j].x += center_x;
        thrust_coordinates[j].y = x_curr * sin(rotate_radius_L) + y_curr * cos(rotate_radius_L);
        thrust_coordinates[j].y += center_y;
    }
    if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
        exit( EXIT_FAILURE );
    }
}

/********************************************
* when the right error key pressed,the ship *
* rotate clockwise                          *
********************************************/
void rotate_right()
{
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGALRM);
    if(sigprocmask(SIG_BLOCK, &block_mask, &old_mask) < 0) {
            exit(EXIT_FAILURE);
    }
    erase_ship(lander);
    angle_degrees += 10;
    angle_radians = angle_degrees * M_PI / 180.0;
    double x_curr, y_curr;
    int i,j;
    
    for(i=0; i<6; i++) {
        x_curr = ship_coordinates[i].x - center_x;
        y_curr = ship_coordinates[i].y - center_y;
        ship_coordinates[i].x = x_curr * cos(rotate_radius_R) - y_curr * sin(rotate_radius_R);
        ship_coordinates[i].x += center_x;
        ship_coordinates[i].y = x_curr * sin(rotate_radius_R) + y_curr * cos(rotate_radius_R);
        ship_coordinates[i].y += center_y;
    }
    draw_ship(lander);
    
    for(j=0;j<3;j++) {
        x_curr = thrust_coordinates[j].x - center_x;
        y_curr = thrust_coordinates[j].y - center_y;
        thrust_coordinates[j].x = x_curr * cos(rotate_radius_R) - y_curr * sin(rotate_radius_R);
        thrust_coordinates[j].x += center_x;
        thrust_coordinates[j].y = x_curr * sin(rotate_radius_R) + y_curr * cos(rotate_radius_R);
        thrust_coordinates[j].y += center_y;
    }
    if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
        exit( EXIT_FAILURE );
    }
}

/*******************************************
* when the space bar is pressed,the thrust *
* actived by the -t option                 *
********************************************/
void active_thrust(FILE * lander)
{
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGALRM);
    if(sigprocmask(SIG_BLOCK, &block_mask, &old_mask) < 0) {
            exit(EXIT_FAILURE);
    }
    xA = thrust * cos(angle_radians);
    yA = gravity + thrust * sin(angle_radians);
    draw_thrust(lander);
    if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
            exit( EXIT_FAILURE );
    }
}

/*********************************************
*   make the ship fall if the gravity is     *
* greater than zero actived by the -t option *
**********************************************/
void ship_fall()
{
    old_velocity_x = new_velocity_x;
    old_velocity_y = new_velocity_y;
    int i,j;
    
    for(i=0; i < 6; i++) {
        ship_coordinates[i].x = ship_coordinates[i].x + old_velocity_x * T + 1/2 * xA * T*T;
        ship_coordinates[i].y = ship_coordinates[i].y + old_velocity_y * T + 1/2 * yA * T*T;
    }
    
    for(j=0;j<3;j++) {
        thrust_coordinates[j].x = thrust_coordinates[j].x + old_velocity_x * T + 1/2 * xA * T*T;
        thrust_coordinates[j].y = thrust_coordinates[j].y + old_velocity_y * T + 1/2 * yA * T*T;
    }
    
    center_x = center_x +  old_velocity_x * T + 1/2 * xA * T*T;
    center_y = center_y +  old_velocity_y * T + 1/2 * yA * T*T;

    new_velocity_x = old_velocity_x + xA * T;
    new_velocity_y = old_velocity_y + yA * T;
}


/*********************************************
*     detecting line segment intersection    *
**********************************************/
bool intersection(double Ax, double Ay, double Bx, double By, double Cx, double Cy,double Dx, double Dy)
{
    double theCos, theSin, distAB, newX, ABpos;
    // Fail if either line is undefined
    if ((Ax == Bx && Ay == By) || (Cx == Cy && Dx == Dy) )
        return FALSE;
    // fail if either segments share an end point
    if((Ax == Cx && Ay == Cy) || (Bx == Cx && By == Cy) || (Ax == Dx && Ay == Dy) || (Bx == Dx && By == Dy))
        return FALSE;
    // Translate the system so that point A is on the original
    Bx -= Ax; By -= Ay;
    Cx -= Ax; Cy -= Ay;
    Dx -= Ax; Dy -= Ay;
    // discover the length of A-B
    distAB = sqrt(Bx*Bx + By*By);
    // Rotate the system so that point B is on the positive X axis
    theCos = Bx/distAB;
    theSin = By/distAB;
    newX = Cx * theCos + Cy * theSin;
    Cy = Cy * theCos - Cx * theSin; Cx = newX;
    newX = Dx * theCos + Dy * theSin;
    Dy = Dy * theCos -Dx * theSin; Dx = newX;
    // fail if segment C-D doesn't cross line A-B
    if((Cy < 0. && Dy < 0.) || (Cy > 0. && Dy >0.))
        return FALSE;
    // discover the position of intersection point along line A-B
    ABpos = Dx + (Cx - Dx) * Dy / (Dy - Cy);
    // fai if segment C-D crosses line A-B outside segment A-B
    if(ABpos < 0. || ABpos > distAB)
        return FALSE;
    // there is intersection
    return TRUE;
}

/**********************************************
* prevent the ship from giong above the window*
***********************************************/
int out_of_window()
{
    int i;
    for(i=0; i<5; i++) {
        if((ship_coordinates[i].x < 0) || (ship_coordinates[i].x > 639))
            return 1;
        if((ship_coordinates[i].y < -3) || (ship_coordinates[i].y > 479))
            return 1;
    }
    return 0;
}

/******************************************
*            handler for signals          *
*******************************************/
void handle_timeout(int signal)
{
    if(signal == SIGALRM) {
        struct itimerval timer;
        if(getitimer(ITIMER_REAL, &timer) < 0)  {
            exit(EXIT_FAILURE);
        }
        
        // set blocked signal set to block_mask to block SIGALRM
        if(sigprocmask(SIG_BLOCK, &block_mask, &old_mask) < 0) {
            exit(EXIT_FAILURE);
        }
        // critical code
        erase_ship(lander);
        ship_fall();
        draw_ship(lander);
        yA = gravity;
        xA = 0;
        
        if(Improve) {
            fuel ++;
            if(fuel % 25 == 0)
                erase_fuel();
        }
        
        int i,j;
        for(i=0; i<5; i++) {
            Ax = ship_coordinates[i].x;
            Ay = ship_coordinates[i].y;
            Bx = ship_coordinates[i+1].x;
            By = ship_coordinates[i+1].y;
            for(j=0; j<num-1; j++){
                Cx = points[j].x;
                Cy = points[j].y;
                Dx = points[j+1].x;
                Dy = points[j+1].y;
                if(intersection(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy)) {
                    break;
                }
            }
            if(intersection(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy)) {
                break;
            }
        }
        if(!intersection(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy)) {
            int j;
            Ax = ship_coordinates[0].x;
            Ay = ship_coordinates[0].y;
            Bx = ship_coordinates[5].x;
            By = ship_coordinates[5].y;
            for(j=0; j<num-1; j++){
                Cx = points[j].x;
                Cy = points[j].y;
                Dx = points[j+1].x;
                Dy = points[j+1].y;
                if(intersection(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy)) {
                    break;
                }
            }
        }
        // unblock signal by setting mask to old calue before added SIGALRM
        if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
            exit(EXIT_FAILURE);
        }

        if(intersection(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy)) {
            if((angle_degrees == 90) && (new_velocity_y < 20) && (Cy == Dy)) {
                mvprintw(8, 10, "Landed !!!");
                if(Improve) {
                    draw_landed();
                    fflush(lander);
                }
                running = 0;
            }
            else {
                mvprintw(8, 10, "Crashed !!!");
                if(Improve) {
                    explode();
                    draw_crash();
                    fflush(lander);
                }
                running = 0;
            }
            timer.it_interval.tv_sec = 0;
            timer.it_interval.tv_usec = 0;
            timer.it_value.tv_sec = 0;
            timer.it_value.tv_usec = 0;
        }

        // check if the ship goes out of the window
       if(out_of_window()) {
            running = 0;
            timer.it_interval.tv_sec = 0;
            timer.it_interval.tv_usec = 0;
            timer.it_value.tv_sec = 0;
            timer.it_value.tv_usec = 0;
        }
        
        // check if the ship goes out of fuel
        if(reduce == 51){
            mvprintw(8, 10, "Out of fuel !!!");
            running  = 0;
            timer.it_interval.tv_sec = 0;
            timer.it_interval.tv_usec = 0;
            timer.it_value.tv_sec = 0;
            timer.it_value.tv_usec = 0;
        }
        
        if(setitimer(ITIMER_REAL, &timer, NULL) < 0)
            exit(EXIT_FAILURE);
    }
}

/******************************************
*   handle the command line arguments     *
*******************************************/
void comm_line_argu(int argc, char *argv[])
{
    opterr = 0;
    int a;
    while((a = getopt(argc,argv, "t:g:f:i")) != -1) {
        switch(a) {
                case 'g':
                    if(atof(optarg) < 0) {
                        printf("gravity < 0, > 20 not allowed\n");
                        exit(EXIT_FAILURE);
                    }
                    else {
                        gravity = atof(optarg);
                        Gravity = 1;
                    }
                    break;
                
                case 't':
                    if(atof(optarg) > 0) {
                        printf("thrust > 0, < -20 not allowed\n");
                        exit(EXIT_FAILURE);
                    }
                    else {
                        thrust = atof(optarg);
                        Thrust = 1;
                    }
                    break;
                case 'f':
                    input = fopen(optarg, "r");
                    if(input == NULL) {
                        fprintf(stderr,"Could not open the file!\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'i':
                    Improve = 1;
                    break;
                case ':':
		    fprintf(stderr,"bad option, or good option with no value.\n");
		    exit(EXIT_FAILURE);
        	case '?':
		    fprintf(stderr,"bad option, or good option with no value.\n");
		    exit(EXIT_FAILURE);
        }
    }
}

/******************************************
* draw a "crash" on the Sketchpad window  *
*******************************************/
void draw_crash()
{
    fprintf(lander,"setColor 0 255 0\n");

    fprintf(lander, "drawSegment 200 2 150 2\n");
    fprintf(lander, "drawSegment 150 2 150 50\n");
    fprintf(lander, "drawSegment 150 50 200 50\n");
   
    fprintf(lander, "drawSegment 210 50 210 2\n");
    fprintf(lander, "drawSegment 210 2 260 2\n");
    fprintf(lander, "drawSegment 260 2 260 20\n");
    fprintf(lander, "drawSegment 260 20 210 20\n");
    fprintf(lander, "drawSegment 210 20 260 50\n");

    fprintf(lander, "drawSegment 270 50 295 2\n");
    fprintf(lander, "drawSegment 295 2 320 50\n");
    fprintf(lander, "drawSegment 283 26 307 26\n");
    
    fprintf(lander, "drawSegment 330 50 380 50\n");
    fprintf(lander, "drawSegment 380 50 380 30\n");
    fprintf(lander, "drawSegment 380 30 330 30\n");
    fprintf(lander, "drawSegment 330 30 330 2\n");
    fprintf(lander, "drawSegment 330 2 380 2\n");

    fprintf(lander, "drawSegment 390 2 390 50\n");
    fprintf(lander, "drawSegment 440 2 440 50\n");
    fprintf(lander, "drawSegment 390 24 440 24\n");
}

/******************************************
* draw a "crash" on the Sketchpad window  *
*******************************************/
void draw_landed()
{
    fprintf(lander,"setColor 0 255 0\n");
    
    fprintf(lander, "drawSegment 110 2 110 50\n");
    fprintf(lander, "drawSegment 110 50 140 50\n");
    
    fprintf(lander, "drawSegment 150 50 175 2\n");
    fprintf(lander, "drawSegment 175 2 200 50\n");
    fprintf(lander, "drawSegment 163 26 187 26\n");

    fprintf(lander, "drawSegment 210 50 210 2\n");
    fprintf(lander, "drawSegment 210 2 260 50\n");
    fprintf(lander, "drawSegment 260 50 260 2\n");

    fprintf(lander, "drawSegment 270 50 270 2\n");
    fprintf(lander, "drawSegment 270 2 320 24 \n");
    fprintf(lander, "drawSegment 320 24 270 50\n");

    fprintf(lander, "drawSegment 380 2 330 2\n");
    fprintf(lander, "drawSegment 330 2 330 50\n");
    fprintf(lander, "drawSegment 330 50 380 50\n");
    fprintf(lander, "drawSegment 330 24 380 24\n");

    fprintf(lander, "drawSegment 390 50 390 2\n");
    fprintf(lander, "drawSegment 390 2 440 24 \n");
    fprintf(lander, "drawSegment 440 24 390 50\n");
}

/******************************************
*     draw fuel gauge as ship runs        *
*******************************************/
void draw_fuel()
{
    fprintf(lander,"setColor 0 0 0\n");
    fprintf(lander, "drawSegment 2 2 2 50\n");
    fprintf(lander, "drawSegment 2 50 30 50\n");
    fprintf(lander, "drawSegment 30 50 30 2\n");
    fprintf(lander, "drawSegment 30 2 2 2\n");
    int i;
    for(i=3;i<50;i++) {
        fprintf(lander, "drawSegment 2 %d 30 %d\n", i, i);
    }
    fprintf(lander, "drawSegment 30 40 35 40\n");
    fprintf(lander, "drawSegment 35 40 35 48\n");
    fprintf(lander, "drawSegment 35 48 40 48\n");
}

/******************************************
*     reduce fuel gauge as ship runs      *
*******************************************/
void erase_fuel()
{
    reduce = reduce + 3;
    int i;
    if(reduce < 50) {
        for(i=0;i<reduce;i++) {
            fprintf(lander, "eraseSegment 2 %d 30 %d\n", i, i);
        }
    }
}

/******************************************
*     explode the ship if it crashes      *
*******************************************/
void explode()
{
    fprintf(lander,"setColor 100 100 100\n");
    erase_ship(lander);
    int i;
    double x[6],y[6];
    for(i=0;i<5;i++) {
        x[i] = ship_coordinates[i].x;
        y[i] = ship_coordinates[i].y;
        x[i+1] = ship_coordinates[i+1].x;
        y[i+1] = ship_coordinates[i+1].y;
        fprintf(lander,"drawSegment %ld %ld %ld %ld\n",lround(x[i]+5),lround(y[i]+5),lround(x[i+1]+10),lround(y[i+1]+10));
    }
    fflush(lander);
}


