#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <X11/cursorfont.h>
#include <X11/Xatom.h>

#include "Malloc.h"
#include "basics.h"

#include "widglist.h"
#include "fileop.h"

#include "graf.h"
#include "inout.h"
#include "calc.h"
#include "feld.h"
#include "CBstrom.h"

#include "msant_strings.h"

#ifdef HASXPM
#ifdef HASXTUTIL
#include <X11/XTutil/XTutil.h>
#else
#include "XTutil.h"
#endif
#include "icon1"
#else
#include "icon2"
#endif

/* I know this whole drawing-window stuff should be moved
 * to be a widget of its own. It's a little difficult though
 * since the code interacts heavily with its surrounding (what a
 * widget can't do directly). This is not good style :-{
 */

typedef struct hdata Zeichnung_;

static  char XtNiconName[]="iconName";

extern double strtod();

#define ARMAX 30    /*Puffer-Gr"osse f. Segmentmarken-Puffer*/
#define PWID 2      /*Segmentmarken-Radius*/
#define MARKSIZE 4  /* Gr"osse des Koordinaten-Referenzpunktes */
#define MINL 30     /*ist eine Kante<MINL wird gepr"uft, ob die
                      Nummer links oder rechts geschrieben wird*/
#define ABST 10     /*Abstand Kanten-Nummern von den Kanten (in
                      Pix.)*/

static  XtResource app_resources[]={
	{ XtNskalFak, XtCSkalFak, XtRFloat,
	 sizeof(float), XtOffsetOf(Zeichnung_,fak), XtRString, "200." },
	{ XtNmuRel, XtCMuRel, XtRFloat,
	 sizeof(float), XtOffsetOf(Zeichnung_,mu), XtRString, "1." },
	{ XtNrefPntX, XtCRefPntX, XtRShort,
	 sizeof(short), XtOffsetOf(Zeichnung_,off.x), XtRImmediate, (XtPointer)0 },
	{ XtNrefPntY, XtCRefPntY, XtRShort,
	 sizeof(short), XtOffsetOf(Zeichnung_,off.y), XtRImmediate, (XtPointer)0 },
	{ XtNorigX, XtCOrigX, XtRShort,
	 sizeof(short), XtOffsetOf(Zeichnung_,orig.x), XtRImmediate, (XtPointer)ORIGX },
	{ XtNorigY, XtCOrigY, XtRShort,
	 sizeof(short), XtOffsetOf(Zeichnung_,orig.y), XtRImmediate, (XtPointer)ORIGY },
	{ XtNcanvasWidth, XtCWidth, XtRDimension,
	 sizeof(Dimension), XtOffsetOf(Zeichnung_,width), XtRImmediate, (XtPointer)STUBW },
	{ XtNcanvasHeight, XtCHeight, XtRDimension,
	 sizeof(Dimension), XtOffsetOf(Zeichnung_,height), XtRImmediate, (XtPointer)STUBH },
	{ XtNresourcesFound, XtCResourcesFound, XtRBoolean,
	 sizeof(Boolean), XtOffsetOf(Zeichnung_,resources_found), XtRImmediate, (XtPointer)False },
	{ XtNforeground, XtCForeground, XtRPixel,
	 sizeof(Pixel), XtOffsetOf(Zeichnung_,foreground), XtRString, XtDefaultForeground },
	{ XtNfieldColour, XtCFieldColour, XtRPixel,
	 sizeof(Pixel), XtOffsetOf(Zeichnung_,field_colour), XtRString, XtDefaultForeground },
	{ XtNcoordColour, XtCCoordColour, XtRPixel,
	 sizeof(Pixel), XtOffsetOf(Zeichnung_,coord_colour), XtRString, XtDefaultForeground },
	{ XtNpatchColour, XtCPatchColour, XtRPixel,
	 sizeof(Pixel), XtOffsetOf(Zeichnung_,patch_colour), XtRString, XtDefaultBackground },
	{ XtNmessages, XtCMessages, XtRString,
	 sizeof(String), XtOffsetOf(Zeichnung_,raw_strings), XtRString, "" },
};

/* Actions */
static void MovePointer();
static void EndMode();
static void Quit();
static void New();
static void InsertPoint();
static void MovePoint();
static void DeletePoint();
static void AddPolygon();
static void ModPolygon();
static void RefPoint();
static void Distance();
static void Undelete();
static void Validate();
static void LoadFile();
static void SaveFile();
static void SaveAs();
static void CloneVect();

static XtActionsRec actions[]={
	{nActMovePointer,MovePointer},
	{nActEndMode,EndMode},
	{nActQuit,Quit},
	{nActNew,New},
	{nActInsertPoint,InsertPoint},
	{nActMovePoint,MovePoint},
	{nActDeletePoint,DeletePoint},
	{nActAddPolygon,AddPolygon},
	{nActPolygon,ModPolygon},
	{nActRefPoint,RefPoint},
	{nActDistance,Distance},
	{nActCloneVect,CloneVect},
	{nActUndelete,Undelete},
	{nActValidate,Validate},
	{nActLoadFile,LoadFile},
	{nActSaveFile,SaveFile},
	{nActSaveAs,SaveAs},
	{nActSegment,Act_Segment},
	{nActSegmentAll,Act_SegmentAll},
	{nActExcitation,Act_Excitation},
	{nActCalculation,Act_Calculation},
};

#define QUADR(x) (x)*(x)


#define SEL(hdataptr) ((Selection *) ((hdataptr)->sel.firstel))

extern char *getenv();


static void followrefpoint();
static void disable_radius();
static void ClearList();
static void ClearUndel();
static void drawcoords();
static void ElimRedPoly();

static void CleanName();

static Polygon * SearchLine();


#define gcthin gcxor

static long maxreq;
static struct hdata zeichnung1;

static FILE *Feil;
       




/* Referenzpunkt nachf"uhren */

static void followrefpoint(p1,ort)

Zeichnung p1;
koord    ort;

{
if (getflag(chboxlist[CHBX_REFP_SET].t_wid)) {
            p1->off.x= ort.x-p1->orig.x;
            p1->off.y=-ort.y+p1->orig.y;
}
}

/* Redundante (mehrfach selektierte Polynome) aus der
   Selektionsliste einer zeichnung entfernen */

static void ElimRedPoly(p1)

struct hdata *p1;

{
Selection *sel,*comp;

for (sel=SEL(p1); sel; sel=NEXTSEL(sel)) {
 for (comp=NEXTSEL(sel); comp; comp=NEXTSEL(comp)){
  if (comp->selpoly==sel->selpoly) unselect(p1,&comp);
 }
}
}




/* Die Geometrie wurde ver"andert, und das File,
   die Strom- und Feldberechnung entsprechen nicht mehr
   dem aktuellen Stand */

void modified(p1,new_region)

struct hdata *p1;
Boolean new_region;
{
 register Polygon *polp;
 register Punkt   *pnkt;
 int		  status=0;

 /* Speisesegment suchen */

 p1->speiseseg=0;
 for (polp=(Polygon*)p1->borders.firstel; polp; polp=NEXTPOLY(polp)){
  for (pnkt=PLIST(polp); pnkt; pnkt=NEXT(pnkt)){
   if (pnkt->speis!=0.) {
    p1->speiseseg=pnkt;
    break;
   }
  }
 }
 if (ST_VALID(p1,ST_FILE)) XClearWindow(disp,win1);
 strominvalid(p1);/* Stromdaten werden ung"ultig */
 destroyfield(p1);/* Felddaten werden ung"ultig */
 koppinvalid(p1); /* Koppeladmittanz wird ung"ultig */
 printmessg(0,0,outputlist[MSSG_KOPPY].t_wid);
 if (new_region) {
   PatchRegion(p1);
 }

 status=p1->status; /* get the most recent status */
 if (new_region) status|=ST_REGI;
 status|=ST_GEOM;
 status&=~ST_FILE;
 NotifyStatusChanged(p1,status);
}


int noselection(p1)

struct hdata *p1;

{
if (SEL(p1)) {
 if (SELECTED(SEL(p1))) return(0);
}
return(1);
}

/* Die Selektion *sel l"oschen; *sel zeigt
   nachher auf die folgende Selektion */

void unselect(p1,sel)

struct hdata *p1;
Selection **sel;

{
/* wenn die selektierte Kante ein Stift ist, Radius 
 * abschalten
 */

 if (*sel) {
  if ISSTIFT((*sel)->selected) {
     disable_radius(0);
  }
  Free(frepo(&p1->sel.firstel,sel));
 }

}

void unselectall(p1)

struct hdata *p1;

{
 Selection *sel, *sel1;

 disable_radius(0);
 sel=SEL(p1);
 while (sel) {
   sel1=sel;
   sel=NEXTSEL(sel);
   Free(sel1);
 }
 p1->sel.firstel=p1->sel.lastel=0;
}
   
 
void init_before_realize(wid)
Widget wid;

