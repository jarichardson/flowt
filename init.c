/*#include <stdio.h>*/
#include "init.h"

/**************************************************************
FUNCTION:  get_config_data
DESCRIPTION:  This function reads the configuration file,
and sets some global variables.

INPUTS: (IN) FILE *in  (file handle from which to read)
OUTPUTS: int -1=error, 0=no error
***************************************************************/

int READ_PARAMETER(char *config_file){

char line[MAX_LINE];
char var[30];
char value[30];
char *ptr;

FILE *in_config;

printf("Reading in Parameters...\n");
fprintf(log_file, "ENTER[READ_PARAMETER]\n");

/*open configuration file*/
in_config = fopen(config_file, "r");
if (in_config == NULL) {
	fprintf(stderr,
	"Cannot open configuration file=[%s]:[%s]. Exiting.\n", config_file, strerror(errno));
	return(1);
}

/* use each line to compare to needed values*/
while (fgets(line, MAX_LINE, in_config) != NULL) {
	/*if first character is comment, new line, space, return to next line*/
	if (line[0] == '#' || line[0] == '\n' || line[0] == ' ')  continue;
	
	sscanf (line,"%s = %s",var, value); /*split line into before '=' and after*/
	
	/*char *problem, double xlength, double ylength, int imax, int jmax, 
	double delx, double dely, double delt, double t_end, int itermax, double eps, 
	double omg, double upwind, double Re, double GX, double GY, double UI, 
	double VI, double PI, int wW, int wE, int wN, int wS*/
	
	/*DOUBLES*/
	if (!strncmp(var, "xlength", strlen("xlength"))) {
		/*printf("%s is %s\n", var,value);*/
		xlength = strtod(value,&ptr);
		fprintf(log_file, ">xlength %.3f\n", xlength);
	} 
	else if (!strncmp(var, "ylength", strlen("ylength"))) {
		/*printf("%s is %s\n", var,value);*/
		ylength = strtod(value,&ptr);
		fprintf(log_file, ">ylength %.3f\n", ylength);
	}
	else if (!strncmp(var, "delx", strlen("delx"))) {
		/*printf("%s is %s\n", var,value);*/
		delx = strtod(value,&ptr);
		if(delx!=0.0) fprintf(log_file, ">delx %.3f\n", delx);
	}
	else if (!strncmp(var, "dely", strlen("dely"))) {
		/*printf("%s is %s\n", var,value);*/
		dely = strtod(value,&ptr);
		if(dely!=0.0) fprintf(log_file, ">dely %.3f\n", dely);
	}
	else if (!strncmp(var, "delt", strlen("delt"))) {
		/*printf("%s is %s\n", var,value);*/
		delt = strtod(value,&ptr);
		fprintf(log_file, ">delt %.3f\n", delt);
	}
	else if (!strncmp(var, "t_end", strlen("t_end"))) {
		/*printf("%s is %s\n", var,value);*/
		t_end = strtod(value,&ptr);
		fprintf(log_file, ">t_end %.3f\n", t_end);
	}
	else if (!strncmp(var, "eps", strlen("eps"))) {
		/*printf("%s is %s\n", var,value);*/
		eps = strtod(value,&ptr);
		fprintf(log_file, ">eps %.3f\n", eps);
	}
	else if (!strncmp(var, "omg", strlen("omg"))) {
		/*printf("%s is %s\n", var,value);*/
		omg = strtod(value,&ptr);
		fprintf(log_file, ">omg %.3f\n", omg);
	}
	else if (!strncmp(var, "upwind", strlen("upwind"))) {
		/*printf("%s is %s\n", var,value);*/
		upwind = strtod(value,&ptr);
		fprintf(log_file, ">upwind %.3f\n", upwind);
	}
	else if (!strncmp(var, "Re", strlen("Re"))) {
		/*printf("%s is %s\n", var,value);*/
		Re = strtod(value,&ptr);
		fprintf(log_file, ">Re %.3f\n", Re);
	}
	else if (!strncmp(var, "GX", strlen("GX"))) {
		/*printf("%s is %s\n", var,value);*/
		GX = strtod(value,&ptr);
		fprintf(log_file, ">GX %.3f\n", GX);
	}
	else if (!strncmp(var, "GY", strlen("GY"))) {
		/*printf("%s is %s\n", var,value);*/
		GY = strtod(value,&ptr);
		fprintf(log_file, ">GY %.3f\n", GY);
	}
	else if (!strncmp(var, "UI", strlen("UI"))) {
		/*printf("%s is %s\n", var,value);*/
		UI = strtod(value,&ptr);
		fprintf(log_file, ">UI %.3f\n", UI);
	}
	else if (!strncmp(var, "VI", strlen("VI"))) {
		/*printf("%s is %s\n", var,value);*/
		VI = strtod(value,&ptr);
		fprintf(log_file, ">VI %.3f\n", VI);
	}
	else if (!strncmp(var, "PI", strlen("PI"))) {
		/*printf("%s is %s\n", var,value);*/
		PI = strtod(value,&ptr);
		fprintf(log_file, ">PI %.3f\n", PI);
	}
	
	/*INTS*/
	else if (!strncmp(var, "imax", strlen("imax"))) {
		/*printf("%s is %s\n", var,value);*/
		imax = (int)atoi(value);
		fprintf(log_file, ">imax %d\n", imax);
	}
	else if (!strncmp(var, "jmax", strlen("jmax"))) {
		/*printf("%s is %s\n", var,value);*/
		jmax = (int)atoi(value);
		fprintf(log_file, ">jmax %d\n", jmax);
	}
	else if (!strncmp(var, "itermax", strlen("itermax"))) {
		/*printf("%s is %s\n", var,value);*/
		itermax = (int)atoi(value);
		fprintf(log_file, ">itermax %d\n", itermax);
	}
	else if (!strncmp(var, "wW", strlen("wW"))) {
		/*printf("%s is %s\n", var,value);*/
		wW = (int)atoi(value);
		fprintf(log_file, ">wW %d\n", wW);
	}
	else if (!strncmp(var, "wE", strlen("wE"))) {
		/*printf("%s is %s\n", var,value);*/
		wE = (int)atoi(value);
		fprintf(log_file, ">wE %d\n", wE);
	}
	else if (!strncmp(var, "wN", strlen("wN"))) {
		/*printf("%s is %s\n", var,value);*/
		wN = (int)atoi(value);
		fprintf(log_file, ">wN %d\n", wN);
	}
	else if (!strncmp(var, "wS", strlen("wS"))) {
		/*printf("%s is %s\n", var,value);*/
		wS = (int)atoi(value);
		fprintf(log_file, ">wS %d\n", wS);
	}
	
	/*CHARS*/
	else if (!strncmp(var, "problem", strlen("problem"))) {
		/*printf("%s is %s\n", var,value);*/
		strcpy(problem, value);
		fprintf(log_file, ">problem %s\n", problem);
	}
	  
	else continue;
}


(void) fclose(in_config);

/*Set the grid interval distances if they weren't already set*/
if(delx==0){
	delx=xlength/imax;
	if(delx==0) {
		printf("ERROR[READ_PARAMETER]: delx calculated to be 0.0. Check xlength, imax\n");
		return(-1);
	}
	fprintf(log_file, ">delx %0.3f\n", delx);
	printf("   delX calculated (xlength/imax): %0.3f\n",delx);
}

if(dely==0){
	dely=ylength/jmax;
	if(delx==0) {
		printf("ERROR[READ_PARAMETER]: delx calculated to be 0.0. Check xlength, imax\n");
		return(-1);
	}
	fprintf(log_file, ">dely %0.3f\n", dely);
	printf("   delY calculated (ylength/jmax): %0.3f\n",dely);
}

/*Check periodic flow conditions.*/
/*e.g. if left side is periodic, right side must be periodic*/
if((wW==4)||(wE==4)){ /*if either side (left,right) is periodic*/
	if(wW!=wE){ /*but if these sides aren't the same!*/
		printf("ERROR[READ_PARAMETER]: One vertical boundary set as PERIODIC, but other vertical boundary not!\n");
		fprintf(log_file, "Error: left and right values don't match but one is PERIODIC (=4).\n");
		fprintf(log_file, " Left boundary = %d\n Right boundary = %d\n",wW,wE);
		return(-1);
	}
}

if((wS==4)||(wN==4)){ /*if either side (top,bottom) is periodic*/
	if(wS!=wN){ /*but if these sides aren't the same!*/
		printf("ERROR[READ_PARAMETER]: One horizontal boundary set as PERIODIC, but other horizontal boundary not!\n");
		fprintf(log_file, "Error: bottom and top values don't match but one is PERIODIC (=4).\n");
		fprintf(log_file, " Bottom boundary = %d\n Top boundary = %d\n",wS,wN);
		return(-1);
	}
}

fprintf(log_file, "EXIT[READ_PARAMETER]\n\n");
printf("Parameters Read. Written to %s.\n", log_name);

return(0);
/*END OF FUNCTION READ_PARAMETER*/
}


