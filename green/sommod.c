#include <math.h>
#include <stdio.h>
#include <errno.h>

#include "sommod.h"
#include "../common/Malloc.h"

#ifdef HASNRECIPES
#include "nrutil.h"
#include "nr.h"
#endif

#define IM_NAH_MAX 	2

#define NO_MEM  	"kein Speicher"
#define BAD_SEQ		"falsche Reihenfolge"
#define DERKF_FMT	"ERROR in pathint: derkf terminates with idid==%i"

#define EPS		1E-8
#define MAXIT		30

typedef struct diel_{
  complex *Gtab,*G1tab;			/* Tabellen der Sommerfeldintegrale */
  int   nentries;			/* Tabellengr"osse 		*/
  float kD;				/* Substrath"ohe (mit k mult. ) */
  float er;				/* rel. DK			*/
  float pol;				/* Polstelle von N[l]		*/
  float res;				/* Residuum von 1/N[l]		*/
  float sfwdivj;			/* 'Amplitude' der Oberfl"achenwelle */
					/* =(- 2 Pi) (-eps^2 up)/(uep)^2 lp res */
  float im_nah_coefs[IM_NAH_MAX+1];     /* Koeff. Reihenentw. Im-teil   */
} diel_data;

static void error_diel(mess)
 char *mess;
{
fprintf(stderr,"%s\n",mess);
}

complex *dielGtab(dp)
Diel dp;
{return dp->Gtab;}

complex *dielG1tab(dp)
Diel dp;
{return dp->G1tab;}

int dielGtabSize(dp)
Diel dp;
{return dp->nentries;}

/* Die Polstelle, das Residuum von 1/N[x]  sowie
 * die 'Amplitude' der Oberfl"achenwelle mit
 *
 *  N[x]= -ue_ Sin[ue_ kD] + er u Cos[ue_ kD]
 *
 *  ue_=sqrt(er-x^2);
 *  u  =sqrt(x^2-1);
 *
 * auf der reellen Achse bestimmen.
 *
 * Die 'Amplitude' betr"agt
 *
 * 1/2 (-2 Pi I) (- er^2 up) / uep^2 lp res
 *
 * und ist rein imagin"ar, da lp>1, uep Sinh[uep]= -uep_ Sin[uep_]
 */

static float Pol_Res_P1(dp)
 Diel dp;
{
 float fak;
 float er=dp->er;
 float kD=dp->kD;
 /* Die Anfangswerte sind grob gen"ahert; die einzige
  * Bedingung lautet x1>x0, f1>0, f0<0, f monoton
  * steigend im Intervall x0..x
  */
 float x1=(float)sqrt(er);
 float x0=1;
 float f1=er*(float)sqrt(er-1);
 float f0=-(er-1)*kD;           

 float ue,uekD,f,x,delta,xa;
 int  it=0;

 x=x0;
 do {
   delta=(x1-x0)/(f0-f1)*f0;
   xa=x;
   x=x0+delta;
   ue=(float)sqrt(er-x*x);
   uekD=ue*kD;
   f=(-ue*(float)sin(uekD)+er*(float)(sqrt(x*x-1)*cos(uekD)));
   if (f>0.) {
    x1=x; f1=f;
   } else {
    x0=x; f0=f;
   }
   it++;
 } while ( (fabs(xa-x)>EPS) && (it<MAXIT));
           /* Vorsicht beim Abbruchkriterium; Rundungsfehler! */
 
 printf("iter %i\n",it);

 if (fabs(xa-x)>EPS ) {
  return(1E55); /* Verfahren konvergiert nicht */
 }


 xa=sqrt(x*x-1)*kD;
 fak=(float)(sin(uekD)*(1+er*xa) + cos(uekD)*uekD*(1+er/xa));
 dp->res=(1./(fak*x/ue));
 /* ein faktor x/ue k"urzt sich raus, da im residuum vorhanden */
 dp->sfwdivj=-(M_PI*er*er*xa/uekD/fak); /* negativ, da 1/ue_^2=-1/ue^2 */

 return((float)x);
}

/* Die Koeffizienten der Imaginaerteilentwicklung berechnen
 * dazu m"ussen Pol und Residuum bereits initialisiert sein!
 *
 * Die iterative Berechnung ist wegen Differenzen grosser Zahlen
 * nur f"ur kleine Ordnungen geeignet!
 */

