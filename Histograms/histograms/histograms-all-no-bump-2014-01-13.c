/*
 * author: Bozhin Karaivanov
 * file: histograms-all-no-bump.c
 * date created: 13.01.2014
 * last modified: 14.01.2014
 * last modified: 22.01.2014
 *
 * This program calculates histograms of all the four parameters of the avalanche
 * process. It is supposed that the t-parameter is the leading one, and
 * the resulting histograms (non-normalized probability density distributions)
 * does not contain bumps in their tail.
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define _NO_BUMP 1
#if _NO_BUMP!=0
#pragma message("Histograms will be compiled to not produce bumps.")
#endif
#include <stdio.h>
#include <stdlib.h>
#define MAXL 100000
#define MAXL2 100000000
#define XDIM 2000
#define UNSIGNED_LONG_SIZE sizeof(unsigned long)
#define ERROR_MEMORY_ALLOCATION "%s: ERROR: Memory allocation: %s\n"
#define ERROR_OPEN_OUT_FILE "%s: ERROR: Cannot open p%s-output file!\n"
#define ERROR_OPEN_IN_FILE "%s: ERROR: Cannot open input file: %s\n"


unsigned long *t_histogram;
unsigned long *s_histogram;
unsigned long *a_histogram;
unsigned long *x_histogram;

unsigned t, t_max;
unsigned s, s_max;
unsigned a, a_max;
unsigned x, x_max;

unsigned L;

FILE*fin;
FILE*fps;
FILE*fpt;
FILE*fpa;
FILE*fpx;

void print_help(char*);
void raise_allocation_error(char*, char*);
void raise_open_out_error(char*, char*);
void raise_open_in_error(char*, char*);
void finalize();

int main(int argc, char**argv) {
    unsigned i, l, L1, L2, left, right;
    int ifile, ss, aa; /* If there is s>L2 or a>L2*/
    unsigned long iarea;

    if (argc<5) {
        print_help(argv[0]);
        exit(1);
    }
    /*
     * Get lattice size
     */
    L = atoi(argv[1]);
    if (L > MAXL) {
        fprintf(stderr, "L must be less then %d. You have entered %d.\n", MAXL, L);
        exit(100);
    }
    if (L < 1) {
        fprintf(stderr, "L must be greater than 1. You have entered %d.\n", L);
        exit(101);
    }
    L2 = L * L;
    if (L2 > MAXL2) {
        L2 = MAXL2;
    }
    L1 = L + 1;
    printf("L = %d.\n", L);
    /*
     * Allocate memory for histogram data
     */
    if (!(t_histogram = (unsigned long*)malloc(L1 * UNSIGNED_LONG_SIZE))) {
        raise_allocation_error(argv[0], "t");
    }
    if (!(x_histogram = (unsigned long*)malloc(XDIM * UNSIGNED_LONG_SIZE))) {
        raise_allocation_error(argv[0], "x");
    }
    if (!(s_histogram = (unsigned long*)malloc(L2 * UNSIGNED_LONG_SIZE))) {
        raise_allocation_error(argv[0], "s");
    }
    if (!(a_histogram = (unsigned long*)malloc(L2 * UNSIGNED_LONG_SIZE))) {
        raise_allocation_error(argv[0], "a");
    }
    /*
     * Open output files
     */
    if (!(fpt = fopen(argv[2], "w"))) {
        raise_open_out_error(argv[0], "t");
    }
    if (!(fps = fopen(argv[3], "w"))) {
        raise_open_out_error(argv[0], "s");
    }
    if (!(fpa = fopen(argv[4], "w"))) {
        raise_open_out_error(argv[0], "a");
    }
    if (!(fpx = fopen(argv[5], "w"))) {
        raise_open_out_error(argv[0], "x");
    }

    t_max = 0;
    s_max = 0;
    a_max = 0;
    x_max = 0;
    iarea = 0l;
    
    /* First pass. Build t- and x-histograms and first part of the s- and a-histograms */
    for (i = 0; i < L1; i++)
        t_histogram[i] = 0l;
    for (i = 0; i < XDIM; i++)
        x_histogram[i] = 0l;
    for (i = 0; i < L2; i++) {
        s_histogram[i] = 0l;
        a_histogram[i] = 0l;
    }
    printf("Processing data files...\n");
    ss = 0;
    aa = 0;
    for (ifile = 6; ifile < argc; ifile++)
    {
        if (!(fin = fopen(argv[ifile], "r"))) {
            raise_open_in_error(argv[0], argv[ifile]);
        }
        /* Read input file */
        while (!feof(fin)) {
            fscanf(fin, "%d %d %d %d", &t, &s, &a, &x);
#if _NO_BUMP!=0
            if ((0 < t) && (t < L1)) {
#endif
            if (t > t_max) t_max = t;
            if (s > s_max) s_max = s;
            if (a > a_max) a_max = a;
            if (x > x_max) x_max = x;
#if _NO_BUMP==0
            if (0 <= t && t < L1) {
#endif
            t_histogram[t]++;
#if _NO_BUMP==0
            } else {
                fprintf(stderr, "t > L in file %s.\n", argv[ifile]);
                finalize();
                exit(10);
            }
#endif
            if (0 <= x && x < XDIM) {
                x_histogram[x]++;
            } else {
                fprintf(stderr, "x > XDIM in file %s.\n", argv[ifile]);
                finalize();
                exit(11);
            }
            if (s < L2) {
                s_histogram[s]++;
            } else {
                ss = 1;
            }
            if (a < L2) {
                a_histogram[a]++;
            } else {
                aa = 1;
            }
            iarea ++;
#if _NO_BUMP!=0
            }
#endif
        }
        /* Close current input file */
        fclose(fin);
    }
    t_histogram[0] = --iarea;
    x_histogram[0] = iarea;
    s_histogram[0] = iarea;
    a_histogram[0] = iarea;
    
    /* Some debug information */
    printf("t_max = %d\n", t_max);
    printf("s_max = %d\n", s_max);
    printf("a_max = %d\n", a_max);
    printf("x_max = %d\n", x_max);
    printf("Number of points = %ld\n", iarea);
    /* Export currently acumulated information */
    /* s-histogram and a-histogram */
    printf("Writing data to ps-file...\n");
    printf("Writing data to pa-file...\n");
    for (i=0; i<L2; i++) {
        if (s_histogram[i]) fprintf(fps, "%d %ld\n", i, s_histogram[i]);
        if (a_histogram[i]) fprintf(fpa, "%d %ld\n", i, a_histogram[i]);
    }
    /* t-histogram and x-histogram */
    printf("Writing data to pt-file...\n");
    for (i=0; i<L1; i++) {
        if (t_histogram[i]) fprintf(fpt, "%d %ld\n", i, t_histogram[i]);
    }
    printf("Writing data to px-file...\n");
    for (i=0; i<XDIM; i++) {
        if (x_histogram[i]) fprintf(fpx, "%d %ld\n", i, x_histogram[i]);
    }
    /* Deallocate memory used for t- and x-histograms */
    free(t_histogram); t_histogram = NULL;
    free(x_histogram); x_histogram = NULL;
    fclose(fpt);
    fclose(fpx);
    
    l = 1;
    while (ss != 0 || aa != 0) {
        ss = 0;
        aa = 0;
        for (i = 0; i < L2; i++) {
            s_histogram[i] = 0l;
            a_histogram[i] = 0l;
        }
        left = l++ * L2;
        right = l * L2;
        for (ifile = 6; ifile < argc; ifile++)
        {
            if (!(fin = fopen(argv[ifile], "r"))) {
                raise_open_in_error(argv[0], argv[ifile]);
            }
            /* Read input file */
            while (!feof(fin)) {
                fscanf(fin, "%d %d %d %d", &t, &s, &a, &x);
#if _NO_BUMP!=0
                if ((0 < t) && (t < L1)) {
#endif
                if (left <= s) {
                    if (s < right) {
                        s_histogram[s%L2]++;
                    } else {
                        ss = 1;
                    }
                }
                if (left <= a) {
                    if (a < right) {
                        a_histogram[a%L2]++;
                    } else {
                        aa = 1;
                    }
                }
#if _NO_BUMP!=0
                }
#endif
            }
            /* Close current input file */
            fclose(fin);
        }
        /* Export currently acumulated information */
        /* s-histogram and a-histogram */
        printf("Writing data to ps-file...\n");
        printf("Writing data to pa-file...\n");
        for (i=0; i<L2; i++) {
            if (s_histogram[i]) fprintf(fps, "%d %ld\n", left + i, s_histogram[i]);
            if (a_histogram[i]) fprintf(fpa, "%d %ld\n", left + i, a_histogram[i]);
        }
    }
    fclose(fps);
    fclose(fpa);

    finalize();
    /* All done */
    printf("All done!\n");
    return 0;
}


