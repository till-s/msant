#include <mex.h> 
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc intsqmom;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,intsqmom,"qmom"},
};
 
int C2F(libqmomdrv)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
