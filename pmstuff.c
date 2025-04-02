#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xos.h>

#include "widget/PmPanner.h"

#include "basics.h"

#include "graf.h"
#include "CBstrom.h"

#include "pmstuff.h"
#include "pan.h"

void PmSetCleanPixmap(canvas)
 Widget canvas;
{
 PmPannerPixmap pmdata;
 Pixmap		clean,old;

 XtVaGetValues(canvas,XtNuserData,&pmdata,NULL);

 if (pmdata->isclean) return;

 XtVaGetValues(pmdata->panner,XtNbackgroundPixmap,&old,NULL);

 clean=PmCopyCleanPixmap(pmdata);

 XtVaSetValues(pmdata->panner,XtNbackgroundPixmap,clean,NULL);
 if (old!=None) XFreePixmap(XtDisplay(canvas),old);
}

void PmNewPixmap(canvas,cld,cad)
 Widget    canvas;
 XtPointer cld,cad;
{
 Zeichnung p1=(Zeichnung)cad;
 PmPannerPixmap pmdata;
 Widget		panner;
 Pixmap 	new,old;
 Region		clip;
 Display	*di=XtDisplay(canvas);
 Dimension 	pan_w,pan_h,can_w,can_h,sw=0;
 int		cmd=(int)cld,offset;

 XtVaGetValues(canvas,
	XtNuserData,&pmdata,
	XtNwidth,&can_w,
	XtNheight,&can_h,
	NULL);

 panner=pmdata->panner;

 XtVaGetValues(panner,
	XtNwidth,&pan_w,
	XtNheight,&pan_h,
	XtNbackgroundPixmap,&old,
#ifdef XAW3D
	XtNshadowWidth,&sw,
#endif
	NULL);


 new=PmCopyCleanPixmap(pmdata);

 switch(cmd) {
	case (int)PM_NEW_GRAF_PIXMAP:

 	  clip=ScaledRegion(p1,pan_w-2*sw,can_w-2*sw);
 	  XOffsetRegion(clip,sw,sw);
 	  XSetRegion(di,gcfield,clip);
 	  XSetForeground(di,gcfield,p1->foreground);
 
 	  XFillRectangle(di,new,gcfield,0,0,pmdata->width,pmdata->height);

 	  XSetRegion(di,gcfield,p1->pregion);
 	  XDestroyRegion(clip);

	  break;
	
	case (int)PM_NEW_STROM_PIXMAP:
	  offset=sw+20*pan_w/can_w;
	  drawstrom(p1,new,pan_w-2*offset,pan_h-2*offset,
		offset,pan_h-offset,
		-1,-1);
	  break;
	
	default: 
	  XFreePixmap(di,new);
	  XtAppWarning(app,
		"PmNewPixmap: callback registered with wrong client_data arg");
	
	  return;
  }

 XtVaSetValues(panner,XtNbackgroundPixmap,new,NULL);
 pmdata->isclean=False;

 if (old!=None) XFreePixmap(di,old);
}
