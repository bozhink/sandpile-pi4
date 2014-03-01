#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NQMOMENTS 30
const char*eo = "%s: ERROR: Cannot open output file \"%s\"!\n";
const char*ei = "%s: ERROR: Cannot open input file \"%s\"!\n";
const char*pr = "Processing file \"%s\" for lattice size %d.\n";
const char*amh = "%s: ERROR: Memory allocation: \"%s\"!\n";

double*x;
double*px;
int L, LM;
FILE*fout;      /* Output file */
FILE*fin;       /* Current input file */
char*finname;   /* Current input file name */

double minQMoment = 0.0;
double maxQMoment = 5.0;
double qvalues[NQMOMENTS];
double qmoments[NQMOMENTS];
double qstride;

void print_help (char*);

int main (int argc, char**argv) {
    int i, j, ifile;
    int t, dbls;
    unsigned long ht;
    double pt, xq;
    if (argc < 6) {
        print_help (argv[0]);
    }
    /* Open output file */
    if (!(fout = fopen (argv[1], "w"))) { fprintf (stderr, eo, argv[0], argv[1]); exit (2); }
    
    /* Get Qmax */
    maxQMoment = atof (argv[2]);
    
    /* Initialization */
    dbls = sizeof (double);
    qstride = (maxQMoment - minQMoment) / (NQMOMENTS - 1);
    for (i = 0; i < NQMOMENTS; i++) { qvalues[i] = minQMoment + qstride * i; }

    /* Writing q-values as the q-moments of lattice with size 0 */
    fprintf (fout, "0");
    for (i = 0; i < NQMOMENTS; i++) { fprintf (fout, " %g", qvalues[i]); }
    fprintf (fout, "\n");

    /* Scanning input files */
    for (ifile = 3; ifile < 3*(argc/3); ifile += 3) {
        L = atoi (argv[ifile]);
        LM = atoi (argv[ifile+1]);
        finname = argv[ifile+2];

        printf (pr, finname, L);

        if (!(fin = fopen (finname, "r"))) { fprintf (stderr, ei, argv[0], finname); fclose (fout); exit (4); }
        if (!(x  = (double*) malloc (LM * dbls))) { fprintf (stderr, amh, argv[0], finname); fclose (fin); fclose (fout); exit (5); }
        if (!(px = (double*) malloc (LM * dbls))) { fprintf (stderr, amh, argv[0], finname); fclose (fin); fclose (fout); free (x); exit (6); }
        for (i=0; i<LM; i++) { x[i]=0.0; px[i]=0.0; }
        /* Read histograms */
        for ( i = 0; (!feof (fin)) && i < LM; i++) {
            fscanf (fin, "%d %ld %lf", &t, &ht, &pt);
            x[i] = 1.0*t;
            px[i] = pt;
        }
        /* Calculation of q-moments */
        for (i = 0; i < NQMOMENTS; i++) {
            xq = pow(x[0], qvalues[i]);
            qmoments[i]  = xq * px[0];
            for (j = 1; j < LM; j++) {
                xq = pow (x[j], qvalues[i]);
                qmoments[i]  += xq * px[j];
            }
        }
        /* Writing q-moments to file */
        fprintf (fout, "%d", L);
        for (i = 0; i < NQMOMENTS; i++) { fprintf (fout, " %g", qmoments[i]); }
        fprintf (fout, "\n");

        /* Cleaning data */
        fclose (fin); free (px); free (x);
    }
    fclose (fout);
    exit (0);
}

void print_help (char * s) {
    fprintf (stderr, "Usage: %s <QOF> <QMAX> <LS> <LM> <HF> [, <LS> <LM> <HF>, ...]\n", s);
    fprintf (stderr, "Where:\n");
    fprintf (stderr, "\t QOF = q-moments output file\n");
    fprintf (stderr, "\tQMAX = maximal value of q\n");
    fprintf (stderr, "\t  LS = lattice size\n");
    fprintf (stderr, "\t  LM = maximal value\n");
    fprintf (stderr, "\t  HF = histogram file\n");
    exit (1);
}





