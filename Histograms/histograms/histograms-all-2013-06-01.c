/*
 * file: histograms-all.c
 * This program calculates histograms and cumulative histograms
 */

#include <stdio.h>
#include <stdlib.h>
#define MAXL 10000
#define XDIM 2000
const char*fu1 = "Usage: %s <LS> <TFN> <SFN> <AFN> <XFN> <IFN>[, <IFN>, ...]\n";
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
unsigned long *shistogram;
unsigned long *ahistogram;
unsigned long *xhistogram;

int L;

FILE*fin;
FILE*fps;
FILE*fpt;
FILE*fpa;
FILE*fpx;

void finalize();

int main(int argc, char**argv) {
    int i, ifile, L1, L2, l, left, right;
    int t, s, a, X, tmax, smax, amax, xmax;
    int ss, aa; /* If there is s>L2 or a>L2*/
    int uls;
    unsigned long iarea;
    uls = sizeof(unsigned long);

    if (argc<5) {
        fprintf(stderr, fu1, argv[0]);
        fprintf(stderr, fu2);
        fprintf(stderr, fu3);
        fprintf(stderr, fu4);
        fprintf(stderr, fu5);
        fprintf(stderr, fu6);
        fprintf(stderr, fu7);
        fprintf(stderr, fu8);
        exit(1);
    }
    /*
     * Get lattice size
     */
    L = atoi(argv[1]);
    if (L>MAXL) {
        L2 = MAXL * MAXL;
    } else {
        L2 = L * L;
    }
    L1 = L + 1;
    printf("L = %d\n", L);
    /*
     * Allocate memory for histogram data
     */
    if (!(thistogram=(unsigned long*)malloc(L1*uls))) { fprintf(stderr, eam, argv[0], "t"); exit(2); }
    if (!(xhistogram=(unsigned long*)malloc(XDIM * uls))) { fprintf(stderr, eam, argv[0], "x"); finalize(); exit(2); }
    if (!(shistogram=(unsigned long*)malloc(L2*uls))) { fprintf(stderr, eam, argv[0], "s"); finalize(); exit(2); }
    if (!(ahistogram=(unsigned long*)malloc(L2*uls))) { fprintf(stderr, eam, argv[0], "a"); finalize(); exit(2); }
    /*
     * Open output files
     */
    if (!(fpt = fopen(argv[2], "w"))) { fprintf(stderr, eot, argv[0]); finalize(); exit(4); }
    if (!(fps = fopen(argv[3], "w"))) { fprintf(stderr, eos, argv[0]); finalize(); exit(4); }
    if (!(fpa = fopen(argv[4], "w"))) { fprintf(stderr, eoa, argv[0]); finalize(); exit(4); }
    if (!(fpx = fopen(argv[5], "w"))) { fprintf(stderr, eox, argv[0]); finalize(); exit(4); }

    tmax=0;
    smax=0;
    amax=0;
    xmax=0;
    iarea=0l;
    
    /* First pass. Build t- and x-histograms and first part of the s- and a-histograms */
    for (i=0; i<L1; i++) thistogram[i] = 0l;
    for (i=0; i<XDIM; i++) xhistogram[i] = 0l;
    for (i=0; i<L2; i++) { shistogram[i]=0l; ahistogram[i]=0l; }
    printf("Processing data files...\n");
    ss = 0;
    aa = 0;
    for (ifile = 6; ifile < argc; ifile++)
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
                if (s < L2) {
                    shistogram[s]++;
                } else {
                    ss = 1;
                }
                if (a < L2) {
                    ahistogram[a]++;
                } else {
                    aa = 1;
                }
                iarea ++;
            }
        }
        /* Close current input file */
        fclose(fin);
    }
    thistogram[0] = --iarea;
    xhistogram[0] = iarea;
    shistogram[0] = iarea;
    ahistogram[0] = iarea;
    
    /* Some debug information */
    printf("t_max = %d\n", tmax);
    printf("s_max = %d\n", smax);
    printf("a_max = %d\n", amax);
    printf("x_max = %d\n", xmax);
    printf("Number of points = %ld\n", iarea);
    /* Export currently acumulated information */
    /* s-histogram and a-histogram */
    printf("Writing data to ps-file...\n");
    printf("Writing data to pa-file...\n");
    for (i=0; i<L2; i++) {
        if (shistogram[i]) fprintf(fps, "%d %ld\n", i, shistogram[i]);
        if (ahistogram[i]) fprintf(fpa, "%d %ld\n", i, ahistogram[i]);
    }
    /* t-histogram and x-histogram */
    printf("Writing data to pt-file...\n");
    for (i=0; i<L1; i++) {
        if (thistogram[i]) fprintf(fpt, "%d %ld\n", i, thistogram[i]);
    }
    printf("Writing data to px-file...\n");
    for (i=0; i<XDIM; i++) {
        if (xhistogram[i]) fprintf(fpx, "%d %ld\n", i, xhistogram[i]);
    }
    /* Deallocate memory used for t- and x-histograms */
    free(thistogram); thistogram = NULL;
    free(xhistogram); xhistogram = NULL;
    fclose(fpt);
    fclose(fpx);
    
    l = 1;
    while (ss!=0 || aa!=0) {
        ss = 0;
        aa = 0;
        for (i=0; i<L2; i++) {
            shistogram[i] = 0l;
            ahistogram[i] = 0l;
        }
        left = l++ * L2;
        right = l * L2;
        for (ifile = 6; ifile < argc; ifile++)
        {
            if (!(fin=fopen(argv[ifile], "r"))) { fprintf(stderr, eif, argv[0]); finalize(); exit(3); }
            /* Read input file */
            while (!feof(fin)) {
                fscanf(fin, "%d %d %d %d", &t, &s, &a, &X);
                if ((0 < t) && (t < L1)) {
                    if (left <= s) {
                        if (s < right) {
                            shistogram[s%L2]++;
                        } else {
                            ss = 1;
                        }
                    }
                    if (left <= a) {
                        if (a < right) {
                            ahistogram[a%L2]++;
                        } else {
                            aa = 1;
                        }
                    }
                }
            }
            /* Close current input file */
            fclose(fin);
        }
        /* Export currently acumulated information */
        /* s-histogram and a-histogram */
        printf("Writing data to ps-file...\n");
        printf("Writing data to pa-file...\n");
        for (i=0; i<L2; i++) {
            if (shistogram[i]) fprintf(fps, "%d %ld\n", left + i, shistogram[i]);
            if (ahistogram[i]) fprintf(fpa, "%d %ld\n", left + i, ahistogram[i]);
        }
    }
    fclose(fps);
    fclose(fpa);

    finalize();
    /* All done */
    printf("All done!\n");
    return 0;
}

void finalize() {
    if (ahistogram) free(ahistogram); ahistogram=NULL;
    if (shistogram) free(shistogram); shistogram=NULL;
    if (xhistogram) free(xhistogram); xhistogram=NULL;
    if (thistogram) free(thistogram); thistogram=NULL;
}