static void imag_coefs(dp)
 Diel dp;
{
 	/* Materialkonstanten und abgel. Gr. */
 float er  =dp->er;
 float kD  =dp->kD;
 float a   =kD*sqrt(er-1);
 float ca  =cos(a);
 float sa  =sin(a);
 float pol =dp->pol;
 float residue=dp->res;
 	/* Koeffizienten des Nennerpolynoms  */
 float tmp =(a*ca+sa);
 float A   =tmp*kD/2./a;
 float alf =er/kD*a*ca/tmp;
 float bet =2.*(1.-er)*sa/tmp;
	/* Pole der Partialbruchzerl. Darstellung */
        /* Z"ahlerkoeffizienten d. Zerl. */
 float zp00,zpa,zpb;

 /* Wegen der Differenzen grosser Zahlen */
 double q2,pa,pb,pa2,pb2;
 double I00,I1,I2,K,fak;

 float polpot;
 int  m; 

 if (dp->pol==0.) {
  error_diel(BAD_SEQ);
  return;
 }

 q2  =er-1.;
 tmp =sqrt(alf*alf-bet);
 pa  =alf+tmp; pa2=pa*pa;
 pb  =alf-tmp; pb2=pb*pb;

 /* Dies sind die Koeffizienten der Partialbruchzerl. */
 zp00 =(q2+pa*pb)/(q2-pa2)/(q2-pb2);
 zpa  =pa/(q2-pa2)/(pb-pa);
 zpb  =pb/(q2-pb2)/(pa-pb);

 /* initialisieren der Iteration */

 tmp=kD/a;
 I00=atan(1./sqrt(er-1.))/sqrt(er-1.);
 I1 =atan(1./pa)/pa;
 I2 =atan(1./pb)/pb;

 fak=1.;
 K=1.;
 /* das komplette Residuum berechnen */
 residue*=-2*M_PI*pol*sqrt(pol*pol-1)/(pol*pol-er);
 polpot=1.;
 
 /* mit pi^2 multipliziert */
 zp00*=q2;
 zpa*= pa2;
 zpb*= pb2;

  
 dp->im_nah_coefs[0]= -er*fak*(2./A*(	zp00*I00 +
					zpa*I1+			      
					zpb*I2) +
                                residue*polpot);

 for (m=1;m<=IM_NAH_MAX; m++) {
  fak/=-(m*m*4.);
  I00=-K+(1+q2)*I00;  /* hier werden unangenehme Differenzen gebildet */
  I1 =-K+(1+pa2)*I1;
  I2 =-K+(1+pb2)*I2;
  K*=(float)(m<<1)/(float)((m<<1)+1);
  polpot*=pol*pol;

  if (q2>  1000.)  I00=K/q2;  /* N"aherungswert */
  if (pa2> 1000.) I1 =K/pa2;  /* N"aherungswert */
  if (pb2> 1000.) I2 =K/pb2;  /* N"aherungswert */

  dp->im_nah_coefs[m]= -er*fak*(2./A*(	zp00*I00 +
					zpa*I1+			      
					zpb*I2)
                                + residue*polpot);
 }

 m=4;
 tmp=0;
 do {
  m--;
  tmp+=dp->im_nah_coefs[m];
 } while(m);
 printf("sum of coefs %g\n",tmp);
}
                           

Diel init_diel(kD,er)
 float kD,er;
{
 Diel dp=0;

 if ((dp=(Diel)Malloc(sizeof(diel_data)))==0) {
   /* no memory */
   error_diel(NO_MEM);
   return(0);
 }
 dp->Gtab=0;
 dp->G1tab=0;
 dp->nentries=0;
 dp->kD=kD;
 dp->er=er;

 /* Polstelle und Residuum berechnen */
 dp->pol=Pol_Res_P1(dp);

 /* Koeffizienten der Reihenentwicklung berechnen */
 imag_coefs(dp);
 return(dp);
}

void destroy_diel(dp)
 Diel dp;
{
 if (dp->Gtab) Free((char*)(dp->Gtab));
 if (dp->G1tab) Free((char*)(dp->G1tab));
 Free((char*)dp);
}

 
/* Die berechneten Daten ausgeben */

void print_diel(dp)
  Diel dp;
{
 int i;

 fprintf(stdout,"Substratdicke k*D: %g, rel. DK er: %g\n",
		dp->kD,dp->er);
 fprintf(stdout,"Polstelle: %g, Residuum %g\n",
		dp->pol,dp->res);

 fprintf(stdout,"Koeffizienten der Nahfeldentw.; Imag.teil\n");

 for(i=0;i<=IM_NAH_MAX; i++) {
   fprintf(stdout,"C[%i]=%g\n",i,dp->im_nah_coefs[i]);
 }

}


/* Die Reihenentwicklungen f"ur die Nahkopplung auswerten;
 * diese wird nach Koordinaten aufgeteilt berechnet:
 * 
 * Egi . 1/kw Integrate[ I [[G(krij)]] , {u,u0-k2/w,u0+kw/2}
 *
 * ---> Egj.Egi term1 + Egj.Eni term2
 *
 */ 

