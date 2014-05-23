#include "init.h"
/*#include "boundary.h"*/
/*#include "uvp.h"*/

int main(int argc, char **argv) {
int i,j,ret;
double t;
struct tuple itret;

/*Outfile declarations*/
FILE *ufile;
FILE *vfile;
FILE *uvfile;
FILE *pfile;
char ufile_name[25] = "velU.xyd";
char vfile_name[25] = "velV.xyd";
char uvfile_name[25] = "velUV.xyd";
char pfile_name[25] = "pressure.xyd";

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
printf("\nEnd Time: %0.3f\n",t_end);
printf("Starting time stepping\n\n");

while(t<=t_end) {
	printf("\rcurrent t = %0.6f",t);
	fprintf(log_file,"t: %0.6f ------------------------------\n",t);
	
	/*Calculate next time step, dt*/
	dt = tau * COMP_DELT(delt, imax, jmax, delx, dely, U, V, Re);
	/*printf("  (next increment: %0.6f)",dt);*/
	if(dt<0) return(-1);
	

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
	/*printf(" residual: %0.5f;   iteration: %d\n",itret.res,itret.iterct);*/
	printf("  (P residual: %0.6f)",itret.res);
	if(itret.res<0) return(-1); /*if error, exit.*/

	/*Adapt U and V to next time step!*/
	ret = ADAP_UV();
	if(ret!=0) return(-1); /*if error, exit.*/
	
	/*Increment t for next time step*/
	t+=dt;
}

/*WRITE OUT RESULTS***********************************************************/
printf("Finished time iteration!\n");
printf("\nWriting grids to output files...\n");

ufile  = fopen(ufile_name, "w+");
vfile  = fopen(vfile_name, "w+");
uvfile = fopen(uvfile_name, "w+");
pfile  = fopen(pfile_name, "w+");

if ((((ufile == NULL)||(vfile == NULL))||(pfile == NULL))||(uvfile == NULL)){
	/*One of the files does not open*/
	fprintf(log_file, "ERROR[MAIN]: Cannot open HORIZONTAL, VERTICAL, or COMBINED VELOCITY or PRESSURE file=[%s],[%s],[%s]:[%s].\n", ufile_name, vfile_name, pfile_name, strerror(errno));
	printf("ERROR[MAIN]: Cannot open an output file (HORIZONTAL, VERTICAL, or COMBINED VELOCITY or PRESSURE): [%s],[%s],[%s]:[%s].\n", ufile_name, vfile_name, pfile_name, strerror(errno));
	return(-1);
} else if ((ufile != NULL)&&(vfile != NULL)&&(pfile != NULL)&&(uvfile != NULL)){
	/*All files are good*/
	fprintf(log_file, "Velocities Files: (h) %s\t(v) %s\t(uv) %s\n", ufile_name,vfile_name,uvfile_name);
	fprintf(log_file, "Pressure File: %s\n", pfile_name);

	
	for(i=1;i<=imax;i++){
		for(j=1;j<=jmax;j++) {
			fprintf(ufile,"%d\t%d\t%0.3f\n",i,j,U[i][j]);
			fprintf(vfile,"%d\t%d\t%0.3f\n",i,j,V[i][j]);
			fprintf(uvfile,"%d\t%d\t%0.3f\t%0.3f\n",i,j,U[i][j],V[i][j]);
			fprintf(pfile,"%d\t%d\t%0.3f\n",i,j,P[i][j]);
		}
	}
	
	printf("Velocities Output Files: (h) %s\t(v) %s\t(uv) %s\n", ufile_name,vfile_name,uvfile_name);
	printf("Pressure Output File: %s\n", pfile_name);
}

	fclose(ufile);
	fclose(vfile);
	fclose(uvfile);
	fclose(pfile);

printf("\nFlowt Complete.\n");

return(0);
}












