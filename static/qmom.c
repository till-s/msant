#ifdef mexFunction	/* hack to detect scilab make */
#define IS_SCILAB
#endif

/* Compute the integral
 *
 *
 *     ~             -n  w  (n-1)                        -n   1   (n-1)  n-1
 *     I(n,w,p) = n w   /  x       log( x - p ) dx  = n w    /   w      t    log( w t - p ) w dt
 *                     0                                    0
 *
 *
 *                     1  (n-1)                               n |1       1  (n-1)
 *              = n   /  t       log( w t - p ) dt  = log(w) t  |   + n /  t      log(t - p/w) dt
 *                   0                                          |0     0
 *
 * The routine 'qmom' below computes the 'normalized' second term for both n and n+1:
 * so that the complete integral can be obtained by
 *
 *     ~
 *     In(n,w,p)  = log(w) + qmom(n, p/w)
 *
 * Different numerical techniques are used depending on the location of the integration
 * path with respect to the singularies t=-p/w and t=0 (if n>1).
 */

#ifdef IS_SCILAB
#include <stack-c.h>
#include <mex.h>
#endif

#include <math.h>
#include <complex.h>

#define NUM_GAUSS_WEIGHTS 20
#define EPS               (1.0E-8)

extern double dpsi_(double *);
extern void dgauss_(int *n, double *a, double *b, double *eps, double *zero, double *weight, int *ierr, double *work);
extern void drecur_(int *n, int *ipoly, double *al, double *be, double *a, double *b, int *ierr);
extern double d1mach_(int*);

typedef struct NuParmsRec_ {
	double nu;
	double nudigamma;
	double w[NUM_GAUSS_WEIGHTS];
	double t[NUM_GAUSS_WEIGHTS];
} NuParmsRec, *NuParms;

double eulergamma;
double pi;
double gausseps;

void qmoms_init()
{
double one = 1.0;
int    three = 3;
	eulergamma = dpsi_(&one);
	pi         = acos(-1.0);
	gausseps   = d1mach_(&three);
}

int
nuParmsInit(NuParms pp, double nu)
{
double     a[NUM_GAUSS_WEIGHTS];
double     b[NUM_GAUSS_WEIGHTS];
double     w[NUM_GAUSS_WEIGHTS];
int        n    = NUM_GAUSS_WEIGHTS;
double     al   = 0.;
double     be   = nu;
double     eps  = EPS;
int        type = 6; /* generate jacobi polynomials */
int        i,ierr;
int        rval = 0;

	pp->nu        = nu;
	pp->nudigamma = dpsi_(&nu);

	drecur_(&n, &type, &al, &be, a, b, &ierr);
	if ( ierr ) {
		printf("Warning: drecur ierr is %i\n",ierr);
		rval = -1;
	}

	/* adjust scale factor for interval 0..1 */
	b[0] = 1.0/(nu+1.0);

	/* compute weights and abscissae         */
	dgauss_(&n, a, b, &gausseps, pp->t, pp->w, &ierr, w);
	if ( ierr ) {
		printf("Warning: dgauss ierr is %i\n",ierr);
		rval = -1;
	}

	/* adjust abscissae for interval 0..1    */
	for ( i=0; i<n; i++ ) {
		pp->t[i] = (1.0+pp->t[i])/2.0;
	}

	return rval;
}



