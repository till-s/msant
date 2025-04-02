#include <stdio.h>
#include <sys/types.h>
/* #include <varargs.h> */
#include <stdarg.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#ifdef OPENLOOK
#include <Xol/OpenLook.h>
#include <Xol/OblongButt.h>
#include <Xol/StaticText.h>
#include <Xol/Notice.h>
#include <Xol/ControlAre.h>
#include <Xol/Caption.h>
#else
#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#endif

#include "Malloc.h"
#include "popup.h"
#include "msant_strings.h"

#ifdef   NOTICE
#define  LABEL_RESOURCE XtNstring
#else
#define  LABEL_RESOURCE XtNlabel
#endif

/*
#define POPUP_DEBUG
*/

typedef struct notice_data_{
    caddr_t retVal;
    Boolean done,bell;
    Widget  wid;
    struct _Choice *subwid_list;
    } notice_data;

typedef struct _Choice{
    struct _Choice *next;
    Widget wid;
    struct mix{
     notice_data * ptr1;
     caddr_t       clientdata;
     } stru;
    } Choice;

int	notices_up=0;
static XtAppContext app=0;

static void		CBUp();
static void		CBDummy();
#ifdef NOT_USED
static Dimension	GetWidth();
static void		SetWidth();
#endif
static NoticeInstance	CreateNoticeWithPointer(Widget,va_list);
static void		NoticeCB();
static void		FreeList();
static void		raise_handler();

static void CBUp(wid,cld,cad)
Widget wid;
XtPointer cld,cad;
{
int up=(int)cld;
#ifdef POPUP_DEBUG
fprintf(stderr,"callback: %i\n",up);
#endif
if (up) notices_up++;
else    notices_up--;
if (notices_up<0) XtWarning("notices_up lt 0\n");
}


static void CBDummy(wid,cld,cad)
Widget wid;
XtPointer cld,cad;
{
}

#ifdef NOT_USED
static Dimension GetWidth(wid)
Widget wid;
{
 static Dimension w,sw;
 static Arg args[2]={{"shadowWidth",(XtArgVal)&sw},{XtNwidth,(XtArgVal)&w}};
 sw=0;
 XtGetValues(wid,args,2);
 printf("%s: w %u, sw %u\n",XtName(wid),w,sw);
 return w+2*sw;
}

static void SetWidth(wid,w)
Widget wid;
Dimension w;
{
 static Dimension sw;
 static Arg args[2]={{"shadowWidth",(XtArgVal)&sw},{XtNwidth,(XtArgVal)0}};
 sw=0;
 XtGetValues(wid,args,1);
 args[1].value=(XtArgVal)(w-2*sw);
 XtSetValues(wid,args+1,1);
}
#endif 
 

int Notice(Widget top, ...)
{va_list ap;
 NoticeInstance ndp;
 int rval;

 va_start(ap,top);
 ndp=CreateNoticeWithPointer(top, ap);
 va_end(ap);

#ifndef NOTICE
 XBell(XtDisplay(ndp->wid),0);
#endif

 XFlush(XtDisplay(ndp->wid));
 while(XtAppPending(app)) XtAppProcessEvent(app,XtIMAll);

 XtPopup(ndp->wid,XtGrabExclusive);

 for(ndp->done=FALSE;ndp->done!=TRUE;) {
  XEvent xevent;

  XtAppNextEvent(app,&xevent);
  XtDispatchEvent(&xevent);
  }
 
 rval=(int)ndp->retVal;

 DestroyNotice(ndp);

 return(rval);
 }

XtCallbackRec cbl[]={
 {CBDummy,(caddr_t)NULL},
 {(XtCallbackProc) NULL, (caddr_t)NULL},
};

NoticeInstance CreateNotice(Widget top, ...)
{
va_list ap;
NoticeInstance rval;

va_start(ap,top);
rval=CreateNoticeWithPointer(top,ap);
va_end(ap);
return(rval);
}

