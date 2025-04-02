#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xos.h>

#ifdef XAW3D
#include <X11/Xaw3d/Porthole.h>
#else
#include <X11/Xaw/Porthole.h>
#endif

#include "widget/PmPanner.h"
#include "pan.h"

static void DestroyData();

static void
PortholeCallback(w, panner_ptr, report_ptr)
Widget w;
XtPointer panner_ptr, report_ptr;
{
    Arg args[10];
    Cardinal n = 0;
/*
    Dimension scale;
*/
    XawPannerReport *report = (XawPannerReport *) report_ptr;
    Widget panner = (Widget) panner_ptr;

    XtSetArg (args[n], XtNsliderX, report->slider_x); n++;
    XtSetArg (args[n], XtNsliderY, report->slider_y); n++;
    if (report->changed & ~(XawPRSliderX | XawPRSliderY)) {
/*
	*
	* could change the panner size here if canvas was
	* resized.
	*
        XtVaGetValues(panner,XtNdefaultScale,&scale,0);
        XtSetArg (args[n], XtNheight, report->canvas_height*scale/100); n++;
        XtSetArg (args[n], XtNwidth, report->canvas_width*scale/100); n++;
*/
        XtSetArg (args[n], XtNsliderWidth, report->slider_width); n++;
        XtSetArg (args[n], XtNsliderHeight, report->slider_height); n++;
        XtSetArg (args[n], XtNcanvasWidth, report->canvas_width); n++;
        XtSetArg (args[n], XtNcanvasHeight, report->canvas_height); n++;
    }
    XtSetValues (panner, args, n);
}

static void
PannerCallback(w, closure, report_ptr)
Widget w;
XtPointer closure, report_ptr;
{
    Widget panned=(Widget)closure;
    Arg args[2];
    XawPannerReport *report = (XawPannerReport *) report_ptr;

    XtSetArg (args[0], XtNx, -report->slider_x);
    XtSetArg (args[1], XtNy, -report->slider_y);

    XtSetValues(panned, args, 2);
}

Widget CreatePmPanner(name,parent,canvas,w,h,args,nargs)
 char 		*name;
 Widget		parent,canvas;
 Dimension	*w,*h;
 Arg		*args;
 int		nargs;
{
 Widget    porthole,panner;
 Dimension can_w,can_h,sw=0,pw,ph,bw=0,bord;
 Dimension st,ib;
 float	   vh,vw;
 PmPannerPixmap data;
 Screen	   *screen;
 int	   depth;
 GC		   gc;
 XtGCMask  msk=GCForeground;
 XGCValues vals;
 Pixel	   bg;
 Pixmap    bg_pm;

 porthole=canvas;
 while (porthole && !XtIsSubclass(porthole,portholeWidgetClass))
  porthole=XtParent(porthole);

 if (!porthole) {
	XtAppError(XtWidgetToApplicationContext(parent),
	  "CreatePmPanner: canvas is not a child of a porthole widget");
 }

 panner=XtCreateManagedWidget(name,pmPannerWidgetClass,parent,args,nargs);
 XtVaGetValues(panner,
	XtNshadowWidth,&sw,
	XtNborderWidth,&bw,
	XtNinternalSpace,&ib,
	XtNshadowThickness,&st,
	NULL);
 
 if (sw>0) {
   if (st>sw) st=sw;
   ib=sw;
   bw=0;
 } else ib=st;
 XtVaSetValues(panner,
	XtNborderWidth,bw,
	XtNinternalSpace,ib,
	XtNshadowThickness,st,
	NULL);

 bord=2*(sw+bw);

 pw=*w; ph=*h;

 if (pw>=bord) pw-=bord; else pw=0;
 if (ph>=bord) ph-=bord; else ph=0;

 XtVaGetValues(canvas,
	XtNwidth,&can_w,XtNheight,&can_h,
	XtNscreen,&screen,XtNdepth,&depth,
	XtNbackground,&bg,
	NULL);
 
 if (can_w!=0 && can_h!=0) {
   vw=(float)pw/(float)can_w; vh=(float)ph/(float)can_h;

   if (vw==0) vw=vh;
   if (vh==0) vh=vw;
   if (vh>vw) vh=vw; else vw=vh;

   pw=(Dimension)((float)vw*(float)can_w);
   ph=(Dimension)((float)vh*(float)can_h);

   *w=pw+bord; *h=ph+bord;

   XtVaSetValues(panner,
	XtNwidth,*w,XtNheight,*h,
	XtNcanvasWidth,can_w,XtNcanvasHeight,can_h,
	NULL);
 } /* else defer the size calculation for later */

 XtAddCallback(panner,XtNreportCallback,PannerCallback,
	(XtPointer)canvas);
 XtAddCallback(porthole,XtNreportCallback,PortholeCallback,
	(XtPointer)panner);

 data=(PmPannerPixmap)XtMalloc(sizeof(PmPannerPixmapRec));

 bg_pm=XCreatePixmap(XtDisplay(canvas),
		RootWindowOfScreen(screen),
		*w,*h,depth);
 data->clean_pm=XCreatePixmap(XtDisplay(canvas),
		RootWindowOfScreen(screen),
		*w,*h,depth);
 vals.foreground=bg;
 gc=XtGetGC(canvas,msk,&vals);
 XFillRectangle(XtDisplay(canvas),data->clean_pm,gc,0,0,*w,*h);
 XFillRectangle(XtDisplay(canvas),bg_pm,gc,0,0,*w,*h);
 XtReleaseGC(canvas,gc);
 data->width=*w;
 data->height=*h;
 data->depth=depth;
 data->isclean=True;

 data->panner=panner;
 XtVaSetValues(canvas,XtNuserData,(XtPointer)data,NULL);
 XtVaSetValues(panner,XtNbackgroundPixmap,bg_pm, NULL);

 XtAddCallback(canvas,XtNdestroyCallback,DestroyData,NULL);
 return panner;
}

static void DestroyData(wid,cld,cad)
 Widget wid;
 XtPointer cld,cad;
{
 PmPannerPixmap pmdata;
 XtVaGetValues(wid,XtNuserData,&pmdata,0);

 if (pmdata->clean_pm!=None)
   XFreePixmap(XtDisplay(wid),pmdata->clean_pm);

 XtFree((char*)pmdata);
}

Pixmap PmCopyCleanPixmap(pmdata)
 PmPannerPixmap pmdata;
{
 Widget panner=pmdata->panner;
 Pixmap new_pixmap;
 new_pixmap=XCreatePixmap(XtDisplay(panner),
		RootWindowOfScreen(XtScreen(panner)),
		pmdata->width,
		pmdata->height,
		pmdata->depth);

 XCopyArea(XtDisplay(panner),pmdata->clean_pm,new_pixmap,
	DefaultGCOfScreen(XtScreen(panner)),
	0,0,
	pmdata->width,pmdata->height,
	0,0);
 return new_pixmap;
}