double **RMATRIX(int nrl, int nrh, int ncl, int nch){
/*Reserves memory for matrix of size [nrl,nrh]x[ncl,nch]*/
	int i;
	double **m;
	/*Allocate row pointers*/
	if((m=(double**)malloc((unsigned)(nrh-nrl+1)*sizeof(double*)))==NULL){
		printf("no more memory\n");
		exit(0);
	}
	
	m -= nrl;
	/*allocate rows and set previously allocated row pointers to point to these*/
	
	for(i=nrl;i<=nrh;i++){
		if((m[i]=(double*)malloc((unsigned)(nch-ncl+1)*sizeof(double)))==NULL){
			printf("no more memory\n");
			exit(0);
		}
		
		m[i] -= ncl;
	}
	/*return pointer to the array of row pointers*/
	
	return m;
	
}

int FREE_RMATRIX(double **m, int nrl, int nrh, int ncl, int nch){
/*frees memory of matrix allocated by RMATRIX*/
	int i;

	fprintf(log_file, " ENTER[INTI_UVP]   ");
	for(i=nrh;i>nrl;i--) free((char*)(m[i]+ncl));
	free((char*) (m+nrl));
	
	fprintf(log_file, "Cleared memory cols[0,%d] x rows[0,%d]    EXIT[INTI_UVP]\n",(nch-ncl),(nrh-nrl));
	return(0);
}

