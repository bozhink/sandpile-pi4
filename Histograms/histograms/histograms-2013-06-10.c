/*
 * file: histograms.c
 * This program calculates histograms and cumulative histograms
 * using linked lists
 *
 * author: Bozhin Karaivanov
 * date: 10.06.2013
 */

#include <stdio.h>
#include <stdlib.h>
#define L_MAX 100001
const char*fu1 = "Usage: %s <LS> <TFN> <SFN> <AFN> <XFN> <IFN>[, <IFN>...]\n";
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

unsigned long*thistogram;
unsigned long*shistogram;
unsigned long*ahistogram;
unsigned long*xhistogram;
int L;

FILE*fin;
FILE*fps;
FILE*fpt;
FILE*fpa;
FILE*fpx;

typedef struct node *link;
struct node {
    unsigned x;
    unsigned hx;
    link next;
};

link shead;
link ahead;

void finalize();

int main(int argc, char**argv) {
    int i, ifile, L1, L2;
    unsigned t, s, a, x, tmax, smax, amax, xmax;
    unsigned iarea;
    link tmp, hist;

    if (argc<7) {
        fprintf(stderr, fu1, argv[0]);
        fprintf(stderr, fu2); fprintf(stderr, fu3);
        fprintf(stderr, fu4); fprintf(stderr, fu5);
        fprintf(stderr, fu6); fprintf(stderr, fu7);
        fprintf(stderr, fu8);
        exit(1);
    }
    /*
     * Get lattice size
     */
    L = atoi(argv[1]);
    printf("L = %d\n", L);
    L1 = L+1;
    if (L1 > L_MAX) L1 = L_MAX;
    L2 = L*L;
    if (L2 > L_MAX) {
        L2 = L_MAX;
    }
    /*
     * Open output files
     */
    if (!(fpt = fopen(argv[2], "w"))) { fprintf(stderr, eot, argv[0]); finalize(); exit(4); }
    if (!(fps = fopen(argv[3], "w"))) { fprintf(stderr, eos, argv[0]); finalize(); exit(4); }
    if (!(fpa = fopen(argv[4], "w"))) { fprintf(stderr, eoa, argv[0]); finalize(); exit(4); }
    if (!(fpx = fopen(argv[5], "w"))) { fprintf(stderr, eox, argv[0]); finalize(); exit(4); }
    
    /*
     * Allocate memory for histogram arrays
     */
    if (!(thistogram = (unsigned long*) malloc(L1*sizeof(unsigned long)))) {
        fprintf(stderr, "ERROR: %s: Can not allocate memory for t-array\n", argv[0]);
        finalize();
        exit(3);
    }
    if (!(xhistogram = (unsigned long*) malloc(L1*sizeof(unsigned long)))) {
        fprintf (stderr, "ERROR: %s: Can not allocate memory for x-array\n", argv[0]);
        finalize();
        exit(3);
    }
    if (!(shistogram = (unsigned long*) malloc(L2*sizeof(unsigned long)))) {
        fprintf (stderr, "ERROR: %s: Can not allocate memory for s-array\n", argv[0]);
        finalize();
        exit(3);
    }
    if (!(ahistogram = (unsigned long*) malloc(L2*sizeof(unsigned long)))) {
        fprintf (stderr, "ERROR: %s: Can not allocate memory for a-array\n", argv[0]);
        finalize();
        exit(3);
    }
    for (i = 0; i < L1; i++) {
        thistogram[i] = xhistogram[i] = 0l;
    }
    for (i = 0; i < L2; i++) {
        shistogram[i] = ahistogram[i] = 0l;
    }
    /*
     * Allocation of s- and a-linked lists
     */
    if (!(shead = malloc(sizeof(link)))) {
        fprintf (stderr, "ERROR: %s: Can not allocate memory for s-node\n", argv[0]);
        finalize();
        exit(2);
    }
    shead->x = L2;
    shead->hx = 0;
    shead->next = NULL;
    if (!(ahead = malloc(sizeof(link)))) {
        fprintf (stderr, "ERROR: %s: Can not allocate memory for a-node\n", argv[0]);
        finalize();
        exit(2);
    }
    ahead->x = L2;
    ahead->hx = 0;
    ahead->next = NULL;
    
    /*
     * Processing data file
     */
    printf("Processing data file...\n");
    tmax=0;
    smax=0;
    amax=0;
    xmax=0;
    iarea=0;
    for (ifile = 6; ifile < argc; ifile++)
    {
        if (!(fin=fopen(argv[ifile], "r"))) {
            fprintf(stderr, eif, argv[0]);
            finalize();
            exit(3);
        }
        while (!feof(fin))
        {
            fscanf (fin, "%d %d %d %d", &t, &s, &a, &x);
            if (t < L1 && t >= 0) {
                iarea++;
                /* t-parameter */
                if (t > tmax) tmax = t;
                thistogram[t]++;
                /* x-parameter */
                if (x > xmax) xmax = x;
                xhistogram[x]++;
                /* s-parameter */
                if (s > smax) smax = s;
                if (s < L2) {
                    shistogram[s]++;
                } else {
                    for (hist = shead; hist != NULL; hist = hist->next) {
                        if (hist->x == s) {
                            hist->hx++;
                            break;
                        } else {
                            if (hist->next != NULL) {
                                if (hist->next->x > s) {
                                    if (!(tmp = malloc(sizeof(link)))) {
                                        fprintf (stderr, "ERROR: %s: Can not allocate memory for s-node\n", argv[0]);
                                        finalize();
                                        exit(2);
                                    }
                                    tmp->x = s;
                                    tmp->hx = 0;
                                    tmp->next = hist->next;
                                    hist->next = tmp;
                                }
                            } else {
                                if (!(tmp = malloc(sizeof(link)))) {
                                    fprintf (stderr, "ERROR: %s: Can not allocate memory for s-node\n", argv[0]);
                                    finalize();
                                    exit(2);
                                }
                                tmp->x = s;
                                tmp->hx = 0;
                                tmp->next = hist->next; // Here this must be NULL
                                hist->next = tmp;
                            }
                        }
                    }
                }
                /* a-parameter */
                if (a > amax) amax = a;
                if (a < L2) {
                    ahistogram[a]++;
                } else {
                    for (hist = ahead; hist != NULL; hist = hist->next) {
                        if (hist->x == a) {
                            hist->hx++;
                            break;
                        } else {
                            if (hist->next != NULL) {
                                if (hist->next->x > a) {
                                    if (!(tmp = malloc(sizeof(link)))) {
                                        fprintf (stderr, "ERROR: %s: Can not allocate memory for a-node\n", argv[0]);
                                        finalize();
                                        exit(2);
                                    }
                                    tmp->x = a;
                                    tmp->hx = 0;
                                    tmp->next = hist->next;
                                    hist->next = tmp;
                                }
                            } else {
                                if (!(tmp = malloc(sizeof(link)))) {
                                    fprintf (stderr, "ERROR: %s: Can not allocate memory for a-node\n", argv[0]);
                                    finalize();
                                    exit(2);
                                }
                                tmp->x = a;
                                tmp->hx = 0;
                                tmp->next = hist->next; // Here this must be NULL
                                hist->next = tmp;
                            }
                        }
                    }
                }
            }
        }
        fclose(fin);
    }
    --iarea;
    thistogram[0] = xhistogram[0] = shistogram[0] = ahistogram[0] = iarea;
    
    /*
     * Export data
     */
    for (i = 0; i < L1; i++) {
        if (thistogram[i]) fprintf(fpt, "%ld %ld\n", i, thistogram[i]);
        if (xhistogram[i]) fprintf(fpx, "%ld %ld\n", i, xhistogram[i]);
    }
    for (i = 0; i < L2; i++) {
        if (shistogram[i]) fprintf(fps, "%ld %ld\n", i, shistogram[i]);
        if (ahistogram[i]) fprintf(fpa, "%ld %ld\n", i, ahistogram[i]);
    }
    for (hist = shead; hist != NULL; hist = hist->next) {
        fprintf(fps, "%d %d\n", hist->x, hist->hx);
    }
    for (hist = ahead; hist != NULL; hist = hist->next) {
        fprintf(fpa, "%d %d\n", hist->x, hist->hx);
    }
    
    finalize();

    fclose(fps);
    fclose(fpa);
    fclose(fpt);
    fclose(fpx);
    /*
     * Some debug information
     */
    printf("t_max = %d\n", tmax);
    printf("s_max = %d\n", smax);
    printf("a_max = %d\n", amax);
    printf("x_max = %d\n", xmax);
    printf("Number of points = %ld\n", iarea);
    /*
     * All done
     */
    printf("All done!\n");
    return 0;
}

void finalize() {
    link tmp;
    while (shead->next != NULL) {
         tmp = shead->next;
         shead->next = tmp->next;
         free(tmp);
    }
    while (ahead->next != NULL) {
         tmp = ahead->next;
         ahead->next = tmp->next;
         free(tmp);
    }
    free(shead); shead = NULL;
    free(ahead); ahead = NULL;
    if (thistogram) free(thistogram); thistogram = NULL;
    if (xhistogram) free(xhistogram); xhistogram = NULL;
    if (shistogram) free(shistogram); shistogram = NULL;
    if (ahistogram) free(ahistogram); ahistogram = NULL;
}

