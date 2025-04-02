/*
 * SlPaned.c - SlPaned Composite Widget.
 *
 */
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#ifdef XAW3D
#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/GripP.h>
#include <X11/Xaw3d/ThreeD.h>
#include <X11/Xaw3d/PanedP.h>
#else
#include <X11/Xaw/XawInit.h>
#include <X11/Xaw/GripP.h>
#include <X11/Xaw/PanedP.h>
#endif

#include "SlPanedP.h"

#include <ctype.h>

#define PaneInfo(w)	((Pane)(w)->core.constraints)
#define IsPane(w)       ((w)->core.widget_class != gripWidgetClass)


/*****************************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************************/

#define offset(field) XtOffsetOf(SlPanedRec, slPaned.field)

static XtResource resources[] = {
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
	 offset(foreground), XtRString, (XtPointer) XtDefaultForeground},
    {XtNlineWidth, XtCLineWidth, XtRInt, sizeof(int),
	 offset(line_width), XtRImmediate, (XtPointer) 1},
    {XtNinternalBorderColor, XtCBorderColor, XtRPixel, sizeof(Pixel),
         XtOffsetOf(SlPanedRec,paned.internal_bp), XtRString,
	 (XtPointer) XtDefaultBackground},
    {XtNinternalBorderWidth, XtCBorderWidth, XtRDimension, sizeof(Dimension),
         XtOffsetOf(SlPanedRec,paned.internal_bw), XtRImmediate,
	 (XtPointer) DEFAULT_GRIP_SIZE},

};

#undef offset

/* our callback */
static void our_handle_grip();

static Boolean PaneSetValues();
static void    InsertChild();
static void Initialize();
static void GetGCs(), ReleaseGCs();
static Boolean SetValues();



static XtCallbackProc super_handle_grip=NULL;

#define SuperClass ((PanedWidgetClass)&panedClassRec)

SlPanedClassRec slPanedClassRec = {
   {
/* core class fields */
    /* superclass         */   (WidgetClass) SuperClass,
    /* class name         */   "SlPaned",
    /* size               */   sizeof(SlPanedRec),
    /* class_initialize   */   XawInitializeWidgetSet,
    /* class_part init    */   NULL,
    /* class_inited       */   FALSE,
    /* initialize         */   Initialize,
    /* initialize_hook    */   NULL,
    /* realize            */   XtInheritRealize,
    /* actions            */   NULL,
    /* num_actions        */   0,
    /* resources          */   resources,
    /* resource_count     */   XtNumber(resources),
    /* xrm_class          */   NULLQUARK,
    /* compress_motion    */   TRUE,
    /* compress_exposure  */   TRUE,
    /* compress_enterleave*/   TRUE,
    /* visible_interest   */   FALSE,
    /* destroy            */   ReleaseGCs,
    /* resize             */   XtInheritResize,
    /* expose             */   XtInheritExpose,
    /* set_values         */   SetValues,
    /* set_values_hook    */   NULL,
    /* set_values_almost  */   XtInheritSetValuesAlmost,
    /* get_values_hook    */   NULL,
    /* accept_focus       */   NULL,
    /* version            */   XtVersion,
    /* callback_private   */   NULL,
    /* tm_table           */   NULL,
    /* query_geometry	  */   XtInheritQueryGeometry,
    /* display_accelerator*/   XtInheritDisplayAccelerator,
    /* extension          */   NULL
   }, {
/* composite class fields */
    /* geometry_manager   */   XtInheritGeometryManager,
    /* change_managed     */   XtInheritChangeManaged,
    /* insert_child       */   InsertChild,
    /* delete_child       */   XtInheritDeleteChild,
    /* extension          */   NULL
   }, {
/* constraint class fields */
    /* subresources       */   NULL,
    /* subresource_count  */   0,
    /* constraint_size    */   sizeof(SlPanedConstraintsRec),
    /* initialize         */   NULL,
    /* destroy            */   NULL,
    /* set_values         */   PaneSetValues,
    /* extension          */   NULL
   }
};

WidgetClass slPanedWidgetClass = (WidgetClass) &slPanedClassRec;

/* substitute the callback that handles the grip action for ours */
static void change_grip_callback(w)
 Widget w;
{
XtPointer      cld;
XtCallbackList list;

XtVaGetValues(w,XtNcallback,&list,NULL);
if (list==NULL || list[0].callback==NULL || list[1].callback!=NULL) {
	XtAppError(XtWidgetToApplicationContext(w),
	  "change_grip_callback: not exactly one callback on list");
}

cld=(XtPointer)list[0].closure;
super_handle_grip=list[0].callback;
XtRemoveCallback(w,XtNcallback,super_handle_grip,cld);
XtAddCallback(w,XtNcallback,our_handle_grip,cld);
}