int INTI_UVP(int imax, int jmax, double UI, double VI, double PI){
/* Initialize U, V, and P to UI, VI and PI constants on the entire domain*/
	int i;
	int j;
	
	
	fprintf(log_file, "ENTER[INTI_UVP]\n");
	printf("\nInitializing matrices cols[0,%d] x rows[0,%d]...",imax,jmax);

	
	V = RMATRIX(0,imax+1,0,jmax+1);
	U = RMATRIX(0,imax+1,0,jmax+1);
	P = RMATRIX(0,imax+1,0,jmax+1);
	
	F = RMATRIX(0,imax,0,jmax);
	G = RMATRIX(0,imax,0,jmax);
	RHS = RMATRIX(0,imax,0,jmax);
	
	for(i=0;i<=imax+1;i++){
		for(j=0;j<=jmax+1;j++){
			U[i][j] = UI;
			V[i][j] = VI;
			P[i][j] = PI;
		}
	}
	printf("done.\n");
	fprintf(log_file, "EXIT[INTI_UVP]\n\n");
	
	return(0);
}

double COMP_DELT(double delt, int imax, int jmax, double delx, double dely, double** U, double** V, double Re){
/* Calculate delt stepsize from (3.50). If tau<0, use delt from configuration file.*/
	double tauRe;
	double tauX;
	double tauY;
	double tauMin;
	
	double u_max = 0.0;
	double v_max = 0.0;
/*	double u_i;*/
/*	double v_i;*/
	
	int i;
	int j;

	fprintf(log_file, "ENTER[COMP_DELT]\n");
	
	/*Equation 3.50*/
	tauRe = (Re/2.0);
	tauRe *= 1.0/(1.0/(delx*delx) + 1.0/(dely*dely));
	
	/*Find greatest u and v*/
	for(i=0;i<imax;i++){
		for(j=0;j<jmax;j++){
		
			if(u_max<abs(U[i][j])) u_max=abs(U[i][j]);
			if(v_max<abs(V[i][j])) v_max=abs(V[i][j]);
		}
	}
	
	tauX = delx/u_max;
	tauY = dely/v_max;
	
	/*Find lowest tau*/
	if(tauRe<tauX && tauRe<tauY) tauMin = tauRe;
	else if(tauX<tauY) tauMin = tauX;
	else tauMin = tauY;
	
	if((tauMin<=0) && (delt>0)) {
		fprintf(log_file, "tau = delt = %0.6f\nEXIT[COMP_DELT]\n\n",delt);
		return(delt);
	}
	else if (tauMin<=0) {
		printf("\nERROR[COMP_DELT]: del t could not be found (all options are negative time):\n");
		printf("Reynold's del t: %0.3f\n",tauRe);
		printf("max hor. vel. del t: %0.3f\n",tauX);
		printf("max vert. vel del t: %0.3f\n",tauY);
		printf("user input del t: %0.3f\n", delt);
		printf("Program will now exit.\n");
		fprintf(log_file, "tau timestep not found! del t could not be found (all options are negative time)\n");
		return(-1);
	}
	
	fprintf(log_file, "tau = %0.6f\n",tauMin);
	fprintf(log_file, "EXIT[COMP_DELT]\n\n");
	return(tauMin);

}