{
 int rval;
 char buff[128];
#ifndef HASXPM
 unsigned int w=icon_width,h=icon_height;
#endif
 zeichnung=&zeichnung1;
 

 disp=XtDisplay(wid);
 w_toplevel=wid;
 maxreq=XMaxRequestSize(disp);

#ifdef HASXPM
 icon_window=CreateIconWindow(disp,(Window)NULL,False,
			icon_bits,0,0);
#else
 icon_window=CreateIconWindow(disp,(Window)NULL,False,
			icon_bits,&w,&h);
#endif

 app=XtWidgetToApplicationContext(wid);
 XtVaGetApplicationResources(wid,
	&zeichnung1,app_resources,XtNumber(app_resources),
	NULL);

 if (!zeichnung->resources_found) {
        Notice(wid,0,mWrnNoResources,"Ok",0,0);
        exit(1);
   }

 if (0!=(rval=ReadStrings(zeichnung->raw_strings)) ){
	 sprintf(buff,"incorrect message-strings resource -- cannot execute");
	 if(rval>0) {
		 sprintf(buff+strlen(buff)," (missing %i items)",rval);
	 } else {
		 strcat(buff," (more items than expected)");
	 }

	 Notice(wid,0,buff,"Ok",0,0);
	 exit(1);
 }

 zeichnung->mode=none;
 zeichnung->borders.firstel=0;
 zeichnung->borders.lastel=0;
 zeichnung->speiseseg=0;
 zeichnung->stift.ort.x=-1; 
 zeichnung->jmagn.raw=zeichnung->jelek.raw=0;
 zeichnung->nstrom=0;
 zeichnung->sel.firstel=0;
 zeichnung->sel.lastel=0;
 zeichnung->Vre=0;
 zeichnung->Vim=0;
 zeichnung->pregion=0;
 zeichnung->field_pix=(Pixmap)None;
 zeichnung->koppdist.x=0;
 zeichnung->koppdist.y=0;
 zeichnung->undel.buff.firstel=0;
 zeichnung->undel.buff.lastel=0;
 zeichnung->undel.size=0;
 zeichnung->input.x=0;
 zeichnung->input.y=0;
 zeichnung->status=ST_FILE;
 input_koords[0].wid.wid_name=(char*)zeichnung;
 Feil=0;
}

void init_after_wids(w,argc,argv)

Widget	w;
int	argc;
char	*argv[];

{XGCValues xgc;
 XtGCMask  mask1;
 Widget	   canvas;
 int i;
 char mess[100],*err;
 Arg  args[5];
 Dimension width,height;
 XWindowAttributes atts;

 /* Der Eventhandler zur Behandlung des Window-Manager-Menus
  * muss nach der ganzen Initialisierung als erster registriert
  * werden, da OpenLook w"ahrend dieser einen eigenen installiert!
  * die WM_DELETE_WINDOW property ist deshalb bereits gesetzt!
  * (muss von uns nicht mehr gesetzt werden)
  */
 A_wm_delete_window=XInternAtom(disp,"WM_DELETE_WINDOW",True);
 A_wm_protocols=XInternAtom(disp,"WM_PROTOCOLS",True);
 deal_with_wmproto(w_toplevel);

 if ((err=register_to_widgetlist(input_haupt+1,
                                sizeof(TextFieldTabEl)
                                ))!=0) {
  sprintf(mess,"init_after..: %s not found in input_haupt!",err);
  XtAppError(app,mess);
 }
 if ((err=register_to_widgetlist(input_fname+1,
                                sizeof(TextFieldTabEl)
                                ))!=0) {
  sprintf(mess,"init_after..: %s not found in input_fname!",err);
  XtAppError(app,mess);
 }
 if ((err=register_to_widgetlist(input_radius+1,
                                sizeof(TextFieldTabEl)
                                ))!=0) {
  sprintf(mess,"init_after..: %s not found in input_radius!",err);
  XtAppError(app,mess);
 }
 if ((err=register_to_widgetlist(input_koords+1,
                                sizeof(TextFieldTabEl)
                                ))!=0) {
  sprintf(mess,"init_after..: %s not found in input_koords!",err);
  XtAppError(app,mess);
 }
 if ((err=register_to_widgetlist(outputlist,
                                sizeof(WidTabEl)
                                ))!=0) {
  sprintf(mess,"init_after..: %s not found in outputlist!",err);
  XtAppError(app,mess);
 }
 if ((err=register_to_widgetlist(buttonlist,
                                sizeof(WidTabEl)
                                ))!=0) {
  sprintf(mess,"init_after..: %s not found in buttonlist!",err);
  XtAppError(app,mess);
 }
 if ((err=register_to_widgetlist(canvaslist,
                                sizeof(WidTabEl)
                                ))!=0) {
  sprintf(mess,"init_after..: %s not found in canvaslist!",err);
  XtAppError(app,mess);
 }
 if ((err=register_to_widgetlist(chboxlist,
                                sizeof(WidTabEl)
                               ))!=0) {
  sprintf(mess,"init_after..: %s not found in chboxlist!",err);
  XtAppError(app,mess);

 }

 /* das Zeichenfenster auf die richtige Gr"osse bringen 
  * (nur vergr"ossern ist m"oglich)
  */

  i=0;
  XtSetArg(args[i],XtNwidth,&width); i++;
  XtSetArg(args[i],XtNheight,&height); i++;
  XtGetValues(canvaslist[CNVS_GRAFIKFENSTER].t_wid,args,i);

  i=0;
  if (zeichnung->width>width) {
    width=zeichnung->width;
  }
  if (zeichnung->height>height) {
    height=zeichnung->height;
  } 

  zeichnung->orig.y=height-zeichnung->orig.y;

  XtMakeResizeRequest(canvaslist[CNVS_GRAFIKFENSTER].t_wid,
	width,height,0,0);


 /* einige Cursors
  *
  */

 canvas=canvaslist[CNVS_GRAFIKFENSTER].t_wid;

 curs_none =None;
 curs_cross=XCreateFontCursor(disp,XC_crosshair);
 curs_mess =XCreateFontCursor(disp,XC_crosshair);
 curs_move =XCreateFontCursor(disp,XC_hand2);
 curs_wait =XCreateFontCursor(disp,XC_watch);
 curs_point=XCreateFontCursor(disp,XC_dotbox);
 curs_readout=XCreateFontCursor(disp,XC_question_arrow);


 win= XtWindow(canvas);
 XGetWindowAttributes(XtDisplay(canvas),win,&atts);
 win_depth=atts.depth;

 win1=XtWindow(canvaslist[CNVS_STROMFENSTER].t_wid);
 
 SetCursor(none);

 XtVaGetValues(canvas,
	XtNbackground,&white,
	NULL);
 zeichnung->canvas_colour=white;

 xgc.foreground=black=zeichnung->foreground;
 xgc.background=white;
 xgc.graphics_exposures=False;
 mask1=GCForeground|GCBackground|GCGraphicsExposures;

 gc=XCreateGC(disp,win, mask1, &xgc);


 xgc.line_style=LineOnOffDash;
 xgc.dashes=(char) 1;
 xgc.foreground=zeichnung->coord_colour;
 gccoord=XCreateGC(disp,win,
       mask1|GCLineStyle|GCDashList,
       &xgc);

 xgc.dashes=(char) 2;
 xgc.foreground=black;
 gcdotted=XtGetGC(canvas,
       mask1|GCLineStyle|GCDashList,
       &xgc);

 xgc.foreground=zeichnung->field_colour;
 gcfield=XCreateGC(disp,win,
  mask1|GCLineStyle,
  &xgc);

 xgc.line_width=3;
 xgc.foreground=black;
 gcthick=XtGetGC(canvas,
  mask1|GCLineWidth,
  &xgc);

 xgc.function=GXinvert;
 xgc.plane_mask=black^white;
 gcxor=XCreateGC(disp,win,
  GCPlaneMask|GCGraphicsExposures|GCFunction|GCForeground,
  &xgc);


 zeichnung->xbuf1=InitializePBuffer(disp,win,gc,100);
 zeichnung->xbuf2=InitializePBuffer(disp,win1,gc,100);

 /* Version ausgeben */
 sprintf(mess,fTestVersion,VERSION);
 printf("%s\n",mess);
 printmessg(0,mess,outputlist[MSSG_VERSION].t_wid);


 /* Filename initialisieren */
 if (argc>1) { /* Name von der Kommandozeile */
    printmessg(0,argv[1],input_fname[TXTFLD_FNAME].wid.t_wid);
 }
 getmessg(input_fname[TXTFLD_FNAME].wid.t_wid,mess);
 if (strlen(mess)>0) {
  LoadFile(buttonlist[BUTT_DATEI].t_wid,-1,0,0);
 }
 /* Klonvektor initialisieren */

 sprintf(mess,fDxDy,
		zeichnung->koppdist.x/zeichnung->fak,
		-zeichnung->koppdist.y/zeichnung->fak);
 printmessg(0,mess,outputlist[MSSG_KOPPVEK].t_wid);

 disable_radius(0);
 getparams(input_haupt);

/* Synchronisieren, bevor der Scrollbar verschoben werden kann! */

  while(XtAppPending(app)) XtAppProcessEvent(app,XtIMAll);

  AdjustView(canvaslist[CNVS_GRAFIKFENSTER].t_wid,&zeichnung->orig.x,&zeichnung->orig.y);


}

static Polygon * SearchLine(list,ort,len,nearest)

Punkt ** nearest;
Polygonliste * list;
koord   ort;
float * len;

{
Punkt *minptr,*ptr;
float d,min=0. /* keep compiler happy */,s;
koord vec;
Polygon  *border,*minpoly;


minptr=NULL;

border=(Polygon*)list->firstel; minpoly=NULL;

while (border){
ptr=PLIST(border);
if (ptr==ENDE(border)){ /* Nur ein Punkt? --> Stift! */
 d=hypot((double)(ort.x-ptr->ort.x) , (double)(ort.y-ptr->ort.y) );
 if ((minptr==NULL) || (d<min))  {
   min=d; minptr=ptr; minpoly=border;
   if (len!=NULL) *len=1E-9; /* dummy-Wert >0 */
 }
} else {
 while (ptr!=NULL) {
  vec.x=ort.x-ptr->ort.x;
  vec.y=ort.y-ptr->ort.y;
  s=(vec.x*ptr->richtung.x + vec.y*ptr->richtung.y)/ptr->len;
  d=sqrt(fabs((double) ( QUADR(vec.x) + QUADR(vec.y) - QUADR(s))));
  /* Wurzel aus Absolutbetrag wegen Rundungsfehlern bei sehr kleinen
   * vec.x, vec.y
   */
  if (s>ptr->len) d=hypot(d,s-ptr->len);
  if (s<0)        d=hypot(d,s);
  if ((d<min) || (minptr==NULL)) 
    {min=d; minptr=ptr; minpoly=border;
     if (len!=NULL) *len=s;}
  ptr=NEXT(ptr);
  }
}
 border=NEXTPOLY(border);
}
*nearest=minptr;
return(minpoly);
} 


