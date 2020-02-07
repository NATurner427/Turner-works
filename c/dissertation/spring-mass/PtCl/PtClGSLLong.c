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
double suma=0;  /* summation of normal mode sampling of acoustic branch */
double sumnu3=0;  /* summation of normal mode sampling of optical nu3 branch */
double sumnu1=0;  /* summation of normal mode sampling of optical nu1 branch */
double sumnu2=0;  /* summation of normal mode sampling of optical nu2 branch */
double DWa=0; /*the MSRD for the acoustic branch of the chosen pair*/
double DWnu3=0; /*the MSRD for the optical nu3  branch of the chosen pair*/
double DWnu1=0; /*the MSRD for the optical nu1 branch of the chosen pair*/
double DWnu2=0; /*the MSRD for the optical nu2 branch of the chosen pair*/
double wave=0; /*eigenvalue converted to wavenumber*/
double k=0; /*wavevector */
double step = 0; /*discrete k step */
double tot=0; /*sum of all branch MSRDs*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~inputs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*number of atoms in calculation*/
int order=800;
/*printf("order?\n");
scanf("%i",&order);*/
/*number of primitive cells in calculation*/
int N=order/4; 
/*Mass inputs in Kg*/
double  MassPt=3.2394457E-25; /*3.2394457E-25*/
double MassX=5.8871086E-26;
/*distance between atom pair in Angstroms*/
double dist=3.10019;
/* Force constants in Newtons per meter*/
double K1 = 166;	/* RAMAN fine structure NN Love values 134.6 66.3 */
double K2 = 45.4; 	/*model a) 2/3 K2/K1 ratio spectra NN 132.7 88.5 */
double K3 = -2.3; 	/*model b) NN and NNN Love values 166,45.4,-4.7,-4.7,0*/
double K4 = -2.3;   /*model c) NN and NNN Love values 166,45.4,-2.3,-2.3,0*/
double K5 = 36;     /*model d) NN and NNN Love values 166,45.4,-2.3,-2.3,36*/
				
/* temperature*/
double T=298;
/*construct reduced mass of pair*/
double MassA=(MassX);
double MassB=(MassPt);
double MuAB=(1/((1/MassA)+(1/MassB)));

