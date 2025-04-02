#include <stdlib.h>
#include <math.h>
#include <ctype.h>

double atof();

float atof_(fl,str)
char *str;
float *fl;
{
 *fl=(float)atof(str);
}

int isyesorno_(str)
char *str;
{
int res;
int ch=(int)str[0];
res= isalpha(ch)?
 ((toupper(ch)=='Y')?1:-1) :
 0;
return res;
}


#ifdef NEVER
float bsj0_(arg)
float *arg;
{return(j0(*arg));}

float bsy0_(arg)
float *arg;
{return(y0(*arg));}

float bsj1_(arg)
float *arg;
{return(j1(*arg));}

float bsy1_(arg)
float *arg;
{return(y1(*arg));}
#endif