/*Punktliste, und einen noch nicht gespeicherten Punkt
 *zeichnen. Dieser Extrapunkt ist in die selektierte Kante
 *einzuf"ugen (insert/add-mode). Ein negativer x-Wert zeigt an,
 *dass kein solcher Punkt gezeichnet werden soll.
 */

void drawlist(data,x,y)

 struct hdata *data;
 int x,y;

{
 register Punkt *ptr,*ptr1,*second;
 Polygon *border;
 Selection *sel;
 register koord ort;
 int   drawclone;
 register int delx=data->translate.x;
 register int dely=data->translate.y;
 
 if (data->mode!=add){
 for (sel=SEL(data);
      sel;
      sel=NEXTSEL(sel)) {
 if ((ptr=SELECTED(sel))!=NULL) {
  if (PLIST(sel->selpoly)==ENDE(sel->selpoly)) { /* Stift ? */
    int intdiam=irint(2.*ptr->len);
    if (intdiam==0) intdiam=1;
    XDrawArc(disp,win,gcthick,
             ptr->ort.x-intdiam/2,ptr->ort.y-intdiam/2,
    	     intdiam,intdiam,
	     0,360*64);
   }else{
   if ((ptr1=NEXT(ptr))==NULL) 
     ptr1=PLIST(SELPOLY(sel));
   XFillArc(disp,win,gc,ptr->ort.x-PWID,ptr->ort.y-PWID,
            PWID*4,PWID*4,0,360*64);
   XDrawLine(disp,win,gcthick,
            ptr->ort.x,ptr->ort.y,ptr1->ort.x,ptr1->ort.y);
  }
 }
 /* im copy und move -mode eine um (x,y) verschobene Kopie der
    selectierten Polygone zeichnen */
 if (data->mode==copypol) {
  ptr=PLIST(sel->selpoly);
  if (ptr==ENDE(sel->selpoly)) { /* Stift ? */
    int intdiam=irint(2.*ptr->len);
    if (intdiam==0) intdiam=1;
    XDrawArc(disp,win,gcthin,
             ptr->ort.x + delx -intdiam/2,ptr->ort.y + dely -intdiam/2,
    	     intdiam,intdiam,
	     0,360*64);
  }else{
  ResetPBuffer(data->xbuf1,win,gcthin);
  for (ptr=PLIST(sel->selpoly); ptr; ptr=NEXT(ptr) ){
    ort.x=ptr->ort.x+delx;
    ort.y=ptr->ort.y+dely;
    PutPBuffer(data->xbuf1,ort);
  }
 ort.x=PLIST(sel->selpoly)->ort.x+delx;
 ort.y=PLIST(sel->selpoly)->ort.y+dely;
 PutPBuffer(data->xbuf1,ort);
 FlushPBuffer(data->xbuf1);
 }
 } /*if mode==copypol*/
   
 }
 }

 if (data->mode==klon) {
     delx+=data->koppdist.x;
     dely+=data->koppdist.y;
 } else {
     delx=data->koppdist.x;
     dely=data->koppdist.y;
}

 drawclone=getflag(chboxlist[CHBX_CLONESHOW].t_wid) && (delx || dely);

 for (border=(Polygon*)data->borders.firstel; border!=0;
      border=NEXTPOLY(border)) {
  ptr=PLIST(border);
  if (ptr==NULL) continue;
  if (NEXT(ptr)==NULL) { /* Stift */
    int intdiam=irint(2.*ptr->len);
    if (intdiam==0) intdiam=1;
    XDrawArc(disp,win,gcthin,
             ptr->ort.x-intdiam/2,ptr->ort.y-intdiam/2,
    	     intdiam,intdiam,
	     0,360*64);

    if (drawclone) {
     XDrawArc(disp,win,gcdotted,
             ptr->ort.x+delx-intdiam/2,
	     ptr->ort.y+dely-intdiam/2,
    	     intdiam,intdiam,
	     0,360*64);
    }
  }else{
  ResetPBuffer(data->xbuf1,win,gc);
  if (drawclone) {
   ResetPBuffer(data->xbuf2,win,gcdotted);
  }
  while (ptr!=NULL) {
    PutPBuffer(data->xbuf1,ptr->ort);
    if (drawclone) {
     ort.x=ptr->ort.x+delx;
     ort.y=ptr->ort.y+dely;
     PutPBuffer(data->xbuf2,ort);
    }
    ptr=NEXT(ptr); 
   } 

  PutPBuffer(data->xbuf1,PLIST(border)->ort); 
  FlushPBuffer(data->xbuf1);
  if (drawclone) {
     ort.x=PLIST(border)->ort.x+delx;
     ort.y=PLIST(border)->ort.y+dely;
     PutPBuffer(data->xbuf2,ort);
     FlushPBuffer(data->xbuf2);
  }
 }
 }

 /* Im add Mode die selektierte Linie wieder l"oschen,
    und den Extrapunkt zeichnen */

 if ((x>=0) && !noselection(data) && data->mode==add) {
  /* nur l"oschen, wenn nicht nur eine Kante vorhanden */
  second=NEXT(SELECTED(SEL(data)));
  if (second==0) second=PLIST(SELPOLY(SEL(data)));

  if (ENDE(SELPOLY(SEL(data)))!=NEXT(PLIST(SELPOLY(SEL(data))))){
   XSetForeground(disp,gc,data->canvas_colour);
   XDrawLine(disp,win,gc,SELECTED(SEL(data))->ort.x,
                        SELECTED(SEL(data))->ort.y,
                        second->ort.x,
                        second->ort.y);
   XSetForeground(disp,gc,data->foreground);
  }
  /* ExtraPunkt zeichnen */
  XDrawLine(disp,win,gc,
            SELECTED(SEL(data))->ort.x,
            SELECTED(SEL(data))->ort.y,
            x,
            y);
  XDrawLine(disp,win,gc,
            x,
            y,
            second->ort.x,
            second->ort.y);
 }


/* Den Referenzpunkt zeichnen */  
XDrawLine(disp,win,gc,
   data->off.x+data->orig.x - MARKSIZE, -data->off.y+data->orig.y,
   data->off.x+data->orig.x + MARKSIZE, -data->off.y+data->orig.y);
XDrawLine(disp,win,gc,
   data->off.x+data->orig.x , -data->off.y+data->orig.y- MARKSIZE,
   data->off.x+data->orig.x , -data->off.y+data->orig.y+ MARKSIZE);
XDrawArc(disp,win,gc,
   data->off.x+data->orig.x-MARKSIZE , -data->off.y+data->orig.y-MARKSIZE,
   2*MARKSIZE+1,2*MARKSIZE+2,0,360*64);
   
  
  
}
  
 

void drawpoints(data)

struct hdata *data;

{
int max,j,k;
Punkt *ptr;
Polygon *border;

static XArc   xarc[ARMAX];
static short first=1;

if (first) {
 for (j=0;j<ARMAX;j++){
  xarc[j].width=xarc[j].height=PWID*2;
  xarc[j].angle1=0;
  xarc[j].angle2=360*64;
 }
 first=0;
 }
max=(maxreq-3)/3;
if (max>ARMAX) max=ARMAX;

for (border=(Polygon*)data->borders.firstel; 
     border; 
     border=NEXTPOLY(border)){
 if (PLIST(border)==ENDE(border)) continue; /* Stift */
 for (ptr=PLIST(border);
      ptr;
      ptr=NEXT(ptr)){
  j=0;
  while ((ptr->segs-j)>max) {
   for (k=1;k<=max;k++) {
    xarc[k-1].x=(short)(-PWID)+
                (short)(ptr->ort.x+((k+j)*ptr->richtung.x)/ptr->segs);
    xarc[k-1].y=(short)(-PWID)+
                (short)(ptr->ort.y+((k+j)*ptr->richtung.y)/ptr->segs);
    }
   XDrawArcs(disp,win,gc,xarc,max);
   j+=max;
  }
  for (k=1;k<=ptr->segs-j;k++) {
    xarc[k-1].x=(short)(-PWID)+ 
                (short)(ptr->ort.x+((k+j)*ptr->richtung.x)/ptr->segs);
    xarc[k-1].y=(short)(-PWID)+
                (short)(ptr->ort.y+((k+j)*ptr->richtung.y)/ptr->segs);
  }
  XDrawArcs(disp,win,gc,xarc,ptr->segs-j);

}
}

 /* Speisung einzeichnen */

  if (0!=(ptr=data->speiseseg)) {
   if ISSTIFT(ptr) { /* Stift*/
    int intdiam=irint(2.*ptr->len);
    if (intdiam==0) intdiam=1;
    XDrawArc(disp,win,gcthick,
             ptr->ort.x-intdiam/2,ptr->ort.y-intdiam/2,
    	     intdiam,intdiam,
	     0,360*64);
   }else{
   if (ptr->segs != 0){
    k=(int)(ptr->speis*ptr->segs);
    XDrawLine(disp,win,gcthick,
      (short)(ptr->ort.x+k*ptr->richtung.x/ptr->segs),
      (short)(ptr->ort.y+k*ptr->richtung.y/ptr->segs),
      (short)(ptr->ort.x+(k+1)*ptr->richtung.x/ptr->segs),
      (short)(ptr->ort.y+(k+1)*ptr->richtung.y/ptr->segs)); 
   } else {
    XFillArc(disp,win,gc,
      (short)(ptr->ort.x+ptr->richtung.x*ptr->speis-PWID),
      (short)(ptr->ort.y+ptr->richtung.y*ptr->speis-PWID),
      PWID*4,PWID*4,0,360*64);
   }
  }
  }


}



