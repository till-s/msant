#include <math.h>
#include <ctype.h>
#include <stdio.h>

#include "basics.h"
#include "widglist.h"
#include "graf.h"
#include "calc.h"
#include "CBstrom.h"
#include "feld.h"

#include "msant_strings.h"

void Act_Segment(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
Zeichnung p1;
Selection *sel;
p1=zeichnung;

if (p1->mode!=none) {XBell(disp,RING);return;}
if (noselection(p1)) {
 printmessg(1,mNoSelection,w);
 return;
}
 storeparam(input_haupt,TXTFLD_NSEGS,p1);
 printmessg(0,0,outputlist[MSSG_M].t_wid);
 for (sel=(Selection*)p1->sel.firstel; sel; sel=NEXTSEL(sel)){
  if (!ISSTIFT(SELECTED(sel))) {
   SELECTED(sel)->segs=p1->nsegs;
  } else {
   SELECTED(sel)->segs=1;
  }
 }
 modified(p1,False);
 ClearScr();
 drawpatch(p1);
 drawlist(p1,-1,-1);
 drawpoints(p1);
 drawnums(p1);
}

void Act_SegmentAll(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
Zeichnung p1;
Punkt *ptr;
Polygon *border;

p1=zeichnung;
if (p1->mode!=none) {XBell(disp,RING);return;}
if (p1->borders.firstel==0) return;

/* Parameter neu einlesen; daten werden sowieso ung"uldig */
 getparams(input_haupt);
 storeparam(input_haupt,TXTFLD_NSEGS,p1);
 for (border=(Polygon*)p1->borders.firstel;border;border=NEXTPOLY(border)){
  ptr=PLIST(border);
  while (ptr!=NULL) {
   if (!ISSTIFT(ptr)) {
    if (p1->nsegs!=0) 
      ptr->segs=
        (int)(ptr->len*p1->freq*sqrt(p1->epsi*p1->mu)/C_VELOC*p1->nsegs/p1->fak)+1;
    else
      ptr->segs=0;
   } else {
    ptr->segs=1;
   }
  
   ptr=NEXT(ptr);
  }
 }
 modified(p1,False);
 ClearScr();
 drawpatch(p1);
 drawlist(p1,-1,-1);
 drawpoints(p1);
 drawnums(p1);
}
    
void Act_Excitation(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
Zeichnung p1;

p1=zeichnung;
if (p1->mode!=none) {XBell(disp,RING); return;}
printmessg(0,mSelectExcit,outputlist[MSSG_M].t_wid);
SetCursor(speise);
p1->mode=speise;
BusyButtons(buttonlist,True);
}

void Act_Calculation(wid,ev,args,narg)
 Widget wid;
 XEvent *ev;
 String *args;
 Cardinal *narg;
{
 Zeichnung 	z=zeichnung;
 char		mess[128];
 int 		ch;

 if (z->mode!=none) {XBell(disp,RING); return;}

 sprintf(mess,fWrnActionPars,nActCalculation);

 if (*narg != 1) {
	XtWarning(mess);
	return;
 }
 switch (ch=toupper((int)*args[0])) {
	case 'B': /* Boundary values */
	case 'I': /* Inner field */
	case 'C': /* Coupling */
	  break;

	default:
	  XtWarning(mess);
	  return;
 }

 
 if (!ST_VALID(z,ST_STRM) || (getflag(chboxlist[CHBX_KOPPFLAG].t_wid)!=(z->jelek.raw!=0))
                  || ch=='B') { /* zuerst Strom berechnen */
   Strom(wid,z);
 }

 /* Strom kann abgebrochen worden sein! */
 if (!ST_VALID(z,ST_STRM)) return;

 if        (ch=='I') { 
   Feld(wid,z);
 } else if (ch=='C') {
   setflag(chboxlist[CHBX_CLONESHOW].t_wid,1);
   XtSetSensitive(chboxlist[CHBX_CLONESHOW].t_wid,False);
   Kopp(wid,z);
   XtSetSensitive(chboxlist[CHBX_CLONESHOW].t_wid,True);
 }
}
