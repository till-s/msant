#include <stdio.h>
#include "fft.h"
#include "util.h"

extern double drand48();
main(argc,argv)
 int argc;
 char *argv[];
{
int i,dim,pot,j;
complex *arr;
complex *W;
float   *Wr,*cpy;
float err=0.,sum=0.;
real  dreh;
FILE *feil=fopen("feil","w");

if (argc<2) {
 printf("usage: tst ld(dimension) [IMSL]\n");
 exit(0);
}

sscanf(argv[1],"%i",&pot);

if((pot<2)||(pot>20)) {
 printf("dim: %i out of range (2..20)\n",pot);
 exit(1);
}
dim=1<<pot;

arr=(complex*)malloc(sizeof(complex)*dim);
if (arr==0) {
  printf("no memory (arr[])\m");
  exit(1);
}

printf("filling with random numbers...\n");
for (i=0;i<dim;i++) {
  arr[i].re=(real)drand48();
  arr[i].im=(real)drand48();
}
printf("...done!\n");

printf("FFTinit...\n");
if (argc<3) {
W=fft_init(pot,0);
if(W==0) {
  printf("no memory (W[])\m");
  free(arr);
  exit(1);
}
} else {
 Wr=(float*)malloc(sizeof(float)*(4*dim+15));
 cpy=(float*)malloc(sizeof(float)*(dim*2));
 fftci_(&dim,Wr);
}
printf("...done!\n");

printf("FFT...");
if(argc<3) {
printf("using fft()\n");
fft(pot,arr,arr,0);
}else{
printf("using IMSL\n");
f2tcf_(&dim,arr,arr,Wr,cpy);
free(Wr); free(cpy);
}
printf("...done!\n");

#ifdef MATH
fprintf(feil,"{\n");
for (i=0;i<dim;i++) {
 float tmp;
 tmp=arr[i].re/dim-arr2[i].re;
 err+=tmp*tmp;
 tmp=arr[i].im/dim-arr2[i].im;
 err+=tmp*tmp;
 sum+=arr2[i].re*arr2[i].re+arr2[i].im*arr2[i].im;
 fprintf(feil,"%f,\n",arr[i].re);
/*
 printf("%g +I %g          ",arr[i].re/dim,arr[i].im/dim);
 printf("%g +I %g\n",arr2[i].re,arr2[i].im);
*/
}
fprintf(feil,"}\n");
fclose(feil);
#endif
/*
printf("%g %g\n",err/dim,sum/dim);
*/
}
