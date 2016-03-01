/*  ONE Card number: 1461696
    Computing Science UNIX ID: jinzhu
    lecture section: A2
    lab section: D01
    TA's name: Chenyang Huang  */

#include <stdio.h>
#include <stdlib.h>  /* For system(), exit()  */
#include <string.h>  /* For strlen(), strncmp() */
#include <math.h>    /* For lround() */
#include "draw1b.h"

int main(int argc, char * argv[ ])
{
    int i;
    FILE * input;
    FILE * executable;
    /* open a pipe to a child process running sektchpad */
    executable = popen("java -jar Sketchpad.jar", "w");
    /* Two-line start-up message */
    fprintf(stdout,"./draw1b started on ");
    fflush(stdout);
    /* Print out current date */
    system("date");
    input = fopen(argv[1], "r");
    if(input == NULL)
    {
        printf("Could not open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Input file: %s\n", argv[1]);
        parseFile(input, executable);
        pclose(executable);
        fclose(input);
    }
    return 0;
}


/* parseFile */
void parseFile ( FILE * input, FILE * executable )
{
	char line [ LINE_LEN + 1 ];
	char command [ LINE_LEN + 1 ];
    struct image im[IMAX];
    char Image[ MAX ], lineTo[ MAX ], End[ MAX ], print[ MAX ], draw[ MAX ], translate[ MAX ], child[ MAX ];
    char draw_name[IMAX], print_name[IMAX], trans_name[IMAX];
    int Image_num=0;
    int i=0, j=0;
    
    /* Initialize . Defensive */
    memset(command, 0, LINE_LEN + 1);
	memset(line, 0, LINE_LEN + 1);
    
	while  (fgets(line, LINE_LEN + 1, input) != NULL)
   	 {
        // A commment
		if(strncmp(line,"#",1) == 0)
        {
			printf("%s",line);
        }
         
        // Image command:
        else if(strncmp(line,"Image",5) == 0)
        {
            sscanf(line, "%s %s %lf %lf",Image, &im[i].name, &im[i].x_curr[j], &im[i].y_curr[j]);
            Image_num ++;
        }
         
        // lineTo commmand:
        else if(strncmp(line,"lineTo",6) == 0)
		{
            double delta_x, delta_y;
			sscanf(line, "%s %lf %lf", lineTo, &delta_x, &delta_y);
			im[i].x_curr[j+1] = im[i].x_curr[j] + delta_x;
			im[i].y_curr[j+1] = im[i].y_curr[j] + delta_y;
            j++;
            im[i].points++;
		}
        else if(strncmp(line, "End", 3) == 0)
        {
            j=0;
            i++;
        }
         
        // draw command:
		else if(strncmp(line, "draw", 4) == 0)
		{
            int n, k;
            double x1,y1,x2,y2;
            sscanf(line, "%s %s",draw, draw_name);
            for(k=0; k<Image_num; k++)
            {
                if((strncmp(&im[k].name, draw_name, 1)==0))
                {
                    for(n=0; n<im[k].points; n++)
                    {
                        x1 = im[k].x_curr[n];
                        y1 = im[k].y_curr[n];
                        x2 = im[k].x_curr[n+1];
                        y2 = im[k].y_curr[n+1];
                        fprintf(executable, "drawSegment %ld %ld %ld %ld\n",lround(x1), lround(y1), lround(x2), lround(y2));
                        fflush(executable);
                    }
                }
            }
        }
        
        // print command:
        else if(strncmp(line, "print", 5) == 0)
        {
			int n, k;
			sscanf(line, "%s %s", print, print_name);
			printf("Print Image %s =\n", print_name);
			for(k=0; k<Image_num; k++)
			{
				if((strncmp(&im[k].name, print_name, 1))==0)
				{
					for(n=0; n<=im[k].points; n++)
					{
						printf("%ld %ld\n", lround(im[k].x_curr[n]), lround(im[k].y_curr[n]));
					}
            		printf("End Image %s\n", &im[k].name);
				}
			}
        }
         
        // translate command:
		else if(strncmp(line, "translate", 9) == 0)
        {
            int n, k=0;
            double trans_x, trans_y;
            sscanf(line, "%s %s %lf %lf", translate, trans_name, &trans_x, &trans_y);
            for(k=0; k<Image_num; k++)
            {
                if((strncmp(&im[k].name, trans_name, 1)==0))
                {
                    for(n=0; n<=im[k].points; n++)
                    {
                        im[k].x_curr[n] = im[k].x_curr[n] + trans_x;
                        im[k].y_curr[n] = im[k].y_curr[n] + trans_y;
                    }
                }
            }
        }
                
        // child command:
		else if(strncmp(line,"child", 5) == 0)
        {
            sscanf(line, "%s %[^\n]", child, command);
            fprintf(executable, "%s\n", command);
        }
            
     } /* while */
}



