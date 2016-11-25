#define MD 2048         // Max rowcount for index
#define BUFLEN 128      // Length of buffer
#define CHRLEN 64       // Length for chromosome


/* The Struct bellow is used for index created and data load in bdgFile */
typedef struct {
    int  chrcount;          // Count of chr index
    long lnum[MD];          // Chr line number
    long fpdex[MD];         // File pointer index
    char chr[MD][CHRLEN];   // Chromsome name
} INDEX;

typedef struct {
    long lnum;          // Number of bdg lines for this chromosome
    long *start;        // starts of bdgfile for this chromosome
    float *value;       // values of bdgfile
} BDG;


/* The Struct bellow is used for loading region data */
struct _REG {
    char chr[CHRLEN];           // chromosome
    long rnum, rid[MD];         // number of regions and regionID
    long start[MD], end[MD];    // start and end of the regions
    struct _REG *next;          // next struct
};
typedef struct _REG REG;


struct _SEC {
    char  chr[CHRLEN];        // chromosome
    long  posn, rid;          // position num and regionID
    long  *pos;               // positions
    float *value;             // position value
    struct _SEC *next;        // next struct
};
typedef struct _SEC SEC;


/* Function prototype */
void CreatIndex( INDEX *, char * );
BDG *LoadBdg ( char *, char const *, INDEX const * );
int ChrTest ( char const *, char const * );
REG *GetRegion ( char * );
SEC *GetValue ( BDG *, REG * );
void Destory ( void *, int );
void Write ( char *, SEC *);
