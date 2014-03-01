/*
 * file: pi4a.c
 * This program simulates the symmetric 
 * pi/4-sandpile model with probabilities
 * alpha=1/Z, beta=1/Z, gamma=(Z-2)/Z.
 * This program does not generate
 * zero-avalanches.
 */
#define _CRT_SECURE_NO_WARNINGS 1
#ifndef __NOBUMP__
#define __NOBUMP__ 0
#endif
#include <stdio.h>
#include <stdlib.h>
#ifdef __unix__
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#include <time.h>
#define THRESHOLD 2
#define THRESHOLD2 THRESHOLD / 2
#ifndef Z
#define Z 4
#endif
#ifndef L
#define L 2000
#endif
#define L1 (L+1)
#define PERIOD 34359738352
int reset;

unsigned **lattice; /* Lattice array */
unsigned *row1, *row2;
int *arow1, *arow2;
int **area;
int left, right, ileft, iright;
int t; /* Avalanche's length */
int s; /* Avalanche's size */
int a; /* Avalanche's area */
int X; /* Maximal transversal extent */

FILE*fp; /* Output file handler */

void dosrand();
void init();
void finalize(int);
void randomizeAll();
void randomize();
void update();
int updaterow(int, int, int*, int*);
#ifdef __unix__
static void on_intr (int catch_signal);
#endif

int main(int argc, char**argv) {
    int i, N, reset_number;
    time_t tic, toc;
    
    tic = time(NULL);
    if (argc<3) {
        fprintf(stderr, "Usage: %s <N> <FN>\n", argv[0]);
        fprintf(stderr, "Where:\n");
        fprintf(stderr, "\t N  =  number of iterations\n");
        fprintf(stderr, "\tFN  =  output file name\n");
        exit(1);
    } else {
        if (!(fp = fopen(argv[2], "a"))) {
            fprintf(stderr, "%s: ERROR: Cannot open output file!\n", argv[0]);
            exit(2);
        }
    }
#ifdef __unix__
    if ((signal(SIGINT, on_intr) == SIG_ERR) ) {
        perror("signal");
        exit(5);
    }
#endif
    N = atoi(argv[1]);  fprintf(stderr, "Number of iterations = %d\n", N);
    
    init();
    
    reset=0;
    reset_number = 0;
    for (i=0; i<N; i++) {
#if __NOBUMP__ != 0
#pragma message("This simulation will genrate probability density distribution without bump.")
    again:
#else
#pragma message("This simulation will genrate probability density distribution with bump.")
#endif
        if (!(reset % PERIOD)) { dosrand(); reset = 0; printf("Reset number: %d\n", ++reset_number); }
        update();
# if __NOBUMP__ !=0
        if (t >= L) goto again;
#endif
        fprintf(fp, "%d %d %d %d\n", t, s, a, X);
        if (!(i % (N / 1000))) printf("\rItetation #%d.", i);
    }
    printf("\n");
    finalize(2);
    fclose(fp);
    toc = time(NULL);
    printf("Time elapsed: %d\n", (int) (toc - tic));
    return 0;
}

/*
 * Initialization of the build-in pseudo-random generator
 */
void dosrand() {
#ifdef __unix__
#pragma message("dosrand() will be compiled for *nix operating system")
    int fd;
    unsigned seed;
    fd = open("/dev/urandom", O_RDONLY);
    if (fd==-1 || read(fd, &seed, sizeof(seed))<=0) {
        fprintf(stderr, "Critical error: Cannot open \"/dev/urandom\"\n");
        exit(2);
    }
    close(fd);
    srand(seed);
#else
#pragma message("dosrand() will be compiled for default operating system")
    srand((unsigned int) time(NULL));
#endif
}

/*
 * Initialization method
 */