int
qmoms(NuParms prm, double *pInu, double *pInup1, double u, double c)
{
double complex p = u+I*c;
double         r = cabs(p);
double         t;
double         log1mp;
register double complex z;
register double complex Z = 1.;
register double complex Y = 0.;
register double complex d = 1.;
register double        nu = prm->nu;
register int            k = 0;
register double complex zz;

#ifdef DEBUG
	printf("qmoms(%g, %g + I %g)\n",nu,u,c);
#endif

	if ( r < 0.5 || fabs(c) < .01 && r < 0.6 ) {
		double   m = round(nu);
		double   n = 0;
		double dnu = nu-m; 
		double complex ptom;
		double complex R;

		if ( r == 0.0 ) {
			*pInu   = -1.0/nu;
			*pInup1 = -1.0/(nu+1.0);
			return 0;
		}

		/* hypergeometric series */
		while ( fabs(creal(d)) + fabs(cimag(d)) >  EPS ) {
			Z *= p;
			n  = n+1.0;
			if ( n != m ) {
				d  = Z/(n-nu);
				Y += d;
			} else {
				ptom = Z;
			}
		}
		if ( n < m ) {
			ptom = cpow(p,m);
		}
		if ( dnu == 0.0 ) {
			R = log(r) + I*pi; /* log(-p) */
		} else {
			R = (cpow(-p,dnu)*pi/sin(pi*dnu) - 1/dnu);
		}
		Y += R * ptom; /* this is nu*F so far */
		/* calculate (F-1)/nu */
		Y -= 1.0/nu;
	} else {

		z = 1.0/p;

		/* use hypergeometric series if |z| < 0.5 */
		if ( r >= 2.0 ) {
			/* hypergeometric series */
			while ( fabs(creal(d)) + fabs(cimag(d)) >  EPS ) {
				Z *= z;
				k++;
				d  = Z/(nu+k);
				Y += d;
			}
#ifdef DEBUG
			printf("Sum of %i terms (hypergeometric series): %g + I %g\n",k, creal(Y), cimag(Y));
#endif
			/* Y = Y*nu+1; would be the hypergeometric function but we need (F(1,nu,nu+1)-1)/nu */
		}
		/* use psi expansion if we get close to 1 */
		else if ( (t=cabs(zz=(1-z))) < 0.5 && nu < 100. && nu*t < 10 ) {
			/* psi expansion; doesn't work for nu too big, however but there the gauss integration should be OK */
			double dp = eulergamma - prm->nudigamma;
			double logr;
			double complex L;
			if ( t > EPS ) {
				/* higher terms which don't appear if we are very close to 1.0 */

				double          A = 1.;
				double         DP = dp;
				double       num1 = nu-1.0;
				double     num1dk; /* (nu - 1)/k */

				L = clog(zz);

				do {
					k++;
					num1dk = num1/k;
					A     *= num1dk+1.0;
					Z     *= zz;
					DP    += num1dk/(k+num1);
					d      = A*(DP-L)*Z;
					Y     += d;
				} while ( k < 100 && fabs(creal(d)) + fabs(cimag(d)) >  EPS );
				if ( k >= 500 )
					return -1;	/* no convergence */
			}
			/* again; nu*(sum + dlgamma(1) - dlgamma(n) - log(zz)) = 2F1(1,nu;nu+1;z) */
			Y += dp - 1.0/nu + I*pi;
			/* contains everything but the logarithm clog(zz); since we want it to cancel with clog(-zz) we add  I pi */
			logr    = log(r);
#ifdef DEBUG
			printf("PSI expansion yields: %g + I %g\n",creal(Y)+logr, cimag(Y));
#endif
			*pInu   = logr + creal(Y);
			/* we get the integral for nu+1 almost for free and it's needed by the caller anyways */
			*pInup1 = logr + creal(Y/z) - 1.0/(nu+1.0);
			if ( t > EPS ) {
				/* add log(1-z)*(z-1)/z only if we are not too close to 1 */
				*pInup1 -= creal( L*zz/z );
			}
			return 0;
		} else {
			/* gauss jacobi quadrature - problematic if z too close to 0..1 on real axis, however (0.5..1 is covered by psi exp) */
			for ( k=0; k<NUM_GAUSS_WEIGHTS; k++ ) {
				Y += prm->w[k] * 1./(p - prm->t[k]);
			}
		}
	}
	/* case where p is close to 1 doesn't end up here */
	log1mp  = creal(clog(1.0-p));
	*pInu   = log1mp + creal(Y);
	*pInup1 = log1mp + creal(p*Y) - 1.0/(nu+1.0); /* p*Y = Y/z */
	return 0;
}
	
	
#ifdef IS_SCILAB
	
void
C2F(qmoms_wrap)(double *pInu, double *pInup1, double *nu, double *u, double *c, int *iscmplx, int *M, int *N)
{
static int inited = 0;
NuParmsRec pp;
int        i;

	if ( !inited ) {
		inited=1;
		qmoms_init();
	}

	if ( nuParmsInit(&pp, *nu) )
		Scierror(999, "Computation of gauss weights/abscissae failed");

	for ( i=0; i< *N * *M; i++ ) {
		if ( qmoms(&pp, pInu+i, pInup1+i, u[i], *iscmplx ? (c[i]) : 0.) ) {
			Scierror(999, "Warning qmoms failed\n");
		}
	}
}

#endif
