#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>


#define c 29980000000		
#define pi 3.14159265
#define a 10.855		
#define hbar 1.054571800E-34
#define Kb   1.380648520E-23

int main()
{ 
/*intialize variables*/
int  i, j,d, n=0;
char filename[64];
double evalue_i=0;
double eigen=0;
double argument=0; /*trig argument*/
double sine=0; /*denominator of hyperbolic cotangent*/
double cosine=0; /*numerator of hyperbolic cotangent*/
double cotangent=0; /*hyperbolic cotangent*/
double atomB=0; /* value of jth element from corresponding eigenvector*/  
double atomA=0; /* value of ith element from corresponding eigenvector*/
double RD=0; /*Relative displacement of atoms B and A*/
double sum1=0;  /* summation of normal mode sampling of acoustic branch */
double sum2=0;  /* summation of normal mode sampling of optical branch */
double DW1=0; /*the MSRD for the acoustic branch of the chosen pair*/
double DW2=0; /*the MSRD for the optical branch of the chosen pair*/
double wave=0; /*eigenvalue converted to wavenumber*/
double k=0; /*wavevector */
double step = 0; /*discrete k step */
double tot=0; /*sum of all branch MSRDs*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~inputs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*number of atoms in calculation*/
int order=200;
/*printf("order?\n");
scanf("%i",&order);*/
/*number of primitive cells in calculation*/
int N=order/2; 
/*Mass inputs in Kg*/
double  MassPt=3.2394457E-25; 
double MassX=5.8871086E-26;
/*distance between atom pair in Angstroms*/
double dist=a/2;
/* Force constant in Newtons per meter*/
double K1= 166;
/* temperature*/
double T=293;
/*construct reduced mass of pair*/
double MassA=(MassX);
double MassB=(MassPt);
double MuAB=(1/((1/MassA)+(1/MassB)));

/*construct pre-factor*/
double prefactor =hbar/N;
		prefactor=prefactor/2;
		prefactor=prefactor/MuAB;
/*construct massfactor for eigenvector b Pt atom*/
MassB=(MuAB/MassB);
MassB=sqrt(MassB);
/*construct mass factor for eigenvector a X atom*/
MassA=(MuAB/MassA);
MassA=sqrt(MassA);	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Construct Mass Matrix~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
double  Mass1=(1/MassX);
double Mass2=(1/MassPt);
Mass1=sqrt(Mass1);
Mass2=sqrt(Mass2);
gsl_matrix *mass = gsl_matrix_alloc (order,order);
for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
			if (i==j && (i % 2) == 0)
			{
				gsl_matrix_set (mass, i, j, Mass1);
			}
			else if (i==j && (i % 2) != 0)
			{
				gsl_matrix_set (mass, i, j, Mass2);
			}
			else
			{
				gsl_matrix_set (mass,i,j,0);
			}
		}
    }	
printf("Mass matrix with gsl is\n");	
	for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            printf("%g   ",gsl_matrix_get(mass,i,j));
        }
        printf("\n\n");
    }
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Construct force constant matrix~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
gsl_matrix *force = gsl_matrix_alloc (order, order);
    for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
			if (i ==j )
			{
				gsl_matrix_set(force, i, j, K1*2);
			}
			else if (i == j-1 || i-1 == j)
			{
				gsl_matrix_set(force, i, j, -K1);
			}
			else if (i == 0 && j == order-1)
			{
				gsl_matrix_set(force, i, j, -K1);
			}
			else if (j == 0 && i == order-1)
			{
				gsl_matrix_set(force, i, j, -K1);
			}
			else
			{
				gsl_matrix_set(force, i, j, 0);
			}
		}
    }
/*printf("Force Matrix with gsl is\n");
	for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            printf("%g   ",gsl_matrix_get(force, i, j));
        }
        printf("\n\n");
	}*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~create dynamical matrix~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
printf("Dynamical Matrix being created with GSL\n");
gsl_matrix *D = gsl_matrix_alloc (order, order);
gsl_blas_dgemm(CblasNoTrans,CblasNoTrans, 1.0, force, mass, 0, D);

gsl_matrix *Dynamic = gsl_matrix_alloc (order, order);
gsl_blas_dgemm(CblasNoTrans,CblasNoTrans, 1.0, mass, D, 0, Dynamic);
			
/*for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            printf(" %g   ", gsl_matrix_get(Dynamic, i,j));
        }
        printf("\n\n");
	}*/		
