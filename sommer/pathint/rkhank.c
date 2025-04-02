/* Testprogramm zum Testen der Konvergenz
 * einer RK-Integration der DGL von l Hankel2(0, l kr)
 *
 * DGL (Differentiation nach l) (nach Abramowitz & Stegun)
 *
 * w= l H
 *
 * w" - w'/l + (kr^2 + 1/l^2) w = 0
 */

#include <math.h>
#include <stdio.h>

typedef float real;

static real sqrarg;
#define SQR(x) ((sqrarg=(x))==0.?0.:sqrarg*sqrarg)
#define DERIV(x,y,yprime,kr2) ( (yprime)/(x) - ((kr2)+1/SQR(x))*(y))


void nsteps(real kr2, real *w, real *wprime, real *x, real *h, int n)
{
real y=*w,yprime=*wprime,lam=*x,k1,k2,k3,k4;
real s=*h, sd2=s/2., sd8=s/8.;
int  i;
for (i=0; i<n; i++) {
   k1=s*DERIV(lam,y,yprime,kr2);
   lam+=sd2;
   k2=s*DERIV(lam,y+sd2*yprime+sd8*k1,yprime+k1/2,kr2);
   k3=s*DERIV(lam,y+sd2*yprime+sd8*k1,yprime+k2/2,kr2);
   lam+=sd2;
   k4=s*DERIV(lam,y+s*yprime+sd2*k3,yprime+k3,kr2);
   y+=s*(yprime+(k1+k2+k3)/6.);
   yprime+=(k1+k2+k2+k3+k3+k4)/6.;
}
*w=y;
*wprime=yprime;
*x=lam;
}

void usage(char *name)
{
fprintf(stderr,"Runge Kutta Integration of Bessel's DGL for x Z0(x kr)\n");
fprintf(stderr,"usage: %s xstart xend kr h [<Y>]\n",name);
fprintf(stderr,"use option Y if you want the Neumann function\n");
}

void main(int argc, char **argv)
{
real w,w1,wprime,x,xend,h,kr;
real wtrue,w1true;
char ch='J';
int  n;
if (argc<5 || argc>6) {usage(argv[0]); exit(1);}
if (argc==6) {
  sscanf(argv[5],"%c",&ch);
  if ((ch=toupper(ch))!=(int)'Y') {
    usage(argv[0]); exit(1);
  }
}
sscanf(argv[1],"%g",&x);
sscanf(argv[2],"%g",&xend);
sscanf(argv[3],"%g",&kr);
sscanf(argv[4],"%g",&h);
if (h==0. 					  /* unallowed stepsize */
    || (xend<=0. && x>=0.) || (x<=0. && xend>=0.) /* don't cross singularity */
    || (xend>x && h<0.) || (xend<x && h>0.)){	  /* always go towards xend */
  fprintf(stderr,"invalid parameters (crossing x=0., stepsize out of range)\n");
  exit(1);
}

if (ch=='Y') {
  w=(real)y0(kr*x)*x;
  wprime=(real)(y0(kr*x)-y1(kr*x)*x);
  wtrue=(real)y0(kr*xend)*xend;
  w1true=(real)y0(kr*xend)*xend;
} else {
  w=(real)j0(kr*x)*x;
  wprime=(real)(j0(kr*x)-j1(x)*x);
  wtrue=(real)j0(kr*xend)*xend;
  w1true=(real)j1(kr*xend)*xend;
}
if ((n=floor((xend-x)/h))>0)
  nsteps(kr*kr, &w, &wprime, &x, &h, n);
if ( (h=xend-x)!=0. )
  nsteps(kr*kr, &w, &wprime, &x, &h, n);

w1=w/x-wprime;

printf("%g %c0(%g*%g)= %g, error=%g (%g%%)\n",
	x,ch,kr,x,w,w-wtrue,100.*(1-w/wtrue));
printf("%g %c1(%g*%g)= %g, error=%g (%g%%)\n",
	x,ch,kr,x,w1,w1-w1true,100.*(1-w1/w1true));

}