/*construct pre-factor*/
double prefactor=hbar/(2*MuAB);
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
/*printf("Mass matrix with gsl is\n");	
	for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            printf("%g   ",gsl_matrix_get(mass,i,j));
        }
        printf("\n\n");
    }*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Construct force constant matrix~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
gsl_matrix *force = gsl_matrix_alloc (order, order);

for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
			if ((i%4)==0 && j%4==0 && i == j)
			{
				gsl_matrix_set(force, i, j, K1 + K2 + K3 + K4);	
			}
			else if ((i%4)==1 && (j%4)==1 && i==j)
			{
				gsl_matrix_set(force, i, j,2*K1 +2*K5);
			}
			else if ((i%4)==2 && (j%4)==2 && i==j)
			{
				gsl_matrix_set(force, i, j, K1 + K2 + K3 + K4);
			}
			else if ((i%4)==3 && (j%4)==3 && i == j)
			{
				gsl_matrix_set(force, i, j,2*K2 +2*K5);
			}
			else if (i == j-1)
			{
					if ((i%4) == 0)
					{
						gsl_matrix_set(force, i, j, -K1);
					}
					else if ((i%4) == 1)
					{
						gsl_matrix_set(force, i, j, -K1);
					}
					else if ((i%4) == 2)
					{
						gsl_matrix_set(force, i, j, -K2);
					}
					else if ((i%4) == 3)
					{
						gsl_matrix_set(force, i, j, -K2);
					}
			}
			else if (j == i-1)
			{
					if ((j%4) == 0)
					{
						gsl_matrix_set(force, i, j, -K1);
					}
					else if ((j%4) == 1)
					{
						gsl_matrix_set(force, i, j, -K1);
					}
					else if ((j%4) == 2)
					{
						gsl_matrix_set(force, i, j, -K2);
					}
					else if ((j%4) == 3)
					{
						gsl_matrix_set(force, i, j, -K2);
					}
			}
			else if (i == j-2)
			{
					if ((i%4)==0)
					{
						gsl_matrix_set(force, i, j, -K3);
					}
					else if ((i%4)==1)
					{
						gsl_matrix_set(force, i, j, -K5);
					}
					else if ((i%4)==2)
					{
						gsl_matrix_set(force, i, j, -K4);
					}
					else if ((i%4)==3)
					{
						gsl_matrix_set(force, i, j, -K5);
					}
			}
			else if (j == i-2)
			{
					if ((j%4)==0)
					{
						gsl_matrix_set(force, i, j, -K3);
					}
					else if ((j%4)==1)
					{
						gsl_matrix_set(force, i, j, -K5);
					}
					else if ((j%4)==2)
					{
						gsl_matrix_set(force, i, j, -K4);
					}
					else if ((j%4)==3)
					{
						gsl_matrix_set(force, i, j, -K5);
					}
			}
			else if (i == 0 && j == order-1)
			{
				gsl_matrix_set(force, i, j, -K2);
			}
			else if (j == 0 && i == order-1)
			{
				gsl_matrix_set(force, i, j, -K2);
			}
			else if (i == 0 && j == order-2)
			{
				gsl_matrix_set(force, i, j, -K4);
			}
			else if (j == 0 && i == order-2)
			{
				gsl_matrix_set(force, i, j, -K4);
			}
			else if (i == 1 && j == order-1)
			{
				gsl_matrix_set(force, i, j, -K5);
			}
			else if (j == 1 && i == order-1)
			{
				gsl_matrix_set(force, i, j, -K5);
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
FILE* f1 = fopen("eigenvalues_A.txt", "w+");
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
				atomB=gsl_matrix_get(evector,3,i);
				atomB=atomB*MassB;
				/*atomA is 1st element in chain, X atom*/
				atomA=gsl_matrix_get(evector,2,i);
				atomA=atomA*MassA;
				RD=atomB-atomA;
				RD=pow(RD,2);
				RD=cotangent*RD;
				/*running sum from each mode sampled*/
				suma=suma+RD;
				/*summation multiplied by prefactor of hbar reduced mass and primitive cells sampled*/
				DWa=prefactor*suma;
				DWa=DWa*1E20;
			}
		else
			{
				printf("Don't divide by 0!\n\n");
			}
		
		printf ("%i\n\n",n);
		printf ("eigenvalue = %g\n", evalue_i);
        printf ("corresponding wavenumber = %g\n", wave);
		printf("Running sum of MSRD in Angstroms squared of Acoustic Branch is %g\n\n", DWa);
        /*printf ("corresponding eigenvector: \n");
        gsl_vector_fprintf (stdout,  &evec_i.vector, "%g");*/
		printf("\n\n");
		n++;
		}
	
	      
DWa=prefactor*suma;
DWa=DWa*1E20;
printf("Total MSRD in Angstroms squared of Acoustic Branch is %g\n\n", DWa);
fclose(f1);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~loop for nu3 optical branch~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
FILE* f2 = fopen("eigenvalues_Nu3.txt", "w+");
n=0;
for (i = N; i < 2*N; i++)
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
				atomB=gsl_matrix_get(evector,3,i);
				atomB=atomB*MassB;
				/*atomA is 1st element in chain, X atom*/
				atomA=gsl_matrix_get(evector,2,i);
				atomA=atomA*MassA;
				RD=atomB-atomA;
				RD=pow(RD,2);
				RD=cotangent*RD;
				/*running sum from each mode sampled*/
				sumnu3=sumnu3+RD;
				/*summation multiplied by prefactor of hbar reduced mass and primitive cells sampled*/
				DWnu3=prefactor*sumnu3;
				DWnu3=DWnu3*1E20;
			}
		else
			{
				printf("Don't divide by 0!\n\n");
			}
		
		printf ("%i\n\n",n);
		printf ("eigenvalue = %g\n", evalue_i);
        printf ("corresponding wavenumber = %g\n", wave);
		printf("Running sum of MSRD in Angstroms squared is %g\n\n", DWnu3);
        /*printf ("corresponding eigenvector: \n");
        gsl_vector_fprintf (stdout,  &evec_i.vector, "%g");*/
		printf("\n\n");
		n++;
		}
		      
