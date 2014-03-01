/*
 * floathist.c
 * This program calculates histograms of a data array <value>-<occurrencies>
 * and exports the floating-point normed histogram.
 *
 * author: Bozhin Karaivanov
 * date: 10.06.2013
 */

#include <stdio.h>
#include <stdlib.h>

int N;
unsigned *x;
unsigned long *h;
double *px;

void finalize();

int main(int argc, char**argv) {
    FILE *fin;
    FILE *fout;
    int i;
    double norm; /* normalization factor */
    unsigned xx;
    unsigned long hh;
    
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input histogram> <output histogram>\n", argv[0]);
        exit(1);
    }
    
    if (!(fin=fopen(argv[1],"r"))) {
        fprintf(stderr, "%s: Cannot open input histogram file!\n", argv[0]);
        exit(2);
    }
    if (!(fout=fopen(argv[2],"w"))) {
        fprintf(stderr, "%s: Cannot open output histogram file!\n", argv[0]);
        fclose(fin);
        exit(3);
    }
    
    /* Get the number of data points */
    N = 0;
    while (!(feof(fin))) {
        fscanf(fin, "%d %ld", &xx, &hh);
        N++;
    }
    fclose(fin);
    /* Allocate memory for histogram arrays */
    if (!(x = (unsigned*) malloc (N * sizeof(unsigned)))) {
        fprintf(stderr, "%s: Cannot allocate memory for x-array!\n", argv[0]);
        finalize();
        exit(4);
    }
    if (!(h = (unsigned long*) malloc (N * sizeof(unsigned long)))) {
        fprintf(stderr, "%s: Cannot allocate memory for h-array!\n", argv[0]);
        finalize();
        exit(4);
    }
    if (!(px = (double*) malloc (N * sizeof(double)))) {
        fprintf(stderr, "%s: Cannot allocate memory for px-array!\n", argv[0]);
        finalize();
        exit(4);
    }
    /* Re-open the input file */
    if (!(fin=fopen(argv[1],"r"))) {
        fprintf(stderr, "%s: Cannot open input histogram file!\n", argv[0]);
        fclose(fout);
        exit(2);
    }
    /* Read data */
    for (i=0; i<N; i++) {
        fscanf(fin, "%d %ld", &xx, &hh);
        x[i] = xx;
        h[i] = hh;
    }
    fclose(fin);
    /* Calculate histograms */
    norm = 2.0 * h[0];
    for (i=0; i<N; i++) px[i] = h[i]/norm;
    /* Write to output file */
    for (i=0; i<N; i++) {
        fprintf(fout, "%d %ld %20.16f\n", x[i], h[i], px[i]);
    }
    finalize();
    fclose(fout);
    return 0;
}

void finalize() {
    if (x) free (x); x = NULL;
    if (h) free (h); h = NULL;
    if (px) free(px); px = NULL;
}

