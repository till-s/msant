#ifndef Malloc_h
#define Malloc_h


/* to look for memory leaks Malloc.c can be compiled and linked in */
void inittrace();
void  count();
char* Malloc();
void  Free();
char* Realloc();

#endif