static NoticeInstance CreateNoticeWithPointer(Widget wid, va_list ap)
{
register Choice *pFirst=NULL,*pLast=NULL, *pChoice;
String defname="notice",name;
Widget w_text,w_control;
static void NoticeCB(),FreeList();
static void raise_handler();
String label;
Dimension max_width,width,scratch1;
int	  def_dist;
Arg args[10];
int j,nbuttons;

NoticeInstance ndp;


if ((name=va_arg(ap,String))==0) name=defname;

/* beim ersten Aufruf app setzten */
if (app==0) {
 app=XtDisplayToApplicationContext(XtDisplay(wid));
}

ndp=(NoticeInstance)Malloc(sizeof(notice_data));

j=0;
XtSetArg(args[j],XtNmappedWhenManaged,False);j++;
#ifdef OPENLOOK
#ifdef NOTICE
XtSetArg(args[j],XtNemanateWidget,wid);j++;
#else
XtSetArg(args[j],XtNwindowHeader,False);j++;
XtSetArg(args[j],XtNresizeCorners,False);j++;
XtSetArg(args[j],XtNwmProtocol,cbl);j++;
#endif
#endif


ndp->wid=
    XtCreatePopupShell(
	    name,
#ifdef NOTICE
	    noticeShellWidgetClass,
#else
	    transientShellWidgetClass,
#endif
	    wid,args,j);

#ifdef NOTICE
j=0;
XtSetArg(args[j],XtNtextArea,&w_text);j++;
XtSetArg(args[j],XtNcontrolArea,&w_control);j++;
XtGetValues(ndp->wid,args,j);
#else

XtAddEventHandler(ndp->wid,VisibilityChangeMask,False,raise_handler,0);

#ifdef OPENLOOK
j=0;
XtSetArg(args[j],XtNposition,OL_TOP);j++;
XtSetArg(args[j],LABEL_RESOURCE,va_arg(ap,String));j++;
w_text=XtCreateManagedWidget("text",
	captionWidgetClass,ndp->wid,
	args,j);
j=0;
w_control=XtCreateManagedWidget("control",controlAreaWidgetClass,
		w_text,args,j);
#else
j=0;
w_control=XtCreateManagedWidget("control",formWidgetClass,
		ndp->wid,args,j);

j=0;
XtSetArg(args[j],LABEL_RESOURCE,va_arg(ap,String));j++;
XtSetArg(args[j],XtNborderWidth,0);j++;
w_text=XtCreateManagedWidget("text",
	labelWidgetClass,w_control,
	args,j);

#endif
#endif


nbuttons=0;
max_width=0;
while((label=va_arg(ap,String))!= 0) {
 pChoice=(Choice*) Malloc(sizeof(Choice));
 if(pChoice==NULL){
  fprintf(stderr,fNoMem,"Notice()");
  fprintf(stderr,"\n");
  exit(1);}
 
 pChoice->next=NULL;
 pChoice->stru.ptr1=ndp;
 pChoice->stru.clientdata=va_arg(ap,caddr_t);

 j=0;
 XtSetArg(args[j],XtNlabel,label);j++;
#ifndef OPENLOOK
 XtSetArg(args[j],XtNfromVert,w_text);j++;
 XtSetArg(args[j],XtNborderWidth,0);j++;
 if (pLast) {XtSetArg(args[j],XtNfromHoriz,pLast->wid);j++;}
#endif
 pChoice->wid=
  XtCreateManagedWidget(
    label,
#ifdef OPENLOOK
    oblongButtonWidgetClass,
#else
    commandWidgetClass,
#endif
    w_control,args,j);

    XtVaGetValues(pChoice->wid,XtNwidth,&width,0);
    if (max_width< width) max_width=width;

 XtAddCallback(
    pChoice->wid,
#ifdef OPENLOOK
    XtNselect,
#else
    XtNcallback,
#endif
    NoticeCB,
    &pChoice->stru);
 XtAddCallback(ndp->wid,XtNpopupCallback,CBUp,(XtPointer)-1);
 XtAddCallback(ndp->wid,XtNpopdownCallback,CBUp,(XtPointer)0);

 nbuttons++;
 if (pFirst==NULL) pFirst=pChoice;
  else pLast->next=pChoice;
 pLast=pChoice;
 }

j=0;
#ifndef OPENLOOK
XtVaGetValues(pFirst->wid,XtNhorizDistance,&def_dist,0);
scratch1=nbuttons*(max_width+(Dimension)def_dist)-(Dimension)def_dist;


XtVaGetValues(w_text,XtNwidth,&width,0);

if ( width > scratch1 ) {
  width=(width-nbuttons*max_width)/(nbuttons+1);
  XtSetArg(args[j],XtNhorizDistance,width); j++;
  XtVaGetValues(w_control,XtNdefaultDistance,&def_dist,0);
  XtVaSetValues(pFirst->wid,XtNhorizDistance,width+(Dimension)def_dist,0);
} else {
  XtVaSetValues(w_text,XtNwidth,scratch1,0);
}
#endif

XtSetArg(args[j],XtNwidth,max_width); j++;
for (pChoice=pFirst; pChoice; pChoice=pChoice->next) {
  XtSetValues(pChoice->wid,args,j);
}


XtRealizeWidget(ndp->wid);
PlaceNotice(ndp,wid);
ndp->subwid_list=pFirst;
return (ndp);
}


