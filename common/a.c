#include <math.h>
#include <stdio.h>

int main()
{
	double a,b,c;
	a = 1.0;
	b = 1.0E-6;
	c = a+b/3.0;
	printf("%g %g %g %g\n",a,b,c,(c-a)*3.0-b);
}
