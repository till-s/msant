#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>		/* for XtN and XtC defines */

#ifdef XAW3D
#include <X11/Xaw3d/XawInit.h>		/* for XawInitializeWidgetSet */
#include <X11/Xaw3d/ThreeDP.h>
#else
#include <X11/Xaw/XawInit.h>		/* for XawInitializeWidgetSet */
#endif

#include "PmPannerP.h"			/* us */

#include <X11/Xos.h>
#include <X11/Xmu/Drawing.h>

/*
 * resources for the pmPanner
 */
static XtResource resources[] = {
#define poff(field) XtOffsetOf(PmPannerRec, pmPanner.field)
#ifdef XAW3D
    { XtNshadowWidth, XtCShadowWidth, XtRDimension, sizeof(Dimension),
	poff(shadow_width), XtRImmediate, (XtPointer) 2 },
#else
    { XtNuserData, XtCUserData, XtRPointer, sizeof(XtPointer),
	poff(user_data), XtRPointer, (XtPointer) NULL },
#endif
    { XtNemboss, XtCEmboss, XtRBoolean, sizeof(Boolean),
	poff(emboss), XtRImmediate, (XtPointer) False },
    { XtNtransparentSlider, XtCTransparentSlider, XtRBoolean, sizeof(Boolean),
	poff(transparent_slider), XtRImmediate, (XtPointer) True },
    { XtNresizeCallback, XtCResizeCallback, XtRCallback, sizeof(XtPointer),
	poff(resize_callback), XtRCallback, (XtPointer) NULL },
#undef poff
};


/*
 * widget class methods used below
 */
static void Initialize();		/* create gc's */
static void Destroy();			/* clean up widget */
static void Resize();			/* need to rescale ourselves */
static void Redisplay();		/* draw ourselves */
static Boolean SetValues();		/* set all of the resources */
static XtGeometryResult QueryGeometry();  /* say how big we would like to be */

PmPannerClassRec pmPannerClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass) &pannerClassRec,
    /* class_name		*/	"PmPanner",
    /* widget_size		*/	sizeof(PmPannerRec),
    /* class_initialize		*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	XtInheritRealize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	XtInheritTranslations,
    /* query_geometry		*/	QueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  { /* simple fields */
    /* change_sensitive		*/	XtInheritChangeSensitive
  },
  { /* Panner fields */
    /* ignore			*/	0
  },
  { /* pmPanner fields */
    /* ignore                   */	0
  }
};

WidgetClass pmPannerWidgetClass = (WidgetClass) &pmPannerClassRec;

#define PIXMAP_OKAY(pm) ((pm) != None && (pm) != XtUnspecifiedPixmap)

#define STIPPLE(pw) \
  XmuLocateBitmapFile (pw->core.screen, pw->panner.stipple_name, \
        NULL, 0, NULL, NULL, NULL, NULL)

/*****************************************************************************
 *                                                                           *
 *			    pmPanner utility routines                          *
 *                                                                           *
 *****************************************************************************/

/* callback to register on superclass'es list
 * (this is a hack; because the superclasses
 * actions call a private drawing routine we cannot 
 * interfere with the drawing otherwise)
 */

static void draw_thumb(w,cld,cad)
    Widget w;
    XtPointer cld,cad;
{
 PmPannerWidget pw=(PmPannerWidget)w;
 unsigned int	lw2=(unsigned int)pw->panner.line_width;
 int		pad;
 int		x,y,ib;
 Window		hack,win=XtWindow(w);
 Display	*di=XtDisplay(w);

 if (!XtIsRealized(w)) return;

 ib=pw->panner.internal_border;
 pad=pw->panner.internal_border+lw2;
 lw2*=2;
 x=pw->panner.knob_x+pad-1;
 y=pw->panner.knob_y+pad-1;

 if (!PIXMAP_OKAY(pw->pmPanner.foreground_pixmap)) {
   if (pw->pmPanner.transparent_slider)
     XClearArea(di, win,
	x,y,pw->panner.knob_width-lw2+1,pw->panner.knob_height-lw2+1,False);
#ifdef XAW3D
     hack=pw->pmPanner.threed->core.window;
     pw->pmPanner.threed->core.window=win;
     ((ThreeDWidgetClass)XtClass(pw->pmPanner.threed))->threeD_class.shadowdraw(
	pw->pmPanner.threed,NULL,NULL,pw->pmPanner.emboss);
     pw->pmPanner.threed->core.window=hack;
#endif
   return;
 }

 if (!pw->pmPanner.transparent_slider) return;

 XCopyArea(di,
	pw->pmPanner.foreground_pixmap,win,
	pw->pmPanner.stipple_gc,
	x,y,pw->panner.knob_width-lw2+1,pw->panner.knob_height-lw2+1,x,y);
 /* redraw the rectangle */
 if (pw->panner.line_width)
	XDrawRectangle (di, win, pw->panner.shadow_gc, 
			(int)	       (pw->panner.knob_x+ib),
			(int)	       (pw->panner.knob_y+ib),
		    	(unsigned int) (pw->panner.knob_width - 1), 
		    	(unsigned int) (pw->panner.knob_height - 1));
}

