#include "init.h"

int COMP_FG(){
/*Compute F, G, from Eqs (3.36) and (3.37). At boundary, use Eq (3.42)*/
/*double U, double V, double F, double G, int imax, int jmax, double delt, double delx, double dely, double GX, double GY, double gamma, double Re*/
	int i,j;
	double gamma;

	/*F diffusive terms*/
	double ddu_dxx;
	double ddu_dyy;
	double duu_dx;
	double duv_dy;

	/*G diffusive terms*/
	double ddv_dxx;
	double ddv_dyy;
	double duv_dx;
	double dvv_dy;

	double RS; /*Right Hand Side*/
	
	fprintf(log_file,"ENTER[COMP_FG]\n");

	/*Block F*/
	fprintf(log_file,"Deriving F, ");
	for(i=1;i<=(imax-1);i++) {
		for(j=1;j<=jmax;j++) {
			/*  Set gamma = to the max of u*dt/dx and v*dt/dy  */
			if((gamma = abs(U[i][j]*tau/delx)) < abs(V[i][j]*tau/dely)) gamma = abs(V[i][j]/dely) ;
			
			/*Calculate diffusive terms (Eq. 3.19a)*/
			ddu_dxx = (U[i+1][j] - (2*U[i][j]) + U[i-1][j])/(delx*delx);
			
			ddu_dyy = (U[i][j+1] - (2*U[i][j]) + U[i][j-1])/(dely*dely);
			
			duu_dx  =  (   (U[i][j]+U[i+1][j])/2) * ((U[i][j]+U[i+1][j])/2) - (   (U[i][j]+U[i-1][j])/2) * ((U[i-1][j]+U[i][j])/2);
			duu_dx += ((abs(U[i][j]+U[i+1][j])/2) * ((U[i][j]-U[i+1][j])/2) - (abs(U[i][j]+U[i-1][j])/2) * ((U[i-1][j]-U[i][j])/2)) * gamma;
			duu_dx /= delx;
			
			duv_dy  =  (   (V[i][j]+V[i+1][j])/2) * ((U[i][j]+U[i][j+1])/2) - (   (V[i][j-1]+V[i+1][j-1])/2) * ((U[i][j-1]+U[i][j])/2);
			duv_dy += ((abs(V[i][j]+V[i+1][j])/2) * ((U[i][j]-U[i][j+1])/2) - (abs(V[i][j-1]+V[i+1][j-1])/2) * ((U[i][j-1]-U[i][j])/2)) * gamma;
			duv_dy /= dely;
			
			/*FINAL ADDITION*/
			RS = ((ddu_dxx + ddu_dyy)/Re) - duu_dx - duv_dy + GX;
			RS *= tau;
			
			F[i][j] = U[i][j] + RS;
		}	
	}
	
	/*Block G*/
	fprintf(log_file,"G, ");
	for(i=1;i<=imax;i++) {
		for(j=1;j<=(jmax-1);j++) {
			/*  Set gamma = to the max of u*dt/dx and v*dt/dy  */
			if((gamma = abs(U[i][j]*tau/delx)) < abs(V[i][j]*tau/dely)) gamma = abs(V[i][j]/dely) ;
			
			/*Calculate diffusive terms (Eq. 3.19b)*/
			ddv_dxx = (V[i+1][j] - (2*V[i][j]) + V[i-1][j])/(delx*delx);
			
			ddv_dyy = (V[i][j+1] - (2*V[i][j]) + V[i][j-1])/(dely*dely);
			
			dvv_dy  =  (   (V[i][j]+V[i][j+1])/2) * ((V[i][j]+V[i][j+1])/2) - (   (V[i][j]+V[i][j-1])/2) * ((V[i][j-1]+V[i][j])/2);
			dvv_dy += ((abs(V[i][j]+V[i][j+1])/2) * ((V[i][j]-V[i][j+1])/2) - (abs(V[i][j]+V[i][j-1])/2) * ((V[i][j-1]-V[i][j])/2)) * gamma;
			dvv_dy /= dely;
			
			duv_dx  =  (   (U[i][j]+U[i][j+1])/2) * ((V[i][j]+V[i+1][j])/2) - (   (U[i-1][j]+U[i-1][j+1])/2) * ((V[i-1][j]+V[i][j])/2);
			duv_dx += ((abs(U[i][j]+U[i][j+1])/2) * ((V[i][j]+V[i+1][j])/2) - (abs(U[i-1][j]+U[i-1][j+1])/2) * ((V[i-1][j]+V[i][j])/2)) * gamma;
			duv_dx /= delx;
			
			/*FINAL ADDITION*/
			RS = ((ddv_dxx + ddv_dyy)/Re) - dvv_dy - duv_dx + GY;
			RS *= tau;
			
			G[i][j] = V[i][j] + RS;
		
		}	
	}

	fprintf(log_file,"and Boundary F & G.\n");
	/*Boundary F*/
	for(j=1;j<=jmax;j++){
		F[0][j] = U[0][j];
		F[imax][j] = U[imax][j];	
/*		printf("Fj %d: %0.5f/%0.5f\n",j,F[0][j],U[imax][jmax]);*/
	}

	/*Boundary G*/
	for(i=1;i<=imax;i++){
		G[i][0] = V[i][0];
		G[i][jmax] = V[i][jmax];
/*		printf("Gi %d: %0.5f/%0.5f\n",i,G[i][0],G[i][jmax]);*/
	}
	
	fprintf(log_file,"EXIT[COMP_FG]\n\n");
	
	return(0);
}

