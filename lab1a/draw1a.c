/*  name: Jinzhu Li
    ONE Card number: 1461696
    Computing Science UNIX ID: jinzhu
    lecture section: A2
    lab section: D01
    TA's name: Chenyang Huang  */


#include <stdio.h>  
#include <stdlib.h>  /* For system(), exit()  */
#include <string.h>  /* For strlen(), strncmp() */

#define MIN_BUF 256

char Keyword [ MIN_BUF ];

/* parseFile */
void parseFile ( FILE * fp )
{
	int Image, lineTo, End, print, draw, translate, child, octothorp;
    Image = lineTo = End = print = draw = translate = child = octothorp = 0;
    /* Read the first word from input file */
    if ( ( fscanf (fp, "%s%*[^\n]",Keyword) ) == 1)
    {
        while ( ! feof(fp) )
        {
            if ( strncmp(Keyword,"Image",strlen("Image")) == 0 )
                Image++;
            else if ( strncmp(Keyword,"lineTo",strlen("lineTo")) == 0)
                lineTo++;
            else if ( strncmp(Keyword,"End",strlen("End")) == 0)
                End++;
            else if ( strncmp(Keyword,"print",strlen("print")) == 0)
                print++;
            else if ( strncmp(Keyword,"draw",strlen("draw")) == 0)
                draw++;
            else if ( strncmp(Keyword,"translate",strlen("translate")) == 0)
                translate++;
            else if ( strncmp(Keyword,"child",strlen("child")) == 0)
                child++;
            else if ( strncmp(Keyword,"#",strlen("#")) == 0)
                octothorp++;
            fscanf ( fp, "%s%*[^\n]",Keyword );
        } /* while */
        printf ( "%d Image definition(s)\n", Image );
        printf ( "%d lineTo commment(s) within Figures\n", lineTo );
        printf ( "%d End commment(s)\n", End );
        printf ( "%d print commment(s)\n", print );
        printf ( "%d draw commment(s)\n", draw );
        printf ( "%d translate commment(s)\n", translate );
        printf ( "%d child commment(s)\n", child );
        printf ( "%d commment(s)\n", octothorp );
    }
    else
        printf("Empty File.\n");
}

int main ( int argc, char * argv[] )
{
	int i;
	FILE * fp;
    /* Two-line start-up message */
	fprintf ( stdout,"./draw1a started on " );
	fflush (stdout);
    system ( "date" );
    for ( i = 1; i < argc; i++)
	{
		fp = fopen ( argv[i], "r" );
		if ( fp == NULL )
		{
			printf ( "Could not open file %s\n", argv[i] );
			exit(EXIT_FAILURE);
		}
		else
		{
			printf ( "Input file: ./%s\n", argv[i] );
			parseFile(fp);
			fclose (fp);
		}
	}
	return 0;
}  /* main */
		
			
			
	
	
	