DWnu3=prefactor*sumnu3;
DWnu3=DWnu3*1E20;
printf("Total MSRD of nu3 Optical branch in Angstroms squared is %g\n\n", DWnu3);
tot= DWa+DWnu3;
printf("Total MSRD in Angstroms squared is %g\n\n", tot);
fclose(f2);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~loop for nu1 optical branch~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
FILE* f3 = fopen("eigenvalues_Nu1.txt", "w+");
n=0;
for (i = 2*N; i < 3*N; i++)
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
		if(i % 2 == 1)
			{
				k= step*n;
				printf("k is %lf\n\n",k);
				fprintf (f3,"%lf %lf\n",k, wave);
			}
		else
			{
				k= -k;
				printf("k is %lf\n\n",k);
				fprintf (f3,"%lf %lf\n",k, wave);
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
				atomB=gsl_matrix_get(evector,3,i);
				atomB=atomB*MassB;
				/*atomA is 1st element in chain, X atom*/
				atomA=gsl_matrix_get(evector,2,i);
				atomA=atomA*MassA;
				RD=atomB-atomA;
				RD=pow(RD,2);
				RD=cotangent*RD;
				/*running sum from each mode sampled*/
				sumnu1=sumnu1+RD;
				/*summation multiplied by prefactor of hbar reduced mass and primitive cells sampled*/
				DWnu1=prefactor*sumnu1;
				DWnu1=DWnu1*1E20;
			}
		else
			{
				printf("Don't divide by 0!\n\n");
			}
		
		printf ("%i\n\n",n);
		printf ("eigenvalue = %g\n", evalue_i);
        printf ("corresponding wavenumber = %g\n", wave);
		printf("Running sum of MSRD in Angstroms squared of Acoustic Branch is %g\n\n", DWnu1);
        /*printf ("corresponding eigenvector: \n");
        gsl_vector_fprintf (stdout,  &evec_i.vector, "%g");*/
		printf("\n\n");
		n++;
		}
DWnu1=prefactor*sumnu1;
DWnu1=DWnu1*1E20;
printf("Total MSRD of nu2 Optical branch in Angstroms squared is %g\n\n", DWnu1);
tot= DWa+DWnu3+DWnu1;
printf("Total MSRD in Angstroms squared is %g\n\n", tot);
fclose(f3);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~loop for Nu2 optical branch~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
FILE* f4 = fopen("eigenvalues_Nu2.txt", "w+");
n=0;
for (i = 3*N; i < order; i++)
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
		if(i % 2 == 1)
			{
				k= step*n-step*N;
				printf("k is %lf\n\n",k);
				fprintf (f4,"%lf %lf\n",k, wave);
			}
		else
			{
				k= -k;
				printf("k is %lf\n\n",k);
				fprintf (f4,"%lf %lf\n",k, wave);
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
				atomB=gsl_matrix_get(evector,3,i);
				atomB=atomB*MassB;
				/*atomA is 1st element in chain, X atom*/
				atomA=gsl_matrix_get(evector,2,i);
				atomA=atomA*MassA;
				RD=atomB-atomA;
				RD=pow(RD,2);
				RD=cotangent*RD;
				/*running sum from each mode sampled*/
				sumnu2=sumnu2+RD;
				/*summation multiplied by prefactor of hbar reduced mass and primitive cells sampled*/
				DWnu2=prefactor*sumnu2;
				DWnu2=DWnu2*1E20;
			}
		else
			{
				printf("Don't divide by 0!\n\n");
			}
		
		printf ("%i\n\n",n);
		printf ("eigenvalue = %g\n", evalue_i);
        printf ("corresponding wavenumber = %g\n", wave);
		printf("Running sum of MSRD in Angstroms squared of nu1 optical Branch is %g\n\n", DWnu2);
        /*printf ("corresponding eigenvector: \n");
        gsl_vector_fprintf (stdout,  &evec_i.vector, "%g");*/
		printf("\n\n");
		n++;
		}
	
	      
