/*  ONE Card number: 1461696
 Computing Science UNIX ID: jinzhu
 lecture section: A2
 lab section: D01
 TA's name: Chenyang Huang  */

#include <stdio.h>
#include <stdlib.h>  /* For system(), exit()  */
#include <string.h>  /* For strnlen(), strncmp() */
#include <math.h>    /* For lround(), cos(), sin() */
#include "draw2.h"
#include <assert.h>

int main(int argc, char * argv[ ])
{
    FILE * input;
    FILE * executable;
    executable = popen("java -jar Sketchpad.jar", "w");
    fprintf(stdout,"./draw2 started on ");
    fflush(stdout);
    system("date");
    input = fopen(argv[1], "r");
    if(input == NULL || argc > 2)
        // the number of command line argumentes shouldn't over two
    {
        printf("draw2: 0, error\n");
        pclose(executable);
        fclose(input);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Input file: ./%s\n", argv[1]);
        parseFile(input, executable);
        pclose(executable);
        fclose(input);
    }
    return 0;
}