gsl_matrix_free (mass);
gsl_matrix_free (force);
gsl_matrix_free (D);	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~initialize eigenfunctions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/	
/*initiate eigensystem spaces*/
gsl_vector *evalue = gsl_vector_alloc (order);
gsl_matrix *evector = gsl_matrix_alloc (order, order);
gsl_eigen_symmv_workspace *work = gsl_eigen_symmv_alloc (order*4);

/* call to GSL to get eigenvalues and eigenvectors of Dynamical Matrix*/	
gsl_eigen_symmv (Dynamic, evalue, evector, work);
/* clear work space*/
gsl_eigen_symmv_free (work);	
/*sort eigen values and vectors */
gsl_eigen_symmv_sort (evalue, evector, GSL_EIGEN_SORT_ABS_ASC);

step=((pi)/(N*a));
printf("step is %lf\n\n",step);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~loop for acoustic branch~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
FILE* f1 = fopen("eigenvalues1.txt", "w+");
n=0;
for (i = 0; i < N; i++)
      {
		/* get ith eigenvalue and eigenvector*/
        evalue_i = gsl_vector_get (evalue, i);
        gsl_vector_view evec_i = gsl_matrix_column (evector, i);
		/*converting eigenvalues (squared angular frequencies) to wavenumbers*/
		wave = fabs(evalue_i);
		wave=sqrt(wave);
		wave=wave/(c*pi*2);
		/*convert from index to wavevector and break degeneracy*/
		/*create bulk index converted to wavevector eigenvalue converted to wavenumber list for plotting purposes*/
		if(i==0)
			{
			k=0;
			fprintf(f1,"%lf %lf\n",k,wave);
			}
		
		else if(i % 2 == 1)
			{
				k= step*n;
				printf("k is %lf\n\n",k);
				fprintf (f1,"%lf %lf\n",k, wave);
			}
		else
			{
				k= -k;
				printf("k is %lf\n\n",k);
				fprintf (f1,"%lf %lf\n",k, wave);
			}
		/*calculate contribution of eigenset to MSRD of a chosen pair*/
		if(wave>1)
			{
				/*calculate argument of trig function*/
				evalue_i = gsl_vector_get (evalue, i);
				eigen = fabs(evalue_i);
				eigen=sqrt(eigen);
				argument=((eigen*hbar)/(2*Kb*T));
				/*calculate hyperbolic cotangent of argument*/
				sine=sinh(argument);
				cosine=cosh(argument);
				cotangent=(cosine/sine);
			
				cotangent=cotangent/eigen;
				/*calculate wavevector dot R*/
				/*difference term finding relative displacement of pair*/
				/*atomB is 2nd element in chain, Pt atom*/
				atomB=gsl_matrix_get(evector,1,i);
				atomB=atomB*MassB;
				argument=k*dist;
				atomB=atomB*cos(argument);
				/*atomA is 1st element in chain, X atom*/
				atomA=gsl_matrix_get(evector,0,i);
				atomA=atomA*MassA;
				RD=atomB-atomA;
				RD=fabs(RD);
				RD=cotangent*RD;
				/*running sum from each mode sampled*/
				sum1=sum1+RD;
				/*summation multiplied by prefactor of hbar reduced mass and primitive cells sampled*/
				DW1=prefactor*sum1;
				DW1=DW1*1E20;
			}
		else
			{
				printf("Don't divide by 0!\n\n");
			}
		
		printf ("%i\n\n",n);
		printf ("eigenvalue = %g\n", evalue_i);
        printf ("corresponding wavenumber = %g\n", wave);
		printf("Running sum of MSRD in Angstroms squared of Acoustic Branch is %g\n\n", DW1);
        /*printf ("corresponding eigenvector: \n");
        gsl_vector_fprintf (stdout,  &evec_i.vector, "%g");*/
		printf("\n\n");
		n++;
		}
	
	      
