#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#ifdef XAW3D
#include <X11/Xaw3d/Toggle.h>
#ifdef USE_VIEWPORT
#include <X11/Xaw3d/Viewport.h>
#else
#include <X11/Xaw3d/Porthole.h>
#endif
#else
#include <X11/Xaw/Toggle.h>
#ifdef USE_VIEWPORT
#include <X11/Xaw/Viewport.h>
#else
#include <X11/Xaw/Porthole.h>
#endif
#endif

#include "widget/TextField.h"

#include  "graf.h"
#include  "basics.h"
#include  "CBstrom.h"
#include  "widglist.h"
#include  "msant_strings.h"

#ifndef USE_VIEWPORT
#include  "pmstuff.h"
#endif

#define BELLCHR 7

static void QuitHandler();
static void ShowCanvasHandler();
static void TextFieldFocusOutHandler();

#ifdef USE_VIEWPORT
#define scrollable_class viewportWidgetClass
#else
#define scrollable_class portholeWidgetClass
#endif

void sens_desens(w,flag,fw)

Widget w,fw;
Boolean flag;

/* TextField Widget readonly machen;
   (funktioniert mit devguide nicht)
   den InputFocus nach fw umleiten.
 */
{
XEvent ev;

Widget shell;

shell=w;
while (!XtIsShell(shell=XtParent(shell)));
 
ev.type=KeyPress;

if (flag) {
 XtSetSensitive(w,True);
 XtCallActionProc(w,nActFocusSet,&ev,NULL,0);
} else {
 if (fw) XtCallActionProc(fw,nActFocusSet,&ev,NULL,0);
 XtSetSensitive(w,False);
}
}

void printmessg(bell,str,wid)

 Boolean bell;
 char *str;
 Widget wid;

{
  Arg   argu[5];
  int   j,len;
  TfTextPosition  curs;
  j=0;
  if (str==NULL) {
     if(bell) {
       XBell(XtDisplay(wid),0);
       return;
     }
     str="";
  }


  if (bell) { 
    if (wid) 
     Notice(wid,0,str,mAckCont,0,NULL);
    else {
     fprintf(stderr,"%c\n%s\n",BELLCHR,str);
    }
  }
  else{
   if (XtIsSubclass(wid,labelWidgetClass)) {
     XtSetArg(argu[j],XtNlabel,str);j++;
   } else {
     XtVaGetValues(wid,XtNinsertPosition,&curs,NULL);
     if (curs>(len=strlen(str))){
	curs=(TfTextPosition)len;
        XtSetArg(argu[j],XtNinsertPosition,curs);j++;
     }
     XtSetArg(argu[j],XtNstring,str);j++;
   }
   XtSetValues(wid,argu,j);
  }
}

void getmessg(wid,str1)

Widget wid;
char  *str1;

{
Arg arg;
String str;
if (XtIsSubclass(wid,labelWidgetClass))
 XtSetArg(arg,XtNlabel,&str);
else
 XtSetArg(arg,XtNstring,&str);
XtGetValues(wid,&arg,1);
strcpy(str1,str);
}



int getflag(wid)
Widget wid;
{
Arg arg;
Boolean flag;
XtSetArg(arg,XtNstate,&flag);
XtGetValues(wid,&arg,1);
return((flag==TRUE)?1:0);
}

void setflag(wid,val)
Widget wid;
int    val;
{
Arg arg;
Boolean flag=val?True:False;
XtSetArg(arg,XtNstate,flag);
XtSetValues(wid,&arg,1);
XtCallCallbacks(wid,XtNcallback,0);
}

void init_TextField(wid,inputlist)

Widget wid;
char *inputlist;

{
TextFieldTabEl *tf_list=(TextFieldTabEl*)inputlist;
XtAddEventHandler(wid, FocusChangeMask,False, TextFieldFocusOutHandler,tf_list-1);
XtAddEventHandler(wid, (long)0,True, TextFieldFocusOutHandler,tf_list-1);

/* Das erste Element in der Tabelle enth"alt die Adresse der zugeh"origen
 * struct Variablen 
 */
}