static void reset_stipple_gc (pw)	/* used when resources change */
    PmPannerWidget pw;
{
    XtGCMask  valuemask = GCForeground;
    XGCValues values;
    Pixmap    pm=None;
    Boolean   cached=False;
    Display   *di=XtDisplay((Widget)pw);

    if (pw->pmPanner.stipple_gc) XtReleaseGC ((Widget) pw, pw->pmPanner.stipple_gc);

    values.foreground = pw->panner.foreground;


    if (pw->panner.stipple_name) pm = STIPPLE (pw);
    if (!PIXMAP_OKAY(pm)) {
      pm=XmuCreateStippledPixmap(
		pw->core.screen,
		BlackPixelOfScreen(pw->core.screen),
		WhitePixelOfScreen(pw->core.screen),
		1);
      cached=True;
    }

    values.stipple=pm;
    values.fill_style=FillStippled;
    valuemask |= GCFillStyle|GCStipple;

    pw->pmPanner.stipple_gc = XtGetGC ((Widget) pw, valuemask, &values);
    if (cached)	XmuReleaseStippledPixmap(pw->core.screen,pm);
    else	XFreePixmap(di,pm);
}

static void draw_bg_pixmap(pw,copy)
    PmPannerWidget pw;
    Boolean	   copy;
{
 Display 	*disp=XtDisplay((Widget)pw);
 Pixmap  	pm=pw->core.background_pixmap;
 unsigned int	w,h;
#ifdef XAW3D
 Widget		threed;
 Window		hack;
#endif

 if (!PIXMAP_OKAY(pm)) return;

 if (!pw->pmPanner.stipple_gc) reset_stipple_gc(pw);

 w= pw->pmPanner.pm_width;
 h= pw->pmPanner.pm_height;

 if (copy) XCopyArea(disp,
		pw->pmPanner.foreground_pixmap,pm,
		pw->pmPanner.stipple_gc,
		0,0,w,h,0,0);

 XFillRectangle(disp,pm,pw->pmPanner.stipple_gc,0,0,w,h);

#ifdef XAW3D
  threed=pw->pmPanner.threed;
  hack=XtWindow(threed);
  threed->core.window=pm;
 /* draw the shadows */
 ((ThreeDWidgetClass)threeDWidgetClass)->threeD_class.shadowdraw(threed,
	NULL,NULL,pw->pmPanner.emboss);
 threed->core.window=hack;
#endif
}

/* background_pixmap resource has been changed,
 * release the foreground, get a new copy and draw
 * to the background
 */

static Boolean new_fg_pixmap(pw)
    PmPannerWidget pw;
{
 Window		root;
 int		x,y;
 unsigned int   bw=pw->core.border_width,width=pw->core.width,height=pw->core.height,depth;
 Display	*disp=XtDisplay(pw);
 Pixmap		pm;
 Boolean 	rval=False;

 if (PIXMAP_OKAY(pw->pmPanner.foreground_pixmap)) {
    XFreePixmap(XtDisplay(pw),pw->pmPanner.foreground_pixmap);
    pw->pmPanner.foreground_pixmap=None;
 }

 if (PIXMAP_OKAY(pw->core.background_pixmap)) {

   XGetGeometry(disp,pw->core.background_pixmap,&root,&x,&y,&width,&height,&bw,&depth);
   pw->pmPanner.pm_width=width;
   pw->pmPanner.pm_height=height;

   pm=XCreatePixmap(disp,pw->core.background_pixmap,width,height,depth);

   if (!pw->pmPanner.stipple_gc) reset_stipple_gc(pw);

   XCopyArea(
	disp,
	pw->core.background_pixmap,pm,
	pw->pmPanner.stipple_gc,
	0,0,width,height,0,0);

   pw->pmPanner.foreground_pixmap=pm;
   rval=True;
 }

#ifdef XAW3D
 XtResizeWidget(pw->pmPanner.threed,width,height,bw);
#endif

 draw_bg_pixmap(pw,False);

 return rval;
}


/*****************************************************************************
 *                                                                           *
 * 			     pmPanner class methods                            *
 *                                                                           *
 *****************************************************************************/

static void Redisplay(w,ev,reg)
Widget w;
XEvent *ev;
Region reg;
{
XtSuperclass(w)->core_class.expose(w,ev,reg);
draw_thumb(w,NULL,NULL);
}


