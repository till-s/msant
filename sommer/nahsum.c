#include <math.h>
#include <stdio.h>

#define EPS 1.E-8

main(argc,argv)
  int argc;
  char *argv[];
{
 double q,zn,sum,an,xi,xin,er;

 printf("er? ");
 scanf("%lg",&er);
 printf("xi? ");
 scanf("%lg",&xi);

 q=(1-er)/(1+er);

 zn=1;
 xin=0;
 sum=asinh(xi);

 do {
  zn*=q;
  xin+=2*xi;
  an=zn*(asinh(xin+xi)-asinh(xin-xi));
  sum+=an;
 }
 while (fabs(an)>EPS);

 sum*=2.;

 printf("Gx, statisch =  %lg, (eps %g) \n",sum,EPS);
}