/* obsolet
static void CnvsFocusHandler(wid,cld,ev,cont)
Widget    wid;
XtPointer cld;
XEvent    *ev;
Boolean   *cont;
{
 static Widget hadfocus=0;
 extern Widget w_toplevel;

 switch (ev->type) {
	case EnterNotify:
 	  hadfocus=OlGetCurrentFocusWidget(w_toplevel);
	  OlSetInputFocus(wid,RevertToParent,CurrentTime);
	  break;
	case LeaveNotify:
	  if (hadfocus) OlCallAcceptFocus(hadfocus,CurrentTime);
	  break;
	default: return;
 }
}
*/

/*
 * InputFocus f"ur Canvas setzten (funktioniert mit Golit nicht )
 */
void init_Canvas(wid,inputlist)
Widget wid;
char  *inputlist;
{
WidTabEl *w_list=(WidTabEl*)inputlist;
XtCallbackProc proc;
char 	 mess[64];
int	 i;

for (i=0; i<N_CNVS; i++) {
if (w_list[i].t_wid==wid) {
	switch (i) {
	  case CNVS_GRAFIKFENSTER: proc=CBExpose; break;
	  case CNVS_STROMFENSTER:  proc=CBExpStrom;   break;
	
	  default: sprintf(mess,"Canvas %s not found in list\n",XtName(wid));
		   XtAppError(app,mess);
		   exit(1);
        }
	XtAddEventHandler(wid, ExposureMask, False, ShowCanvasHandler,(XtPointer)proc);
}
}
}



static void TextFieldFocusOutHandler(wid,cld,event,cont)

Widget       wid;
XtPointer    cld;
XEvent    *event;
Boolean    *cont;

{
TextFieldTabEl *inputlist=(TextFieldTabEl*)cld;
char str[80];
int  index;
switch (event->type) {
 default: return;

 case ClientMessage: if (event->xclient.data.b[0]!='H') return;
 case FocusOut: 
	for (index=1; inputlist[index].wid.wid_name; index++) {
		if (inputlist[index].wid.t_wid==wid) break;
	}
	if (inputlist[index].wid.wid_name)
	  if (inputlist[index].update) {
	    storeparam(inputlist,index,zeichnung);
	    return;
	  } else ;
	else {
	  sprintf(str,"Widget '%s' not found in inputlist\n",XtName(wid));
	  XtAppError(app,str);
	  return;
	}
}
}

void BusyButtons(blist,busy)

WidTabEl blist[];
Boolean   busy;

{
int i;
Arg arg;

XtSetArg(arg,XtNsensitive,!busy);

i=-1;
while(blist[++i].wid_name) {
 XtSetValues(blist[i].t_wid,&arg,1);
}

}

#ifdef NEVER
void AdjustView(wid,ortx,orty)
Widget   wid;
Position *ortx,*orty;
{
XawViewportSetCoordinates(GetScrollable(wid),*ortx,*orty);
}
#endif

void deal_with_wmproto(toplevel)
 Widget toplevel;
{
 XSetWMProtocols(XtDisplay(toplevel),XtWindow(toplevel),&A_wm_delete_window,1);
 XtInsertEventHandler(toplevel,0,True,QuitHandler,0,XtListHead);
}

Widget GetTxtFldWid(wid)

Widget wid;
{
 while( wid && !XtIsSubclass(wid,textFieldWidgetClass))
	wid=XtParent(wid);
 return wid;
}

static void QuitHandler(wid,p1,ev,cont)

Widget wid;
XtPointer p1;
XEvent	*ev;
Boolean *cont;

{
static Boolean lock=False;

if (ev->type!=ClientMessage) return;

/* in CheckIfQuit werden Notices erzeugt, welche
 * Events verarbeiten. Der WM schickt offenbar auch
 * bei insensitiver toplevelShell
 * die ClientMessages, welche wir daher mittels eines Flag
 * weglocken
 */
if (!lock && ev->xclient.message_type==A_wm_protocols &&
    *((Atom*)&ev->xclient.data)==A_wm_delete_window) {
 lock=True;
 if ( CheckIfQuit(buttonlist[BUTT_QUIT].t_wid,zeichnung)) exit(0);
 *cont=False;
 lock=False;
}
}

