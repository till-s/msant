/*
 * $Id: VpFrame.c,v 1.9 2000/10/02 22:20:05 cvs Exp $
 */

#ifdef THISISACOMMENT
 *
 * The VpFrame Widget
 *
 *  Copyright (C) 1997  by Till Straumann   <strauman@sun6hft.ee.tu-berlin.de>

 *
 * This widget provides 3d look to the viewport widget.
 * A ThreeD widget creates an additional subwindow which
 * is used as the actual clip window. The threeD itself
 * is used to draw the shadows.
 * 
 * Reparent notify events are intercepted to 'rereparent' the
 * clipped window (the viewport child that can be scrolled).
 *
 * a resource 'embossed' is provided to control the appearance,
 * i.e. the viewport child appears embossed if this resource is
 * set true (default).
 *
 * If you can modify the 'Viewport' code, look for the line
 * where the 'clip' widget is created and substitute 'widgetClass'
 * for 'vpFrameWidgetClass'. An include directive 
 *
 * #include "VpFrame.h"
 * 
 * has to be inserted also.
 *
 * As an alternative, the hack as follows may be used when creating
 * an instance of viewport:
 *
 * WidgetClass hack=widgetClass;
 * Widget      viewport;
 * 
 * /* make the viewport code get a widget of class 'VpFrameWidgetClass'
 *  * when it creates its 'clip' child
 *  */
 * widgetClass=vpFrameWidgetClass;
 * viewport=XtCreateManagedWidget("",....);
 * /* reinstall the original widget class */
 * widgetClass=hack;
 *
 * COPYRIGHT NOTICE:
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Library Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Library Public License for more details.

 *  You should have received a copy of the GNU General Library Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Log: VpFrame.c,v $
 * Revision 1.9  2000/10/02 22:20:05  cvs
 *  - keep compilers that complain about empty source files happy...
 *
 * Revision 1.8  2000/06/24 20:38:49  till
 *  - leave VpFrame in the library; just comment out everything ifndef XAW3D
 *
 * Revision 1.7  1999/10/28 18:11:55  cvs
 *  - another bug in Viewport :-(. ReparentHandler(): call XReparentWindow
 *    only if findVpChild() succeeds.
 *
 * Revision 1.6  1999/10/18 22:37:57  cvs
 *  - Imakefile supports building a (static) library now.
 *  - all (resource etc.) strings go to strDefs.c if SEPARATE_STRINGS is defined.
 *
 * Revision 1.5  1998/07/27 16:50:51  cvs
 * set origin of reparented window
 *
 * Revision 1.4  1998/05/20 19:07:12  till
 * no unbalanced quotes in comment (cvs log msg)
 *
 * Revision 1.3  1998/05/10 22:59:22  till
 * if Viewport.c is _not_ patched, we must catch reparenting events
 * on viewports window.
 *
 * Revision 1.2  1998/05/02 16:37:25  till
 * comment contained a /* comment */; replaced outer /**/ by ifdef
 *
 * Revision 1.1  1998/05/02 15:06:40  till
 * added many comments and usage info
 * renamed to VpFrameWidgetClass to avoid possible name clashes
 *
 * Revision 1.4  1998/05/02 13:23:03  till
 * added UnrealizeCB to unregister the EventHandler
 *
 * Revision 1.3  1998/04/29 21:19:23  till
 * re-added expose method
 *
 * Revision 1.2  1998/04/27 22:34:13  till
 *  fixed some 'unused variable' kind of things
 *
 * Revision 1.1  1998/04/27 22:20:49  till
 * - created the 'vpFrame' widget to provide 3d look to the viewport
 *   (and possibly other widgets)
 *
 
#endif /* THISISACOMMENT */

#ifdef XAW3D

#include <X11/Xlib.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include <X11/Xaw3d/XawInit.h>
#include "VpFrameP.h"
#include <X11/Xosdefs.h>

#include <X11/Xaw3d/Viewport.h>
#ifdef DEBUG
#include <stdio.h>
#endif

/* Initialization of defaults */

static void Destroy(), Realize(), Resize(), DrawShadows(), Redisplay(),
	    ReparentHandler(),Initialize(),UnrealizeCB(); 

#ifndef SEPARATE_STRINGS
/* include the automatically generated strings of resource names */
#ifdef __VMS
#include "VpFrame_res_c."
#include "VpFrame_class_c."
#include "VpFrame_type_c."
#else
#include "VpFrame_res_c"
#include "VpFrame_class_c"
#include "VpFrame_type_c"
#endif /* __VMS */
#endif

/* resources */
static XtResource resources[]={
	{XtNembossed,XtCEmbossed,XtRBoolean,sizeof(Boolean),
		XtOffsetOf(VpFrameRec,vpFrame.embossed),XtRImmediate,(XtPointer)True},
	{XtNunrealizeCallback,XtCCallback,XtRCallback,sizeof(XtPointer),
		XtOffsetOf(VpFrameRec,vpFrame.unrealize_cbl),XtRCallback,(XtPointer)NULL},
};

VpFrameClassRec vpFrameClassRec = {
    { /* core fields */
    /* superclass		*/	(WidgetClass) &threeDClassRec,
    /* class_name		*/	"VpFrame",
    /* widget_size		*/	sizeof(VpFrameRec),
    /* class_initialize		*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* resource_count		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	NULL,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	NULL,
    /* query_geometry           */	XtInheritQueryGeometry,
    /* display_accelerator      */	XtInheritDisplayAccelerator,
    /* extension                */	NULL
    },
    { /* simple fields */
    /* change_sensitive         */      XtInheritChangeSensitive
    },
    { /* threeD fields */
    /* shadow draw              */      DrawShadows
    },
    { /* vpFrame fields */
    /* happy */      			NULL
    }
};

WidgetClass vpFrameWidgetClass = (WidgetClass) &vpFrameClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static Widget findViewportChild(VpFrameWidget fw, Window win);


static void Realize (gw, valueMask, attrs)
    Widget gw;
    XtValueMask *valueMask;
    XSetWindowAttributes *attrs;
{
VpFrameWidget fw=(VpFrameWidget)gw;
Dimension   sw=fw->threeD.shadow_width;
Display	    *d=XtDisplay(gw);

 /* 
  * daisychain through the superclasses
  */
 (*vpFrameWidgetClass->core_class.superclass->core_class.realize)
	 (gw, valueMask, attrs);

 /* create the new clip window */
 fw->vpFrame.clipwin=XCreateSimpleWindow(d,
					XtWindow(gw),
					sw,sw,
					fw->core.width-2*sw,
					fw->core.height-2*sw,
					0,0,
					fw->core.background_pixel);
 XMapWindow(d,fw->vpFrame.clipwin);

 /* add event handler to catch the reparenting */

 XtAddEventHandler(gw,SubstructureNotifyMask,False,ReparentHandler,0);

 /* add callback to remove the event handler if the widget is
  * ever unrealized.
  */
 XtAddCallback(gw,XtNunrealizeCallback,UnrealizeCB,(XtPointer)0);
}

static void Destroy (gw)
     Widget gw;
{
    VpFrameWidget w = (VpFrameWidget) gw;
    if (!XtIsRealized(gw)) return;
    XDestroyWindow(XtDisplay(gw),w->vpFrame.clipwin);
    XtRemoveEventHandler(gw,SubstructureNotifyMask,True,ReparentHandler,0);
}

/* we need a resize method to resize the clipping window (Xt knows 
 * nothing about it) if we are resized
 */
static void Resize (w)
Widget w;
{
VpFrameWidget fw=(VpFrameWidget)w;
Dimension   sw=fw->threeD.shadow_width;
Display	    *d=XtDisplay(w);

  if (!XtIsRealized(w)) return;
  
  XMoveResizeWindow(d,fw->vpFrame.clipwin,
			sw,sw,
			fw->core.width-2*sw,fw->core.height-2*sw);
}


/* we want our own drawShadows method because braindamaged
 * ThreeD class provides no resource to control the appearance
 * (i.e. 'in' or 'out')
 */
static void DrawShadows(w,ev,r)
	Widget w;
	XEvent *ev;
	Region r;
{
VpFrameWidget fw=(VpFrameWidget)w;

/* 
 * use the embossed resource
 */
 (*((ThreeDWidgetClass)threeDWidgetClass)->threeD_class.shadowdraw)(w, ev, r, 
		! fw->vpFrame.embossed);
}

/*
 * we know something we shouldn't know about (because it's encapsulated
 * in the viewport implementation code) -- therefore the whole thing with
 * this 'VpFrameWidget' must be considered a hack.
 * What we know is, that the viewport creates a child called 'clip' and
 * that the viewport's child will get its window reparented to clip's
 * window, so only the part contained in the latter is visible.
 *
 * What we do is fooling the viewport to create clip, not of class
 * 'WidgetClass', but of class 'VpFrameWidgetClass'. (We do this by
 * hacking the 'widgetClass' global variable as described at the top.)
 * The VpFrame has an additional window which serves as the actual clipping
 * window.
 * This event handler catches the 'ReparentNotify' event which is generated
 * when the viewport's child is reparented and reparents it once more, making
 * this time VpFrame's clipping window the real parent window.
 *
 * NOTE: The buggy viewport sets its core.window field to clip's window. There-
 *		 fore destroying viewport's child and recreating a new one results in
 *		 the new child's window being a child of clip's window. That's why we
 *		 must catch creation events as well.
 */

static void ReparentHandler(w,cld,ev,ctd)
	Widget w;
	XtPointer cld;
	XEvent *ev;
	Boolean *ctd;
{
VpFrameWidget fw=(VpFrameWidget)w;
Window		  target;
#ifdef DEBUG
Window		  oldpar;
#endif
Position	  x=0,y=0;
Widget		  vpChild;

/* ReparentNotify is selected with the SubstructureNotifyMask, so we must
 * make sure it's the event we are interested in
 */
	switch(ev->type) {
		case ReparentNotify:
			if (ev->xreparent.parent==fw->vpFrame.clipwin) return;
			target=ev->xreparent.window;
#ifdef DEBUG
			oldpar=ev->xreparent.parent;
#endif
			break;

		case CreateNotify:
			if (ev->xcreatewindow.parent != fw->core.window) return;
			target=ev->xcreatewindow.window;
#ifdef DEBUG
			oldpar=ev->xcreatewindow.parent;
#endif
			break;

		default: return;
	}
#ifdef DEBUG
	fprintf(stderr,"clip %x, parent %x, win %x\n",
			(unsigned int)fw->vpFrame.clipwin,(unsigned int)oldpar,(unsigned int)target);
#endif
	if (vpChild=findViewportChild(fw,target)) {
		XtVaGetValues(vpChild,XtNx,&x,XtNy,&y,0);
		XReparentWindow(XtDisplay(w),target,fw->vpFrame.clipwin,x,y);
	} else {
		/* if we didn't find the right child, there's something wrong and
		 * we better don't reparent anything.
		 *
		 * Unfortunately, Viewport has yet another bug :-( :-( :-( in the
		 * ChangedManaged() method. The way this method searches for the
		 * Viewport's child is wrong. It relies on the (wrong) assumption that
		 * XtDestroy(scrollbar) unmanages the destroyed widget immediately. 
		 * However, this is done in Phase 2 only.
		 * Under some circumstances, ChangedManaged() will then try to reparent
		 * an already destroyed scrollbar ...
		 * 
		 */
		XtWarning("VpFrameWidget (ReparentHandler) viewport child not found");
	}
}

static void Initialize(Widget req, Widget new)
{
/* Viewport sets the background_pixmap of the 'clip' widget to None -
 * we override this setting here.
 */
new->core.background_pixmap=0;
}

/* ARGSUSED */
static void Redisplay (gw, ev, r)
    Widget gw;
    XEvent *ev;
    Region r;
{
VpFrameWidget fw=(VpFrameWidget)gw;
 /* we don't call our method directly (as is erroneously done in the
  * ThreeD class). This indirect way of calling the method allows
  * subclasses to replace our routine and we will call their's
  */
 (*((VpFrameWidgetClass)XtClass(gw))->threeD_class.shadowdraw)(gw, ev, r, 
		! fw->vpFrame.embossed);
}

static void UnrealizeCB(Widget wid, XtPointer cld, XtPointer cad)
{
VpFrameWidget fw=(VpFrameWidget)wid;
#ifdef DEBUG
  fprintf(stderr,"UnrealizeCB() entered\n");
#endif
  XDestroyWindow(XtDisplay(wid),fw->vpFrame.clipwin);
  XtRemoveEventHandler(wid,SubstructureNotifyMask,True,ReparentHandler,0);
}

/*
 * assume our parent is a viewport and search its list of
 * children for the one having the window 'win'. We do 
 * this to be able to retrieve the position of 'win' without
 * having to ask the server. (We're hacking anyway :-)
 */

static Widget findViewportChild(VpFrameWidget fw, Window win)
{
Widget 		viewport=XtParent((Widget)fw);
XtAppContext	app = XtWidgetToApplicationContext(viewport);
WidgetList	cs;
Cardinal	i,ncs;

if (!XtIsSubclass(viewport,viewportWidgetClass)) {
	XtAppError(app,
		"VpFrame widget (findViewportChild): parent not a viewport");
	return (Widget)0;
}
XtVaGetValues(viewport,XtNchildren,&cs,XtNnumChildren,&ncs,0);
for (i=0; i<ncs; i++) 
	if (XtWindow(cs[i])==win) return cs[i];

return (Widget) 0;
}
#else
typedef int KeepCompilerHappy;
#endif