void init() {
    int i, j;
    /* Allocation of memory for lattice array */
    lattice = (unsigned**) malloc ( 2 * sizeof(unsigned*) );
    for (i=0; i<2; i++) lattice[i] = (unsigned*) malloc ( L1 * sizeof(unsigned) );
    if (!lattice) {
        fprintf(stderr, "%s: ERROR: Cannot allocate memory for lattice's array!\n");
        exit(3);
    }
    area = (int**) malloc( 2 * sizeof(int) );
    for (i=0; i<2; i++) area[i] = (int*) malloc( L1 * sizeof(int) );
    if (!area) {
        fprintf(stderr, "%s: ERROR: Cannot allocate memory for area array!\n");
        finalize(1);
        exit(3);
    }
    
    /* Randomization */
    dosrand();
    for (i = 0; i < 2; i++) {
        for (j = 0; j < L1; j++) {
            lattice[i][j] = rand() % THRESHOLD;
            area[i][j] = 0;
        }
    }
    left = 0;
    right = L;
    ileft = left;
    iright = right;
    reset = 2 * L;
}

/*
 * Finalization method
 */
void finalize(int mode) {
    int i;
    switch (mode) {
        case 2:
            for (i = 0; i < 2; i++ ) free(area[i]);
            free(area);
        default:
            for (i = 0; i < 2; i++) free(lattice[i]);
            free(lattice);
    }
}

/*
 * Randomization method
 */
void randomizeAll() {
    int i, j;
    if (ileft > left) ileft = left;
    if (iright < right) iright = right;
    for (i=0; i<2; i++) {
        for (j = ileft; j <= iright; j++) lattice[i][j]  = rand() % THRESHOLD;
        for (j=0; j<L1; j++) area[i][j] = 0;
    }
}

void randomize() {
    int j;
    if (ileft > left) ileft = left;
    if (iright < right) iright = right;
    for (j = ileft; j <= iright; j++) {
        row2[j]  = rand() % THRESHOLD;
        arow2[j] = 0;
    }
}

/*
 * alpha=1/Z, beta=1/Z, gamma=(Z-2)/Z
 */
void update() {
    int i, j, toppled, x;
    s = 0;  t = 0;
    
    randomizeAll();
    reset += 2*(iright-ileft) + 3;
    
    row1  = lattice[0];  row2  = lattice[1];
    arow1 = area[0];     arow2 = area[1];

    /* We start with a toppling in [0][0] position */
    row1[0] = THRESHOLD;
    a = 1;  X = 1;
    
    ileft = 0;   iright = 0;
    toppled = updaterow(ileft, iright, &left, &right);
    
    i=0;
    while ((toppled==1) && (++i < L1)) {
        t = i;
        for (j=left; j<=right; j++)    a += arow2[j];
        x = right-left+1;   if (X < x) X = x;
        
        row1  = lattice[i % 2];   row2  = lattice[(i+1) % 2];
        arow1 = area[i % 2];      arow2 = area[(i+1) % 2];
        
        randomize();
        reset += iright-ileft+2;
        
        ileft = left;  iright = right;
        toppled = updaterow(ileft, iright, &left, &right);
    }
}

int updaterow(int ileft, int iright, int*oleft, int*oright) {
    int toppled, j, j1;
    toppled = 0;
    *oleft = iright+1;
    *oright = ileft;
    for (j = ileft; j <= iright; j++) {
        j1 = j+1;
        while (row1[j] >= THRESHOLD) {
            s++;
            toppled = 1;
            row1[j] -= THRESHOLD;
            switch ( rand() % Z ) {
                case 0:
                    row2[j] += THRESHOLD;
                    arow2[j] = 1;
                    if (j < *oleft)  *oleft  = j;
                    if (j > *oright) *oright = j;
                    break;
                case 1:
                    row2[j1] += THRESHOLD;
                    arow2[j1] = 1;
                    if (j1 < *oleft)  *oleft  = j1;
                    if (j1 > *oright) *oright = j1;
                    break;
                default:
                    row2[j]  += THRESHOLD2;
                    row2[j1] += THRESHOLD2;
                    arow2[j] = 1;
                    arow2[j1] = 1;
                    if (j < *oleft)   *oleft  = j;
                    if (j1 > *oright) *oright = j1;
            }
        }
    }
    return toppled;
}

#ifdef __unix__
static void on_intr (int catch_signal) {
    fprintf(stderr, "pi4-simulation interrupted or killed\n");
    finalize(2);
    if (fp) fclose(fp);
    exit(1);
}
#endif