void somkop_nahe(u,c,dp,kw,term1,term2)
  float u,c,kw;
  Diel dp;
  complex *term1,*term2;
{
 int m;
 float kr2=u*u+c*c,krpot=1.;
 double sum;



/* Berechnung des Imagin"arteils der Nahkopplung */

 sum=dp->im_nah_coefs[0];
 for (m=1;m<=IM_NAH_MAX; m++) {
   krpot*=kr2;
   sum+=krpot*dp->im_nah_coefs[m];
 }

 (*term1).re=-sum;
  
}


/*
* calculate Sommerfeld-integral by path integration
* along the branch-cut of sqrt( l^2-1). The chosen path
* for l is l=1-I t.
*
* input parameters: krho:  distance to observation point (scaled by
*			   k0=2 Pi f/c; free space wavelength)
*		    eps  : permittivity of substrate
*		    kD   : substrate thickness, scaled by k0
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

int  alloc_work(neqs,rwork,iwork,extension)
  int   neqs;
  float **rwork;
  int	**iwork;
  char  *extension;
{
#ifndef HASNRECIPES 
#ifndef HASIMSL
  if((*rwork=(float*)Malloc((33+7*neqs)*sizeof(float)))==0) {
        error_diel(NO_MEM);
	return 0;
  }
  if ((*iwork=(int*)Malloc(33*sizeof(int)))==0) {
	Free(*rwork);*rwork=0;
        error_diel(NO_MEM);
	return 0;
  }
  return -1;
#endif
#endif
}

/* returns error code of ODE integrator in error */

int pathint(diel,y,krho,neqs,end,alf,rwork,iwork,error)
  Diel	  diel;
  float   *y,krho,end;
  complex alf;
  int     neqs;
  float   *rwork;
  int	  *iwork;
  int	  *error;
{
  float t,tol,tend;
  float	tmp,tmp1,kD;
  float abtol;
  int	i,rval=0;
  char  *buf=0;
#if defined(HASNRECIPES) || defined(HASIMSL)
  int   nsteps,nfns;
#endif
#ifndef HASNRECIPES
#ifndef HASIMSL
  float	rpar[1];
  int	info[15],lrw,liw,ipar[1];
#endif
#endif

#ifndef HASNRECIPES
#ifndef HASIMSL
  liw=33;
  lrw=33+7*neqs;
  info[0]=0; /* 1st call for this problem */
  info[1]=0; /* abtol,tol are scalars */
  info[2]=0; /* solution only at tend wanted */
#endif
#endif
  derivsglob_.kr=krho;
  tmp=krho*krho;
  derivsglob_.akr2.re=tmp*alf.re;
  derivsglob_.akr2.im=tmp*alf.im;
  derivsglob_.er=diel->er;
  derivsglob_.kD=kD=diel->kD;
  derivsglob_.akD.re=kD*alf.re;
  derivsglob_.akD.im=kD*alf.im;
  derivsglob_.er2=(diel->er*diel->er);
  derivsglob_.alpha=alf;
  derivsglob_.n=neqs;
  t=0.;
  tend=end/krho;
  tol=1.E-4;
  abtol=1.E-6;
#ifdef HASNRECIPES
  /* bsstep seems not to work (hangs) */
  greeninit_(&t,y-1);
  odeint(y-1,neqs,tend,t,tol,0.01,0.0,nsteps,nfns,derivs_,rkqs);
  *nfns=-1;
#else
  /* integrate backwards to enhance precision
   * (the hankel function of the first kind is growing
   *  exponentially when integrating forward)
   */
  greeninit_(&tend,y);
  derkf_(derivs_,&neqs,&tend,y,&t,info,&tol,&abtol,error,
	rwork,&lrw,iwork,&liw,rpar,ipar);
  if (*error!=2) {
	rval=-1;
	if ((buf=Malloc(128))==0) {
	  buf=DERKF_FMT;
        } else {
	  sprintf(buf,DERKF_FMT,error);
	}
	error_diel(buf);
	if (buf) {Free(buf); buf=0;}
  }
#endif
/* 
 * Multiply by dl/dt= alf which was not done in derivs.
 * Change sign (backwards integration).
 */
  for (i=0; i<4; i+=2) {
	/* multiplication by alf */
	tmp=alf.re*y[i];
	tmp1=alf.im*y[i+1];
	y[i+1]=-((alf.re+alf.im)*(y[i]+y[i+1])-tmp-tmp1);
	y[i]=-(tmp-tmp1);
  }
#ifdef DEBUG
  for(i=0; i<neqs; i+=2) {
	fprintf(stderr,"( %g %cI %g )\n",
		y[i],(y[i+1]<0.?'-':'+'),fabs(y[i+1]));
  }
#endif
return rval;
}

