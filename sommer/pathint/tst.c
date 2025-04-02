#include <stdio.h>
#include <math.h>
#include <float.h>

#ifdef NEVER
float r1mach_();
#endif

float cts[]={FLT_MIN,FLT_MAX,0,FLT_EPSILON,0};

main()

{
int i;
float x=0.5;
printf("%g\n",(float)j0(x));
exit(0);
#ifdef NEVER
cts[2]=FLT_EPSILON/FLT_RADIX;
cts[4]=log10(FLT_RADIX);
 for (i=1; i<6; i++) {
  printf(" %g, %g\n",r1mach_(&i),cts[i-1]);
 }
#endif
}
