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


void error(int line_number, FILE * executable)
    // print out error line and send message to standard error
{
    fprintf(executable, "end\n");
    fprintf(stderr, "draw2: %d, error.\n", line_number);
    exit(EXIT_FAILURE);
}

void parseFile ( FILE * input, FILE * executable )
{
    int line_number = 0;
    int rval = 0;
    char *line;
    char token[MAX];
    char draw_name[MAX], print_name[MAX], trans_name[MAX], rotate_name[MAX], End_name[MAX], command[MAX];
    char lineTo[MAX], draw[MAX], print[MAX], translate[MAX], rotate[MAX], child[MAX];
    line = malloc(sizeof(char)*100);
    assert(line != NULL);
    double *angle_degrees, *angle_radians;
    int Image_num = 0;
    int valid_token = 0;
    int valid_image = 0;
    int valid_End = 0;
    int lineto = 0;
    int empty = 0;
    int i=0, j=0;
    struct image *im;
    im = malloc(sizeof(struct image)*100);
    assert(im != NULL);
    
    while  (fgets(line, LINE_LEN + 1, input) != NULL)
    {
        line_number ++;
        sscanf(line, "%s%*[^\n]", token);
        if(line[0] != '\n') //ignore blank lines
        {
            // A commment
            if(strncmp(token,"#",1) == 0)
            {
                printf("%s",line);
            }
        
            // Image command
            else if(strncmp(token,"Image",5) == 0)
            {

                rval = sscanf(line, "%s %s %lf %lf ", draw, &im[i].name, &im[i].x_curr[j], &im[i].y_curr[j]);
                if(rval == 4)
                {
                    im[i].points = 0;
                    Image_num ++;
                    valid_image = 1;
		    empty = 0;
                }
                else
                    error(line_number,executable);
            }
        
            // lineTo commmand
            else if(strncmp(token,"lineTo",6) == 0)
            {
                double delta_x, delta_y;
                rval = sscanf(line, "%s %lf %lf", lineTo, &delta_x, &delta_y);
                if(rval == 3)
                {
                    // the lineTo command must be inside image definition
                    if(valid_image == 1) // after an Image command
                    {
                        lineto ++;
                        if(strncmp(token,"End",3) != 0)
                            valid_End ++;
                        if(lineto > valid_End) // before a End command
                            error(line_number,executable);
                        else
                        {
                            im[i].x_curr[j+1] = im[i].x_curr[j] + delta_x;
                            im[i].y_curr[j+1] = im[i].y_curr[j] + delta_y;
                            j++;
                            im[i].points++;
			    empty = 1;
                        }
                    }
                    else
                        error(line_number,executable);
                }
                else
                    error(line_number,executable);
            }
            else if(strncmp(token, "End", 3) == 0)
            {
                rval = sscanf(line, "End Image %s", End_name);
                if(rval == 1)
                {
                    if(strncmp(End_name, &im[i].name, 1) != 0)
                    {
                        error(line_number,executable);
                    }
                    else
                    {
                        j=0;
                        i++;
                        valid_image = 0;
                        lineto = 0;
                        valid_End = 0;
                    }
                }
                else
                    error(line_number,executable);
            }
        
            // draw command:
            else if(strncmp(token, "draw", 4) == 0)
            {
                int n, k;
                double x1,y1,x2,y2;
                rval = sscanf(line, "%s %s", draw, draw_name);
                if(rval == 2 && valid_image == 0)
                    // draw command cannot be the first token inside an image definition
                {
                    for(k=0; k<Image_num; k++)
                    {
                        int m=0;
                        while(*draw_name != im[m].name)
                        {
                            m++;
                            if(m==Image_num)
                                error(line_number,executable);
                        }
                        
                        if((strncmp(&im[k].name, draw_name, 1) == 0))
                        {
                            valid_token = 1;
                            for(n=0; n<im[k].points; n++)
                            {
                                x1 = im[k].x_curr[n];
                                y1 = im[k].y_curr[n];
                                x2 = im[k].x_curr[n+1];
                                y2 = im[k].y_curr[n+1];
                                fprintf(executable, "drawSegment %ld %ld %ld %ld\n",lround(x1), lround(y1), lround(x2), lround(y2));
                            }
                        }

                    }
                    if(!valid_token)
                    {
                        
                        error(line_number,executable);
                    }
                }
                else
                    error(line_number,executable);
            }
        
            // print command:
            else if(strncmp(token, "print", 5) == 0)
            {
                int n, k;
                rval = sscanf(line, "%s %s", print, print_name);
                if(rval == 2 && valid_image == 0)
                    // translate command cannot be the first token inside an image definition
                {
                    printf("Print Image %s =\n", print_name);
		    if(empty == 0) // deal with empty image
		      printf("End Image %s\n",print_name); 
		    else
		    {
		      for(k=0; k<Image_num; k++)
		      {
                        if((strncmp(&im[k].name, print_name, 1))==0)
                        {
                            valid_token = 1;
                            for(n=0; n<=im[k].points; n++)
                            {
                                printf("%g %g\n", im[k].x_curr[n], im[k].y_curr[n]);
                            }
                            printf("End Image %s\n", &im[k].name);
                        }
		      }
                    }
                    if(!valid_token)
                    {
                        error(line_number,executable);
                    }
                }
                else
                    error(line_number,executable);
            }
            
            // translate command:
            else if(strncmp(token, "translate", 9) == 0)
            {
                int n, k=0;
                double trans_x, trans_y;
                rval = sscanf(line, "%s %s %lf %lf", translate, trans_name, &trans_x, &trans_y);
                if(rval == 4 && valid_image == 0)
                    // translate command cannot be the first token inside an image definition
                {
                    for(k=0; k<Image_num; k++)
                    {
                        if((strncmp(&im[k].name, trans_name, 1)==0))
                        {
                            valid_token = 1;
                            for(n=0; n<=im[k].points; n++)
                            {
                                im[k].x_curr[n] = im[k].x_curr[n] + trans_x;
                                im[k].y_curr[n] = im[k].y_curr[n] + trans_y;
                            }
                        }
                    }
                    if(!valid_token)
                    {
                        error(line_number,executable);
                    }
                }
                else
                    error(line_number,executable);
            }
            
            // rotate command
            else if(strncmp(token, "rotate", 6) == 0 )
            {
             
                int n, k=0;
                double rotate_x, rotate_y;
                angle_degrees = malloc(sizeof(double));
                angle_radians = malloc(sizeof(double));
                rval = sscanf(line, "%s %s %lf", rotate, rotate_name, angle_degrees);
                if(rval == 3 && valid_image == 0)
                    // rotate command cannot be the first token inside an image definition
                {
                    *angle_radians = *angle_degrees * M_PI / 180.0;
                    for(k=0; k<Image_num; k++)
                    {
                        if((strncmp(&im[k].name, rotate_name, 1)==0))
                        {
                            valid_token = 1;
                            for(n=0; n<=im[k].points; n++)
                            {
                                rotate_x = im[k].x_curr[n]*cos(*angle_radians) - im[k].y_curr[n]*sin(*angle_radians);
                                rotate_y = im[k].x_curr[n]*sin(*angle_radians) + im[k].y_curr[n]*cos(*angle_radians);
                                im[k].x_curr[n] = rotate_x;
                                im[k].y_curr[n] = rotate_y;
                            }
                        }
                    }
                    if(!valid_token)
                    {
                        error(line_number,executable);
                    }
                }
                else
                    error(line_number,executable);
                free(angle_radians);
                free(angle_degrees);
            }
            
            // child command:
            else if(strncmp(token,"child", 5) == 0)
            {
                if(valid_image == 0)
                    // child command cannot be the first token inside an image definition
                {
                    sscanf(line, "%s %[^\n]", child, command);
                    fprintf(executable, "%s\n", command);
                }
                else
                    error(line_number,executable);
            }
            
            else
                error(line_number,executable);
        }
    }
    free(im);
    free(line);
}