/* Oberfl"achenwellenanteil im Fernfeld berechnen
 */

void SFW(result,diel,krho)
 float *result;
 Diel  diel;
 float krho;
{
 float p=diel->pol;
 float a=diel->sfwdivj;
 float tmp,pkrho;

 pkrho=p*krho;
 tmp=a/pkrho;
 result[2]=(float)(-tmp*y1(pkrho));
 result[3]=(float)(-tmp*j1(pkrho));

 a+=a;
 result[0]=result[2]+(float)a*y0(pkrho);
 result[1]=result[3]+(float)a*j0(pkrho);
}

static void free_tables(dp)
Diel dp;
{
  if (dp->Gtab) {Free((char*)(dp->Gtab)); dp->Gtab=0;}
  if (dp->G1tab) {Free((char*)(dp->G1tab)); dp->G1tab=0;}
  dp->nentries=0;
}
/* fill the table  (return its size) */

int dielGetChunk(dp,size,krho_proc)
Diel dp;
int  size;
float (*krho_proc)();
{
int	i,newsize;
complex *pt,*pt1;
float	*rwork=0,*y=0,sfw[4];
int	*iwork=0,neqs;
complex alf;
float	end,twokD,krho;
int	error;

neqs=8; /* don't integrate sinh numerically (isn't faster but less accurate) */
end=7.0; /* parameter at end of integration path (argument to H0 is krho+alf*end) */
alf.re=0.8; /* integration path has to be in the 4th quadrant in order for */
alf.im=-1.; /* H0 to decrease rapidly and not to cross the branch cuts of  */
	    /* sqrt(l^2-1) 						   */
twokD=dp->kD; twokD+=twokD;

/* try to get memory */
if ((y=(float*)Malloc(sizeof(float)*neqs))==0) goto cleanup;
if (!alloc_work(neqs,&rwork,&iwork,0)) goto cleanup;

/* get memory for the tables */
if (dp->nentries==0) {
  if ((dp->Gtab=(complex*)Malloc(sizeof(complex)*size))==0 ||
      (dp->G1tab=(complex*)Malloc(sizeof(complex)*size))==0) {
	free_tables(dp);
	goto cleanup;
  }
} else {
  if ((pt=(complex*)Realloc((char*)(dp->Gtab),
			    sizeof(complex)*(dp->nentries+size)))==0) {
	goto cleanup;
  } else {
	dp->Gtab=pt;
  }
  if ((pt=(complex*)Realloc((char*)(dp->G1tab),
			    sizeof(complex)*(dp->nentries+size)))==0) {
	/* undo previous allocation */
  	if ((pt=(complex*)Realloc((char*)(dp->Gtab),
			    sizeof(complex)*(dp->nentries)))==0) {
	  free_tables(dp);
	} else {
	  dp->Gtab=pt;
	}
	goto cleanup;
  } else {
	dp->G1tab=pt;
  }
}
newsize=dp->nentries+size;
pt=dp->Gtab+dp->nentries;
pt1=dp->G1tab+dp->nentries;
for (i=dp->nentries; i<newsize; i++) {
 krho=krho_proc(i);
 if (pathint(dp,y,krho,neqs,end,alf,rwork,iwork,&error)) {
	error_diel("dielGetChunk: aborting..");
	newsize=i-1;
	if ((pt=(complex*)Realloc((char*)(dp->Gtab),
				sizeof(complex)*newsize)) == 0 ||
	    (pt1=(complex*)Realloc((char*)(dp->G1tab),
				sizeof(complex)*newsize)) == 0) {
	  free_tables(dp);
	} else {
	  dp->Gtab=pt; dp->G1tab=pt1;
	  dp->nentries=newsize;
	}
	goto cleanup;
 }
 /* add surface wave pole contribution */
 SFW(sfw,dp,krho);
 /* multiply the result by I/2/kD for sake of compatibility
  * with old version.
  */
 pt->re =-(y[1]+sfw[1])/twokD; /* Re[Gtab] */
 pt->im = (y[0]+sfw[0])/twokD; /* Im[Gtab] */
 pt1->re=-(y[3]+sfw[3])/twokD; /* Re[G1tab] */
 pt1->im= (y[2]+sfw[2])/twokD; /* Im[G1tab] */
 pt++; pt1++;
}
dp->nentries=newsize;

cleanup:
  if (y) {Free((char*)y); y=0;}
  if (rwork) {Free((char*)rwork); rwork=0;}
  if (iwork) {Free((char*)iwork); iwork=0;}
  return dp->nentries;	
}
