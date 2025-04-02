#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <string.h>
#include "msant_strings.h"

char * msant_strings[N_MESSAGES];

int ReadStrings(raw)
 String raw;
{
int   i;
char *chpt;

chpt=(char*)raw;
i=0;

while (i<N_MESSAGES){
 msant_strings[i]=chpt;
 if ((chpt=strchr(chpt,2))==0) break;
 *chpt++=0;
 i++;
}

if ((i=N_MESSAGES-1-i)>0) return i;

return ((chpt==0)?0:-1);
}