static void NoticeCB(wid,client_data,call_data)
   Widget wid;
   struct mix * client_data;
   caddr_t call_data;

{
  client_data->ptr1->retVal=client_data->clientdata;
  client_data->ptr1->done=TRUE;
  XtPopdown(client_data->ptr1->wid);
  return;
}

static void FreeList(list)
  Choice *list;

{
Choice *ptr,*next;
 ptr=list;
 while(ptr!=NULL) {
  next=ptr->next;
  Free(ptr);
  ptr=next;}
}

void DestroyNotice(ndp)

NoticeInstance ndp;

{
 FreeList(ndp->subwid_list);
 XtDestroyWidget(ndp->wid);
 Free(ndp);
}

static void raise_handler(w,cld,ev,ctd)
 Widget w;
 XtPointer cld;
 XEvent *ev;
 Boolean *ctd;
{
 register int state=ev->xvisibility.state;
 if(state==VisibilityFullyObscured ||
    state==VisibilityPartiallyObscured)
  XRaiseWindow(XtDisplay(w),XtWindow(w));
}


void PlaceNotice(ndp,emanate)
 NoticeInstance ndp;
 Widget emanate;

#ifdef NOTICE
{
 Arg arg;
 XtSetArg(arg,XtNemanateWidget,emanate);
 XtSetValues(wid,&arg,1);
}
#else

{
 static Dimension scr[]={0,0};
 Position         pos[2],neupos[2];
 Dimension        wi[2],em[2],bw;
 Arg		  args[5];
 int		  ac,i;
 Widget		  wid=ndp->wid;

 if (scr[0]==0) {
   scr[0]=WidthOfScreen(XtScreen(wid));
   scr[1]=HeightOfScreen(XtScreen(wid));
 }

 ac=0;
 XtSetArg(args[ac],XtNwidth,&wi[0]); ac++;
 XtSetArg(args[ac],XtNheight,&wi[1]); ac++;
 XtSetArg(args[ac],XtNborderWidth,&bw); ac++;

 XtGetValues(emanate,args,ac);
 bw+=bw;
 for (i=0;i<2;i++) em[i]=wi[i]+bw;

 XtGetValues(wid,args,ac);
 bw+=bw;
 for (i=0;i<2;i++) wi[i]+=bw;

 XtTranslateCoords(emanate,0,0,&pos[0],&pos[1]);

 for (i=0; i<2; i++) {
    neupos[i]=pos[i]+em[i];
    if (neupos[i]+(Position)wi[i] > (Position)scr[i]) { /* hats rechts Platz? */
   /* nein! */
    neupos[i]=pos[i]-(Position)wi[i];
    }

    if (neupos[i]<0)        neupos[i]=0;
    if (neupos[i]+(Position)wi[i]>(Position)scr[i]) 
	neupos[i]=(Position)(scr[i]-wi[i]);
 }
 

 ac=0;
 XtSetArg(args[ac],XtNx,neupos[0]); ac++;
 XtSetArg(args[ac],XtNy,neupos[1]); ac++;
 XtSetValues(wid,args,ac);
}

void NoticePopup(ndp)
 NoticeInstance ndp;
{
 ndp->done=False;
 XtPopup(ndp->wid,XtGrabExclusive);
#ifdef POPUP_DEBUG
fprintf(stderr,"notice popped up\n");
#endif
}


void NoticePopdown(ndp)
 NoticeInstance ndp;
{
 XtPopdown(ndp->wid);
#ifdef POPUP_DEBUG
fprintf(stderr,"notice popped down\n");
#endif
}

Boolean NoticeDone(ndp)
 NoticeInstance ndp;
{
 return ndp->done;
}

int NoticeRval(ndp)
 NoticeInstance ndp;
{
 return (int)ndp->retVal;
}

#endif
