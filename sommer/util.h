#ifndef UTILS_H
#define UTILS_H
#include <math.h>
#include <stdio.h>

typedef float real ;
typedef  struct complex_{
  real re,im;
} complex;

/* use this for 'printf' */
#define CVF(x) ((float)(x))

/* Drucken komplexer Zahlen mit printf("(%g %cj %g)",PC(z)) */

#define PC(cm)((float)(cm).re),signbit((cm).im)?'-':'+',(float)fabs((cm).im)


#endif
