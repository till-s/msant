#ifndef DIVFUNCS_H
#define DIVFUNCS_H

#include <math.h>

#define irint(x) ((int)rint(x))
#ifndef signbit
#define signbit(x) (x<0.)
#endif
#define atan2pi(y,x) (atan2(y,x)/PI)

#endif