/*ARGSUSED*/
static void Initialize (greq, gnew, args, num_args)
    Widget greq, gnew;
    ArgList args;
    Cardinal *num_args;
{
    PmPannerWidget  new = (PmPannerWidget) gnew;

    new->pmPanner.stipple_gc = NULL;
    reset_stipple_gc(new);
    new->pmPanner.foreground_pixmap = None;
#ifdef XAW3D
    new->pmPanner.threed=XtVaCreateWidget(XtName(gnew),threeDWidgetClass,gnew,
	XtNmappedWhenManaged,False,
	XtNbackground,new->core.background_pixel,
	XtNforeground,new->panner.foreground,
	XtNwidth,new->core.width,
	XtNheight,new->core.height,
	XtNborderWidth,new->core.border_width,
	XtNshadowWidth,new->pmPanner.shadow_width,
	NULL);
#endif
    if (new_fg_pixmap(new)) {
      new->core.width=new->pmPanner.pm_width;
      new->core.height=new->pmPanner.pm_height;
      XtSuperclass(gnew)->core_class.resize(gnew); /* adjust everything */
    } 
   
    XtAddCallback(gnew,XtNreportCallback,draw_thumb,NULL);
}

static void Destroy (gw)
    Widget gw;
{
    PmPannerWidget pw = (PmPannerWidget) gw;

    XtReleaseGC (gw, pw->pmPanner.stipple_gc);
    if (PIXMAP_OKAY(pw->pmPanner.foreground_pixmap)) {
	XFreePixmap(XtDisplay(gw),pw->pmPanner.foreground_pixmap);
	pw->pmPanner.foreground_pixmap=None;
    }
#ifdef XAW3D
    XtDestroyWidget(pw->pmPanner.threed);
#endif
}


static void Resize (gw)
    Widget gw;
{
 PmPannerWidget pw = (PmPannerWidget) gw;
 /* give the application a chance to change the pixmap */
 XtCallCallbackList(gw,pw->pmPanner.resize_callback,(XtPointer)0);
 XtSuperclass(gw)->core_class.resize(gw);
#ifdef XAW3D
 if (!PIXMAP_OKAY(pw->core.background_pixmap))
   XtResizeWidget(pw->pmPanner.threed,pw->core.width,pw->core.height,pw->core.border_width);
#endif
}


/* ARGSUSED */
static Boolean SetValues (gcur, greq, gnew, pars, num_pars)
    Widget gcur, greq, gnew;
    ArgList pars;
    Cardinal *num_pars;
{
    PmPannerWidget cur = (PmPannerWidget) gcur;
    PmPannerWidget new = (PmPannerWidget) gnew;
    Boolean redisplay = FALSE;
    Boolean new_gc=False,valid_pixmap=PIXMAP_OKAY(new->core.background_pixmap);
    Boolean new_pixmap=(cur->core.background_pixmap != new->core.background_pixmap);
#ifdef XAW3D
    Arg	    args[10];
    int	    nargs=0;
#endif

    if (cur->pmPanner.emboss!= new->pmPanner.emboss)
	redisplay=True;

    if (cur->panner.foreground != new->panner.foreground ||
        cur->panner.stipple_name != new->panner.stipple_name) {
	reset_stipple_gc(new);
	new_gc=True;
	redisplay=True;
    }

    if (new_pixmap) 	valid_pixmap=new_fg_pixmap(new);
    else if (new_gc)	draw_bg_pixmap(new,True);

    if (valid_pixmap && 
	(new->core.width != cur->pmPanner.pm_width ||
	 new->core.height!= cur->pmPanner.pm_height)) {

	new->core.width=new->pmPanner.pm_width;
	new->core.height=new->pmPanner.pm_height;
        XtSuperclass(gnew)->core_class.resize(gnew); /* adjust everything */
	redisplay=True;
    }

#ifdef XAW3D
    if (cur->panner.foreground != new->panner.foreground) {
       XtSetArg(args[nargs],XtNforeground,new->panner.foreground); nargs++;
    }
    if (cur->core.background_pixel != new->core.background_pixel) {
       XtSetArg(args[nargs],XtNbackground,new->core.background_pixel); nargs++;
       redisplay=True;
    }
    if (cur->pmPanner.shadow_width != new->pmPanner.shadow_width) {
       XtSetArg(args[nargs],XtNshadowWidth,new->pmPanner.shadow_width); nargs++;
    }
    
    if (nargs>0) XtSetValues(new->pmPanner.threed,args,nargs);
#endif
    return redisplay;
}


static XtGeometryResult QueryGeometry (gw, intended, pref)
    Widget gw;
    XtWidgetGeometry *intended, *pref;
{
 PmPannerWidget pw = (PmPannerWidget) gw;

 pref->request_mode = (CWWidth | CWHeight);
 if (PIXMAP_OKAY(gw->core.background_pixmap)) {
    pref->width=pw->pmPanner.pm_width;
    pref->height=pw->pmPanner.pm_height;

    if (((intended->request_mode & (CWWidth | CWHeight)) ==
	 (CWWidth | CWHeight)) &&
	intended->width == pref->width &&
	intended->height == pref->height)
      return XtGeometryYes;
    else if (pref->width == pw->core.width && pref->height == pw->core.height)
      return XtGeometryNo;
    else
      return XtGeometryAlmost;
 } else return XtSuperclass(gw)->core_class.query_geometry(gw,intended,pref);
}
