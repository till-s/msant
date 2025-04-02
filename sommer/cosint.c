#include <math.h>
#include <string.h>
#include <stdio.h>
#include "sommod.h"


/* Iterative Berechnung des Integrals
 *
 *  IC=Integrate[ Cos[r]/r /.r->Sqrt[u^2+c^2] , u]
 *
 * F"ur die Stammfunktion I(n) von r^n du kann eine Rekursions-
 * formel angegeben werden:
 * 
 *  I(n)=1/(n+1) ( u r^n + c^2 n I(n-2) )
 * 
 * Diese Formel wird in die Reihenentwicklung von cos(x)/x
 * eingesetzt; dadurch ergibt sich
 *
 *  IC=Sum[a(n),{n,0,Infinity}]
 *
 *  a(n+1)= 1/(2n + 2)^2 ( u b(n+1) - c^2 a(n) )
 *  a(0)  = ArcSinh[u/c], bzw. Log[u+Sqrt[u^2+c^2]]=Log[u+r]
 * 
 *  b(n+1)= - r^2 / (2n) / (2n+1) b(n)
 *  b(1)  = - r
 *
 * Zur Berechnung des Integrals 
 * 
 *  IS=Integrate[ Sin[r]/r /.r->Sqrt[u^2+c^2], u]
 * 
 * modifizieren sich die Rekursionsformeln wie folgt
 *
 *  a(n+1)= 1/(2n+3)^2 (u b(n+1) - c^2 a(n))
 *  a(0)  = u
 *
 *  b(n+1)= -r^2 / (2n+2) / (2n+1) b(n)
 *  b(1)  = -r^2/2
 *
 ********************************************************
 *
 * Das Doppelintegral 
 *
 *  Integrate[Cos[r]/r /.r->Sqrt[z^2+h^2+c^2], u,z]
 *
 * wird nach der selben Methode berechnet, wobei sich die Rekursions-
 * formeln
 *
 *  b(n+1)  = -r^2/(2n+1)/(2n) b(n)
 *  b(1)    =-r
 * 
 *  au(n+1) = 1/(2n+2)^2 ( u b(n+1) - (z^2+c^2) au(n) )
 *  au(0)   = ln(u+r)
 *
 *  az(n+1) = 1/(2n+2)^2 ( z b(n+1) - (u^2+c^2) az(n) )
 *  az(0)   = ln(z+r)
 *
 *  auz(n+1)= 1/(2n+3) ( z au(n+1) + u az(n+1) - c^2/(2n+2) auz(n) )
 *  auz(0)  = -c ArcTan[ z u / (c r) ] + u az(0) + z au(0)
 *
 * ergeben. Der Startwert auz(0) ber"ucksichtigt die Tatsache, dass
 * die von der Reihe berechnete Stammfunktion stets an den vier Grenz-
 * punkten der Integrationsintervalle ausgewertet wird, und daher einige
 * Terme verschwinden m"ussen.
 */

typedef float real;
typedef struct complex_{
	real re,im;
} complex;

#define EPS  1.0E-8
#define EPS2 1.0E-16
#define MAXIT  30	/* maximale Iterationen beim Polstellensuchen */

complex CIntd(u,z,c)
 real u,z,c;
{
 register real c2  = c*c;
 register real z2  = z*z;
 register real u2  = u*u;
 register real r   = (real)sqrt(u2+c2+z2);
 register real b   =-r;
 register int  n=1,n2=1;

 /* Startwerte f"ur die Cosinusreihe */
 register real Cau = (real)log(u+r);
 register real Caz = (real)log(z+r);
 register real Cauz=- c*(real)atan2((double)(u*z),(double)(c*r))
		    + u*Caz + z*Cau;
 register double Csum=Cauz;

 /* Startwerte f"ur die Sinusreihe */
 register real Sau = u;
 register real Saz = z;
 register real Sauz= u*z;
 register double Ssum=Sauz;

 complex  rval;

 u2+=c2;
 z2+=c2;
 do {
  n2+=(n<<1)+1;
  n++;
  Cau=(u*b-z2*Cau)/n2;
  Caz=(z*b-u2*Caz)/n2;
  Cauz=(z*Cau+u*Caz-c2*Cauz/n)/(n+1);
  b*=r/n;
  n2+=(n<<1)+1;
  n++;
  Sau=(u*b-z2*Sau)/n2;
  Saz=(z*b-u2*Saz)/n2;
  Sauz=(z*Sau+u*Saz-c2*Sauz/n)/(n+1);
  b*=-r/n;
  Csum+=Cauz;
  Ssum+=Sauz;
 } while ((fabs(Sauz)>EPS*fabs(Ssum)) ||
 	  (fabs(Cauz)>EPS*fabs(Csum)));

#ifdef DEBUG
 printf("(u*Caz-c2*Cauz/n)/Cau=%g\n", (Cauz*(n+1)/Cau-z));

 printf(" u: %g, c: %g , z: %g CInt --> %g (%i-Iterations to eps= %g)\n",
       (float)u,(float)c,(float)z,(float)Csum,(int)(n/2),EPS);
#endif

 rval.re=(real)Csum;
 rval.im=(real)Ssum;
 return(rval);
}
  