DWnu2=prefactor*sumnu2;
DWnu2=DWnu2*1E20;
printf("Total MSRD in Angstroms squared of nu1 optical Branch is %g\n\n", DWnu2);
tot= DWa+DWnu3+DWnu1+DWnu2;
printf("Total MSRD in Angstroms squared is %g\n\n", tot);
fclose(f4);
gsl_vector_free (evalue);
gsl_matrix_free (evector);
gsl_matrix_free (Dynamic);
/*Create final files with important values in header:MSRD, number of atoms, spring constants*/
char line[130];
FILE*	f5 = fopen("eigenvalues_A.txt", "rt");
FILE*	f6 = fopen("eigenvalues_Nu3.txt", "rt");
FILE*	f7 = fopen("eigenvalues_Nu1.txt", "rt");
FILE*	f8 = fopen("eigenvalues_Nu2.txt", "rt");
sprintf(filename, "dispersion_%lf_bond_NN_%lf_%lf_NNN%lf_%lf.txt",dist,K1,K2,K3,K5);
FILE* f9 = fopen(filename, "w+");
fprintf (f9,"###############################################################\n");
fprintf (f9,"#Number of atoms in chain %i\n", order);
fprintf (f9,"#Number of cells %i\n", N);
fprintf (f9,"#Short Pt-X force constant %lf N/M\n", K1);
fprintf (f9,"#Long Pt-X force constant %lf N/M\n", K2);
fprintf (f9,"#Short X-X force constant %lf N/M\n", K3);
fprintf (f9,"#Long X-X force constant %lf N/M\n", K4);
fprintf (f9,"#Pt-Pt force constant %lf N/M\n", K5);
fprintf (f9,"#Distance between atom pair %lf x10^-10 M\n", dist);
fprintf (f9,"#Temperature %lf K\n", T);
fprintf (f9,"#Acoustic MSRD %g x10^-20 M^2\n", DWa);
fprintf (f9,"#Nu3 Optical MSRD %g x10^-20 M^2\n", DWnu3);
fprintf (f9,"#Nu1 Optical MSRD %g x10^-20 M^2\n", DWnu1);
fprintf (f9,"#Nu2 Optical MSRD %g x10^-20 M^2\n", DWnu2);
fprintf (f9,"#Total MSRD %g x10^-20 M^2\n", tot);
fprintf (f9,"###############################################################\n");
while(fgets(line, 130, f5) != NULL)
{
	sscanf (line,"%lf %lf",&k,&wave);
	fprintf (f9,"%lf %lf\n", k, wave);
}
while(fgets(line, 130, f6) != NULL)
{
	sscanf (line,"%lf %lf",&k,&wave);
	fprintf (f9,"%lf %lf\n", k, wave);
}
while(fgets(line, 130, f7) != NULL)
{
	sscanf (line,"%lf %lf",&k,&wave);
	fprintf (f9,"%lf %lf\n", k, wave);
}
while(fgets(line, 130, f8) != NULL)
{
	sscanf (line,"%lf %lf",&k,&wave);
	fprintf (f9,"%lf %lf\n", k, wave);
}
fclose(f5);	
fclose(f6);
fclose(f7);
fclose(f8);
return 0;
}