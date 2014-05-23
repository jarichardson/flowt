#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE 200

FILE *in_file;
char log_name[25];
FILE *log_file;

/*PARAMETERS*/
/* Geometry data */
double xlength;
double ylength;
int  imax;
int  jmax;
double delx;
double dely;

/* Time-stepping data */
double t_end;
double delt; /*user time step*/
double dt; /*actual time step*/
double tau;

/* Pressure-iteration data */
int  itermax;
double res;
double eps;
double omg;
double upwind;

/* Problem-dependent quantities */
double Re;
double GX;
double GY;
double UI;
double VI;
double PI;
int  wW, wE, wN, wS;
char problem[10];

/* Data arrays */
double **U;
double **V;
double **P;
double **RHS;
double **F, **G;

/*Pressure iteration return tuple*/
struct tuple {
    double res;
    int iterct;
};

/*FUNCTIONS*/
double **RMATRIX(int, int, int, int);
int FREE_RMATRIX(double**, int, int, int, int);
int READ_PARAMETER(char*);
int INTI_UVP(int, int, double, double, double);
double COMP_DELT(double, int, int, double, double, double**, double**, double);

/*BOUNDARY FUNCTIONS*/
int SETBCOND();
int SETSPECBCOND();

/*UVP FUNCTIONS*/
int COMP_FG();
int COMP_RHS();
struct tuple POISSON();
int ADAP_UV();



