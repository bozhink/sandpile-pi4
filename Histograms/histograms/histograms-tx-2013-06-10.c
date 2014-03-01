/*
 * file: histograms.c
 * This program calculates histograms and cumulative histograms
 */

#include <stdio.h>
#include <stdlib.h>
#define MAXL 10000
#define XDIM 2000
const char*fu1 = "Usage: %s <LS> <TFN> <XFN> <IFN>[, <IFN>, ...]\n";
const char*fu2 = "Where:\n";
const char*fu3 = "\tLS  = lattice size\n";
const char*fu4 = "\tTFN = t-histogram's output file name\n";
const char*fu5 = "\tSFN = s-histogram's output file name\n";
const char*fu6 = "\tAFN = a-histogram's output file name\n";
const char*fu7 = "\tXFN = x-histogram's output file name\n";
const char*fu8 = "\tIFN = input file name[s]\n";
const char*eam = "%s: ERROR: Memory allocation: %s\n";
const char*eif = "%s: ERROR: Cannot open input file!\n";
const char*eot = "%s: ERROR: Cannot open pt-output file!\n";
const char*eos = "%s: ERROR: Cannot open ps-output file!\n";
const char*eoa = "%s: ERROR: Cannot open pa-output file!\n";
const char*eox = "%s: ERROR: Cannot open px-output file!\n";

unsigned long *thistogram;
unsigned long *xhistogram;

int L;

FILE*fin;
FILE*fpt;
FILE*fpx;

void finalize();

int main(int argc, char**argv) {
    int i, ifile, L1;
    int t, s, a, X, tmax, smax, amax, xmax;
    int uls;
    unsigned long iarea;
    uls = sizeof(unsigned long);

    if (argc<5) {
        fprintf(stderr, fu1, argv[0]);
        fprintf(stderr, fu2);
        fprintf(stderr, fu3);
        fprintf(stderr, fu4);
        fprintf(stderr, fu7);
        fprintf(stderr, fu8);
        exit(1);
    }
    /*
     * Get lattice size
     */
    L = atoi(argv[1]);
    L1 = L + 1;
    printf("L = %d\n", L);
    /*
     * Allocate memory for histogram data
     */
    if (!(thistogram=(unsigned long*)malloc(L1 * uls))) { fprintf(stderr, eam, argv[0], "t"); exit(2); }
    if (!(xhistogram=(unsigned long*)malloc(XDIM * uls))) { fprintf(stderr, eam, argv[0], "x"); finalize(); exit(2); }
    /*
     * Open output files
     */
    if (!(fpt = fopen(argv[2], "w"))) { fprintf(stderr, eot, argv[0]); finalize(); exit(4); }
    if (!(fpx = fopen(argv[3], "w"))) { fprintf(stderr, eox, argv[0]); finalize(); exit(4); }

    tmax=0;
    smax=0;
    amax=0;
    xmax=0;
    iarea=0l;

        /* Initialization */
        for (i=0; i<L1; i++) thistogram[i] = 0l;
        for (i=0; i<XDIM; i++) xhistogram[i] = 0l;
        /* Processing data files */
        printf("Processing data files...\n");
        for (ifile = 4; ifile < argc; ifile++)
        {
            if (!(fin=fopen(argv[ifile], "r"))) { fprintf(stderr, eif, argv[0]); finalize(); exit(3); }
            /* Read input file */
            while (!feof(fin)) {
                fscanf(fin, "%d %d %d %d", &t, &s, &a, &X);
                if ((0 < t) && (t < L1)) {
                    if (t > tmax) tmax = t;
                    if (s > smax) smax = s;
                    if (a > amax) amax = a;
                    if (X > xmax) xmax = X;
                    thistogram[t]++;
                    xhistogram[X]++;
                    iarea ++;
                }
            }
            /* Close current input file */
            fclose(fin);
        }
        /* Some debug information */
        printf("t_max = %d\n", tmax);
        printf("s_max = %d\n", smax);
        printf("a_max = %d\n", amax);
        printf("z_max = %d\n", xmax);
        printf("Number of points = %ld\n", iarea);
        /* Export currently acumulated information */
        /* t-histogram and x-histogram */
        printf("Writing data to pt-file...\n");
        printf("Writing data to px-file...\n");
        thistogram[0] = iarea;
        xhistogram[0] = iarea;
        for (i=0; i<L1; i++) if (thistogram[i]) fprintf(fpt, "%d %ld\n", i, thistogram[i]);
        for (i=0; i<XDIM; i++) if (xhistogram[i]) fprintf(fpx, "%d %ld\n", i, xhistogram[i]);
    fclose(fpt);
    fclose(fpx);
    finalize();
    /* All done */
    printf("All done!\n");
    return 0;
}

void finalize() {
    if (xhistogram) free(xhistogram); xhistogram=NULL;
    if (thistogram) free(thistogram); thistogram=NULL;
}
