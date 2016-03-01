/*  ONE Card number: 1461696
 Computing Science UNIX ID: jinzhu
 lecture section: A2
 lab section: D01
 TA's name: Chenyang Huang  */

#define LINE_LEN 256
#define IMAGE 16
#define MAX 128
#define IMAX 16
#define _XOPEN_SOURCE

struct image{
    char name;
    double x_curr[100], y_curr[100];
    int points;
};

void parseFile(FILE * input, FILE * executable);
void error(int line_number, FILE *executable);
