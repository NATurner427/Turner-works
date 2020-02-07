#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define order 10
		

int main()
{ 
int c1=order, c2=8*order;
double MassMatrix[order][order], ForceMatrix[order][order], DynamicMatrix[order][order], DynamicalMatrix[order][order], WR[order], WORK[order*order];
double AT[order*order];
double sum=0;
int s,  i, j, ok;
FILE*	f1 = fopen("eigenvalues.txt", "w+");
FILE*	f2 = fopen("eigenvectors.txt", "w+");

/*Love states that a 2 unit, 8 atom calc is enough for robust exploration of force constant affects. While for the monotonic case these matrices are 1x1 with a PBC I will do an 8x8 to more 	easily build up to the full case we will want to explore*/

/* need to construct Dynamical matrix for the diagonalization problem by inputing values for the mass and force constant matrices */

/*mass matrix input - should be easiest to construct in the program given that at its most complex it will be only 2 values in a diagonal matrix */

double Mass1=1;
double Mass2=2;
	   Mass1=(1/Mass1);
	   Mass2=(1/Mass2);
	   Mass1=sqrt(Mass1);
	   Mass2=sqrt(Mass2);

    for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
			if (i==j && (i % 2) == 0)
			{
				MassMatrix[i][j]= Mass1;
			}
			else if (i==j && (i % 2) != 0)
			{
				MassMatrix[i][j]=Mass2;
			}
			else
			{
				MassMatrix[i][j]=0;
			}
		}
    }
	
printf("Mass matrix is\n");
	
	for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            printf("%lf   ", MassMatrix[i][j]);
        }
        printf("\n");
    }

/* next construct force constant matrix */

/* define force constants, up to 5 in Loves paper */
/* I think the fastest way to construct the matrix would be to use conditional loops*/

double K1 = 1;

printf("constructing force constant matrix\n");

    for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
			if (i ==j )
			{
				ForceMatrix[i][j]= K1*2;
			}
			else if (i == j-1 || i-1 == j)
			{
				ForceMatrix[i][j]=-K1;
			}
			else if (i == 0 && j == order-1)
			{
				ForceMatrix[i][j]=-K1;
			}
			else if (j == 0 && i == order-1)
			{
				ForceMatrix[i][j]=-K1;
			}
			else
			{
				ForceMatrix[i][j]=0;
			}
		}
    }
	
printf("Force Matrix is\n");
	
	for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            printf("%lf   ", ForceMatrix[i][j]);
        }
        printf("\n");
	}
	
/*multiply matrices to create dynamical matrix */
	
printf("Dynamical Matrix being created\n");

for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
			for(s = 0; s <order; s++)
			{
				sum= sum + ForceMatrix[i][s] * MassMatrix[s][j];
			}
			DynamicMatrix[i][j]=sum;
			sum=0;
		}
	}
for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
			for(s = 0; s <order; s++)
			{
				sum= sum + MassMatrix[i][s] * DynamicMatrix[s][j];
			}
			DynamicalMatrix[i][j]=sum;
			sum=0;
		}
	}			
for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            printf("%lf   ", DynamicalMatrix[i][j]);
        }
        printf("\n");
	}

/*calling to LAPACK to get eigenvalues and eigenvectors */

printf("Dynamical Matrix being transformed for fortran\n");


for (i=0; i<order; i++)		/* to call a Fortran routine from C we */
{							/* have to transform the matrix */
  for(j=0; j<order; j++) AT[j+order*i]=DynamicalMatrix[j][i];
}	

for (i = 0; i < order*order; i++)
    {
		if((i % order) ==0)
		{
        printf("\n");
		}
		printf("%lf   ", AT[i]);
	}
 printf("\n");

dsyev_("Vectors", "Upper", &c1, AT, &c1, WR, WORK, &c2, &ok);      

printf("\n");

if (ok==0)				/* output of eigenvalues and eigenvectors */
{
   for (j=0; j<order; j++)
   {
      printf("%lf    \n", WR[j]);
	  fprintf (f1,"%lf\n", WR[j]);
	  
   }
}
else printf("An error occured %i",&ok);
printf("\n");

for (i = 0; i < order*order; i++)
    {
	if(i % (order) ==0)
		{
			printf("\n");
			 fprintf (f2,"\n");
		}
        printf("%lf   ", AT[i]);
		fprintf (f2,"%lf  ", AT[i]);
	}	
fclose(f1);
fclose(f2);	
}




