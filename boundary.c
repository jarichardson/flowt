#include "init.h"


int SETSPECBCOND(){
/*Set special boundary conditions from the parameter problem, including inflow values*/
	int i,j;
	
/*	printf("\nSolving model: ");*/
	fprintf(log_file,"ENTER[SETSPECBCOND]\n");

	if (!strncmp(problem, "BACKFLOW", strlen("BACKFLOW"))) {
		/*Set top half of left boundary to inflow, u=1*/
/*		printf("BACKFLOW\n\n");*/
/*		fprintf(log_file,"Model: BACKFLOW\n");*/
		for(j=(jmax/2);j<=jmax;j++) {
			U[0][j] = 1;
		}
	} 
	
	
	if (!strncmp(problem, "LIDFLOW", strlen("LIDFLOW"))) {
		/*Set top boundary to right motion*/
			for(i=1;i<=imax;i++) {
				U[i][jmax+1] = 2.0 - U[i][jmax];
		}
	} 
	else {
		printf("ERROR[SETSPECBCOND]: Model Not Found. Check variable 'problem'.\n");
		return(-1);
	
	}
	
fprintf(log_file,"EXIT[SETSPECBCOND]\n\n");
return(0);

}

int SETBCOND(){
/*Set boundary values for U and V from wall parameters (w's) (3.1.2)*/
/*1, free-slip; 2, no-slip; 3, outflow; 4, periodic boundary*/
int i,j;

/*LEFT BOUNDARY********************************************/
if(wW==1){
/*Free-slip*/
	/*U[0][j] must be zero.*/
	/*V[0][j] must = V[1][j]*/
	for(j=1;j<=jmax;j++) {
		U[0][j] = 0;
		V[0][j] = V[1][j];	
	}
}
else if(wW==2) {
/*No-slip*/
	/*U[0][j] must be zero.*/
	/*V[0][j] must = -1*V[1][j], so average = 0*/	
	for(j=1;j<=jmax;j++) {
		U[0][j] = 0;
		V[0][j] = -1*V[1][j];	
	}
}
else if(wW==3) {
/*Outflow*/
	/*U[0][j] must = U[1][j] so dU/dx = 0*/
	/*V[0][j] must = V[1][j] so dV/dx = 0*/
	for(j=1;j<=jmax;j++) {
		U[0][j] = U[1][j];
		V[0][j] = V[1][j];	
	}
}
else if(wW==4) {
/*Periodic*/
	/*U[0][j] gets U[imax-1][j]*/
	/*V[0][j] gets V[imax-1][j]*/
	/*V[1][j] gets V[imax][j]*/
	/*P[1][j] gets P[imax][j]*/
	for(j=1;j<=jmax;j++) {
		U[0][j] = U[imax-1][j];
		V[0][j] = V[imax-1][j];
		V[1][j] = V[imax][j];
		P[1][j] = P[imax][j];
	}
}
else { 
/*If selection is invalid*/
	printf("ERROR: Left boundary code is invalid. Please select condition type [1,4] in configuration file!\n");
	return(-1);
}

/*RIGHT BOUNDARY*******************************************/
if(wE==1){
/*Free-slip*/
	/*U[imax][j] must be zero.*/
	/*V[imax+1][j] must = V[imax][j]*/
	for(j=1;j<=jmax;j++) {
		U[imax][j] = 0;
		V[imax+1][j] = V[imax][j];	
	}
}
else if(wE==2) {
/*No-slip*/
	/*U[imax][j] must be zero.*/
	/*V[imax+1][j] must = -1*V[imax][j], so average = 0*/	
	for(j=1;j<=jmax;j++) {
		U[imax][j] = 0;
		V[imax+1][j] = -1*V[imax][j];	
	}
}
else if(wE==3) {
/*Outflow*/
	/*U[imax][j] must = U[imax-1][j] so dU/dx = 0*/
	/*V[imax+1][j] must = V[imax][j] so dV/dx = 0*/
	for(j=1;j<=jmax;j++) {
		U[imax][j] = U[imax-1][j];
		V[imax+1][j] = V[imax][j];	
	}

}
else if(wE==4) {
/*Periodic*/
	/*U[imax][j] gets U[1][j]*/
	/*V[imax+1][j] gets V[2][j]*/
	for(j=1;j<=jmax;j++) {
		U[imax][j] = U[1][j];
		V[imax+1][j] = V[2][j];
	}
}
else { 
/*If selection is invalid*/
	printf("ERROR: Right boundary code is invalid. Please select condition type [1,4] in configuration file!\n");
	return(-1);
}

/*BOTTOM BOUNDARY******************************************/
if(wS==1){
/*Free-slip*/
	/*V[i][0] = 0*/
	/*U[i][0] must = U[i][1]*/
	for(i=1;i<=imax;i++) {
		V[i][0] = 0;
		U[i][0] = U[i][1];
	}
}
else if(wS==2) {
/*No-slip*/
	/*V[i][0] must be zero.*/
	/*U[i][0] must = -1*U[i][1], so average = 0*/	
	for(i=1;i<=imax;i++) {
		V[i][0] = 0;
		U[i][0] = -1*U[i][1];	
	}
}
else if(wS==3) {
/*Outflow*/
	/*V[i][0] must = V[i][1] so dU/dx = 0*/
	/*U[i][0] must = U[i][1] so dV/dx = 0*/
	for(i=1;i<=imax;i++) {
		V[i][0] = V[i][1];
		U[i][0] = U[i][1];
	}
}
else if(wS==4) {
/*Periodic*/
	/*V[i][0] gets V[i][jmax-1]*/
	/*U[i][0] gets U[i][jmax-1]*/
	/*U[i][1] gets U[i][jmax]*/
	/*P[i][1] gets P[i][jmax]*/
	for(i=1;i<=imax;i++) {
		V[i][0] = V[i][jmax-1];
		U[i][0] = U[i][jmax-1];
		U[i][1] = U[i][jmax];
		P[i][1] = P[i][jmax];
	}
}
else { 
/*If selection is invalid*/
	printf("ERROR: Bottom boundary code is invalid. Please select condition type [1,4] in configuration file!\n");
	return(-1);
}


/*TOP BOUNDARY*********************************************/
if(wN==1){
/*Free-slip*/
	/*V[i][jmax] = 0*/
	/*U[i][jmax+1] must = U[i][jmax]*/
	for(i=1;i<=imax;i++) {
		V[i][jmax] = 0;
		U[i][jmax+1] = U[i][jmax];
	}
}
else if(wN==2) {
/*No-slip*/
	/*V[i][jmax] must be zero.*/
	/*U[i][jmax+1] must = -1*U[i][jmax], so average = 0*/	
	for(i=1;i<=imax;i++) {
		V[i][jmax] = 0;
		U[i][jmax+1] = -1*U[i][jmax];	
	}
}
else if(wN==3) {
/*Outflow*/
	/*V[i][jmax] must = V[i][jmax-1] so dU/dx = 0*/
	/*U[i][jmax+1] must = U[i][jmax] so dV/dx = 0*/
	for(i=1;i<=imax;i++) {
		V[i][jmax] = V[i][jmax-1];
		U[i][jmax+1] = U[i][jmax];
	}
}
else if(wN==4) {
/*Periodic*/
	/*V[i][jmax] gets V[j][1]*/
	/*U[i][jmax+1] gets U[i][2]*/
	for(j=1;j<=jmax;j++) {
		V[i][jmax] = V[j][1];
		U[i][jmax+1] = U[i][2];
	}
}
else { 
/*If selection is invalid*/
	printf("ERROR: Top boundary code is invalid. Please select condition type [1,4] in configuration file!\n");
	return(-1);
}

return(0);

}