static void ShowCanvasHandler(wid,p,ev,cont)
 Widget    wid;
 XtPointer p;
 XEvent    *ev;
 Boolean   *cont;
{
 XtCallbackProc proc=(XtCallbackProc)p;

 proc(wid,0,0);
}

Widget GetScrollable(wid)
 Widget wid;
{
 while (wid && !XtIsSubclass(wid,scrollable_class)) wid=XtParent(wid);
 return wid;
}

Widget GetClip(wid)
 Widget wid;
{
#ifdef USE_VIEWPORT
 return XtNameToWidget(GetScrollable(wid),"*clip");
#else
 /* assume we are a descendant of porthole */
 return GetScrollable(wid);
#endif
}

/* set the coordinates of the viewport origin
 * relative to the child's origin
 */ 
#ifdef USE_VIEWPORT
#define SetScrolledCoordinates(wid,x,y)\
	XawViewportSetCoordinates(GetScrollable(wid),x,y)
#else
void SetScrolledCoordinates(wid,x,y)
 Widget wid;
 Position x,y;
{
 XtVaSetValues(wid,XtNx,-x,XtNy,-y,NULL);
}
#endif



void AdjustView(wid,ortx,orty)
Widget   wid;
Position *ortx,*orty;
{
Position  x=0,y=0,dx=0,dy=0;
Dimension w,h,wcan,hcan;
Widget   clip;

clip=GetClip(wid);

/* There's a form around the canvas */
XtVaGetValues(wid,XtNwidth,&wcan,XtNheight,&hcan,XtNx,&x,XtNy,&y,NULL);
/* TSILL not anymore
XtVaGetValues(XtParent(wid),XtNx,&x,XtNy,&y,NULL);
*/
x+=dx; y+=dy;
if (*ortx < 0) *ortx=0;
if (*orty < 0) *orty=0;
if (*ortx >= wcan) *ortx=wcan-1;
if (*orty >= hcan) *orty=hcan-1;
/* Clipping Widget */
XtVaGetValues(clip,XtNwidth,&w,XtNheight,&h,NULL);
/* XawViewportSetCoordinates needs offset from
 * canvas origin to viewport origin -- this
 * is the opposite of XWindows coordinates of
 * the canvas relative to the viewport
 */
wcan-=(w-2*dx); hcan-=(h-2*dy);
if ( *ortx+x <= 0 ) {
  x=*ortx-10; 
  if (x<0) x=0;
  SetScrolledCoordinates(wid,x,-(y-dy));
} else if ( *ortx+x >= w ) {
  x=*ortx+10-w;
  if ( x > wcan) x=wcan;
  SetScrolledCoordinates(wid,x,-(y-dy));
}
if ( *orty+y <= 0 ) {
  y=*orty-10;
  if (y<0) y=0;
  SetScrolledCoordinates(wid,-(x-dx),y);
} else if ( *orty+y >= h ) {
  y=*orty+10-h;
  if (y > hcan) y=hcan;
  SetScrolledCoordinates(wid,-(x-dx),y);
}
}

/* update the status of the GUI
 * (toolkit dependent stuff has
 *  to be handled in this module)
 */

void NotifyStatusChanged(z,new)
 Zeichnung z;
 int	   new;
{
int old=z->status;

/* routines called (callbacks etc.) might need
 * the new status info.
 */
z->status=(new& ~(ST_GEOM|ST_REGI));

#ifndef USE_VIEWPORT
if (new & ST_REGI) { 
  /* the region has changed; update the 
   * drawing window panner.
   */
  XtCallCallbacks(canvaslist[CNVS_GRAFIKFENSTER].t_wid,
	XtNcallback,(XtPointer)z);
}

if ((new & ST_STRM) != (old & ST_STRM)) {
  /* the boundary values have changed */
  if (new&ST_STRM) 
    XtCallCallbacks(canvaslist[CNVS_STROMFENSTER].t_wid,
	XtNcallback,(XtPointer)z);
  else
    PmSetCleanPixmap(canvaslist[CNVS_STROMFENSTER].t_wid);
}
#endif
}