static void
GetGCs(w)
Widget w;
{
 SlPanedWidget pw = (SlPanedWidget)w;
 XtGCMask  msk=GCForeground|GCFunction|GCSubwindowMode|
		GCPlaneMask|GCLineWidth;
 XGCValues vals;

 vals.foreground=pw->slPaned.foreground;
 vals.function  =GXinvert;
 vals.subwindow_mode=IncludeInferiors;
 vals.plane_mask=pw->slPaned.foreground^pw->core.background_pixel;
 vals.line_width=pw->slPaned.line_width;

 pw->slPaned.linegc=XtGetGC(w,msk,&vals);
}


/* ARGSUSED */
static void Initialize(request, new)
Widget request, new;
{
    SlPanedWidget pw = (SlPanedWidget)new;
    GetGCs( (Widget) pw);
}

static void 
ReleaseGCs(w)
Widget w;
{
    SlPanedWidget pw = (SlPanedWidget)w;
    XtReleaseGC( w, pw->slPaned.linegc );
} 

static void InsertChild(w)
register Widget w;
{
   Pane    	  pane=PaneInfo(w);
   /* insert the child widget in the composite children list with the */
   /* superclass insert_child routine.                                */
   (*SuperClass->composite_class.insert_child)(w);

   if (!IsPane(w)) return;

   if (pane->grip) change_grip_callback(pane->grip);

} /* InsertChild */


/* ARGSUSED */
static Boolean 
SetValues(old, request, new, args, num_args)
Widget old, request, new;
ArgList args;
Cardinal *num_args;
{
    SlPanedWidget old_pw = (SlPanedWidget) old;
    SlPanedWidget new_pw = (SlPanedWidget) new;
    Boolean redisplay = FALSE;

    if ( (old_pw->slPaned.foreground != new_pw->slPaned.foreground) ||
    	 (old_pw->slPaned.line_width != new_pw->slPaned.line_width) ||
	 (old_pw->core.background_pixel != new_pw->core.background_pixel)
	 ) {
        ReleaseGCs(old);
	GetGCs(new);
	redisplay=True;
    }

    return (redisplay);
} /* SetValues */


/* ARGSUSED */
static Boolean 
PaneSetValues(old, request, new)
Widget old, request, new;
{
    Pane old_pane = PaneInfo(old);
    Pane new_pane = PaneInfo(new);

    if (new_pane->grip && ! old_pane->grip)
	change_grip_callback(new_pane->grip);

    return(False);
}

static void our_handle_grip(w,cld,cad)
Widget w;
XtPointer cld,cad;
{
  SlPanedWidget		pw = (SlPanedWidget) XtParent(w);
  int			ch=toupper(*(((XawGripCallData)cad)->params[0]));
  int		        ibw2=pw->paned.internal_bw/2;
  Display		*di=XtDisplay(pw);
  Window		wi=XtWindow(pw);
  int			pts[4],*xy;
  Widget		*chlds=pw->composite.children;
  int			npanes=pw->paned.num_panes;
  Boolean		erase=True;
  Pane 		        pane;
  int			*olddelta,i;

  if (pw->paned.orientation == XtorientVertical) {
	pts[0]=0; pts[1]=(int)pw->core.width; xy=pts+2;
  } else {
	pts[2]=0; pts[3]=(int)pw->core.height; xy=pts;
  }
  olddelta=(int*)XtMalloc(sizeof(int)*npanes);

  for (i=0;i<npanes;i++) {
	olddelta[i]=(int)(PaneInfo(chlds[i])->olddelta);
  }

  switch (ch) {
    case 'S':
	erase=False;
	super_handle_grip(w,cld,cad);
	break;
	
    case 'M':
	erase=False;
	super_handle_grip(w,cld,cad);
    case 'C':
	for (i=0;i<npanes;i++) {
	  pane = PaneInfo(chlds[i]);
	  if (erase || (olddelta[i] != pane->delta) ) {
	    if (!erase) {
	      xy[0]=xy[1]=olddelta[i]-ibw2;
	      XDrawLine(di,wi,pw->slPaned.linegc,pts[0],pts[2],pts[1],pts[3]);
	    }
	    xy[0]=xy[1]=(int)(pane->delta-ibw2);
	    XDrawLine(di,wi,pw->slPaned.linegc,pts[0],pts[2],pts[1],pts[3]);
	  }
	}
	if (erase) super_handle_grip(w,cld,cad);
 
    default:
	break;
  }

  XtFree((char*)olddelta);
}