int COMP_RHS(){
/*Compute Right Hand Side (RHS) of pressure Eq (3.38)*/
	int i,j;
	
	fprintf(log_file,"ENTER[COMP_RHS]\n");
	
	for(i=1;i<=imax;i++) {
		for(j=1;j<=jmax;j++) {
			RHS[i][j]  = (F[i][j] - F[i-1][j]) / delx;
			RHS[i][j] += (G[i][j] - G[i][j-1]) / dely;
			RHS[i][j] /= tau;
		}
	}
		
	fprintf(log_file,"EXIT[COMP_RHS]\n\n");
	return(0);
}

struct tuple POISSON() {
/* SOR iteration for pressure Poisson eq from Eq (3.44).
      Terminate once residual norm is less than tolerance, (res<eps)
      or Terminate once itermax is reached.
      Return number of iteration steps. Return current residual as res.
      If using method #2, set boundary values using Eq (3.48) before each iteration.
*/
/*
double P, double RHS, int imax, int jmax, double delx, double dely, double eps, int itermax, double omg, double res

Algorithm:
*create new pressure matrix
*copy old pressure matrix to new pressure matrix (p(it=1))

*define l2-norm residual

*Iterate till max iteration (it = 1; it<=imax; it++)
*	for boundaries (0, imax & 0, jmax)
*		calculate new pressure(it) (3.48)

*	l2-norm residual = 0

*	for all is (1->imax) (LEFT TO RIGHT)
*		for all js (1->jmax) (DOWN TO UP)
*			calculate next pressure(it) for grid cell (3.44)
			calculate current grid cell residual (3.45)
*			add to l2-norm residual (summation part of 3.46)
			
*	normalize l2-norm residual (rest of 3.46)
*	compare l2-norm residual with eps (maximum allowable residual)
*	if l2-norm res < eps
*		rewrite old pressure matrix with new pressure matrix
*		free mem of iterated (new/local) pressure matrix
*		return res, iters
		
if max iteration reached and completed
	rewrite old pressure matrix with new pressure matrix
	free mem of iterated (new/local) pressure matrix
	return res,iters=itermax (is this an error, or the solution?
	Test for itermax in main()
	
GLOBAL DEFINITION	
struct tuple {
    double res;
    int iterct;
};

RETURN TWO THINGS IN THE STRUCTURE
struct Tuple getPair() {
    Tuple r;
    r = { 0.000158, 51 };
    return r;
}
*/
	/*return structure: .res = residual double; .iterct = iteration count integer*/
	struct tuple itret;
	/*L2-normalized residual value*/
	double resnorm;
	/*Working copy of Pressure Matrix*/
	double **nP;
	/*Working pressure placeholder*/
	double _p;
	/*grid cell residual*/
	double r;
	double dx2, dy2;
	int i,j,it,ret;