real CInt(u,c)
 real u,c;
{
 register real c2=c*c;
 register real r2=u*u+c2;
 register real b=-sqrt(r2); /* b(1) */
 register real a=log(u-b);  /* ln(u+sqrt(u^2+c^2)) */
 register double sum=a;
 register int  n=0,n2=0;
 do{
  n2+=(n<<2)+4;
  n+=2;
  a=(u*b-c2*a)/n2;
  b*= -r2/(n2+n);
  sum+=a;
 } while (fabs(a)>EPS*fabs(sum));
#ifdef DEBUG
 printf(" u: %g, c: %g CInt --> %g (%i-Iterations to eps= %g)\n",
       (float)u,(float)c,(float)sum,(int)(n/2),EPS);
#endif
 return((real)sum);
}

real SInt(u,c)
 real u,c;
{
 register real c2=c*c;
 register real r2=u*u+c2;
 register real b=-r2/2.;     /* b(1) */
 register real a=u;
 register double sum=a;
 register int  n=1,n2=1;
 do{
  n2+=(n<<2)+4;
  n+=2;
  a=(u*b-c2*a)/n2;
  b*= -r2/(n2+n);
  sum+=a;
 } while (fabs(a)>EPS*fabs(sum));
#ifdef DEBUG
 printf(" u: %g, c: %g SInt --> %g (%i-Iterations to eps= %g)\n",
       (float)u,(float)c,(float)sum,(int)(n/2),EPS);
#endif
 return((real)sum);
}

char  *seps=" ,\t";

char *ftab[]={
#define CINT  0
 "CInt","   u,c",
#define SINT  1
 "SInt","   u,c",
#define CINTD4 2
 "CIntd4"," u,h,c",
#define PLOTC 3
 "Plotc","",
#define QUIT  4
 "Quit","",
#define HELP  5
 "Help","",
#define TIME  6
 "Time","",
#define SOMN  7
 "Somn","   w/2,D,c,er",
#define PROX  8
 "Prox","   w/2,c",
#define SOMPROX 9
 "SomProx","w/2,D,c,er",
#define CINTD 10
 "CIntd","  u,h,c",
#define POL   11
 "Pol","    kD, er",
#define IMNAH 12
 "Imnah","  u,c",
 0,
};

void use()
{
int i=0;
while (ftab[i]) {
 fprintf(stderr,"%s %s\n",ftab[i],ftab[i+1]);
 i+=2;
}
}

