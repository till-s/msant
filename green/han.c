#include <math.h>
#include <malloc.h>
#include <stdio.h>

void usage(char*name)
{
fprintf(stderr,"usage: %s alfa.re alfa.im t0 tend rtol atol krho er\n",
	name);
}


void hinit_();
void derivs_();

void main(int argc, char **argv)
{
  int	info[15],idid,lrw,*iwork,liw,ipar[1];
  int   neqs=4,i;
  float rpar[4],*rwork,y[16];
  float t,tend,tol,abtol;

  if (argc<9) {usage(argv[0]); exit(1);}
  sscanf(argv[1],"%g",rpar);
  sscanf(argv[2],"%g",rpar+1);
  sscanf(argv[3],"%g",&t);
  sscanf(argv[4],"%g",&tend);
  sscanf(argv[5],"%g",&tol);
  sscanf(argv[6],"%g",&abtol);
  sscanf(argv[7],"%g",rpar+2);
  sscanf(argv[8],"%g",rpar+3);

  liw=33;
  lrw=33+7*neqs;
  rwork=(float*)malloc(lrw*sizeof(float));
  iwork=(int*)malloc(liw*sizeof(int));
  info[0]=0; /* 1st call for this problem */
  info[1]=0; /* abtol,tol are scalars */
  info[2]=0; /* solution only at tend wanted */

  hinit_(&t,y,rpar);
  derkf_(derivs_,&neqs,&t,y,&tend,info,&tol,&abtol,&idid,
	rwork,&lrw,iwork,&liw,rpar,ipar);
  free((char*)iwork); free((char*)rwork);
  if (idid!=2) {
	fprintf(stderr,"ERROR in pathint: derkf terminates with idid==%i\n",idid);
  }
{ int i;
  printf("calc:\n");
  for (i=0;i<4;i+=2) 
	printf("%g %cj %g\n",y[i],(y[i+1]<0.?'-':'+'),fabs(y[i+1]));
  t=2.;
  hinit_(&tend,y,rpar);
  printf("direct:\n");
  for (i=0;i<4;i+=2) 
	printf("%g %cj %g\n",y[i],(y[i+1]<0.?'-':'+'),fabs(y[i+1]));
}
 printf("integrierter Fehler %g\n",y[4]);
}
