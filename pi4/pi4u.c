/*
 * file: pi4u.c
 * This program simulates the symmetric 
 * pi/4-sandpile model with probabilities
 * alpha=1/Z, beta=1/Z, gamma=(Z-2)/Z.
 * This program does not generate
 * zero-avalanches.
 */
#include <stdio.h>
#include <stdlib.h>
#ifdef __unix__
#include <unistd.h>
#include <fcntl.h>
#endif
#include <time.h>
#define THRESHOLD 2
#define THRESHOLD2 THRESHOLD / 2
#define Z 3


int**lattice; /* Lattice array */
int*left;
int*right;
int L;        /* Lattice size */
int t;        /* Avalanche's length */
int s;        /* Avalanche's size */
int T;

FILE*fp;      /* Output file handler */
char*fname;   /* Output file name */

void dosrand();
void init();
void finalize(int);
void randomize();
void update();

int main(int argc, char**argv) {
    int i, N, reset, period=1000;
    
    if (argc<3) {
        fprintf(stderr, "Usage: %s <N> <L> [<FN>]\n", argv[0]);
        fprintf(stderr, "Where:\n");
        fprintf(stderr, "\t N  =  number of iterations\n");
        fprintf(stderr, "\t L  =  lattice size\n");
        fprintf(stderr, "\tFN  =  output file name\n");
        exit(1);
    }
    if (argc < 4) {
        fp = stdout;
    } else {
        fname = argv[3];
        fp = fopen(fname, "a");
        if (!fp) {
            fprintf(stderr, "%s: ERROR: Cannot open output file!\n", argv[0]);
            exit(2);
        }
    }
    
    N = atoi(argv[1]);
    fprintf(stderr, "Number of iterations = %d\n", N);
    
    L = atoi(argv[2]);
    fprintf(stderr, "Lattice size = %d\n", L);
    
    init();

    T=L;
    reset=0;
    for (i=0; i<N; i++) {
    again:
        if (!((++reset) % period)) dosrand();
        randomize();
        /* Initialize with critical height */
        lattice[0][0]=THRESHOLD; 
        update();
        T = t; if (T>L) T=L;
        if (t > L) goto again;
        fprintf(fp, "%d %d\n", t, s);
    }
    finalize(3);
    fclose(fp);
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
#pragma message("dosrand() will be compiled for default operationg system")
    srand(time(NULL));
#endif
}

/*
 * Initialization method
 */
void init() {
    int i, j, L1, ints;
    ints = sizeof(int);
    L1 = L+1;
    /* Allocation of memory for lattice array */
    lattice = (int**) malloc ( L1 * ints);
    for (i=0; i<L1; i++) {
        lattice[i] = (int*) malloc ( (i+1) * ints );
    }
    if (!lattice) {
        fprintf(stderr, "%s: ERROR: Cannot allocate memory for lattice's array!\n");
        exit(3);
    }
    if (!(left = (int*) malloc(L1*ints))) {
        fprintf(stderr, "%s: ERROR: Cannot allocate memory for LEFT array!\n");
        finalize(1);
        exit(3);
    }
    if (!(right = (int*) malloc(L1*ints))) {
        fprintf(stderr, "%s: ERROR: Cannot allocate memory for RIGHT array!\n");
        finalize(2);
        exit(3);
    }
    for (i=0; i<L1; i++) {
        left[i] = 0;
        right[i] = 0;
        for (j=0; j<i+1; j++) {
            lattice[i][j] = 0;
        }
    }
    /* Randomization */
    dosrand();
}

/*
 * Finalization method
 */
void finalize(int mode) {
    int i;
    switch (mode) {
        case 3:
            free(right);
        case 2:
            free(left);
        case 1:
            for (i=0; i<L+1; i++)
                free(lattice[i]);
            break;
    }
    free(lattice);
}

/*
 * Randomization method
 */
void randomize() {
    int i, j;
    for (i=0; i<T; i++) {
        for (j=left[i]; j<=right[i]; j++) {
            lattice[i][j] = rand() % THRESHOLD;
        }
    }
}

/*
 * alpha=1/Z, beta=1/Z, gamma=(Z-2)/Z
 */
void update() {
    int i, i1, j, j1, p, toppled;
    s = 0;
    t = 0;
    toppled = 1;
    i = 0;
    left[0] = 0;
    right[0] = 0;
    do {
        i1 = i + 1;
        toppled = 0;
        left[i1] = i;
        right[i1] = 0;
        for (j=0; j<i1; j++) {
            j1 = j+1;
            while (lattice[i][j] >= THRESHOLD) {
                t = i1;
                s++;
                toppled = 1;
                lattice[i][j] -= THRESHOLD;
                p = rand() % Z;
                switch (p) {
                    case 0:
                        lattice[i1][j] += THRESHOLD;
                        if (j < left[i1]) left[i1]=j;
                        if (j > right[i1]) right[i1]=j;
                        break;
                    case 1:
                        lattice[i1][j1] += THRESHOLD;
                        if (j1 < left[i1]) left[i1]=j1;
                        if (j1 > right[i1]) right[i1]=j1;
                        break;
                    default:
                        lattice[i1][j]   += THRESHOLD2;
                        if (j < left[i1]) left[i1]=j;
                        lattice[i1][j+1] += THRESHOLD2;
                        if (j1 > right[i1]) right[i1]=j1;
                }
            }
        }
    } while ((toppled==1) && (++i<L));
}