void drawnums(data)

struct hdata * data;

{
Punkt * ptr, *nxt;
XPoint ort;
int      i;
char buf[15];
Polygon *border;

i=0;
for (border=(Polygon*)data->borders.firstel;
     border;
     border=NEXTPOLY(border)){
for (ptr=PLIST(border);
     ptr;
     ptr=NEXT(ptr)){
 nxt=NEXT(ptr);

 if (ISSTIFT(ptr)) {
  ort.x=(short)(ptr->ort.x+ptr->len+ABST/2);
  ort.y=(short)(ptr->ort.y+ptr->len+ABST/2);
 }else{
  if (nxt==NULL) nxt=PLIST(border);

  if ((ptr->len<MINL) &&
     ((-ptr->richtung.x*nxt->richtung.y +
        ptr->richtung.y*nxt->richtung.x)<0)) {
   ort.x=(short)(
       ptr->ort.x+ptr->richtung.x/2+ptr->richtung.y/ptr->len*ABST);
   ort.y=(short)(
       ptr->ort.y+ptr->richtung.y/2-ptr->richtung.x/ptr->len*ABST);
  } else {
   ort.x=(short)(
       ptr->ort.x+ptr->richtung.x/2-ptr->richtung.y/ptr->len*ABST);
   ort.y=(short)(
       ptr->ort.y+ptr->richtung.y/2+ptr->richtung.x/ptr->len*ABST);}
 }
 sprintf(buf,"%u",i++); 
 XDrawString(disp,win,gc,ort.x,ort.y,buf,strlen(buf));
}
}
}



static void drawcoords(widget,ox,oy,wid,hig,textx,texty,dashx1,dashx2,dashy1,dashy2)
Widget widget;
short ox,oy;
Dimension wid,hig;
char * textx, * texty;
char dashx1,dashy1;
char dashx2,dashy2;

{
 Drawable wi=XtWindow(widget);
 XPoint xpoint[8];
 char dlist[2];
 Dimension w;
 
 int i;

  xpoint[0].x=ox-dashx1; /*Anfangspunkt x-Achse*/
  xpoint[0].y=oy;

  xpoint[2].x=ox+wid-70; /*Endpunkt x-Achse*/
  XtVaGetValues(widget,XtNwidth,&w,NULL);
  if (xpoint[2].x>w) xpoint[2].x=w-30;

  xpoint[2].y=oy;
  xpoint[1].x=xpoint[2].x-5;
  xpoint[1].y=xpoint[2].y-5; /*Pfeil x-Achse*/
  xpoint[3].x=xpoint[2].x-5;
  xpoint[3].y=xpoint[2].y+5; /*Pfeil x-Achse*/

  xpoint[4].x=ox;  /*Anfangspunkt y-Achse*/
  xpoint[4].y=oy+dashy1;
  xpoint[6].x=ox;  /*Endpunkt y-Achse*/
  xpoint[6].y=oy-hig+70;
  if (xpoint[6].y<0) xpoint[6].y=30;
  
  xpoint[5].x=xpoint[6].x-5; /*Pfeil y-Achse*/
  xpoint[5].y=xpoint[6].y+5; /*Pfeil y-Achse*/
  xpoint[7].x=xpoint[6].x+5; /*Pfeil y-Achse*/
  xpoint[7].y=xpoint[6].y+5; /*Pfeil y-Achse*/
  
 for (i=0;i<8;i+=4) {
 if (i==0){dlist[0]=dashx1;dlist[1]=dashx2;}
  else    {dlist[0]=dashy1;dlist[1]=dashy2;}
 XSetDashes(disp,gccoord,0,dlist,2);
 XDrawLine(disp,wi,gccoord,xpoint[i].x,xpoint[i].y,
                            xpoint[i+2].x,xpoint[i+2].y);
 dlist[0]=100;
 XSetDashes(disp,gccoord,0,dlist,2);
 XDrawLines(disp,wi,gccoord,&xpoint[i+1],3,CoordModeOrigin);
 }
 XDrawImageString(disp,wi,gccoord,xpoint[2].x+5,
                              xpoint[2].y-5,
			      textx,strlen(textx));
 XDrawImageString(disp,wi,gccoord,xpoint[6].x-5,
                              xpoint[6].y-5,
			      texty,strlen(texty));
}



void CBExpose(w,cld,cad)

Widget w;
XtPointer cld,cad;

{
/*
OlDrawAreaCallbackStruct *expCBstrp=(OlDrawAreaCallbackStruct*)cad;
*/
 ClearScr();
 drawpatch(zeichnung);
 /* ClosePath(&zeichnung->borders); */
 drawlist(zeichnung,-1,-1);
 drawpoints(zeichnung);
 drawnums(zeichnung);
  
}


void ClearScr()

{
Widget wid;
Dimension w,h;
XClearArea(disp,win,0,0,0,0,False);
wid=canvaslist[CNVS_GRAFIKFENSTER].t_wid;
XtVaGetValues(GetClip(wid),XtNwidth,&w,XtNheight,&h,NULL);
drawcoords(wid,
	   zeichnung->orig.x,zeichnung->orig.y,
	   w,h,
	   "X","Y",
           (char)10,(char)10,(char)10,(char)10);
XFlush(disp);
}

/* eine Liste von Polygonen beginnend mit dem Element poly
 * freigeben.
 */
static void ClearList(undel,polist,poly)

UndelBuffer  *undel;
Polygonliste *polist;
Polygon **poly;

{
if (*poly==NULL) return;

*poly=LASTPOLY(*poly);

while((Polygon*)polist->lastel!=*poly){
 RemovePolygon(undel,polist,&polist->lastel);
}
}

/* einen undel-Buffer (undel-Liste) l"oschen */

static void ClearUndel(undel)

UndelBuffer *undel;
{
while(undel->buff.lastel){
/* zuerst die Elemente freigeben */
 ((UndelEl*)undel->buff.lastel)->destroy(((UndelEl*)undel->buff.lastel)->elem);
 Free(frepo(&(undel->buff),&(undel->buff.lastel)));
}
undel->size=0;
}



/* ein Polygon einf"ugen (kann auch ein Stift (Kreis) sein! */

static void AddPolygon(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
Zeichnung  p1;
Selection *sel;
Boolean stift;

p1=zeichnung;
stift = (*narg>0 && toupper((int)*args[0])=='S') ? True:False;

if (p1->mode!=none) {XBell(disp,RING);return;}

/* neues Polygon an die Polygonliste anf"ugen */
if (p1->sel.firstel!=p1->sel.lastel) {
  printmessg(1,mSelOnlyOnePoint,w);
  return;
}

if (noselection(p1)) {
 if (CreateEnqListEl(&p1->sel,p1->sel.lastel,
                     &sel,sizeof(Selection))) return;
 SELECTED(sel)=0;
 SELPOLY(sel)=(Polygon*)p1->borders.lastel;
}else{
 SELECTED(SEL(p1))=NULL;
}
if (stift) {
 p1->mode=speisestift;
 printmessg(0,mPlaceExcitation,outputlist[MSSG_M].t_wid);
 sens_desens(input_radius[TXTFLD_RADIUS].wid.t_wid,True,0);
}else{
 p1->mode=add;
 printmessg(0,mDrawPoints,outputlist[MSSG_M].t_wid);
}
BusyButtons(buttonlist,True);
SetCursor(add);
}



static void InsertPoint(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
struct hdata * p1;
p1=zeichnung;

if (p1->mode!=none) {XBell(disp,RING);return;}
if (noselection(p1)) {
 printmessg(1,mNoSelection,w);
 return;
}
if (p1->sel.firstel!=p1->sel.lastel) {
 printmessg(1,mSelOnlyOnePoint,w);
 return;
}

SetCursor(ins);
printmessg(0,mDrawPoints,outputlist[MSSG_M].t_wid);

p1->mode=ins;
BusyButtons(buttonlist,True);
}