	fprintf(log_file,"ENTER[POISSON]\n");
	printf("  pressure: ");
	
	/*square dx and dy for later*/
	dx2 = delx*delx;
	dy2 = dely*dely;
	
	/*Copy old pressures to working copy of pressure matrix*/
	nP = RMATRIX(0,imax+1,0,jmax+1);
	for(i=0;i<=imax+1;i++){
		for(j=0;j<=jmax+1;j++) {
			nP[i][j] = P[i][j];
		}
	}
	
	for(it=1;it<=itermax;it++) {
	/*Iterate till max iteration (it = 1; it<=imax; it++)*/
		/*for boundaries (1, imax & 1, jmax)*/
		/*calculate new pressure(it) (3.48) (copy inside pressures to outside pressures*/
		for(i=1;i<=imax;i++){
			nP[i][0] = nP[i][1];
			nP[i][jmax+1] = nP[i][jmax];
		}
		for(j=1;j<=jmax;j++){
			nP[0][j] = nP[1][j];
			nP[imax+1][j] = nP[imax][j];
		}
		
		for(i=1;i<=imax;i++){        /*for all is (1->imax) (LEFT TO RIGHT)*/
			for(j=1;j<=jmax;j++) {   /*for all js (1->jmax) (DOWN TO UP)*/
				/*calculate next pressure(it) for grid cell (3.44)*/
				_p = 0.0;
				_p = ((nP[i+1][j] + nP[i-1][j]) / (dx2)) + ((nP[i][j+1] + nP[i][j-1]) / (dy2)) - RHS[i][j];
				_p *= omg / ((2/(dx2)) + (2/(dy2)));
				nP[i][j] *= 1 - omg;
				nP[i][j] += _p;
			}
		}		
		
		resnorm = 0.0; /*reset the residual to zero to recalculate for each iter*/
		
		for(i=1;i<=imax;i++){
			for(j=1;j<=jmax;j++) {
								
				/*add current grid cell residual to ongoing residual  (3.45)*/
				r = ((nP[i+1][j] - nP[i-1][j])/(dx2)) + ((nP[i][j+1] - nP[i][j-1])/(dy2)) - RHS[i][j];

				/*add to l2-norm residual (summation part of 3.46)*/
				resnorm += r*r;
			}
		}
		
		/*normalize l2-norm residual (rest of 3.46)*/
		/*compare l2-norm residual with eps (maximum allowable residual)*/
		if ( (resnorm = pow((resnorm/(imax*jmax)),0.5)) < eps) {
			itret.res = resnorm;
			itret.iterct = it;
			
			/*overwrite pressure matrix with new values*/
			for(i=0;i<=imax+1;i++){
				for(j=0;j<=jmax+1;j++) {
					P[i][j] = nP[i][j];
				}
			}
			
			/*free working pressure matrix memory*/
			ret = FREE_RMATRIX(nP, 0, imax+1, 0, jmax+1);
			if(ret!=0) { /*if error, force exit*/
				printf("ERROR[POISSON]: Did not free working pressure matrix memory [FREE_RMATRIX].\n");
				itret.res = -1;
				return itret;				
			}

			fprintf(log_file," successful iteration: %d    with residual: %0.8f\n",itret.iterct,itret.res);
			fprintf(log_file,"EXIT[POISSON]\n");
			return itret;
		}
	}
	
	/*if residual is still too large at last iteration, code will arrive here (out of iter loop.)
	if max iteration reached and completed
		rewrite old pressure matrix with new pressure matrix
		free mem of iterated (new/local) pressure matrix
		return res,iters=itermax (is this an error, or the solution?
		Test for itermax in main()
	*/

	/*overwrite pressure matrix with new values*/
	for(j=0;j<=jmax+1;j++){
/*		printf("\n\n%d\n",j);*/
		for(i=0;i<=imax+1;i++) {
/*			printf("%0.3f  ",nP[i][j]);*/
		}
	}
	
	for(i=0;i<=imax+1;i++){
		for(j=0;j<=jmax+1;j++) {
			P[i][j] = nP[i][j];
		}
	}
	
