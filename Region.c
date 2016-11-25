#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Share.h"

static REG *NewNote ( void )
{
    REG *note;

    note = (REG *)malloc(sizeof(REG));
    if ( !note ) {
        fprintf(stderr, "[Err::%s] \
            Memory for note allocate failed!\n", __func__);
        return (NULL);
    }

    note->next = NULL; note->rnum = 0L;
    return (note);
}

static void Split ( REG *note, char *line )
{
    char *pword = strtok(line, "\t");

    strcpy(note->chr, pword);
    for (int i=0; i < 2; ++i) {
        pword = strtok(NULL, "\t");
        if ( i==0 )  
            note->start[note->rnum] = atol(pword);
        else 
            note->end[note->rnum] = atol(pword);
    }
    note->rid[note->rnum] = note->rnum; ++note->rnum;
}

int ChrTest ( char const *line, char const *chr_pre )
{
    char *space;
    char chr[CHRLEN] = {'\0'};

    space = strchr(line, 0x9); // 0x9 '\t'
    if ( !space ) {
        fprintf(stderr, "[Err::%s] \
                Only Tab['\\t'] is allowed for Region file!\n", __func__);
        exit(-1);
    }
    strncpy(chr, line, space-line);

    if ( strcmp(chr, chr_pre) ) return 0;
    else  return 1;
}

REG *GetRegion ( char *rfile )
{
    FILE *fp;
    time_t t_e, t_s;
    char buf[BUFLEN];
    char chr_pre[CHRLEN] = "chr_dummy";
    REG *root, *now;

    if ( !(fp = fopen(rfile, "r")) ) {
        fprintf(stderr, "[Err::%s] \
            Failed to open %s\n", __func__, rfile); exit(-1);
    }
    root = now = NewNote();
    
    time(&t_s);
    fprintf(stderr, \
        "[%s] Start to load Region file......\n", __func__);

    while ( fgets(buf, BUFLEN, fp) ) {
        if ( ChrTest(buf, chr_pre) ) {
            Split(now, buf);
        }
        else {
            now->next = NewNote(); now = now->next;
            Split(now, buf); strcpy(chr_pre, now->chr);
        }
    }
    now = root->next; free(root); fclose(fp); time(&t_e);
    fprintf(stderr, "Time elapsed %.2lf(SEC)\n\n", difftime(t_e,t_s));

    return now;
}

