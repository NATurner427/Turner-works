#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define order 160
#define c 29980000000		
#define pi 3.14159265
#define a 10.855
int main()
{ 
int c1=order, c2=4*order;
double MassMatrix[order][order], ForceMatrix[order][order], DynamicMatrix[order][order], DynamicalMatrix[order][order], WR[order], WORK[120*120];
double AT[order*order];
double sum=0;
double branch=2;
int s,  i, j, ok;
char filename[64];
double eigenvalue=0;
double wave=0;	   
/*Love states that a 2 unit, 8 atom calc is enough for robust exploration of force constant affects. While for the monotonic case these matrices are 1x1 with a PBC I will do an 8x8 to more 	easily build up to the full case we will want to explore*/

/* need to construct Dynamical matrix for the diagonalization problem by inputing values for the mass and force constant matrices */

/*mass matrix input - should be easiest to construct in the program given that at its most complex it will be only 2 values in a diagonal matrix */

double MassX=5.8871086E-26; 
double MassPt=3.2394457E-25;
	   MassX=(1/MassX);
	   MassPt=(1/MassPt);   
	   MassX=sqrt(MassX);
	   MassPt=sqrt(MassPt);
	  
    for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
			if (i==j && (i % 2) == 0)
			{
				MassMatrix[i][j]= MassX;
			}
			else if (i==j && (i % 2) != 0)
			{
				MassMatrix[i][j]=MassPt;
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
            printf("%E   ", MassMatrix[i][j]);
        }
        printf("\n");
    }

/* next construct force constant matrix */
/* define force constants, up to 5 in Loves paper */
/* I think the fastest way to construct the matrix would be to use conditional loops*/

double K1 = 166;
double K2 = 45.4; 
double K3 = -4.7;
double K4 = -4.7;
double K5 = 36;
printf("constructing force constant matrix\n");

    for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
			if ((i%4)==0 && j%4==0 && i == j)
			{
				ForceMatrix[i][j]= K1 + K2 + K3 + K4;
			}
			else if ((i%4)==1 && (j%4)==1 && i==j)
			{
				ForceMatrix[i][j]= 2*K1 +2*K5;
			}
			else if ((i%4)==2 && (j%4)==2 && i==j)
			{
				ForceMatrix[i][j]= K1+K2+K3+K4;
			}
			else if ((i%4)==3 && (j%4)==3 && i == j)
			{
				ForceMatrix[i][j]= 2*K2 +2*K5;
			}
			else if (i == j-1)
			{
					if ((i%4) == 0)
					{
						ForceMatrix[i][j]=-K1;
					}
					else if ((i%4) == 1)
					{
						ForceMatrix[i][j]=-K1;
					}
					else if ((i%4) == 2)
					{
						ForceMatrix[i][j]=-K2;
					}
					else if ((i%4) == 3)
					{
						ForceMatrix[i][j]=-K2;
					}
			}
			else if (j == i-1)
			{
					if ((j%4) == 0)
					{
						ForceMatrix[i][j]=-K1;
					}
					else if ((j%4) == 1)
					{
						ForceMatrix[i][j]=-K1;
					}
					else if ((j%4) == 2)
					{
						ForceMatrix[i][j]=-K2;
					}
					else if ((j%4) == 3)
					{
						ForceMatrix[i][j]=-K2;
					}
			}
			else if (i == j-2)
			{
					if ((i%4)==0)
					{
						ForceMatrix[i][j] = -K3;
					}
					else if ((i%4)==1)
					{
						ForceMatrix[i][j] = -K5;
					}
					else if ((i%4)==2)
					{
						ForceMatrix[i][j] = -K4;
					}
					else if ((i%4)==3)
					{
						ForceMatrix[i][j] = -K5;
					}
			}
			else if (j == i-2)
			{
					if ((j%4)==0)
					{
						ForceMatrix[i][j] = -K3;
					}
					else if ((j%4)==1)
					{
						ForceMatrix[i][j] = -K5;
					}
					else if ((j%4)==2)
					{
						ForceMatrix[i][j] = -K4;
					}
					else if ((j%4)==3)
					{
						ForceMatrix[i][j] = -K5;
					}
			}
			else if (i == 0 && j == order-1)
			{
				ForceMatrix[i][j]=-K2;
			}
			else if (j == 0 && i == order-1)
			{
				ForceMatrix[i][j]=-K2;
			}
			else if (i == 0 && j == order-2)
			{
				ForceMatrix[i][j]=-K4;
			}
			else if (j == 0 && i == order-2)
			{
				ForceMatrix[i][j]=-K4;
			}
			else if (i == 1 && j == order-1)
			{
				ForceMatrix[i][j]=-K5;
			}
			else if (j == 1 && i == order-1)
			{
				ForceMatrix[i][j]=-K5;
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
            printf("%E  ", ForceMatrix[i][j]);
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
            printf("%E   ", DynamicalMatrix[i][j]);
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
		printf("%E   ", AT[i]);
	}
 printf("\n");

