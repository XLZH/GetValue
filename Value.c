#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Share.h"

#define REP(vid, num, v) { for(int r=0; r < num; ++r ) { *(vid) = v; (vid)++;} }

static long Binary ( BDG *bdg, long pos )
{
    long low, mid, high;

    low = 0; high = bdg->lnum -1;
    while ( high > low + 1 ) 
    {
        mid = (low + high) / 2;

        if ( bdg->start[mid] < pos ) 
            low = mid;
        else if ( bdg->start[mid] > pos ) 
            high = mid;
        else return mid;
    }
    return low;
}


static SEC *NewSec( void )
{
    SEC *note;

    note = (SEC *)malloc(sizeof(SEC));
    if ( !note ) {
        fprintf(stderr, "[Err::%s] \
                Memory for note allocate failed!\n", __func__);
        return (NULL);
    }
    note->posn = 0L; note->next = NULL;
    return (note);
}


static void Merge ( BDG *bdg, SEC *sec, REG *reg, int index )
{
    long sec_s, sec_e;
    long start = reg->start[index];
    long end = reg->end[index];

    /* Get the index for region of start and end*/
    sec_s = Binary(bdg, start); 
    sec_e = Binary(bdg, end);

    sec->posn = end - start + 1;
    {
        sec->rid = reg->rid[index]; strcpy(sec->chr, reg->chr);   

        sec->pos = (long *)malloc(sec->posn * sizeof(long));
        sec->value = (float *)malloc(sec->posn * sizeof(float));
    }
    long s = start;
    for ( int i=0; i< sec->posn; ++i ) *(sec->pos + i) = s++;

    float *val = sec->value; 
    long *bs = bdg->start;
    int num = 0;
    
    if (sec_s == sec_e) {
        num = end - start; REP(val, num, 0.0); return;
    }
    num = bs[sec_s+1]-start; REP(val, num, bdg->value[sec_s]);

    for (long i=sec_s+1; i< sec_e; ++i) {
        int num = bs[i+1]-bs[i]; 
        REP(val, num, bdg->value[i]);
    }
    num = end-bs[sec_e]+1; REP(val, num, bdg->value[sec_e]);
}


SEC *GetValue ( BDG *bdg, REG *reg )
{
    SEC *root, *now;
    time_t t_s, t_e;
    
    root = now = NewSec ();
    time(&t_s);
    fprintf(stderr, \
            "[%s] Start to extend the value ......\n", __func__);
    for ( int i=0; i < reg->rnum; ++i ) {
        now->next = NewSec(); now = now->next; 
        Merge(bdg, now, reg, i);
    }
    now = root->next; free(root); time(&t_e);

    fprintf(stderr, \
            "Time elapsed %.2lf(SEC)\n\n", difftime(t_e,t_s));
    return (now);
}

void Write ( char *out, SEC *sec )
{
    SEC *now = sec;
    FILE *fp = fopen(out, "a");
    while ( now ) {
        long i;
        for ( i=0; i < now->posn; ++i ) {
            fprintf(fp, "%s\t%ld\t%ld\t%.6f\n", \
                now->chr, now->pos[i], now->rid, now->value[i]);
        } now = now->next;
    } fclose(fp);
}
