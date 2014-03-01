/*
 * file: pi4-ssimple.c
 * Speed (super) simple version of the program
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
int N;
double *dos;

FILE*fp;      /* Output file handler */

void dosrand();
void init();
void finalize();
void randomize();
void update();
unsigned long energy();

int main(int argc, char**argv) {
    int i;
    
    if (argc<4) {
        fprintf(stderr, "Usage: %s <N> <L> [<FN>]\n", argv[0]);
        fprintf(stderr, "Where:\n");
        fprintf(stderr, "\t N  =  number of iterations\n");
        fprintf(stderr, "\t L  =  lattice size\n");
        fprintf(stderr, "\tFN  =  output file name\n");
        exit(1);
    }
    fp = fopen(argv[3], "w");
    if (!fp) {
        fprintf(stderr, "%s: ERROR: Cannot open output file!\n", argv[0]);
        exit(2);
    }
    
    N = atoi(argv[1]);
    fprintf(stderr, "Number of iterations = %d\n", N);
    
    L = atoi(argv[2]);
    fprintf(stderr, "Lattice size = %d\n", L);
    init();

    for (i=0; i<N; i++) {
        lattice[0][0]++; /* Drop sand grain */
        update();
        dos[i] = (1.0*energy()) / (L*L/2.0);
        if (!((i+1)%100)) fprintf(fp, "%d %g\n", i+1, dos[i]);
    }
    printf("%ld\n", energy());
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
    
    dos = (double*) malloc(N * sizeof(double));
    if (!dos) {
        fprintf(stderr, "%s: ERROR: Cannot allocate memory for DOS array!\n");
        finalize();
        exit(3);
    }
    for (i=0; i<N; i++) {
        dos[i] = 0.0;
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
    free(dos);
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
 * alpha=1/4, beta=1/4, gamma=1/2
 */
void update() {
    int i, i1, j, p;
    s = 0;
    t = 0;
    for (i=0; i<L; i++) {
        i1 = i + 1;
        for (j=0; j<i1; j++) {
            while (lattice[i][j] >= THRESHOLD) {
                t = i1;
                s++;
                lattice[i][j] -= THRESHOLD;
                p = rand() % 4;
                switch (p) {
                    case 0:
                        lattice[i1][j] += THRESHOLD;
                        break;
                    case 1:
                        lattice[i1][j+1] += THRESHOLD;
                        break;
                    default:
                        lattice[i1][j]   += THRESHOLD2;
                        lattice[i1][j+1] += THRESHOLD2;
                }
            }
        }
    }
    t++;
}



unsigned long energy() {
    int i, j;
    unsigned long sum=0l;
    for (i=0; i<L; i++) {
        for (j=0; j<=i; j++) {
            sum += (unsigned long) lattice[i][j];
        }
    }
    return sum;
}


