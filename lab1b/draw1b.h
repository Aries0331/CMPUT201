/*  ONE Card number: 1461696
 Computing Science UNIX ID: jinzhu
 lecture section: A2
 lab section: D01
 TA's name: Chenyang Huang  */

#define LINE_LEN 256
#define IMAGE 16
#define MAX 256
#define IMAX 16

struct image{
	char name;
	double x_curr[256],y_curr[256];
    int points;
};

void parseFile(FILE * input, FILE * executable);
