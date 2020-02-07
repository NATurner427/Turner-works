#include <stdio.h>
#include <math.h>

int main()

{
	int n=0, n2;
	double xcord, ycord, zcord, xcord2, ycord2, zcord2,xcord3, ycord3, zcord3, bond1x, bond1y, bond1z, bond2x, bond2y, bond2z, midmag, rotmag, hydro1, hydro2, midx, midy, midz, rot1x, rot1y, rot1z, rot2x, rot2y, rot2z, unitx, unity, unitz, Hx1, Hy1, Hz1, Hx2, Hy2, Hz2, H1mag, H2mag;
	char line[80], atom[3];
	FILE*	f1 = fopen( "PtIhydro.txt", "rt");
	FILE*	f2 = fopen("hydro.txt", "w+");
	while(fgets(line, 80, f1) != NULL)
	{
		n= n+1;
		 /* get a line, up to 80 chars from xyz.  done if NULL */
		 sscanf (line, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf", atom, &xcord, &ycord, &zcord, &xcord2, &ycord2, &zcord2, &xcord3, &ycord3, &zcord3);

		bond1x= xcord2 -xcord;
		bond1y= ycord2 -ycord;
		bond1z= zcord2 -zcord;

		bond2x= xcord3- xcord;
		bond2y= ycord3- ycord;
		bond2z= zcord3- zcord;

		rot1x= (bond1y*bond2z - bond2y*bond1z);
		rot1y= (bond2x*bond1z - bond1x*bond2z);
		rot1z= (bond1x*bond2y - bond2x*bond1y);

		midx= (bond1x + bond2x)/2;
		midy= (bond1y + bond2y)/2;
		midz= (bond1z + bond2z)/2;

		rot2x=(midy*rot1z - rot1y*midz);
		rot2y=(rot1x*midz - midx*rot1z);
		rot2z=(midx*rot1y - rot1x*midy);

		rotmag= sqrt(rot2x*rot2x + rot2y*rot2y + rot2z*rot2z);

		unitx=(rot2x/rotmag);
		unity=(rot2y/rotmag);
		unitz=(rot2z/rotmag);

		Hx1= (cos(2.186) + unitx*unitx*(1-cos(2.186)))*midx + (unitx*unity*(1-cos(2.186)) - unitz*sin(2.186))*midy + (unitx*unitz*(1-cos(2.186)) + unity*sin(2.186))*midz;

		Hy1= ((unitx*unity*(1-cos(2.186)) + unitz*sin(2.186))*midx) 
		
		+((cos(2.186) + unity*unity*(1-cos(2.186)))*midy) + 
		
		((unity*unitz*(1-cos(2.186)) - unitx*sin(2.186))*midz);

		Hz1= ((unitz*unitx*(1-cos(2.186)) - unity*sin(2.186))*midx) + 
		
		((unitz*unity*(1-cos(2.186)) + unitx*sin(2.186))*midy) + 
		((cos(2.186) + unitz*unitz*(1-cos(2.186)))*midz);

		Hx2= (cos(-2.186) + unitx*unitx*(1-cos(-2.186)))*midx + (unitx*unity*(1-cos(-2.186)) - unitz*sin(-2.186))*midy + (unitx*unitz*(1-cos(-2.186)) +unity*sin(-2.186))*midz;

		Hy2= ((unitx*unity*(1-cos(-2.186)) + unitz*sin(-2.186))*midx) +((cos(-2.186) + unity*unity*(1-cos(-2.186)))*midy) + ((unitz*unity*(1-cos(-2.186)) - unitx*sin(-2.186))*midz);

		Hz2= ((unitz*unitx*(1-cos(-2.186)) - unity*sin(-2.186))*midx) + ((unitz*unity*(1-cos(-2.186)) + unitx*sin(-2.186))*midy) + ((cos(-2.186) + unitz*unitz*(1-cos(-2.186)))*midz);

		H1mag= sqrt(Hx1*Hx1 + Hy1*Hy1 + Hz1*Hz1);

		H2mag= sqrt(Hx2*Hx2 + Hy2*Hy2 + Hz2*Hz2);

		Hx1= (.9053/H1mag)*Hx1;

		Hy1= (.9053/H1mag)*Hy1;

		Hz1= (.9053/H1mag)*Hz1;

		Hx2= (.9053/H2mag)*Hx2;

		Hy2= (.9053/H2mag)*Hy2;

		Hz2= (.9053/H2mag)*Hz2;

		H1mag= sqrt(Hx1*Hx1 + Hy1*Hy1 + Hz1*Hz1);

		H2mag= sqrt(Hx2*Hx2 + Hy2*Hy2 + Hz2*Hz2);

		Hx1=Hx1 + xcord;

		Hy1=Hy1 + ycord;

		Hz1=Hz1 + zcord;

		Hx2=Hx2 + xcord;

		Hy2=Hy2 + ycord;

		Hz2=Hz2 + zcord;

		midx=midx + xcord;

		midz=midy + ycord;

		midz=midz + zcord;

		rot1x=rot1x + xcord;

		rot1y=rot1y + ycord;

		rot1z=rot1z + zcord;

		printf( "H %lf %lf %lf \n", Hx1, Hy1, Hz1);

		printf( "H %lf %lf %lf \n\n", Hx2, Hy2, Hz2);

		fprintf (f2, "H %lf %lf %lf\n",n, atom, Hx1, Hy1, Hz1);
		n=n+1;
		fprintf (f2, "H %lf %lf %lf\n",n, atom, Hx2, Hy2, Hz2);

	}
fclose(f1);
fclose(f2);
}
