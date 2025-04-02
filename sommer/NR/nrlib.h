#ifndef _NRLIB_H_
#define _NRLIB_H_

#ifndef _FCOMPLEX_DECLARE_T
#define _FCOMPLEX_DECLARE_T
typedef struct FCOMPLEX {float r,i;} fcomplex;
#endif

#include <stdio.h>


void		fc_polint(float xa[], fcomplex ya[], int n, float x, fcomplex *y, fcomplex *dy);


fcomplex	fc_trapzd(void (* func)(fcomplex*,float*), float a, float b, int n);
fcomplex	fc_qromb(void  (* func)(fcomplex*,float*), float a, float b);

fcomplex	*fc_vector(long nl, long nh);
void     	free_fc_vector(fcomplex *v, long nl, long nh);


#endif
