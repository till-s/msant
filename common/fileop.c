#include  <math.h>
#include  <stdio.h>
#include  <unistd.h>

#include  "basics.h"
#include  "widglist.h"

#include "fileop.h"
#include "msant_strings.h"

/* must not be part of a number */
#define SEPCHAR	'%'
/* SKIPSEP contains SEPCHAR (too lazy to stringify) */
#define SKIPSEP(f) do { char buf[1000]; fscanf((f),"%%%[^\n\r]",buf); } while (0)

void PunkteLesen(FILE *feil, TextFieldTabEl *inputlist, WidTabEl *flaglist)
{
float x,y,speisdst;
char  numstr[50];
int scan,i,inputlist_len,segs;
Punkt     *neu;
Selection sel;
Zeichnung z;

z=(Zeichnung)inputlist[0].wid.wid_name;

 SKIPSEP(feil);

#ifdef DOPPELT
 fscanf(feil,"%lg",&(z->fak));
 fscanf(feil,"%lg",&x);
 fscanf(feil,"%lg",&y);
#else
 fscanf(feil,"%g",&(z->fak));
 fscanf(feil,"%g",&x);
 fscanf(feil,"%g",&y);
#endif

z->koppdist.x=(short)  irint(x*z->fak);
z->koppdist.y=(short) -irint(y*z->fak);

inputlist_len=0;
while(inputlist[++inputlist_len].wid.wid_name!=(char*)0);
for (i=1;i<inputlist_len;i++) {
  fscanf(feil,"%s",numstr);
  printmessg(0,numstr,inputlist[i].wid.t_wid);
}
getparams(inputlist);

SKIPSEP(feil);

while (flaglist->wid_name) {
	fscanf(feil,"%i",&i);
	setflag(flaglist->t_wid, i);
	flaglist++;
}

sel.selpoly=0; sel.selected=0;
if (z->borders.firstel!=0) 
  XtAppError(app,mWrnPGListNotEty);


do {
SKIPSEP(feil);
while ((scan=
#ifdef DOPPELT
             fscanf(feil,"%lg%lg%i%*[ \t]%lg",& x, & y,
#else
             fscanf(feil,"%g%g%i%*[ \t]%g",& x, & y,
#endif
                            & segs,
                            & speisdst)
       )>=3)  /* #npars */{
 CreatePoint(&z->borders,&sel,&neu);
 neu->ort.x=(short)irint(z->orig.x+x* z->fak);
 neu->ort.y=(short)irint(z->orig.y-y* z->fak);

 if (segs<0) { /* Stift ? */
  segs=-segs;
  neu->len=fabs(speisdst)*z->fak;

  if (speisdst<0.) { /* Speisender Stift ?*/
   speisdst=1.e-9;
  } else {
   speisdst=0.;
  }
 }

 neu->segs=segs;

 if (scan==4 && speisdst!= 0.) { /* kompat. mit alten Files */
   neu->speis=speisdst;
   z->speiseseg=neu;
 }
 }
 sel.selected=0;/*neues Polygon beginnen */}
while (scan != EOF);

ClosePath(&z->borders);
{extern void dumpWink();
dumpWink(&z->borders);
}
}

void PunkteSchreiben(FILE *feil, TextFieldTabEl *inputlist, WidTabEl *flaglist)
{
float x,y;
int i,inputlist_len;
Punkt * ptr;
char chpt[80];
Polygon *border;
Zeichnung z;

z=(Zeichnung)inputlist[0].wid.wid_name;
fprintf(feil,VERSIONFORMAT,VERSION);
fprintf(feil,"%c Parameters:\n",SEPCHAR);
fprintf(feil,"%g ",z->fak);
fprintf(feil,"%g ",((float)z->koppdist.x)/z->fak);
fprintf(feil,"%g ",-((float)z->koppdist.y)/z->fak);

inputlist_len=0;
while(inputlist[++inputlist_len].wid.wid_name!=(char*)0);

for (i=1;i<inputlist_len;i++) {
 getmessg(inputlist[i].wid.t_wid,chpt);
 fprintf(feil," %s",chpt);}
fprintf(feil,"\n");

fprintf(feil,"%c Checkbox status:\n", SEPCHAR);
while (flaglist->wid_name) {
	fprintf(feil," %i",getflag(flaglist->t_wid));
	flaglist++;
}
fprintf(feil,"\n");
 
for (border=(Polygon*)z->borders.firstel; border; 
     border=NEXTPOLY(border)){
ptr=PLIST(border);
if ((ptr!=NULL)) fprintf(feil,"%c\n",SEPCHAR);
while (ptr!=NULL) {
int segs;
float speis;

x=(ptr->ort.x-z->orig.x)/ z->fak;
y=(z->orig.y-ptr->ort.y)/ z->fak;
speis=ptr->speis;

segs=ptr->segs;
if (NEXT(ptr)==LAST(ptr)) {/*Stift?*/
 segs=-segs;   
    /* Wenn Punkt ein Stift, segmentzahl < 0 
     * und Radius an Stelle der Speisung schreiben 
     * Wenn speisender Stift, Radius < 0 schreiben
     */
 speis=ptr->len/z->fak;
 if (ptr->speis!=0.) speis=-speis;
}
fprintf(feil,"%15.8g %15.8g %4i", x,  y, segs);
if (speis!=0.) {
 fprintf(feil," %15.8g",speis);
}
fprintf(feil,"\n");
ptr=NEXT(ptr);
}
}
ftruncate(fileno(feil),(off_t)ftell(feil));
}

