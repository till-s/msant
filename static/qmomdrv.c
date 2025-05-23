#include "stack-c.h"
/******************************************
 * SCILAB function : qmom, fin = 1
 ******************************************/

int intsqmom(fname)
   char *fname;
{
 int m1,n1,l1,m2,n2,lr2,lc2,it2,l3,l4;
 CheckRhs(2,2);
 CheckLhs(1,2);
 /*  checking variable nu */
 GetRhsVar(1,"d",&m1,&n1,&l1);
 CheckScalar(1,m1,n1);
 /*  checking variable z */
 GetRhsCVar(2,"d",&it2,&m2,&n2,&lr2,&lc2);
 /* cross variable size checking */
 CreateVar(3,"d",&m2,&n2,&l3);/* named: w */
 CreateVar(4,"d",&m2,&n2,&l4);/* named: w1 */
 C2F(qmoms_wrap)(stk(l3),stk(l4),stk(l1),stk(lr2),stk(lc2),&it2,&m2,&n2);
 LhsVar(1)= 3;
 LhsVar(2)= 4;
 return 0;
}
