#include <stdio.h>
#include <malloc.h>

#include "../common/basics.h"
#include "sommod.h"

void casl_();

char *Malloc(s)
int s;
{
return malloc(s);
}

void Free(pt)
char *pt;
{
free(pt);
}

char *Realloc(addr,s)
char *addr;
int s;
{
return realloc(addr,s);
}

void usage(name)
char *name;
{
fprintf(stderr,"usage: %s krho eps kD\n",name);
fprintf(stderr,"	[y/n] refers to numerically integrating Hankel fns\n");
}

static float kr;

float krho(i)
int i;
{
 return kr+0.1*i;
}


void main(argc,argv)
int argc;
char **argv;
{
float   eps,kD;
int	i,size;
float	end;
float	y[16],sfw[4];
Diel	dp;
complex *Gtab,*G1tab;

if (argc<4) {usage(argv[0]); exit(1);}
sscanf(argv[1],"%g",&kr);
sscanf(argv[2],"%g",&eps);
sscanf(argv[3],"%g",&kD);
end=7.;
dp=init_diel(kD,eps);
dielGetChunk(dp,1024,krho);
Gtab=dielGtab(dp);
G1tab=dielG1tab(dp);
SFW(sfw,dp,kr);

/*
for (i=0;i<dielGtabSize(dp);i++)  {
   kr=krho(i);
   casl_(Gtab+i,G1tab+i,&kr);
	printf("(%g %cI %g) ",Gtab[i].re,(Gtab[i].im<0.?'-':'+'),fabs(Gtab[i].im));
	printf("(%g %cI %g)\n",G1tab[i].re,(G1tab[i].im<0.?'-':'+'),fabs(G1tab[i].im));
}
*/
destroy_diel(dp);
}
