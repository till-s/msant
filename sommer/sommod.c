#include "util.h"
#include "sommod.h"
#include "malloc.h"

#define IM_NAH_MAX 	2

#define NO_MEM  	"kein Speicher"
#define BAD_SEQ		"falsche Reihenfolge"

#define EPS		1E-8
#define MAXIT		30

typedef struct diel_{
  real kD;				/* Substrath"ohe (mit k mult. ) */
  real er;				/* rel. DK			*/
  real pol;				/* Polstelle von N[l]		*/
  real res;				/* Residuum von 1/N[l]		*/
  real im_nah_coefs[IM_NAH_MAX+1];      /* Koeff. Reihenentw. Im-teil   */
} diel_data;

static void error_diel(mess)
 char *mess;
{
fprintf(stderr,"%s\n",mess);
}


/* Die Polstelle sowie das Residuum von 1/N[x] mit
 *
 *  N[x]= -ue_ Sin[ue_ kD] + er u Cos[ue_ kD]
 *
 *  ue_=sqrt(er-x^2);
 *  u  =sqrt(x^2-1);
 *
 * auf der reellen Achse bestimmen.
 */

static real Pol_Res_P1(dp)
 diel dp;
{
 real er=dp->er;
 real kD=dp->kD;
 /* Die Anfangswerte sind grob gen"ahert; die einzige
  * Bedingung lautet x1>x0, f1>0, f0<0, f monoton
  * steigend im Intervall x0..x
  */
 real x1=(real)sqrt(er);
 real x0=1;
 real f1=er*(real)sqrt(er-1);
 real f0=-(er-1)*kD;           

 real ue,uekD,f,x,delta,xa;
 int  it=0;

 x=x0;
 do {
   delta=(x1-x0)/(f0-f1)*f0;
   xa=x;
   x=x0+delta;
   ue=(real)sqrt(er-x*x);
   uekD=ue*kD;
   f=(-ue*(real)sin(uekD)+er*(real)(sqrt(x*x-1)*cos(uekD)));
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
  return((real)signaling_nan(0.)); /* Verfahren konvergiert nicht */
 }


 xa=sqrt(x*x-1)*kD;
 dp->res=(real)(1./
     ((sin(uekD)*(1+er*xa) + cos(uekD)*uekD*(1+er/xa))*x/ue));

 return((real)x);
}

/* Die Koeffizienten der Imaginaerteilentwicklung berechnen
 * dazu m"ussen Pol und Residuum bereits initialisiert sein!
 *
 * Die iterative Berechnung ist wegen Differenzen grosser Zahlen
 * nur f"ur kleine Ordnungen geeignet!
 */

static void imag_coefs(dp)
 diel dp;
{
 	/* Materialkonstanten und abgel. Gr. */
 real er  =dp->er;
 real kD  =dp->kD;
 real a   =kD*sqrt(er-1);
 real ca  =cos(a);
 real sa  =sin(a);
 real pol =dp->pol;
 real residue=dp->res;
 	/* Koeffizienten des Nennerpolynoms  */
 real tmp =(a*ca+sa);
 real A   =tmp*kD/2./a;
 real alf =er/kD*a*ca/tmp;
 real bet =2.*(1.-er)*sa/tmp;
	/* Pole der Partialbruchzerl. Darstellung */
        /* Z"ahlerkoeffizienten d. Zerl. */
 real zp00,zpa,zpb;

 /* Wegen der Differenzen grosser Zahlen */
 double q2,pa,pb,pa2,pb2;
 double I00,I1,I2,K,fak;

 real polpot;
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
  K*=(real)(m<<1)/(real)((m<<1)+1);
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
                           

diel init_diel(kD,er)
 real kD,er;
{
 diel dp=0;

 if ((dp=(diel)malloc(sizeof(diel_data)))==0) {
   /* no memory */
   error_diel(NO_MEM);
   return(0);
 }
 dp->kD=kD;
 dp->er=er;

 /* Polstelle und Residuum berechnen */
 dp->pol=Pol_Res_P1(dp);

 /* Koeffizienten der Reihenentwicklung berechnen */
 imag_coefs(dp);
 return(dp);
}

void destroy_diel(dp)
 diel dp;
{
 free((char*)dp);
}

 
/* Die berechneten Daten ausgeben */

void print_diel(dp)
  diel dp;
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
  real u,c,kw;
  diel dp;
  complex *term1,*term2;
{
 int m;
 real kr2=u*u+c*c,krpot=1.;
 double sum;



/* Berechnung des Imagin"arteils der Nahkopplung */

 sum=dp->im_nah_coefs[0];
 for (m=1;m<=IM_NAH_MAX; m++) {
   krpot*=kr2;
   sum+=krpot*dp->im_nah_coefs[m];
 }

 (*term1).re=-sum;
  
}