DW1=prefactor*sum1;
DW1=DW1*1E20;
printf("Total MSRD in Angstroms squared of Acoustic Branch is %g\n\n", DW1);
fclose(f1);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~loop for optical branch~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
FILE* f2 = fopen("eigenvalues2.txt", "w+");
n=0;
for (i = N; i < order; i++)
      {
		/* get ith eigenvalue and eigenvector*/
        evalue_i = gsl_vector_get (evalue, i);
        gsl_vector_view evec_i = gsl_matrix_column (evector, i);
		/*converting eigenvalues (squared angular frequencies) to wavenumbers*/
		wave = fabs(evalue_i);
		wave=sqrt(wave);
		wave=wave/(c*pi*2);
		/*convert from index to wavevector and break degeneracy*/
		/*create bulk index converted to wavevector eigenvalue converted to wavenumber list for plotting purposes*/
		/*if(i==order/2)
			{
			k=0;
			fprintf(f2,"%lf %lf\n",k,wave);
			}*/
		
		if(i % 2 == 1)
			{
				k= step*n-step*N;
				printf("k is %lf\n\n",k);
				fprintf (f2,"%lf %lf\n",k, wave);
			}
		else
			{
				k= -k;
				printf("k is %lf\n\n",k);
				fprintf (f2,"%lf %lf\n",k, wave);
			}
		/*calculate contribution of eigenset to MSRD of a chosen pair*/
		if(wave>1)
			{
				/*calculate argument of trig function*/
				evalue_i = gsl_vector_get (evalue, i);
				eigen = fabs(evalue_i);
				eigen=sqrt(eigen);
				argument=((eigen*hbar)/(2*Kb*T));
				/*calculate hyperbolic cotangent of argument*/
				sine=sinh(argument);
				cosine=cosh(argument);
				cotangent=(cosine/sine);
			
				cotangent=cotangent/eigen;
				/*calculate wavevector dot R*/
				/*difference term finding relative displacement of pair*/
				/*atomB is 2nd element in chain, Pt atom*/
				atomB=gsl_matrix_get(evector,1,i);
				atomB=atomB*MassB;
				argument=k*dist;
				atomB=atomB*cos(argument);
				/*atomA is 1st element in chain, X atom*/
				atomA=gsl_matrix_get(evector,0,i);
				atomA=atomA*MassA;
				RD=atomB-atomA;
				RD=fabs(RD);
				RD=cotangent*RD;
				/*running sum from each mode sampled*/
				sum2=sum2+RD;
				/*summation multiplied by prefactor of hbar reduced mass and primitive cells sampled*/
				DW2=prefactor*sum2;
				DW2=DW2*1E20;
			}
		else
			{
				printf("Don't divide by 0!\n\n");
			}
		
		printf ("%i\n\n",n);
		printf ("eigenvalue = %g\n", evalue_i);
        printf ("corresponding wavenumber = %g\n", wave);
		printf("Running sum of MSRD in Angstroms squared is %g\n\n", DW2);
        /*printf ("corresponding eigenvector: \n");
        gsl_vector_fprintf (stdout,  &evec_i.vector, "%g");*/
		printf("\n\n");
		n++;
		}
		      
DW2=prefactor*sum2;
DW2=DW2*1E20;
printf("Total MSRD of Optical branch in Angstroms squared is %g\n\n", DW2);
tot= DW1+DW2;
printf("Total MSRD in Angstroms squared is %g\n\n", tot);
gsl_vector_free (evalue);
gsl_matrix_free (evector);
gsl_matrix_free (Dynamic);
fclose(f2);
/*Create final files with important values in header:MSRD, number of atoms, spring constants*/
char line[130];
FILE*	f3 = fopen("eigenvalues1.txt", "rt");
FILE*	f4 = fopen("eigenvalues2.txt", "rt");
sprintf(filename, "dispersion_%i_atoms_%lf_bond_%lf_Temp.txt",order, dist,T);
FILE* f5 = fopen(filename, "w+");
fprintf (f5,"###############################################################\n");
fprintf (f5,"#Number of atoms in chain %i\n", order);
fprintf (f5,"#Number of cells %i\n", N);
fprintf (f5,"#Nearest Neighbor force constant %lf N/M\n", K1);
fprintf (f5,"#Distance between atom pair %lf x10^-10 M\n", dist);
fprintf (f5,"#Temperature %lf K\n", T);
fprintf (f5,"#Acoustic MSRD %g x10^-20 M^2\n", DW1);
fprintf (f5,"#Optical MSRD %g x10^-20 M^2\n", DW2);
fprintf (f5,"#Total MSRD %g x10^-20 M^2\n", tot);
fprintf (f5,"###############################################################\n");
while(fgets(line, 130, f3) != NULL)
{
	sscanf (line,"%lf %lf",&k,&wave);
	fprintf (f5,"%lf %lf\n", k, wave);
}
while(fgets(line, 130, f4) != NULL)
{
	sscanf (line,"%lf %lf",&k,&wave);
	fprintf (f5,"%lf %lf\n", k, wave);
}
fclose(f3);	
fclose(f4);
fclose(f5);
return 0;
}