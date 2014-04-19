#include "init.h"
/*#include "boundary.h"*/
/*#include "uvp.h"*/

int main(int argc, char **argv) {
int ret;
double t;
int  it;
struct tuple itret;

t = 0;

printf("\n***Flowt is a CFD code.***\n");
printf("   by Jacob Richardson\n");
printf("   written by following Numerical Simulation in Fluid Dynamics, Griebel et al, 1998.\n\n");

/*INITIALIZATION**************************************************************/
/*Check arguments*/
if(argc!=2) {
	printf("ERROR: Incorrect amount of arguments! Cannot determine configuration file.\n");
	printf("Syntax: ./Flowt <configuration-file> <optional: output log-file>\n");
	return(-1);
} else if(argc==3){
	sprintf(log_name, "%s", argv[2]);  /*user supplied log file name*/
	printf("Writing log file to: %s.\n", log_name);	
}
else {
	printf("Writing log file to default: log.dat.\n\n");	
	strcpy(log_name, "log.dat");
}


/* Open default file for logging */
log_file  = fopen(log_name, "w+");
if (log_file == NULL) {
	fprintf(stderr, "Cannot open LOG file=[%s]:[%s]. Exiting.\n",
	log_name, strerror(errno));
	return(-1);
} else fprintf(log_file, "Log File: %s\n\n", log_name);



/*load in parameters*/
ret = READ_PARAMETER(argv[1]);
if(ret!=0) return(-1); /*if error, exit.*/

/*Initialize physical matrices*/
ret = INTI_UVP(imax, jmax, UI, VI, PI);
if(ret!=0) return(-1); /*if error, exit.*/

/*TIME STEPPING***************************************************************/
while(t<=t_end) {
	printf("\n---\nt = %0.6f",t);
	fprintf(log_file,"t: %0.6f ------------------------------\n",t);
	
	/*Calculate next time step, tau*/
	tau = COMP_DELT(delt, imax, jmax, delx, dely, U, V, Re);
	printf("  (next increment: %0.6f)\n",tau);
	if(tau<0) return(-1);

	/*RESET BOUNDARY CONDITIONS*********************************************/
	/*Set all of the rectangular boundary conditions*/
	ret = SETBCOND();
	if(ret!=0) return(-1); /*if error, exit.*/

	/*Set any special boundary conditions*/
	ret = SETSPECBCOND();
	if(ret!=0) return(-1); /*if error, exit.*/

	/*CALCULATE PHYSICAL VALUES*********************************************/
	/*Calculate F and G from the Discrete Momentum Equations*/
	ret = COMP_FG();
	if(ret!=0) return(-1); /*if error, exit.*/

	/*Calculate Right Hand Side of Poisson Equation for Pressure*/
	ret = COMP_RHS();
	if(ret!=0) return(-1); /*if error, exit.*/

	/*Derive new pressure values through SOR iteration*/
	itret = POISSON();
	printf(" residual: %0.5f;   iteration: %d\n",itret.res,itret.iterct);
	if(itret.res<0) return(-1); /*if error, exit.*/

	/*Adapt U and V to next time step!*/
	ret = ADAP_UV();
	if(ret!=0) return(-1); /*if error, exit.*/
	
	/*Increment t for next time step*/
	t+=tau;
}


return(0);
}












