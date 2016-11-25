#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Share.h"

void CreatIndex( INDEX *index, char *infile )
{
    FILE *fp;
    time_t t_s, t_e;
    char buf[BUFLEN], chr_now[CHRLEN];
    char chr_pre[CHRLEN] = "chr_dummy";

    if ( !(fp = fopen(infile, "r")) ) {
        fprintf(stderr, "[Err::%s] \
            Failed to open %s\n",__func__, infile); exit(-1);
    } 

    time(&t_s);
    fprintf(stderr,\
            "[%s] Start to creat index for bdgfile......\n",__func__);
    for (int i=-1; fgets(buf, BUFLEN, fp); )
    {
        if ( ChrTest(buf, chr_pre) )
            ++index->lnum[i];
        else {
            index->fpdex[++i] = ftell(fp) - strlen(buf);
            {
                strcpy(chr_now, strtok(buf, "\t"));
                strcpy(index->chr[i], chr_now);
            }
            ++index->lnum[i]; ++index->chrcount;
            strcpy(chr_pre, chr_now);
        }
    } 
    time(&t_e); fclose(fp);
    fprintf(stderr,"Time elapsed %.2lf(SEC)\n\n", difftime(t_e,t_s));
}


static void SplitLine ( char *line, BDG *bdg, int last )
{
    strtok(line, "\t");

    *(bdg->start + bdg->lnum) = atol(strtok(NULL, "\t"));
    if ( last )
        *(bdg->start + bdg->lnum + 1) = atol(strtok(NULL, "\t"));
    else 
        strtok(NULL, "\t");

    *(bdg->value + bdg->lnum) = atof(strtok(NULL, "\t"));
    ++bdg->lnum;
}


BDG *LoadBdg ( char *infile, char const *chr, INDEX const *index )
{
    char buf[BUFLEN];
    long fpdex, lnum;
    time_t t_s, t_e;

    FILE *fp = fopen(infile, "r"); 
    BDG *bdg = (BDG *)malloc(sizeof(BDG));
    memset(bdg, 0, sizeof(BDG));

    if ( !fp ) {
        fprintf(stderr, "[Err::%s] \
            Failed to open %s\n", __func__, infile); exit(-1);
    }

    time(&t_s);
    fprintf(stderr, \
            "[%s] Start to load bdgfile for %s......\n", __func__, chr);
    for (int i=0; i<index->chrcount; ++i) {
        if ( !(strcmp(chr, index->chr[i])) ) {
            fpdex = index->fpdex[i]; lnum = index->lnum[i];

            bdg->start = (long *)malloc((lnum+1) * sizeof(long));
            bdg->value = (float *)malloc((lnum+1) * sizeof(float));
        }
    } 
    fseek (fp, fpdex, SEEK_SET); 

    int last = 0;
    for (int i=0; i < lnum; ++i) {
        if ( i == lnum-1 ) last = 0x1;
        fgets(buf, BUFLEN, fp); SplitLine (buf, bdg, last);
    } 
    time(&t_e); ++bdg->lnum; fclose(fp); 

    fprintf(stderr, "Time elapsed %.2lf(SEC)\n", difftime(t_e,t_s));
    return (bdg);
}

void Destory ( void *data, int type )
{
    if ( type & 0x1 ) {
        BDG *bdg = (BDG *) data;
        free(bdg->start); free(bdg->value); free(bdg);
    } 
    else if ( type & 0x2 ) {
        REG *reg = (REG *) data; free(reg);
    } else {
        SEC *sec = (SEC *) data; SEC *tem;
        while ( sec ) {
            tem = sec; sec = sec->next; 
            free(tem->pos); free (tem->value); free(tem);
        }
    }
}



