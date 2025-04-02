#include <math.h>
#include <malloc.h>
#include <stdio.h>


#ifdef HASNRECIPES
#include "nrutil.h"
#include "nr.h"
#endif

#include "pathint.h"
/*
* calculate Sommerfeld-integral by path integration
* along the branch-cut of sqrt( l^2-1). The chosen path
* for l is l=1-I t.
*
* input parameters: krho:  distance to observation point (scaled by
*			   k0=2 Pi f/c; free space wavelength)
*		    eps  : permittivity of substrate
*		    kdick: substrate thickness, scaled by k0
*		    neqs : must be 2, 6 or 10
*			   if > 2 the Hankel function is obtained
*			   by integrating Bessel's DE along the
*			   same path
*			   if > 6 sinh() is also numerically calculated
*			   by integrating its DE along the path
*		    tend:  parameter t, where to stop the integration
*			   7. is an adequate value
*		    nsteps:(RETURNs) nr. of RK-steps (only supported by IMSL)
*		    nfns:  (RETURNs) nr. of function evaluations (only supported by IMSL)
*/

/* common block to pass variables to the fortran routine 'derivs' */

typedef struct derivs_cmn_ {
	float 	kr;
	complex akr2;
	float	er;
	float	kD;
	complex akD;
	float	er2;
	complex alpha;
	int	n;
} derivs_cmn;

extern derivs_cmn derivsglob_;

#ifdef HASNRECIPES
int nrcp_kmax=0, nrcp_kount;
float *nrcp_xp,*nrcp_yp,nrcp_dxsav;
#endif

#if defined(__STDC__) || defined(ANSI) || defined(NRANSI)
#ifdef HASNRECIPES
void derivs_(float*,float*,float*);
#else
#ifdef HASIMSL
void derivs_(int*,float*,float*,float*);
#else
void derivs_(float*,float*,float*,float*,int*);
#endif
#endif
#else
void derivs_();
#endif

void pathint(y,krho,eps,kdick,neqs,end,alf,nsteps,nfns)
  float   *y,krho,eps,kdick,end;
  complex alf;
  int     neqs,*nsteps,*nfns;
{
  float IuekD;
  float t,tol,tend;
  float	tmp;
  float *rwork=0;
  int	*iwork=0;
  float abtol;
#ifndef HASNRECIPES
#ifndef HASIMSL
  float	rpar[1];
  int	info[15],idid,lrw,liw,ipar[1];
#endif
#endif

#ifndef HASNRECIPES
#ifndef HASIMSL
  liw=33;
  lrw=33+7*neqs;
  rwork=(float*)malloc(lrw*sizeof(float));
  iwork=(int*)malloc(liw*sizeof(int));
  info[0]=0; /* 1st call for this problem */
  info[1]=0; /* abtol,tol are scalars */
  info[2]=0; /* solution only at tend wanted */
#endif
#endif
  derivsglob_.kr=krho;
  tmp=krho*krho;
  derivsglob_.akr2.re=tmp*alf.re;
  derivsglob_.akr2.im=tmp*alf.im;
  derivsglob_.er=eps;
  derivsglob_.kD=kdick;
  derivsglob_.akD.re=kdick*alf.re;
  derivsglob_.akD.im=kdick*alf.im;
  derivsglob_.er2=(eps*eps);
  derivsglob_.alpha=alf;
  derivsglob_.n=neqs;
  t=0.;
  tend=end/krho;
  tol=1.E-4;
  abtol=1.E-6;
#ifdef HASNRECIPES
  /* bsstep seems not to work (hangs) */
  greeninit_(&t,y-1);
  odeint(y-1,neqs,t,tend,tol,0.01,0.0,nsteps,nfns,derivs_,rkqs);
  *nfns=-1;
#else
  /* integrate backwards to enhance precision
   * (the hankel funktion of the first kind is growing
   *  exponentially when integrating forward)
   */
  greeninit_(&tend,y);
  derkf_(derivs_,&neqs,&tend,y,&t,info,&tol,&abtol,&idid,
	rwork,&lrw,iwork,&liw,rpar,ipar);
  if (idid!=2) {
	fprintf(stderr,"ERROR in pathint: derkf terminates with idid==%i\n",idid);
  }
#endif
  if (rwork) {free((char*)rwork); rwork=0;}
  if (iwork) {free((char*)iwork); iwork=0;}
}
