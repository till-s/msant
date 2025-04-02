#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>

#include "nrlib.h"
extern void kern_(fcomplex* res,float * s);

extern struct {
 fcomplex a;
} global_;

void main(int argc, char* argv[])
{
fcomplex res,res1;
float	 a,b;
printf("Integration of E^(a t) from 0 to 100\n");

if (argc!=3) {
 printf("usage: expint Re{a} Im{a}\n");
 exit(1);
}
global_.a.r=atof(argv[1]);
global_.a.i=atof(argv[2]);
a=0.; b=100.;
res=fc_qromb(kern_,0.,100.);
printf("Integral= (%g, %g)\n",res.r,res.i);
}



