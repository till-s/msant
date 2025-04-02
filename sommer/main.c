#include "util.h"
#include <malloc.h>

#define DIM 40
#define NMAX (2*DIM+1)

#define EPSI 1E-8


extern void f2tcf_();

complex somkern(i,j,a,er,kd)
 int i,j;
 real  a,er,kd;
{
 complex rval;
 real lx=a*(real)i,ly=a*(real)j;
 real u2=lx*lx+ly*ly-1;
 real u,f1,f2;
 real er2=er*er;
 double  si,co;

 if (lx==0.) {
  rval.re=rval.im=(real)0.; return(rval);
 }

 if (fabs(u2)<EPSI) {
  rval.re=(er2+er-2)/2/er2/kd/kd/lx;
  rval.im=(real)0.;
  return(rval);
 }

 if (signbit(u2)) {
  
  u=sqrt(-u2);
  sincos(u*kd,&si,&co);
  f1=er2/((er2-1)*co*co+1);
  f2=a/u2/er/lx;

  rval.im=-si*f2*(1+f1);
  rval.re= f2*(er-1 + co*(1-er*f1));
  return(rval);
 } else {

  u=sqrt(u2);
  si=exp(-u*kd);
  f2=a/u2/er/lx;
  
  rval.re=f2*(er-1 + si*(1 - 2*er2/((er+1)+si*si*(er-1))));
  rval.im=(real)0.;
  return(rval);
 }
}

  
  


main(argc,argv)
 int argc;
 char *argv[];
{
 int n,i,j;
 float a,lmaxx=4,lmaxy=4;
 real er=6.,kd=0.1;
 real    *wff1,*wff2,*cpy;
 complex *cwk;
 complex seq[NMAX][NMAX];

 short   test;

 a=lmaxx*lmaxy*4./(real)NMAX/(real)NMAX;

 for (i=-DIM;i<=DIM;i++) {
	for (j=-DIM;j<=DIM;j++) {
		seq[i<0?NMAX+i:i][j<0?NMAX+j:j]=somkern(i,j,a,er,kd);
	}
 }

 n=NMAX;

 wff1=(real*)    malloc(sizeof(real)*(4*n+15));
 wff2=(real*)    malloc(sizeof(real)*(4*n+15));
 cwk =(complex*) malloc(sizeof(complex)*n*n);
 cpy =(real*)    malloc(sizeof(real)*2*n);
 fftci_(&n,wff1);
 fftci_(&n,wff2);
/*
 f2t2d_(&n,&n,seq,&n,seq,&n,wff1,wff2,cwk,cpy);
*/

 free(wff1); free(wff2); free(cwk); free(cpy);

#ifdef DEBUG
 for (i=0;i<NMAX;i++) {
	for (j=0;j<NMAX;j++) {

 printf("( %g %cj %g)",	CVF(seq[i][j].re),
				signbit(CVF(seq[i][j].im))?'-':'+',
				fabs(CVF(seq[i][j].im)));
	}
	printf("\n");
 }
#endif

 do {
 printf("i,j >");
 scanf("%i%i",&i,&j);
 if (i<0||j<0) break;
 printf("( %g %cj %g)\n",	CVF(seq[i][j].re),
				signbit(CVF(seq[i][j].im))?'-':'+',
				fabs(CVF(seq[i][j].im)));
 } while(1);
}