int getreal(x,strpt)
 real *x;
 char **strpt;
{
 char *rval;
 if (rval=*strpt) {
  *x=(real)atof(*strpt); 
  *strpt=strtok(0,seps);
 } else {
  fprintf(stderr,"missing parameters\n");
 }
 return((int)rval);
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

real Pol_Res_P1(kD,er,res)
 real kD,er;
 real *res;
{
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

 /* das Residuum von 1/N[x] bestimmen, falls ein 
  * Zeiger "ubergeben wurde
  */

 if (res) {
  xa=sqrt(x*x-1)*kD;
  *res=(real)(sin(uekD)*(1+er*xa) + cos(uekD)*uekD*(1+er/xa))*x/ue;
 }

 return((real)x);
}
 

main(argc,argv)
 int argc;
 char *argv[];
{
int i,eof=0;
real  u,c,h,er;
char  fu[200],*chpt;
diel dp=0;
complex rval1,rval2;

use();
fu[0]=0;
while (eof!=EOF) {
fprintf(stderr,"Func args?\n-- ");
eof=scanf("%[^\n]",fu),scanf("%*c"); /* \n auch "uberlesen, wenn fu leer */

i=0;
chpt=strtok(fu,seps);
while(ftab[i]) {
 if (strcmp(fu,ftab[i])==0) break;
 i+=2;
}
i/=2;
chpt=strtok(0,seps);
switch (i) {

 case HELP: use(); break;

 case CINT: 

        if(getreal(&u,&chpt) && getreal(&c,&chpt)) {
	 printf("CInt(u,c)-CInt(0,c)=%g\n",(float)(CInt(u,c)-CInt((real)0,c)));
        }
	break;
 case SINT: 
        if(getreal(&u,&chpt) && getreal(&c,&chpt)) {
	 printf("SInt(u,c)-SInt(0,c)=%g\n",(float)(SInt(u,c)-SInt((real)0,c)));
 	}
	break;

 case CINTD4:
 case CINTD:
        if(getreal(&u,&chpt) && getreal(&h,&chpt) && getreal(&c,&chpt)) {
	 if (i==CINTD4) {
 	 printf("CI(u,h,c)+CI(0,0,c)-CI(u,0,c)-CI(0,h,c)=%g\n",
  		(float)(CIntd((real)u,(real)h,(real)c).re+
  	  		CIntd((real)0,(real)0,(real)c).re-
  	  		CIntd((real)u,(real)0,(real)c).re-
  	  		CIntd((real)0,(real)h,(real)c).re));
 	 printf("SI(u,h,c)+SI(0,0,c)-SI(u,0,c)-SI(0,h,c)=%g\n",
  		(float)(CIntd((real)u,(real)h,(real)c).im+
  	  		CIntd((real)0,(real)0,(real)c).im-
  	  		CIntd((real)u,(real)0,(real)c).im-
  	  		CIntd((real)0,(real)h,(real)c).im));
	}else{
	 complex rval;
	 rval=CIntd(u,h,c);
	 printf("CI(u,h,c) = %g + j %g\n",(float)rval.re,(float)rval.im);
	}
	}
	break;

 case PROX:
        if(getreal(&u,&chpt) && getreal(&c,&chpt)) {
 	 printf("(I(u,c)-I(-u,c))/ (2 u Cos(c)/c ) = %g\n",
          	 (CInt((real)u,(real)c)-CInt((real)-u,(real)c))/2/u/cos(c)*c);
	}
 	break;
#ifdef NEVER
 printf("{\n");
 for( i=1; i<100; i++) {
  c+=0.05;
  printf("%f,\n",
          (CInt((real)u,(real)c)-CInt((real)-u,(real)c))/2/u/cos(c)*c - 1);
 }
 printf("}\n");
#endif
 case SOMN:
 case SOMPROX:

        if(getreal(&u,&chpt) && getreal(&h,&chpt) &&
	   getreal(&c,&chpt) && getreal(&er,&chpt)) {

	    double somre=0;
            real   incr;
	    real qu=(1-er)/(1+er),qun=1;
	    real kD=h;
	    int n=0;
	    
	    if (i==SOMN) {
	     qun=1;
	     kD=h;
	    }else{
	     qun=qu;
	     kD=2*h;
	    }

            h*=2.;
	    do {
            if (i==SOMN) {
	     incr=qun*(CIntd(u,kD,c).re-CIntd(-u,kD,c).re);
            } else {
             real cc=(real)hypot(c,kD);
	     incr=qun*(CInt(u,cc)-CInt(-u,cc));
            }
	     qun*=qu;
	     somre+=incr;
	     kD+=h;
	     n++;
	    } while (fabs(incr/somre)>EPS);
            if (i==SOMN) {
	    somre=2.*((CIntd(-u,(real)0.,c).re-CIntd(u,(real)0.,c).re)+(1-qu)*somre);
	    }else{
            somre=2.*h*((CInt(u,c)-CInt(-u,c))/2.+somre);
	    }

	    printf("Re[Somnah]=%lg, %i iterations\n",somre,n);
	    }
	    break;

 case IMNAH:
	   if(dp==0) {
 	    fprintf(stderr,"Kein Dielektrikum--> erst Pol\n");
 	    break;
	   }
  	   if (getreal(&u,&chpt) && getreal(&c,&chpt)) {
	    somkop_nahe(u,c,dp,(real)0,&rval1,&rval2);
            print_diel(dp);
            printf("Imag.teil: %g \n",(float)-rval1.re);
	   }
	   break;

 case POL:
           if(getreal(&h,&chpt) && getreal(&er,&chpt)){
	    if (dp) {
	      destroy_diel(dp);
              dp=0;
            }
            dp=init_diel(h,er);
            print_diel(dp);
	   }
	   break;

 case TIME: printf("1E4 Auswertungen CIntd:..");
	    for (i=1;i<10000;i++){ 
		(void)CIntd((real)0.1,(real)0.1,(real)1);
	    }

 case QUIT: exit(0);
 default  : use(); break;
}
}
}