dsyev_("Vectors", "Upper", &c1, AT, &c1, WR, WORK, &c2, &ok);      

sprintf(filename, "eigenvalues_K1_%lf_K2_%lf.txt", K1, K2);
FILE* f1 = fopen(filename, "w+");

 printf("\n");
if (ok==0)				/* output of eigenvalues*/
{
   for (i=0; i<order; i++)
    { 
	j=i+1;
	 printf("%E\n", WR[i]);
	 eigenvalue=sqrt(WR[i]);
	 eigenvalue=eigenvalue/c;
	 eigenvalue=eigenvalue/pi;
	 eigenvalue=eigenvalue/2;
	 fprintf (f1,"%3i %12lf\n",j, eigenvalue);
	}   
}
else printf("An error occured %i",&ok);
fclose(f1);
/*break degeneracy*/

int index;
double value;
char line[130];
double check1=-999;
double check2=-999;

sprintf(filename, "eigenvalues_K1_%lf_K2_%lf.txt", K1, K2);
f1 = fopen(filename, "rt");
sprintf(filename, "degen_eigenvalunes_K1_%lf_K2_%lf.txt", K1, K2);
FILE* f3 = fopen(filename, "w+");
while(fgets(line, 130, f1) != NULL)
{
	
	sscanf (line,"%i %lf",&index,&value);
	check1=value;
	
	if ((check2==check1) && index < (.25*order))
	{
	index=(index*-1)+1;
		printf("we in there %i %lf 1\n", index, value);
		wave=index-1;
		wave=wave*pi;
		wave=wave/a;
		wave=wave/(.25*order);
		printf("we in there %lf %lf 1\n", wave, value);	
		fprintf (f3,"%3lf %12lf\n", wave, value);
	}
	else if (check2==check1 && index > (.75*order))
	{
	index=(index*-1)+1;
		printf("we in there %i %lf 1\n", index, value);
		wave=index+(order);
		wave=wave*pi;
		wave=wave/a;
		wave=wave/(.25*order);
		printf("we in there %lf %lf 1\n", wave, value);	
		fprintf (f3,"%3lf %12lf\n", wave, value);
	} 
	else if (check2==check1 && index > (order/2))
	{
	index=(index*-1)+1;
		printf("we in there %i %lf 1\n", index, value);
		wave=index+(.5*order);
		wave=wave*pi;
		wave=wave/a;
		wave=wave/(.25*order);
		printf("we in there %lf %lf 1\n", wave, value);	
		fprintf (f3,"%3lf %12lf\n", wave, value);
	}
	else if (check2==check1 && index > (order/4))
	{
	index=(index*-1)+1;
		printf("we in there %i %lf 1\n", index, value);
		wave=index+(.5*order);
		wave=wave*pi;
		wave=wave/a;
		wave=wave/(.25*order);
		printf("we in there %lf %lf 1\n", wave, value);	
		fprintf (f3,"%3lf %12lf\n", wave, value);
	} 
	else if (check2!=check1 && index < (.25*order))
	{
		printf("we in there %i %lf 1\n", index, value);
		wave=index-1;
		wave=wave*pi;
		wave=wave/a;
		wave=wave/(.25*order);
		printf("we in there %lf %lf 2\n", wave, value);
		fprintf (f3,"%3lf %12lf\n",wave, value);
	}
	else if (check2!=check1 && index > (.75*order))
	{
		printf("we in there %i %lf 1\n", index, value);
		wave=index-(order);
		wave=wave*pi;
		wave=wave/a;
		wave=wave/(.25*order);
		printf("we in there %lf %lf 2\n", wave, value);
		fprintf (f3,"%3lf %12lf\n",wave, value);
	}
	else if (check2!=check1 && index > (order/2))
	{
		printf("we in there %i %lf 1\n", index, value);
		wave=index-(.5*order);
		wave=wave*pi;
		wave=wave/a;
		wave=wave/(.25*order);
		printf("we in there %lf %lf 2\n", wave, value);
		fprintf (f3,"%3lf %12lf\n",wave, value);
	}
	else if (check2!=check1 && index > (order/4))
	{
		printf("we in there %i %lf 1\n", index, value);
		wave=index-(.5*order);
		wave=wave*pi;
		wave=wave/a;
		wave=wave/(.25*order);
		printf("we in there %lf %lf 2\n", wave, value);
		fprintf (f3,"%3lf %12lf\n",wave, value);
	}
	
	check2=check1;
}


/* output of eigenvectors*/
printf("\n");
sprintf(filename, "eigenvectors_K1_%lf_K2_%lf.txt", K1, K2);
FILE* f2 = fopen(filename, "w+");
for (i = 0; i < order*order; i++)
    {
	if(i % (order) ==0)
		{
			/*printf("\n");*/
			 fprintf (f2,"\n");
		}
        /*printf("%12lf", AT[i]);*/
		fprintf (f2,"%12lf  ", AT[i]);
	}	
fclose(f1);
fclose(f2);	
fclose(f3);
}




