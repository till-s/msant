#include <stdio.h>
#if 0
#include <g++/std/typeinfo.h>
#endif

struct excp {~excp(){printf("~excp\n");};};

struct excpfi : public excp {};

main()
{
 try {
	throw new excpfi();
 } catch (excp*) {
	printf("caught\n");
 }
}
