#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Share.h"

void Usage(void);

int main ( int argc, char **argv )
{
    if ( argc != 4 ) {
        Usage(); exit(-1);
    }
    INDEX index; time_t t_s, t_e;
    memset(&index, 0, sizeof(INDEX));

    time(&t_s);
    CreatIndex(&index, argv[1]);

    REG *hreg, *nreg; 
    BDG *bdg;  SEC *sec;

    hreg = GetRegion( argv[2] );
    while ( hreg ) {
        /* Load the bdg file */
        bdg = LoadBdg (argv[1], hreg->chr, &index);

        /* Meger to get the value */
        sec = GetValue(bdg, hreg);

        /* Write out the result */
        Write(argv[3], sec);

        nreg = hreg; hreg = hreg->next;
        Destory(nreg, 0x2); Destory (sec, 0x4); Destory(bdg, 0x1);
    } time (&t_e);
    printf("--------------------------------------------\n");
    printf("Run complete: \nTime elapsed %.2f(SEC)\n\n", difftime(t_e,t_s));

    return 0;
}

void Usage(void)
{
    char* Usage =
    "\nUsage: ./GetValue <In.bdg> <In.region> <outfile>\n" \
    "Note:\n\tAll the input file should be ordered(by chr and pos) and tab delimed!\n";

    fprintf(stderr,"%s",Usage);
}

