#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>

#include "Malloc.h"

#define DEBUG

#ifdef DEBUG
#define MAXTR 2048


#ifdef DEBUG
char * trace[MAXTR+1];
#endif

void inittrace()

{
int i;

#ifdef DEBUG
for (i=0; i<=MAXTR; i++) trace[i]=0;
#ifdef SUNOS
malloc_debug(2);
mallopt(M_MXFAST,64);
mallopt(M_NLBLKS,100);
mallopt(M_GRAIN,8);
#endif
#endif
}

void count(w,p1,p2)
Widget w;
XtPointer p1,p2;

{
int i,j;
struct mallinfo minfo;


#ifdef DEBUG
j=0;
for (i=1; i<=MAXTR; i++) if (trace[i]) j++;
fprintf(stderr,"COUNT: %i\n",j);
#ifdef SUNOS
minfo=mallinfo();
printf("%8i arena;	/* total space in arena */\n",minfo.
arena);
printf("%8i ordblks;	/* number of ordinary blocks */\n",minfo.ordblks);
printf("%8i smblks;	/* number of small blocks */\n",minfo.smblks);
printf("%8i hblks;	/* number of holding blocks */\n",minfo.hblks);
printf("%8i hblkhd;	/* space in holding block headers */\n",minfo.hblkhd);
printf("%8i usmblks;	/* space in small blocks in use */\n",minfo.usmblks);
printf("%8i fsmblks;	/* space in free small blocks */\n",minfo.fsmblks);
printf("%8i uordblks;	/* space in ordinary blocks in use */\n",minfo.uordblks);
printf("%8i fordblks;	/* space in free ordinary blocks */\n",minfo.fordblks);
printf("%8i keepcost;	/* cost of enabling keep option */\n",minfo.keepcost);
printf("%8i mxfast;	/* max size of small blocks */\n",minfo.mxfast);
printf("%8i nlblks;	/* number of small blocks in a holding block */\n",minfo.nlblks);
printf("%8i grain;	/* small block rounding factor */\n",minfo.grain);
printf("%8i uordbytes;	/* space (including overhead) allocated in ord. blks */\n",minfo.uordbytes);
printf("%8i allocated;	/* number of ordinary blocks allocated */\n",minfo.allocated);
printf("%8i treeoverhead;/* bytes used in maintaining the free tree */\n",minfo.treeoverhead);
printf("\nVERIFY: %i\n",malloc_verify());
#endif
#endif
}


char * Malloc(size)

int size;

{
char * addr;
int i,j;

addr=malloc(size);

#ifdef DEBUG
trace[0]=addr;

j=MAXTR;
/* freien Eintrag suchen */
while(trace[j]&&j) j--;
if (j) trace[j]=addr; else {
 fprintf(stderr,"TRACE voll");
 exit(-1);
}
#endif
return(addr);
}

char *Realloc(addr,size)
char *addr;
int  size;
{
char *neu=realloc(addr,size);
#ifdef DEBUG
register int j;
if (addr!=neu) {
 j=MAXTR;
 trace[0]=addr;
 while(trace[j]!=addr)j--;
 trace[j]=neu;
 if (j==0) {
   fprintf(stderr,"addr to realloc not in trace\n");
   exit(-1);
 }
}
#endif
return neu;
}

void Free(addr)

char *addr;

{
char str[80];
int j;

free(addr);

#ifdef DEBUG
j=MAXTR;
trace[0]=addr;
while(trace[j]!=addr)j--;
trace[j]=0;
if (j==0) {
 fprintf(stderr,"addr to free not in trace\n");
 exit(-1);
}
#endif
}
#else

char *Malloc(size)
 int size;
{return XtMalloc(size);}

void Free(addr)
 char *addr;
{
 XtFree(addr);
}

char *Realloc(addr,size)
char *addr;
int  size;
{
return realloc(addr,size);
}
#endif