	/*free working pressure matrix memory*/
	ret = FREE_RMATRIX(nP, 0, imax+1, 0, jmax+1);
	if(ret!=0) { /*if error, force exit*/
		printf("ERROR[POISSON]: Did not free working pressure matrix [FREE_RMATRIX].\n");
		itret.res = -1;
		return itret;				
	}
	
	itret.res = resnorm;
	itret.iterct = it;
	
	fprintf(log_file," unsucessful iteration at itermax: %d    with residual: %0.8f\n",itret.iterct,itret.res);
	fprintf(log_file,"EXIT[POISSON]\n\n");
	return itret;

}

int ADAP_UV(){
/*Calculate new velocities from Eq (3.34) & (3.35).*/
	/*Recalculate U from new pressures and old-ish F*/
	int i,j;
	FILE *ufile;
	FILE *vfile;
	char ufile_name[25] = "velU.xyv";
	char vfile_name[25] = "velV.xyv";
	fprintf(log_file,"ENTER[ADAP_UV]\n");
	
	for(i=1;i<=imax-1;i++){
		for(j=1;j<=jmax;j++) {
			U[i][j] = F[i][j] - ((tau/delx) * (P[i+1][j] - P[i][j]));
		}
	}
	
	
	/*Recalculate V from new pressures and old-ish G*/
	for(i=1;i<=imax;i++){
		for(j=1;j<=jmax-1;j++) {
			V[i][j] = G[i][j] - ((tau/dely) * (P[i][j+1] - P[i][j]));
		}
	}
	
	
	ufile  = fopen(ufile_name, "w+");
	vfile  = fopen(vfile_name, "w+");
	
	if ((ufile == NULL)&&(vfile == NULL)){
		/*BOTH ARE BAD*/
		fprintf(stderr, "ERROR[ADAP_UV]: Cannot open HORIZONTAL and VERTICAL VELOCITY files=[%s],[%s]:[%s]. Continuing!\n", ufile_name, vfile_name, strerror(errno));
		return(0);
	} else if ((ufile != NULL)&&(vfile != NULL)){
		/*BOTH ARE GOOD*/
		fprintf(log_file, "Velocities Files: (h) %s\t(v) %s\n", ufile_name,vfile_name);
		
		for(i=1;i<=imax;i++){
			for(j=1;j<=jmax;j++) {
				fprintf(ufile,"%d\t%d\t%0.3f\n",i,j,U[i][j]);
				fprintf(vfile,"%d\t%d\t%0.3f\n",i,j,V[i][j]);
			}
		}
		
		printf("  Velocities Files: (h) %s\t(v) %s\n", ufile_name,vfile_name);
	
	} else { /*ONE IS BAD NOT BOTH*/
	
		if (ufile == NULL) {
			fprintf(stderr, "ERROR[ADAP_UV]: Cannot open HORIZONTAL VELOCITY file=[%s]:[%s]. Continuing!\n", ufile_name, strerror(errno));
			
			/*VFILE IS STILL GOOD*/
			for(i=1;i<=imax;i++){
				for(j=1;j<=jmax;j++) {
					fprintf(vfile,"%d\t%d\t%0.3f\n",i,j,V[i][j]);
				}
			}
			printf("  Vertical Velocities written to %s.\n",vfile_name);
		} else {
			fprintf(log_file, "Horizontal Velocities File: %s\n\n", ufile_name);
		}

	
		if (vfile == NULL) {
			fprintf(stderr, "ERROR[ADAP_UV]: Cannot open VERTICAL VELOCITY file=[%s]:[%s]. Continuing!\n", vfile_name, strerror(errno));
			
			/*UFILE IS STILL GOOD*/
			for(i=1;i<=imax;i++){
				for(j=1;j<=jmax;j++) {
					fprintf(ufile,"%d\t%d\t%0.3f\n",i,j,U[i][j]);
				}
			}
			printf("  Horizontal Velocities written to %s.\n",ufile_name);
			
		} else {
			fprintf(log_file, "Vertical Velocities File: %s\n\n", vfile_name);
		
	
		}
	}
	

	fclose(ufile);
	fclose(vfile);
	
	fprintf(log_file,"EXIT[ADAP_UV]\n");
	return(0);
}