void print_help(char*argv0) {
    fprintf(stderr, "Usage: %s <LS> <TFN> <SFN> <AFN> <XFN> <IFN>[, <IFN>, ...]\n", argv0);
    fprintf(stderr, "Where:\n");
    fprintf(stderr, "\tLS  = lattice size\n");
    fprintf(stderr, "\tTFN = t-histogram's output file name\n");
    fprintf(stderr, "\tSFN = s-histogram's output file name\n");
    fprintf(stderr, "\tAFN = a-histogram's output file name\n");
    fprintf(stderr, "\tXFN = x-histogram's output file name\n");
    fprintf(stderr, "\tIFN = input file name[s]\n");
}

void raise_allocation_error(char*argv0, char*parameter) {
    fprintf(stderr, ERROR_MEMORY_ALLOCATION, argv0, parameter);
    finalize();
    exit(2);
}

void raise_open_out_error(char*argv0, char*parameter) {
    fprintf(stderr, ERROR_OPEN_OUT_FILE, argv0, parameter);
    finalize();
    exit(4);
}

void raise_open_in_error(char*argv0, char*file) {
    fprintf(stderr, ERROR_OPEN_IN_FILE, argv0, file);
    finalize();
    exit(3);
}

void finalize() {
    if (a_histogram) free(a_histogram); a_histogram=NULL;
    if (s_histogram) free(s_histogram); s_histogram=NULL;
    if (x_histogram) free(x_histogram); x_histogram=NULL;
    if (t_histogram) free(t_histogram); t_histogram=NULL;
}

