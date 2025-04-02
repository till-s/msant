#include <math.h>
typedef struct{ float re,im; } complex;

void usage()
 {
 printf("usage: nf er kD\n");
 }
 
void fdump_()
{
 printf("error in catanh\n");
}

main(argc,argv)
int argc;
char *argv[];
{
 float kD,er,ampl;
 complex la1,fi1;
 if (argc<3) {
  usage(); exit(1);
 }
 er=(float)atof(argv[1]);
 kD=(float)atof(argv[2]);
 calc_params_(&er,&kD,&ampl,&la1,&fi1);
 printf(" er: %g, kD: %g, ampl: %g, la1: (%g + j %g), fi1: (%g + j %g)\n",
          er,kD,ampl,la1.re,la1.im,fi1.re,fi1.im);
}