static void DeletePoint(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

/* Delete selected Points */
{
struct hdata * p1;
Selection *sel;
p1=zeichnung;
/* Punkt *predecessor; */
if (p1->mode!=none) {XBell(disp,RING);return;}
if (noselection(p1)) {
 printmessg(1,mNoSelection,w);
 return;
}

 printmessg(0,0,outputlist[MSSG_M].t_wid);

 sel=SEL(p1);
 do {

/* predecessor = LAST(SELECTED(sel)); */
 RemovePoint(&p1->undel,&p1->borders,&SELPOLY(sel),&SELECTED(sel));

 if (SELECTED(sel) && NEXT(SELECTED(sel))==LAST(SELECTED(sel))) {
  /* Nur ein Punkt im Polygon? */
  SELECTED(sel)->len=PWID/p1->fak;
 }
 

 /*die Selektion auch l"oschen, ausser wenn es sich um
  *die letzte Selektion handelt und noch eine Kante selektiert ist
  */
 if (p1->sel.firstel==p1->sel.lastel) break;

 if (p1->sel.firstel!=p1->sel.lastel || SELECTED(sel)==NULL) {
  unselect(p1,&sel);
 }
 
 }while(1);
 if (SELECTED(sel)==NULL)  unselect(p1,&sel);

 modified(p1,True);
 CBExpose(0,0,0);
}

/*
 * MovePointer action syntax:
 *
 *  MovePointer([command,arg[,arg]])
 *
 * command: Up,Down,Left,Right, arg    Bewegung bezgl. akt. Standort
 *	    X,XR,Y,YR,		       Eindimensionale Bewegung bezgl. Ursprung/Referenzpunkt
 *	    P,PR            arg,arg    Zweidimensionale    "      "       "          "
 *
 * arg    : skalierte distanz (float)
 *          f"ur U,D,L oder R kann arg negativ sein.
 * 	    Dann wird in Einheiten des Rasters gerechnet.
 *
 */

static void MovePointer(wid,event,args,narg)

Widget		wid;
XEvent		*event;
String		*args;
Cardinal	*narg;

{
 struct hdata	  *data;
 char str[128],   *chpt;
 koord		  ort;
 register int	  delx,dely;
 register Punkt	  *ptr;
/* register Polygon *selpoly; */
 int 	  	  ch,x,y;
 koord	  	  refpoint;
 double	  	  dscratch;
 koord_el 	  moveto,moveto1 = 0 /* keep compiler happy */;
 Boolean	  wrap=False;

 data=zeichnung;

 sprintf(str,fWrnActionPars,nActMovePointer);

 if (*narg==0 ) {
   switch (event->type) {
     case MotionNotify:
		x=event->xmotion.x; y=event->xmotion.y;
		break;
     case EnterNotify:
     case LeaveNotify:
		if (data->mode==none) return;
		x=event->xcrossing.x; y=event->xcrossing.y;
		wrap=True;
		break;
     default:
		sprintf(str,fWrnActionEv,nActMovePointer);
		XtWarning(str);
		return;
   }
   ort.x=RASTER(x-data->orig.x,data->raster)+data->orig.x;
   ort.y=-RASTER(-y+data->orig.y,data->raster)+data->orig.y;
 } else {
  ort=data->input;
  switch( ch=toupper((int)*args[0]) ) {
	case 'U': case 'D': case 'L':
	case 'R': case 'X': case 'Y':
	case 'P':

	  moveto=(koord_el)((dscratch=strtod(args[1],&chpt))*data->fak);
	  if (chpt==args[1]) goto wrongargs;

	  if (ch=='P') {
	    if (*narg!=3) goto wrongargs;
	    else {
	      moveto1=(koord_el)(strtod(args[2],&chpt)*data->fak);
	      if (chpt==args[2]) goto wrongargs;
	    }
          }

	  if ( (ch=='U' || ch=='D' || ch=='L' || ch=='R') && dscratch<0 ) {
		moveto=(koord_el)(-dscratch*data->raster);
	  }

	  refpoint=data->orig;
          if ( (ch=='X' || ch=='Y' || ch=='P') && 
	       (toupper((int)*(args[0]+1))=='D')     ) {
	       refpoint.x+=data->off.x;
	       refpoint.y-=data->off.y;
	  }
	  switch (ch) {
		case 'U': /* Up */
 		  ort.y=-RASTER(moveto,data->raster)+data->input.y;
		  break;
		case 'D': /* Down */
 		  ort.y= RASTER(moveto,data->raster)+data->input.y;
		  break;
		case 'L': /* Left */
 		  ort.x=-RASTER(moveto,data->raster)+data->input.x;
		  break;
		case 'R': /* Right */
 		  ort.x= RASTER(moveto,data->raster)+data->input.x;
		  break;
		case 'X': /* Goto X */
	        case 'P': /* Goto abs. Point */
		  
 		  ort.x=RASTER(moveto,data->raster)+refpoint.x;
		  if (ch=='P') 
 		    ort.y=-RASTER(moveto1,data->raster)+refpoint.y;
		  break;
		case 'Y': /* Goto Y */
 		  ort.y=-RASTER(moveto,data->raster)+refpoint.y;
		  break;
		default: goto wrongargs;
	  }
	  wrap=True;
	  break;

	default:
	wrongargs:
	  XtWarning(str);
	  return;
  }
 }
 
 if (wrap) {
   AdjustView(wid,&ort.x,&ort.y);
   XWarpPointer(disp,None,XtWindow(wid),0,0,0,0,(int)ort.x,(int)ort.y);
 }

 data->input=ort;
 
 printort(input_koords,&ort,data);

 switch (data->mode) {
  case add:
   /* im add-mode gibt es sicher nur ein selektiertes Polynom */
   /*selpoly=SELPOLY(SEL(data));*/
   ptr=SELECTED(SEL(data));
   if (ptr!=NULL) {
    ClearScr();
    drawpatch(zeichnung);
    drawlist(data,ort.x,ort.y);
    sprintf(str,"%g",hypot((double)(ort.x-ptr->ort.x),
                           (double)(ort.y-ptr->ort.y))/data->fak);
    printmessg(0,str,outputlist[MSSG_M].t_wid); 
   }
   break;

  case move:
  case movepol:
  case copypol:
  case klon:
   /* Alle Selektionen verschieben
    */
   if (data->distort.x<0) return;
   ClearScr();
   delx=ort.x-data->distort.x; 
   dely=ort.y-data->distort.y;
   switch (data->mode) {

    case move:
    case movepol:
     {
      register Selection *sel;
      register Punkt     *point;
      for (sel=SEL(data); sel; sel=NEXTSEL(sel)) {
       if (data->mode==move) {
        point=sel->selected;
        point->ort.x=point->richtung.x+delx;
        point->ort.y=point->richtung.y+dely;
       }else{
        for (point=PLIST(sel->selpoly); point; point=NEXT(point)){
         point->ort.x=point->richtung.x+delx;
         point->ort.y=point->richtung.y+dely;
        }
       }
      }
     }
     drawpatch(zeichnung);
     drawlist(data,-1,-1);
     break;

    case copypol:
    case klon:
     data->translate.x=delx;
     data->translate.y=dely;
     drawpatch(zeichnung);
     drawlist(data,-1,-1);
     drawpoints(data);
     break;
    default: break; /* no other should occur though */
   }

   if (data->mode==klon) {
     delx+=data->koppdist.x;
     dely+=data->koppdist.y;
     sprintf(str,fKlon_g_g,delx/data->fak,-dely/data->fak);
     printmessg(0,str+6,outputlist[MSSG_KOPPVEK].t_wid);


   } else {
     sprintf(str,"%g",hypot((double)(delx),
                            (double)(dely))/data->fak);

   }

   XDrawLine(disp,win,gcthin,data->distort.x,data->distort.y,
			     ort.x,ort.y);
   printmessg(0,str,outputlist[MSSG_M].t_wid); 

   break;


  case ins:
   /* im insert-mode gibt es sicher genau eine sel. Kante */
   ptr=SELECTED(SEL(data));
   if(ptr) {
    sprintf(str,"%g",hypot((double)(ort.x-ptr->ort.x),
                           (double)(ort.y-ptr->ort.y))/data->fak);
    printmessg(0,str,outputlist[MSSG_M].t_wid); 
   }
   break;

  case dist: 
   if (data->distort.x<0) return;
   ClearScr();
   drawpatch(zeichnung);
   drawlist(data,-1,-1);
   drawpoints(data);
   XDrawLine(disp,win,gc,data->distort.x,data->distort.y,ort.x,ort.y);
   sprintf(str,"%g",hypot((double)(ort.x-data->distort.x),
                          (double)(ort.y-data->distort.y))/data->fak);
   printmessg(0,str,outputlist[MSSG_M].t_wid); 
   break;

  case     none: 
   if (ST_VALID(data,ST_FELD)) {
     if (data->rmin.x<=ort.x &&
         data->rmax.x>ort.x &&
         data->rmin.y<=ort.y &&
         data->rmax.y>ort.y) {

         float x,y;
     
         x=floor((float)data->mmax*(float)(ort.x-data->rmin.x)
                     /(float)(data->rmax.x-data->rmin.x));
         y=floor((float)data->nmax*(float)(ort.y-data->rmin.y)
                     /(float)(data->rmax.y-data->rmin.y));
       
         if (data->jelek.raw) {/* mit Kopplung? */
	   dscratch=(double) (data->Vim[(int)y * data->mmax + (int)x]);
           sprintf(str,fCmplxVolt,
                        data->Vre[(int)y * data->mmax + (int)x],
			(signbit(dscratch)?'-':'+'),
			(float)fabs(dscratch));
         }else{
	   dscratch=(double)(data->Vim[(int)y * data->mmax + (int)x]);
           sprintf(str,fCmplxVolt,(float)0.,
			(signbit(dscratch)?'-':'+'),
			(float)fabs(dscratch));
         }

        printmessg(0,str,outputlist[MSSG_M].t_wid); 
     }  else {
        printmessg(0,0,outputlist[MSSG_M].t_wid); 
     }
     break;

   } /* if ST_VALID(data,ST_FELD) */

  case     refpnt:
  default: return;
 }
}

/*
 * EndMode Action Syntax
 *
 *  EndMode([ cmd [,flags]])
 *
 *  cmd:    C | A    Commit/Abort
 *  flags:  C	     Cancel last  (nur bei Commit; f"ur mehrteilige Operationen)
 * 
 * Default ist EndMode(C)
 */

typedef enum {Commit, Abort} Command;
#define EM_CANCEL (1<<0)

static void EndMode(wid,event,args,narg)

Widget   wid;
XEvent   *event;
String 	 *args;
Cardinal *narg;

{
struct hdata *data;
char str[128];
Punkt *ptr,*neu;
koord ort,ortexakt;
int extrax,extray;
int delx,dely;

char *mess;

Polygon *selpoly;
Punkt   *selected;
Selection *sel,dummysel;
int found = 0;
modes mode_local;

Command cmd=Commit;
int	ch,cmd_flags=0;

data=zeichnung;
mess=0;
dummysel.zeigers.nextel=dummysel.zeigers.prevel=0;
extrax=extray=-1;

sprintf(str,fWrnActionPars,nActEndMode);

if (*narg>2) {
	XtWarning(str);
	return;
} else if (*narg>0) {
  switch (ch=toupper((int)*args[0])) {
	case 'C': 
		break;
	case 'A':
		cmd=Abort;
		break;

	default:
		XtWarning(str);
		return;
  }
  if (*narg>1) switch(toupper((int)*args[1])) {

	case 'C' :
		cmd_flags|= EM_CANCEL;
		break;

	default:
		XtWarning(str);
		return;
  }
}


switch (event->type) { /* Only allow these events */
	case ButtonPress:
	case ButtonRelease:
		ortexakt.x=event->xbutton.x;
		ortexakt.y=event->xbutton.y;
		break;

	case KeyPress:
	case KeyRelease:
		ortexakt.x=event->xkey.x;
		ortexakt.y=event->xkey.y;
		break;

	default:
		sprintf(str,fWrnActionEv,nActEndMode);
		XtWarning(str);
		return;
}
  
if (wid!=canvaslist[CNVS_GRAFIKFENSTER].t_wid) {
  /* 
   * Wenn das Event nicht im Canvas-Widget geschah,
   * holen wir die Daten aus dem Eingabefeld
   * (Ich weiss, dass dies ein _sehr_ h"asslicher Stil ist)
   */
  wid=GetTxtFldWid(wid);
  found=1;
  while(input_koords[found].wid.wid_name &&
        input_koords[found].wid.t_wid != wid) found++;
  if (found<NTXTFLD_KOORDS) storeparam(input_koords,found,data);
  ortexakt=data->input;

} 

ort.x= RASTER(ortexakt.x-data->orig.x,data->raster)+data->orig.x;
ort.y=-RASTER(-ortexakt.y+data->orig.y,data->raster)+data->orig.y;
data->input=ort;

printort(input_koords,&ort,data);

switch (mode_local=data->mode) {
case refpnt: /* Referenzpunkt setzen */
  
  switch (cmd) {
   default: return;
   case Commit: 
            data->off.x=ort.x-data->orig.x;
            data->off.y=-ort.y+data->orig.y;
            ClearScr();
            drawpatch(data);
            drawnums(data);
	    printort(input_koords,&ort,data);
            
   case Abort: /* abort */
            data->mode=none;
            BusyButtons(buttonlist,False);
            SetCursor(none);
            break;
  }
  
  break;
       

case add: /*add mode*/

   ClearScr();
  
  switch (cmd) {
 
  case Commit:
   if (cmd_flags & EM_CANCEL) {
    /* letzten Punkt aus Liste l"oschen */
     if (!noselection(data)) {
 
      sel=SEL(data);
      RemovePoint(&data->undel,&data->borders,&SELPOLY(sel),
                  &SELECTED(sel));

      if (SELECTED(sel) && NEXT(SELECTED(sel))==LAST(SELECTED(sel))) {
       /* Nur ein Punkt im Polygon? , als Kreis zeichnen! */
       SELECTED(sel)->len=PWID/data->fak;
      }
      modified(data,True);
      /* wenn die Punktliste leer ist, die Selektion l"oschen */
      if (!noselection(data)) {
       SELECTED(SEL(data))->segs=0;
       followrefpoint(data,SELECTED(SEL(data))->ort);
       extrax=ort.x; extray=ort.y;
      } else {
       data->off.x=data->off.y=0;
      }
     }/* if !noselection() */
   } else { /* Punkt einf"ugen */
    if (!noselection(data))
     if ((SELECTED(SEL(data))->ort.x==ort.x)&&
         (SELECTED(SEL(data))->ort.y==ort.y)  ){
       goto nonewpoint;
     }
    if (CreatePoint(&data->borders,SEL(data),&neu)) return;
    followrefpoint(data,ort);
    neu->ort=ort;
    modified(data,True);
    CorrRicht(SELPOLY(SEL(data)),neu);
nonewpoint: ;
   }

   if (!noselection(data)) break; 

     /* wenn nichts mehr selektiert,
      * add-mode beenden
      */                  
  
  case Abort:
   /* Wenn die Polygonliste leer ist, l"oschen */
   if (!noselection(data)) {
     if (PLIST(SELPOLY(SEL(data)))==ENDE(SELPOLY(SEL(data)))){
        RemovePolygon(0,&data->borders,&SELPOLY(SEL(data)));
     }
   }


   /*ClosePath(&data->borders);*/
   drawnums(data); /*TSILLMARK*/
   SetCursor(none);
   data->mode=none;
   BusyButtons(buttonlist,False);
   unselectall(data);
   
   break;

  default: return;

  }/*switch command (add mode)*/

  break;

case ins: /*insert mode*/

   if (cmd==Commit){
    ClearScr();
    if (CreatePoint(&data->borders,SEL(data),&neu)) return;
    followrefpoint(data,ort);
    neu->ort=ort;
    modified(data,True);
    CorrRicht(SELPOLY(SEL(data)),neu);
    data->mode=add;
   }else{ /* Abort */
    drawnums(data);
    unselectall(data);
    SetCursor(none);
    data->mode=none;
    BusyButtons(buttonlist,False);
   }

  break;

case move:
case movepol:
  delx=dely=0;

  switch (cmd) {
     default: return;

     case Commit: 
              if (data->distort.x<0) {
               /*erster Punkt*/
               followrefpoint(data,ort);
               data->distort=ort;

		/* Den urspr"unglichen Ort zwischenspeichern */
               if (mode_local==move) {
 		for (sel=SEL(data); sel; sel=NEXTSEL(sel)) {
   		sel->selected->richtung=sel->selected->ort;
 		}
	       } else {
 		for (sel=SEL(data); sel; sel=NEXTSEL(sel)) {
  		 for (selected=PLIST(sel->selpoly); 
		     selected; 
		     selected=NEXT(selected)){
   		   selected->richtung=selected->ort;
  		 }
 		}
               }
               mess=mMove;

	       break;
	      }

              delx=ort.x-data->distort.x; 
	      dely=ort.y-data->distort.y;
              if (delx!=0 || dely!=0) {
	       modified(data,True);
               found=1;
              }else{
	       found=0;
	      }

     case Abort: /* Abort; delx,dely=0 von oben */

              for (sel=SEL(data); sel; sel=NEXTSEL(sel)) {
               if (mode_local==move) {
                 selected=sel->selected;
                 selected->ort.x=selected->richtung.x+delx;
                 selected->ort.y=selected->richtung.y+dely;
	         if (found) {
		  selected->segs=0;
		  if (LAST(selected)) LAST(selected)->segs=0;
		  else ENDE(sel->selpoly)->segs=0;
	         }
               }else{
                 for (selected=PLIST(sel->selpoly); selected;
                    selected=NEXT(selected)) {
                   selected->ort.x=selected->richtung.x+delx;
                   selected->ort.y=selected->richtung.y+dely;
                 }
               }
              }
	      ClearScr();
              ClosePath(&data->borders);
	      drawnums(data); /*TSILLMARK if found*/
              SetCursor(none);
	      data->mode=none;
    	      BusyButtons(buttonlist,False);
              if (input_radius[0].wid.wid_name) {
                sens_desens(input_radius[TXTFLD_RADIUS].wid.t_wid,True,0);
              }

	      break;
  } /*switch button */
               
  break;

case copypol:
case klon:

  switch(cmd) {
    default: return;

    case Commit:
             if (data->distort.x<0) { /* erster Punkt */
               data->distort=ort;
               followrefpoint(data,ort);
               switch (data->mode) {
		case copypol: mess=mPlaceCopy; break;
		case klon:    mess=mPlaceClone;  break;
		default: break; /* keep compiler happy */
	       }
               break;
             }
             /* zweiter Punkt; Kopie erzeugen/Klon festschreiben */

             delx=ort.x-data->distort.x;
             dely=ort.y-data->distort.y;

             if (data->mode==klon) {
		data->koppdist.x+=delx;
		data->koppdist.y+=dely;
	 	NOTIFY_OFF(data,ST_FILE);
                if (delx || dely) koppinvalid(data);
     		printmessg(0,0,outputlist[MSSG_KOPPY].t_wid);
		goto abortcopypol;
             }

             /* Ende der Polygonliste merken, falls die
              * neu erzeugten Polygone wegen Speicherplatzmangels
              * wieder gel"oscht werden m"ussen
              * ("Mi"sbrauch" von 'selpoly')
              */
             selpoly=dummysel.selpoly=(Polygon*)data->borders.lastel;

             for (sel=SEL(data); sel; sel=NEXTSEL(sel)){
               /* Polygon erzeugen */
               dummysel.selected=0;
               for (ptr=PLIST(sel->selpoly); ptr; ptr=NEXT(ptr)){

                if (CreatePoint(&data->borders,&dummysel,&neu)){
                  /* neues Element konnte nicht erzeugt werden;
                   * die bisher getane Arbeit r"uckg"angig machen!
                   */
                   ClearList(0,&data->borders,&selpoly);
                   unselectall(data);
                   goto abortcopypol;
                  }
		/* der Punkt darf nicht als ganzes kopiert werden, da
		 * die Listenverzeigerung nicht mitkopiert werden darf
		 */
                 neu->ort.x=ptr->ort.x+delx;
                 neu->ort.y=ptr->ort.y+dely;
		 neu->richtung=ptr->richtung;
		 neu->segs=ptr->segs;
		 neu->len=ptr->len;
		 neu->remove_proc=ptr->remove_proc;
               }
             }
	     modified(data,True);
             /*ClosePath(&data->borders); */


    case Abort: /*Abort*/
    abortcopypol:
	     if (data->mode==klon) {
		sprintf(str,fDxDy,
			data->koppdist.x/data->fak,
			-data->koppdist.y/data->fak);
		printmessg(0,str,outputlist[MSSG_KOPPVEK].t_wid);
 		XtSetSensitive(chboxlist[CHBX_CLONESHOW].t_wid,True);
	     }
	     ClearScr();
	     drawpatch(data);
	     drawnums(data);
             SetCursor(none);
	     data->mode=none;
    	     BusyButtons(buttonlist,False);

             break;
    }/* switch cmd */

    break;
 
              

case dist:
  switch (cmd) {

  case Abort:
    SetCursor(none);
    data->mode=none;
    BusyButtons(buttonlist,False);

  default:
    return;

  case Commit:
  if (data->distort.x<0) {
    followrefpoint(data,ort);
    data->distort=ort;
    mess=mSelScndPoint;}
   else {
    sprintf(str,"%g", hypot((double)(data->distort.x-ort.x),
                            (double)(data->distort.y-ort.y))/data->fak);
    mess=str;
    ClearScr();
    drawpatch(data);
    drawnums(data);
    SetCursor(none);
    data->mode=none;
    BusyButtons(buttonlist,False);
   }
  break;
  }


  break;

case speisestift:
  SetCursor(none);
  data->mode=none;
  BusyButtons(buttonlist,False);
  switch (cmd) {
   float tmp;

   case Commit:

    tmp=0.;
    getfloat(input_radius[TXTFLD_RADIUS].wid.t_wid,&tmp,data);
    if (tmp<=0.) {
     printmessg(1,mInvalidRad,input_radius[TXTFLD_RADIUS].wid.t_wid);
     goto abortspeisestift;
    }

    if (CreatePoint(&data->borders,SEL(data),&neu)) return;

    input_radius[0].wid.wid_name=(char*)neu;
    sens_desens(input_radius[TXTFLD_RADIUS].wid.t_wid, True, 0);
    neu->remove_proc=disable_radius;
    neu->ort=ort;
    neu->segs=1;
    neu->len=tmp*data->fak;
    Richtung(SELPOLY(SEL(data)),neu);
    followrefpoint(data,ort);
    modified(data,False);
    drawpatch(zeichnung);
    drawlist(data,-1,-1);
    drawnums(data);
    break;

   case Abort: 
   default: 
   abortspeisestift:
    printmessg(0,0,outputlist[MSSG_M].t_wid);
    unselectall(data);
    return;
  }

  break;

case speise:
  SetCursor(none);
  data->mode=none;
  BusyButtons(buttonlist,False);
  switch (cmd) {
   float s;
   Punkt * ptr;
   case Commit: 
     /* alte Speisung l"oschen */
     if (data->speiseseg) data->speiseseg->speis=0.;
     data->stift.ort.x=-1;

     if (cmd_flags & EM_CANCEL) {
       break;
     }

       (void) SearchLine(&data->borders,ortexakt,&s,
                                          &ptr);
       if (ptr==NULL) return; /* keine Struktur */
       if (s>ptr->len) {
         return;
       }
       ptr->speis=s/ptr->len;
       break;

   case Abort:
   default: 
    /* Abbrechen; je nachdem, ob Stift- oder Segmentspeisung
     * vorliegt, wird der Radius angezeigt, oder nicht!
     */
    return;

   }
   modified(data,False);
   ClearScr();
   drawnums(data);
   break;

case none: 
  switch (cmd) {
   case Commit:
     selpoly= SearchLine(&data->borders,ortexakt,NULL,
                                 &selected);
     /* ist diese Kante bereits selektiert? */

     found=0;
     for (sel=SEL(data);
          sel;
          sel=NEXTSEL(sel)){
      if (SELECTED(sel)==selected) {
       found=1;
       break;
      }
     }
     if (found) {
      unselect(data,&sel);
      selected=(sel) ? SELECTED(sel) : 0 ;
     }else{
      if (CreateEnqListEl(&data->sel,data->sel.lastel,
                          &sel,sizeof(Selection))) return;
      SELPOLY(sel)=selpoly;
      SELECTED(sel)=selected;

     }
      /* wenn nur eine Selektion existiert, und es sich dabei um
       * einen Stift handelt, wird eine Verbindung zum
       * Radius-Eingabefeld hergestellt
       */
      if (data->sel.firstel!=data->sel.lastel) {
       disable_radius(0);
      }else{
       char * strtmp=0;
       Punkt *stift;
       stift= (selected && (NEXT(selected)!=LAST(selected))) ? 0 : selected;
       input_radius[0].wid.wid_name=(char*)stift;
        sens_desens(input_radius[TXTFLD_RADIUS].wid.t_wid,
                    stift,
                    input_haupt[TXTFLD_RAS].wid.t_wid
                    );
        if (stift) {
         sprintf(str,"%g",stift->len/data->fak);
         strtmp=str;
        }
        printmessg(0,strtmp,input_radius[TXTFLD_RADIUS].wid.t_wid);
      }
     break;
   case Abort:
     unselectall(data);
     break;
   default: return;

   }
  ClearScr();
  drawpatch(data);
  drawnums(data);
  break;

default: return;;
} /* switch data->mode */

printort(input_koords,&ort,data); /* offset may have changed */
drawpatch(zeichnung);
drawlist(data,extrax,extray);
drawpoints(data);
printmessg(0,mess,outputlist[MSSG_M].t_wid);


}
   

static void Distance(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;
{
struct hdata * p1;
p1=zeichnung;

if (p1->mode!=none) {XBell(disp,RING);return;}
printmessg(0,mSelFrstPoint,outputlist[MSSG_M].t_wid);
p1->distort.x=-1;
p1->mode=dist;
BusyButtons(buttonlist,True);
SetCursor(dist);
}


static void MovePoint(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
struct hdata * p1;
p1=zeichnung;
if (p1->mode!=none) {XBell(disp,RING);return;}
if (noselection(p1)) {
 printmessg(1,mNoSelection,w);
 return;
}
 printmessg(0,mMoveVector,outputlist[MSSG_M].t_wid);
 SetCursor(move);
 p1->mode=move;
 BusyButtons(buttonlist,True);
 sens_desens(input_radius[TXTFLD_RADIUS].wid.t_wid,
	     False,
 	     input_haupt[TXTFLD_RAS].wid.t_wid
            );
 p1->distort.x=-1;
 p1->translate.x=p1->translate.y=0;
}

static void CloneVect(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;
{
 struct hdata *p1;
 p1=zeichnung;
 if (p1->mode!=none) {XBell(disp,RING);return;}
 p1->distort.x=-1;
 p1->translate.x=p1->translate.y=0;
 BusyButtons(buttonlist,True);
 XtSetSensitive(chboxlist[CHBX_CLONESHOW].t_wid,False);
 SetCursor(p1->mode=klon);
 setflag(chboxlist[CHBX_CLONESHOW].t_wid,1);
 printmessg(0,mCloneVector,outputlist[MSSG_M].t_wid);
}

/* Parameter: Copy, Move or Delete */


static void ModPolygon(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
struct hdata *p1;
char *str;
Selection *sel, *sel1;
char mess[128];
int  ch;

sprintf(mess,fWrnActionPars,nActPolygon);
if (*narg==0) {XtWarning(mess); return;}
switch(ch=toupper((int)*args[0])) {
	case 'C':
	case 'D':
	case 'M': break;
	default : XtWarning(mess); return;
}

str=0;
p1=zeichnung;
if (p1->mode!=none) {XBell(disp,RING);return;}
if (noselection(p1)) {
 printmessg(1,mNoSelection,w);
 return;
}
ElimRedPoly(p1);
p1->distort.x=-1;
p1->translate.x=p1->translate.y=0;


if (ch=='C'){
   BusyButtons(buttonlist,True);
   SetCursor(copypol);
   p1->mode=copypol;
   str=mMoveVector;
}else

if (ch=='M'){
 BusyButtons(buttonlist,True);
 sens_desens(input_radius[TXTFLD_RADIUS].wid.t_wid,
	     False,
 	     input_haupt[TXTFLD_RAS].wid.t_wid
            );
 SetCursor(movepol);
 p1->mode=movepol;
 str=mMoveVector;
}else

if (ch=='D'){
 sel=SEL(p1); 
 while(sel){
  RemovePolygon(&p1->undel,&p1->borders,&sel->selpoly);
  sel1=sel;
  sel=NEXTSEL(sel);
  unselect(p1,&sel1);
  modified(p1,True);
  CBExpose(0,0,0);
 }
}else{ 
return;};
   
printmessg(0,str,outputlist[MSSG_M].t_wid);
}

static void Undelete(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;
{
struct hdata *p1;
register Polygon *poly;
register Punkt   *pnkt,*spseg;

p1=zeichnung;

if (p1->mode!=none) {XBell(disp,RING);return;}

if (!Undel(& p1->undel)) {
 printmessg(1,mUndelNotPoss,outputlist[MSSG_M].t_wid);
 return;
}
/* Wenn im wiedereingef"ugten Element ein Speisesegment enthalten ist,
 * das aktuelle l"oschen
 */

spseg=p1->speiseseg;
for (poly=(Polygon*)p1->borders.firstel; poly; poly=NEXTPOLY(poly)){
 for (pnkt=PLIST(poly); pnkt; pnkt=NEXT(pnkt)) {
  if (pnkt->speis!=0. && pnkt!= spseg) {
    if (spseg) spseg->speis=0.;
    p1->speiseseg=pnkt;
    p1->stift.ort.x=-1;
    break;
  }
 }
}

modified(p1,True);
CBExpose(0,0,0);
}

/* Gibt 1 zur"uck, falls Programmende zugelassen */
int CheckIfQuit(w,p1)

Widget     w;
Zeichnung p1;

{
int ok=0, ok1=0;


if (p1->mode!=none || notices_up) {XBell(disp,RING);return(ok);}

if (ST_VALID(p1,ST_FILE)) {ok=1;}
else {
 switch(Notice(w,0,mWrnFileNotSaved,
               mAckSave,0,
               mAckCancel,1,
               mAckOverride,2,0)){
  case 0: SaveFile(w,0,0,0);
          ok= ST_VALID(p1,ST_FILE);
          break;

  case 2: ok=1;

  case 1:
  default: break;
 }
}

if (!ST_VALID(p1,ST_STRM) ) {ok1=1;}
else {
  switch (Notice(w,0,mWrnLooseResult,
		mAckCancel,0,
		mAckOverride,1,
		0)) {
    case 1:  ok1=1;
    case 0: 
    default: break;
 }
}

return(ok&&ok1);
}


static void Quit(w,ev,args,narg)

Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
if (CheckIfQuit(w,zeichnung)) exit(0);
}

static void New(w, ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
LoadFile(w,0,0,0); /* normalerweise ist ev!=0 */
}

static void LoadFile(w,ev,args,narg)

Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
struct hdata *p1;
char name[200],*nmonly,buff[200];
FILE *tmpfeil;
Arg  arg;
Cardinal dummy;

int newflag=(ev==0);

 p1=zeichnung;
 if (p1->mode!=none) {XBell(disp,RING);return;}

 if (!ST_VALID(p1,ST_FILE)) {
 
  switch (Notice(w,0,
        mWrnGeoModified,
        mAckYes,0,mAckNo,1,mAckCancel,2,0)) {
   case 0: if (Feil==NULL) {
            sprintf(name,"%s",fDefFileName);
            if ((Feil=fopen(name,"w+"))==NULL) {
	       sprintf(buff,fCantWrtFile,name);
               printmessg(1,buff,w);
               return;}
             printmessg(0,name,outputlist[MSSG_FN].t_wid);
             XtSetArg(arg,XtNiconName,name);
             XtSetValues(w_toplevel,&arg,1);
	    }
            rewind(Feil);
	    PunkteSchreiben(Feil,input_haupt,chboxlist);
            fflush(Feil);
            rewind(Feil);
   case 1:
	    break;
   case 2:
            return;

   default: return;}
 }

 if (newflag==0) {/*load*/
  getmessg(input_fname[TXTFLD_FNAME].wid.t_wid,name);
  CleanName(name);

   if ((tmpfeil=fopen(name,"r+"))==NULL) {
    printmessg(1,mFileNotFound,w);
    return;}
   { unsigned int v; char stri[100];
     v=0;
     int got =
     fscanf(tmpfeil,VERSIONFORMAT,&v);
     if (got < 0 || VERSION != v) {
      sprintf(stri,fFileVersionErr,v,VERSION);
      printmessg(1,stri,w);
      return;
     }
   } 
   printmessg(0,0,outputlist[MSSG_M].t_wid);
   if (Feil!=NULL) fclose(Feil);
   Feil=tmpfeil;
   printmessg(0,name,outputlist[MSSG_FN].t_wid);

   /* extract the name only */
   if (0!=(nmonly=strrchr(name,'/'))) nmonly++;
   else nmonly=name;

   XtSetArg(arg,XtNiconName,nmonly);
   XtSetValues(w_toplevel,&arg,1);
 }
   ClearList(0,&p1->borders,&p1->borders.firstel);
   ClearUndel(&p1->undel);
   p1->speiseseg=0;
   p1->stift.ort.x=-1;
   unselectall(p1);
   ClearScr();

  if (newflag==0) {
   PunkteLesen(Feil,input_haupt,chboxlist);
  }else{ /* Neue Struktur */
   dummy=0;
   AddPolygon(w,0,0,&dummy);
  }
   modified(p1,True);
   NOTIFY_ON(p1,ST_FILE);
   if (newflag==0) {
     CBExpose(NULL,NULL,NULL);
   }
}

static void SaveFile(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
struct hdata * p1;

p1=zeichnung;
if ((p1->mode != none)) {XBell(disp,RING);return;}
if (Feil==NULL) {
 printmessg(1,mNoFileLoaded,w);
 return;}
rewind(Feil);
PunkteSchreiben(Feil,input_haupt,chboxlist);
fflush(Feil);
rewind(Feil);
NOTIFY_ON(p1,ST_FILE);
}


static void SaveAs(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;

{
struct hdata * p1;
char name[200],*nmonly;
FILE * tmpfeil;
char buf[100];
Arg  arg;

p1=zeichnung;
if (p1->mode != none) {XBell(disp,RING);return;}
getmessg(input_fname[TXTFLD_FNAME].wid.t_wid,name);
CleanName(name);

if ((tmpfeil=fopen(name,"r"))!=NULL) {
  fclose(tmpfeil);
  switch (Notice(w,0,
    mWrnFileExists,
    mAckYes,0,mAckNo,1,0)) {
   case 0: break;

   default: return;}
}
if ((tmpfeil=fopen(name,"w+"))==NULL) {
  sprintf(buf,fCantWrtFile,name);
  printmessg(1,buf,w);
  return;};

if (Feil!=NULL) fclose(Feil);

Feil=tmpfeil;
printmessg(0,name,outputlist[MSSG_FN].t_wid);
/* extract the name only */
if (0!=(nmonly=strrchr(name,'/'))) nmonly++;
else nmonly=name;
XtSetArg(arg,XtNiconName,nmonly);
XtSetValues(w_toplevel,&arg,1);
SaveFile(w,0,0,0);
}


static void RefPoint(w,ev,args,narg)
Widget w;
XEvent *ev;
String *args;
Cardinal *narg;
{
struct hdata *p1=zeichnung;
if (p1->mode != none) {XBell(disp,RING);return;}
printmessg(0,mSelRefPoint,outputlist[MSSG_M].t_wid);
SetCursor(refpnt);
p1->mode=refpnt;
BusyButtons(buttonlist,True);
}

 
void SetCursor(mode)
 
modes mode;
 
{
Cursor curs;
switch (mode) {
 case add:
 case ins:
 case copypol:  curs=curs_cross; break;

 case dist:     curs=curs_mess;  break;

 case refpnt:
 case speise:   curs=curs_point; break;

 case klon:
 case move:
 case movepol:  curs=curs_move;  break;

 case none:     if (ST_VALID(zeichnung,ST_FELD)){
                 curs=curs_readout;
                 break;
                }
 default  :     curs=curs_none;  break;
}
XDefineCursor(disp,win,curs);
}

 /* wenn das Radius-Eingabefeld keinem Punkt mehr
  * zugeordnet ist, unsensitiv machen 
  */
static void disable_radius(p)
Punkt *p;

{
 input_radius[0].wid.wid_name=0;
 sens_desens(input_radius[TXTFLD_RADIUS].wid.t_wid,
	     False,
 	     input_haupt[TXTFLD_RAS].wid.t_wid
            );
 printmessg(0,0,input_radius[TXTFLD_RADIUS].wid.t_wid);
}

static void CleanName(name)
  char *name;
{ char *stripped=strdup(name+strspn(name," ")); /* strip spaces */
  char *home=getenv("HOME");

  if (*stripped=='~') {
	strcpy(name,home);
	strcat(name,stripped+1);
  } else {
	strcpy(name,stripped);
  }
  free(stripped);
}
   
static void Validate(wid,ev,args,narg)
Widget wid;
XEvent *ev;
String *args;
Cardinal *narg;
{
XEvent sev;
Widget tfw;
tfw=GetTxtFldWid(wid);
if (tfw) {
	sev.xclient.type	=ClientMessage;
	sev.xclient.display	=XtDisplay(tfw);
	sev.xclient.window	=XtWindow(tfw);
	sev.xclient.message_type=XA_STRING;
	sev.xclient.format	=8;
	sprintf(sev.xclient.data.b, "%s", fValidateMess);
	XSendEvent(XtDisplay(tfw),XtWindow(tfw),
		   False,(long)0,&sev);
}
} 

void PatchRegion(p1)
Zeichnung p1;
{
 XRectangle rect;
 if (p1->pregion) XDestroyRegion(p1->pregion); 
 p1->pregion=ScaledRegion(p1,1,1);

 XSetRegion(disp,gcfield,p1->pregion);
 XSetClipOrigin(disp,gcfield,0,0);

 /* Das Patch umschliessendes Rechteck finden */
 XClipBox(p1->pregion,&rect);

 p1->rmin.x=rect.x;
 p1->rmin.y=rect.y;
 p1->rmax.x=p1->rmin.x+rect.width;
 p1->rmax.y=p1->rmin.y+rect.height;
}

Region ScaledRegion(p1,num,denom)
Zeichnung p1;
int	  num,denom;
{
Region pregion;
Region qregion;
Polygon	*borderj;
Punkt	*ptrj;
XPoint	*p;
int	n,npoints,npointsmax;
char	mess[128];

double  fak=(double)num/(double)denom;

npointsmax=0;
for (borderj=(Polygon*)p1->borders.firstel;
     borderj;
     borderj=NEXTPOLY(borderj)) {
  npoints=0;
  for (ptrj=PLIST(borderj); ptrj; ptrj=NEXT(ptrj)) npoints++;
  if (npoints>npointsmax) npointsmax=npoints;
}

if ((p=(XPoint *)Malloc(sizeof(XPoint)*npointsmax))==NULL){
 sprintf(mess,fNoMem,"XCreateRegion");
 printmessg(1,mess,0);
 return 0;
}

pregion=XCreateRegion();

XXorRegion(pregion,pregion,pregion);

for (borderj=(Polygon*)p1->borders.firstel;
     borderj;
     borderj=NEXTPOLY(borderj)) {
 if ((ptrj=PLIST(borderj))==ENDE(borderj)) continue; /* Stifte "uberspringen */

n=0;
if (num==denom)
  while(ptrj!=NULL) {
     p[n]=ptrj->ort;
     ptrj=NEXT(ptrj);
     n++;
  }
else
  while(ptrj!=NULL) {
     p[n].x=(short)irint((double)ptrj->ort.x*fak);
     p[n].y=(short)irint((double)ptrj->ort.y*fak);
     ptrj=NEXT(ptrj);
     n++;
  }

if (n!=0) {
 qregion=XPolygonRegion(p,n,WindingRule);
 XXorRegion(pregion,qregion,pregion);
 XDestroyRegion(qregion);
}
}
Free(p); p=0;

return pregion;
}

void AddGrafActions()
{
 XtAppAddActions(app,actions,XtNumber(actions));
}
