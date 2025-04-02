/* this is implementation-dependent */
/* check the defined values for your hardware */

/*
 * 2^LD_BITS = wordlength of an integer (in bits)
 *
 * The code does _NOT_ work, if this wordlength is not
 * a power of two.
 *
 */

#define LD_BITS    5
#define MASK_BITS  ((1<<LD_BITS)-1)

void fill0_(bitmap,np)
int      bitmap[];
unsigned *np;
{
 register int w,bit;
 register unsigned n=*np;
 w=0;
 while ( w < (n>>LD_BITS) ) {
  bitmap[w]=0;
  w++;
 }
 if (0!=(bit= (n & MASK_BITS))) 
  bitmap[w] &= ( (-1) << bit );
}

void mark1_(bitmap,np)
int      bitmap[];
unsigned *np;
{
 register unsigned n=(*np-1);
 bitmap[ n>>LD_BITS ] |= (1 << (n&MASK_BITS));
}

int iget_(bitmap,np)
int      bitmap[];
unsigned *np;
{
 register unsigned n=(*np-1);
 return bitmap[ n>>LD_BITS ] & (1 << (n&MASK_BITS));
}
