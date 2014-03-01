/*
 * file: pi4-dsm.c
 * Deterministic sandpile model
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
#define NITER 100

int**lattice; /* Lattice array */
int L;        /* Lattice size */
int t;        /* Avalanche's length */
int s;        /* Avalanche's size */
int T;

FILE*fp;      /* Output file handler */
char*fname;   /* Output file name */

void dosrand();
void init();
void finalize();
void randomize();
void update();

int main(int argc, char**argv) {
    int i, N;
    
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
    for (i=0; i<N; i++) {
    again:
        randomize();
        lattice[0][0]++; /* Drop sand grain */
        update();
        T = t; if (T>L) T=L;
        if (t > L) goto again;
        fprintf(fp, "%d %d\n", t, s);
    }
    /* printf("\n"); */
    finalize();
    fclose(fp);
    return 0;
}

/*
 * This method initializes the build-in pseudo-random generator
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
    int i, j, L1;
    L1 = L+1;
    /* Allocation of memory for lattice array */
    lattice = (int**) malloc ( L1 * sizeof(int*));
    for (i=0; i<L1; i++) {
        lattice[i] = (int*) malloc ( (i+1) * sizeof(int) );
    }
    if (!lattice) {
        fprintf(stderr, "%s: ERROR: Cannot allocate memory for lattice's array!\n");
        exit(3);
    }
    for (i=0; i<L1; i++) {
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
void finalize() {
    int i;
    for (i=0; i<L+1; i++) free(lattice[i]);
    free(lattice);
}

/*
 * Randomization method
 */
void randomize() {
    int i, j;
    for (i=0; i<T; i++) {
        for (j=0; j<i+1; j++) {
            lattice[i][j] = rand() % THRESHOLD;
        }
    }
}

/*
 * alpha=0, beta=0, gamma=1
 */
void update() {
    int i, i1, j;
    s = 0;
    t = 0;
    for (i=0; i<L; i++) {
        i1 = i + 1;
        for (j=0; j<i1; j++) {
            while (lattice[i][j] >= THRESHOLD) {
                t = i1;
                s++;
                lattice[i][j]    -= THRESHOLD;
                lattice[i1][j]   += THRESHOLD2;
                lattice[i1][j+1] += THRESHOLD2;
            }
        }
    }
    t++;
}



