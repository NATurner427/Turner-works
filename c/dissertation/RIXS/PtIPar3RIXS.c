#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>


/*this program chops the large files that come out of the data accusition software from the SSRL into individual scans. Once seperated some corrections are applied, concentration counts correction, Io normalization, and energy shift using the glitch energy in the Io. Once these corrections are applied a large data file with the 3 dimensions of the rixs grid is created. Outputs uncorrected individual scan files, relivent correction values for each of those scans, the post processed individual scans the final RIXS map, and a RIXS grid that chops the elastic scattering region. */ 

int main()

{
	int n=0, m=1, i, j=0, k=0;
	char line[130];
	char filename[64];
	char c;
	char dataset[64];
	double maker;
	double mono, time, Epoch, Io, I1, Mpf_top, Mpf_bot, FF1, FF2, FF3, DTC_FF1, DTC_FF2, DTC_FF3, Emission, Raman, pd1, tempC, seconds, seconds2, localmin, calib, glitch, calibener, calibemiss, norminten, concensum, finalinten, energytransfer, scaledinten;
	double x, y, z, gridx, gridy; 
	double check;
	
		FILE*spec = fopen( "PtI_Par_3", "rt");
	
	sprintf(filename, "RawSpec/PtI_Par_3_%d.txt", n);
	FILE* img = fopen(filename, "wb");
	sprintf(filename, "RawSpec2nd/PtI_Par_3_%d.txt", n);
	FILE* img2 = fopen(filename, "wb");
		/*This loop chops the large initial file into individual scans, and ennumerates them */
	while(fgets(line, 130, spec) != NULL)
  	 {
		int jj = -1;
		while(++jj < strlen(line))
		{
			if ((c = line[jj]) != ' ') break;
		}
		/*printf("%c\n", c);*/
		sscanf (line, "%lf",&maker);
		/*printf("%lf\n", maker);*/
		if (isspace(c))
		{
		    n++;
			/*printf("got one\n");*/
			
			if ( n % 3 == 0 )
			{
				m = n/3;
				
				if (m %2==0)
				{
				j++;
				fclose(img);
				/*printf ("%i %s\n", n, line);*/
				sprintf(filename, "RawSpec/PtI_Par_3_%i.txt", j);
				FILE* img = fopen(filename, "wb");	
				}
				if(m%2 ==1)
				{
				k++;
				fclose(img);
				/*printf ("%i %s\n", n, line);*/
				sprintf(filename, "RawSpec2nd/PtI_Par_3_%i_2nd.txt", k);
				FILE* img2 = fopen(filename, "wb");	
				}
			}
		}
		
		else if (maker <1)
		{
			printf("NO\n");
			continue;
		}
		else if(isdigit(c)) 
		{
		
		 fprintf (img, "%s", line);
		 
		}	
	 }	 
	 /*printf("found %i breaks\n", n);*/
fclose(img);
fclose(img2);
fclose(spec);
	m=0;
	n=0;
	spec = fopen( "PtI_Par_3_concentration", "rt");
	
	sprintf(filename, "Concentrations/PtI_Par_3_%d_Concentration.txt", n);
	img = fopen(filename, "wb");

/* this loop chops the large intial concentration file into concentration files for each of the individual scans */
	while(fgets(line, 130, spec) != NULL)
  	 {
		int jj = -1;
		while(++jj < strlen(line))
		{
			if ((c = line[jj]) != ' ') break;
		}
		/*printf("%c\n", c);*/
		sscanf (line, "%lf",&maker);
		/*printf("%lf\n", maker);*/
		if (isspace(c))
		{
		    n++;
			/*printf("got one\n");*/
			
			if ( n % 2 == 0 )
			{
				m = n/2;
				fclose(img);
				/*printf ("%i %s\n", n, line);*/
				sprintf(filename, "Concentrations/PtI_Par_3_%d_Concentration.txt", m);
				FILE* img = fopen(filename, "wb");	
			}
		}
		else if(isdigit(c)) 
		{
		
		 fprintf (img, "%s", line);
		 
		}
		
	
	 }
	
fclose(img);
fclose(spec);

n=0;
m=70;
i=1;
j=0;
FILE*rixs = fopen( "PtI_Par_3_rixs.txt","w+");
FILE*concentration = fopen("PtI_Par_3_Concentrations.txt", "w+");
FILE*calibration = fopen("PtI_Par_3_Calibrations.txt", "w+");
/*this set of 3 loops works through the corrections that need to be applied to each of the individual scans. It calls to each scan and corresponding concentraion. A set of diagnostic files is created for each scan and for the set as a whole */
while( i <= m)
{

	sprintf(filename, "cookedSpec/cooked_PtI_Par_3_%d.txt", i);
	FILE*cook = fopen(filename, "w+");
	localmin=10000;
	calib=11701.9;
	concensum=0;
	
	/* first loop finds the local min of the glitch in Io and records the corresponding monochromator energy*/
	
	sprintf(filename, "RawSpec/PtI_Par_3_%d.txt", j);
	FILE*raw = fopen( filename, "rt");
	while(fgets(line, 130, raw) != NULL)
	{
	  sscanf (line, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", &mono, &Epoch, &Io, &I1, &Mpf_top, &Mpf_bot, &FF1, &FF2, &FF3, &DTC_FF1, &DTC_FF2, &DTC_FF3, &Emission,    &Raman, &pd1, &tempC, &seconds, &seconds2);
	  if( 11700.5 < mono && mono <11702.5 && Io < localmin)
		{
		localmin= Io;
		/*printf("new local min is %lf at %lf\n" , localmin, mono);*/
		glitch=mono;
		}
	}
	fclose(raw);
	
	
	/* second loop calculates concentration sum*/
	sprintf(filename, "Concentrations/PtI_Par_3_%d_Concentration.txt", i);
	FILE*concen = fopen(filename, "rt");
	while(fgets(line, 130, concen) != NULL)
	{
	  sscanf (line, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", &time, &Epoch, &Io, &I1, &Mpf_top, &Mpf_bot, &FF1, &FF2, &FF3, &DTC_FF1, &DTC_FF2, &DTC_FF3, &Emission,    &Raman, &pd1, &tempC, &seconds, &seconds2);
	  concensum=(concensum + DTC_FF1);
	}
	fclose(concen);
	
	
	/*preliminary arithmetic and tabulation of glitch values and concentration sums for each scan in the set*/
	calib= calib-glitch;
	fprintf(calibration, "%d %lf\n", i, calib);
	fprintf(concentration, "%d %lf\n", i, concensum);
	
	
	
	/* third loop applies calibration correction concentration sum normalization and Io normalization calculates energy transfer values, outputs a full cooked file and an important data only file*/
	sprintf(filename, "RawSpec/PtI_Par_3_%d.txt", j);
	raw = fopen( filename, "rt");	
	while(fgets(line, 130, raw) != NULL)
	{
		n++;
		sscanf (line, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", &mono, &Epoch, &Io, &I1, &Mpf_top, &Mpf_bot, &FF1, &FF2, &FF3, &DTC_FF1, &DTC_FF2, &DTC_FF3, &Emission,    &Raman, &pd1, &tempC, &seconds, &seconds2);
		calibener= mono + calib;
		calibemiss=Emission;
		scaledinten=DTC_FF1*10000;
		norminten= ((scaledinten)/Io);
		finalinten=((norminten)/concensum);
		energytransfer= (calibener - calibemiss);
		fprintf (cook, "%lf %lf %lf %lf %lf %lf 10000 %lf %lf %lf \n", mono, calib, calibener, Emission, energytransfer, DTC_FF1, Io, concensum, finalinten);
		/* outputs only the important data columns for the RIXS map --to a single file!--, and chops out the high end points taken for calibration purposes. This file, is that angled set of points. The next part of the Program needs to be run to fill the unfilled parts of the necessarily square grid with 0 values.*/ 
		if (1 <= n && n <= 58)
		{
			fprintf (rixs, "%lf %lf %lf\n", calibener, energytransfer, finalinten);
		}
	} 
n=0;	
i++;	
j++;
fclose(cook);
fclose(raw);
}
fclose(rixs);
fclose(concentration);
fclose(calibration);
printf("cooker routine finished\n");



x=0;
y=0;
z=0;

/*slice routine. This part of the program works with the now completed rixs data and chops it into 1-D scans of constant energy transfer and constant excitation energy */

gridy= -1;	
while(gridy <= 14)
{	



	rixs = fopen("PtI_Par_3_rixs.txt","rt");
	sprintf(filename, "EnSlices/PtI_Par_3_Energy transfer_%lf.txt", gridy);
	FILE* enslice = fopen(filename, "wb");
	while(fgets(line, 130, rixs) != NULL)
		{
			sscanf (line,"%lf %lf %lf",&x,&y,&z);
			if (y<=(gridy + 0.2) && y>=(gridy - 0.2))
				{
					fprintf(enslice, "%lf %lf\n", x, z);
				}
		}
	gridy=gridy+.25;
	fclose(enslice);
	fclose(rixs);
}	

gridx= 11550;	
while(gridx<= 11580.25)
{	
	rixs = fopen("PtI_Par_3_rixsfullgrid.txt","rt");
	sprintf(filename, "ExSlices/PtI_Par_3_Excitation_%lf.txt", gridx);
	FILE* exslice = fopen(filename, "wb");
	while(fgets(line, 130, rixs) != NULL)
		{
			sscanf (line,"%lf %lf %lf",&x,&y,&z);
			if (x<=(gridx + 0.2) && x>=(gridx - 0.2))
				{
					fprintf(exslice, "%lf %lf\n", y, z);
				}
		}
	gridx=gridx+.25;
	fclose(exslice);
	fclose(rixs);
}	





FILE*grid = fopen("PtI_Par_3_rixsfullgrid.txt","w+");
FILE*dyna = fopen("PtI_Par_3_rixsgrid.txt","w+");
gridx=11550;
gridy=-1;

/* fills the rixs map non value regions with 0 values to give us the square grid we need to plot properly.*/
while(gridy <= 14)
{
	check =gridy+1;
	check=check*4;
	
	if ( check ==12)
	{	
		printf("[##________]gridmaker is 20 percent done\n");
	}
	if ( check ==24)
	{	
		printf("[####______]gridmaker is 40 percent done\n");
	}
	if ( check ==36)
	{	
		printf("[######____]gridmaker is 60 percent done\n");
	}
	if ( check ==48)
	{	
		printf("[########__]gridmaker is 80 percent done\n");
	}
	while (gridx <=11580.25)
	{	
		FILE*rixs = fopen("PtI_Par_3_rixs.txt","rt");
		while(fgets(line, 130, rixs) != NULL)
		{
			sscanf (line,"%lf %lf %lf",&x,&y,&z);
			
			if ( x<=(gridx + 0.2) && x>=(gridx - 0.2) && y<=(gridy + 0.2) && y>=(gridy - 0.2))
			{
				n=1;
			}	
		}
		fclose(rixs);
		
		if (n==0)
		{
			fprintf (grid, "%lf %lf 0\n", gridx, gridy);
			if ( gridy>=2)
			{
			fprintf (dyna, "%lf %lf 0\n", gridx, gridy);
			}
		}
	gridx=gridx+.25;	
	n=0;
	}
gridx=11550;
gridy=gridy+.25;
}

/* now that all the necessary 0 values have been found, copy the actual data into those same data files to complete the grid */

rixs = fopen("PtI_Par_3_rixs.txt","rt");
while(fgets(line, 130, rixs) != NULL)
{
	sscanf (line,"%lf %lf %lf",&x,&y,&z);
	fprintf (grid, "%lf %lf %lf\n", x, y, z);	
	if ( y>=2)
			{
			fprintf (dyna, "%lf %lf %lf\n", x, y, z);
			}
}
fclose(rixs);
fclose(grid);


printf("Rixs for PtI_Par_3 complete\n");
}

