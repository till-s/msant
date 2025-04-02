#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include "nrlib.h"

#define NR_END 1
#define FREE_ARG char*

void nrerror(char text[])
{
fprintf(stderr,"%s\n",text);
exit(1);
}

fcomplex *fc_vector(long nl, long nh)
{
 fcomplex *v;
 v=(fcomplex*)malloc((size_t)((nh-nl+1+NR_END)*sizeof(fcomplex)));
 if (!v) nrerror("allocation failure in fc_vector()");
 return v-nl+NR_END;
}

void     free_fc_vector(fcomplex *v, long nl, long nh)
{
 free((FREE_ARG)(v+nl-NR_END));
}

void fc_polint(float xa[], fcomplex ya[], int n, float x, fcomplex *y, fcomplex *dy)
{
 int i,m,ns=1;
 float deno,denp,dif,dift,ho,hp;
 fcomplex  w;
 fcomplex *c, *d;

 dif=fabs(x-xa[1]);
 c=fc_vector(1,n);
 d=fc_vector(1,n);
 for (i=1; i<=n; i++) {
   if ( (dift=fabs(x=xa[i])) <dif) {
	ns=i;
	dif=dift;
    }
    c[i]=ya[i];
    d[i]=ya[i];
 }
 *y=ya[ns--];
 for (m=1;m<n;m++) {
   for (i=1;i<=n-m;i++) {
	ho=xa[i]-x;
	hp=xa[i+m]-x;
	w=c[i+1];
	w.r-=d[i].r;
	w.i-=d[i].i;
	if ((deno=(ho-hp)) == 0.0) nrerror("Error in 'fc_polint()'");
	denp=hp/deno;
	d[i].r=w.r*denp;
	d[i].i=w.i*denp;
	deno=ho/deno;
	c[i].r=w.r*deno;
	c[i].i=w.i*deno;
   }
   *dy= ((2*ns)<(n-m)?c[ns+1]:d[ns--]);
   y->r+=dy->r;
   y->i+=dy->i;
 }
 free_fc_vector(d,1,n);
 free_fc_vector(c,1,n);
}


fcomplex fc_trapzd( void (*func)(fcomplex*,float*), float a, float b, int n)
{
 float x,tnm,del;
 fcomplex sum,fv1;
 static fcomplex s;
 int  it,j;
 
 if (n==1) {
	(*func)(&s,&a);
	(*func)(&fv1,&b);
        s.r+=fv1.r;
        s.i+=fv1.i;
	del=(b-a)*0.5;
	s.r*=del;
	s.i*=del;
	return s;
 } else {
	for (it=1,j=1;j<n-1;j++) it<<=1;
	tnm=it;
	del=(b-a)/tnm;
	x=a+0.5*del;
	for (sum.r=0.0, sum.i=0.0, j=1; j<=it; j++,x+=del) {
	  (*func)(&fv1,&x);
	  sum.r+=fv1.r;
	  sum.i+=fv1.i;
	}
	s.r=0.5*(s.r+del*sum.r);
	s.i=0.5*(s.i+del*sum.i);
	printf("TR (%g,%g)\n",s.r,s.i);
	return s;
 }
}

#define EPS	1.0e-6
#define EPS2	1.0e-12
#define JMAX	20
#define JMAXP	(JMAX+1)
#define K 	6

fcomplex fc_qromb( void(*func)(fcomplex*,float*),float a,float b)
{
 void fc_polint(float xa[],fcomplex ya[], int n, float x, fcomplex *y, fcomplex *dy);
 fcomplex fc_trapzd(void(*func)(fcomplex*,float*),float a, float b, int n);
 void nrerror(char text[]);

 float h[JMAXP+1];
 fcomplex s[JMAXP];
 fcomplex ss,dss;
 int j;

 h[1]=1.0;
 for (j=1; j<=JMAX; j++) {
   s[j]=fc_trapzd(func,a,b,j);
   if (j>=K) {
	fc_polint(&h[j-K],&s[j-K],K,0.0,&ss,&dss);
	if ((dss.r*dss.r+dss.i*dss.i) <= EPS2*(ss.r*ss.r+ss.i*ss.i)) return ss;
   }
   h[j+1]=0.25*h[j];
 }
 nrerror("Too many steps in fc_qromb\n");
 return ss; /* Never get here */
}
