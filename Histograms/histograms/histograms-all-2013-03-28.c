/*
 * file: histograms.c
 * This program calculates histograms and cumulative histograms
 */

#include <stdio.h>
#include <stdlib.h>
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
double*ps;
double*psi;

int L;

FILE*fin;
FILE*fps;
FILE*fpt;
FILE*fpa;
FILE*fpx;

void finalize(int);

int main(int argc, char**argv) {
    int i, ifile, L1, L2;
    int t, s, a, X, tmax, smax, amax, xmax;
    int uls, dbs;
    double area;
    unsigned long iarea;
    uls = sizeof(unsigned long);
    dbs = sizeof(double);

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
    L = atoi(argv[1]); L1=L+2; L2 = L*L;
    printf("L = %d\n", L);
    /*
     * Allocate memory for histogram data
     */
    if (!(thistogram=(unsigned long*)malloc(L1*uls))) { fprintf(stderr, eam, argv[0], "t"); exit(2); }
    if (!(xhistogram=(unsigned long*)malloc(L1*uls))) { fprintf(stderr, eam, argv[0], "x"); finalize(1); exit(2); }
    if (!(shistogram=(unsigned long*)malloc(L2*uls))) { fprintf(stderr, eam, argv[0], "s"); finalize(2); exit(2); }
    if (!(ahistogram=(unsigned long*)malloc(L2*uls))) { fprintf(stderr, eam, argv[0], "a"); finalize(3); exit(2); }
    /*
     * Initialization
     */
    for (i=0; i<L1; i++) { thistogram[i] = 0l; xhistogram[i] = 0l; }
    for (i=0; i<L2; i++) { shistogram[i] = 0l; ahistogram[i] = 0l; }
    /*
     * Processing data files
     */
    printf("Processing data files...\n");
    tmax=0;
    smax=0;
    amax=0;
    xmax=0;
    iarea=0l;
    for (ifile = 6; ifile < argc; ifile++)
    {
        if (!(fin=fopen(argv[ifile], "r"))) { fprintf(stderr, eif, argv[0]); finalize(4); exit(3); }
        /*
         * Read input file
         */
        while (!feof(fin))
        {
            fscanf(fin, "%d %d %d %d", &t, &s, &a, &X);
            if (t<=L && t>0) {
                if (t>tmax) tmax=t;
                if (s>smax) smax=s;
                if (a>amax) amax=a;
                if (X>xmax) xmax=X;
                thistogram[t]++;
                shistogram[s]++;
                ahistogram[a]++;
                xhistogram[X]++;
                iarea ++;
            }
        }
        /* Close current input file */
        fclose(fin);
    }
    area = 2.0*(--iarea);
    thistogram[0]=iarea;
    shistogram[0]=iarea;
    ahistogram[0]=iarea;
    xhistogram[0]=iarea;
    /*
     * Some debug information
     */
    printf("t_max = %d\n", tmax);
    printf("s_max = %d\n", smax);
    printf("a_max = %d\n", amax);
    printf("z_max = %d\n", xmax);
    printf("Number of points = %ld\n", iarea);
    printf("Calculation of histograms...\n");
    /*
     * Open output files
     */
    if (!(fpt = fopen(argv[2], "w"))) { fprintf(stderr, eot, argv[0]); finalize(4); exit(4); }
    if (!(fps = fopen(argv[3], "w"))) { fprintf(stderr, eos, argv[0]); finalize(4); exit(4); }
    if (!(fpa = fopen(argv[4], "w"))) { fprintf(stderr, eoa, argv[0]); finalize(4); exit(4); }
    if (!(fpx = fopen(argv[5], "w"))) { fprintf(stderr, eox, argv[0]); finalize(4); exit(4); }
    
    /*
     * Calculation of histograms
     */
    if (!(ps  = (double*) malloc(L2*dbs))) { fprintf(stderr, eam, argv[0], "ps");  finalize(4); exit(2); }
    if (!(psi = (double*) malloc(L2*dbs))) { fprintf(stderr, eam, argv[0], "psi"); finalize(5); exit(2); }
    /*
     * s-histogram
     */
    ps[0] = 0.5;           for (i=1; i<L2; i++)    ps[i] = shistogram[i] / area;
    psi[L1-1] = ps[L1-1];  for (i=L2-2; i>=0; i--) psi[i] = ps[i] + psi[i+1];
    printf("Writing data to ps-file...\n");
    for (i=0; i<L2; i++) {
        if (shistogram[i]) fprintf(fps, "%d %ld %18.14lf %18.14lf\n", i, shistogram[i], ps[i], psi[i]);
    }
    fclose(fps);
    free(shistogram);
    /*
     * a-histogram
     */
    ps[0] = 0.5;           for (i=1; i<L2; i++)    ps[i] = ahistogram[i] / area;
    psi[L1-1] = ps[L1-1];  for (i=L2-2; i>=0; i--) psi[i] = ps[i] + psi[i+1];
    printf("Writing data to pa-file...\n");
    for (i=0; i<L2; i++) {
        if (ahistogram[i]) fprintf(fpa, "%d %ld %18.14lf %18.14lf\n", i, ahistogram[i], ps[i], psi[i]);
    }
    fclose(fpa);
    free(ahistogram);
    /*******************/
    free(ps);
    free(psi);
    if (!(ps  = (double*) malloc(L1*dbs))) { fprintf(stderr, eam, argv[0], "ps");  finalize(2); exit(2); }
    if (!(psi = (double*) malloc(L1*dbs))) { fprintf(stderr, eam, argv[0], "psi"); finalize(2); free(ps); exit(2); }
    /*
     * t-histogram
     */
    ps[0] = 0.5;           for (i=1; i<L1; i++)    ps[i] = thistogram[i] / area;
    psi[L1-1] = ps[L1-1];  for (i=L1-2; i>=0; i--) psi[i] = ps[i] + psi[i+1];
    printf("Writing data to pt-file...\n");
    for (i=0; i<L1; i++) {
        if (thistogram[i]) fprintf(fpt, "%d %ld %18.14lf %18.14lf\n", i, thistogram[i], ps[i], psi[i]);
    }
    fclose(fpt);
    free(thistogram);
    /*
     * x-histogram
     */
    ps[0] = 0.5;           for (i=1; i<L1; i++)    ps[i] = xhistogram[i] / area;
    psi[L1-1] = ps[L1-1];  for (i=L1-2; i>=0; i--) psi[i] = ps[i] + psi[i+1];
    printf("Writing data to px-file...\n");
    for (i=0; i<L1; i++) {
        if (xhistogram[i]) fprintf(fpx, "%d %ld %18.14lf %18.14lf\n", i, xhistogram[i], ps[i], psi[i]);
    }
    fclose(fpx);
    free(xhistogram);
    /*
     * All done
     */
    printf("All done!\n");
    free(ps);
    free(psi);

    return 0;
}



void finalize(int mode) {
    switch (mode) {
        default:
        case 6: free(psi);
        case 5: free(ps);
        case 4: free(ahistogram);
        case 3: free(shistogram);
        case 2: free(xhistogram);
        case 1: free(thistogram);
    }
}

