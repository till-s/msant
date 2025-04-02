#include <math.h>
#include <stdio.h>
#include "util.h"

#define PI 3.141592654

void use()
{
printf("usage: hint er kd kr \n");
}

main(argc,argv)
 int argc;
 char *argv[];
{
double dc,ds;
real   er,kd,kr,kr2,krpot,tmp;
int    i,n,n2;
complex lIe,lIo,sume,sumo,a,fi,la2,rval;

if (argc<4) {
 use(); exit(0);
}

i=1;
sscanf(argv[i],"%g",&er);i++;
sscanf(argv[i],"%g",&kd);i++;
sscanf(argv[i],"%g",&kr);i++;

kd+=kd;


a.re=PI/kd;
a.im=log1p(-2/(er+1))/kd; /* ln((er-1)/(er+1)) */

la2.re=1-(a.re*a.re-a.im*a.im);
la2.im= -2.*(a.re*a.im);

tmp=a.re*a.re+a.im*a.im+1;

fi.re=atanh((tmp-sqrt(tmp*tmp-4*a.re*a.re))/2/a.re);
tmp-=2.;
fi.im=atan ((tmp+sqrt(tmp*tmp+4*a.im*a.im))/2/a.im);

lIe=fi;
lIo=a;
krpot=1.; kr2=-kr*kr;
n=0; n2=1;  /* n, (n+2)^2 */

sume=lIe;
sumo=lIo;

do {
  
 krpot*=kr2;

 n++;
 n2+=(n<<1)+1;
 
 tmp   =(a.re/n + la2.re*lIe.re-la2.im*lIe.im)/n2;
 lIe.im=(a.im/n + la2.re*lIe.im+la2.im*lIe.re)/n2;
 lIe.re=tmp;

 sume.re+=krpot*lIe.re;
 sume.im+=krpot*lIe.im;

 n++;
 n2+=(n<<1)+1;

 tmp   =(a.re/n + la2.re*lIo.re-la2.im*lIo.im)/n2;
 lIo.im=(a.im/n + la2.re*lIo.im+la2.im*lIo.re)/n2;
 lIo.re=tmp;

 sumo.re+=krpot*lIo.re;
 sumo.im+=krpot*lIo.im;

}while (n<15);

rval.re=sume.re+kr*sumo.im;
rval.im=sume.im-kr*sumo.re;

printf("Summe: ( %g %cj %g )\n",PC(rval));
}



