#ifndef functab_h
#define functab_h
#define MAXTAB 1024
#define TABSEP 16.0

extern complex H1tab[],H0tab[];
/* Die Tabellen enthalten H0=Y0 + j J0
                          H1=Y1 + j J1
 */

/* Tabelleninterpolation */

#define INTERPOL(mat,tab,kr) {\
  register int index; register float delta=kr*TABSEP;\
  index=(int)delta; delta-=index;\
  mat.re=(tab[index].re*(1. - (delta))+ tab[index+1].re*delta);\
  mat.im=(tab[index].im*(1. - (delta))+ tab[index+1].im*delta);\
 }

#define INTERPOL2(matre,matim,tab,kr,fak) {\
  register int index; register float delta=kr*TABSEP;\
  index=(int)delta; delta-=index;\
  matre=fak*(tab[index].re*(1. - (delta))+ tab[index+1].re*delta);\
  matim=fak*(tab[index].im*(1. - (delta))+ tab[index+1].im*delta);\
 }

#define LINTERPOL(mat,tab,kr) {\
  register int index; register float delta=kr*TABSEP;\
  index=(int)delta; delta-=index;\
  mat=(tab[index].re*(1. - (delta))+ tab[index+1].re*delta);\
 }

/* Stammfunktion der Neumannfkt. f"ur Argumente <=1 */
#define STAMM(u,u2,c,c2) \
 (-atan2pi(u,c)*(2. - c2*(1. -c2/12.))+ \
  c*u*(log(u2+c2)*(0.159155 - 0.0198944 * c2 - 0.00663146 * u2)\
        -0.514367 + 0.0808745 * c2 +0.0225372 * u2))
       
#define SPEISE(u,u2,c,c2) \
 (atan2pi(u,c)*c*(2. - c2/3.)+\
  u*(log(u2+c2)*(0.31831 - 0.0265258*(3.*c2+u2))\
     -0.710424 + 0.283709 * c2 + 0.0768859 * u2))
#endif
