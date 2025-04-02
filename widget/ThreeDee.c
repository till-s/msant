/*
 * $Id: ThreeDee.c,v 1.3 1999/10/18 22:37:57 cvs Exp $
 *
 *  The ThreeDee Widget
 *  -------------------
 *
 *  simple widget with shadows, added a resource to control embossing.
 
 *  Copyright (C) 1997  by Till Straumann   <strauman@sun6hft.ee.tu-berlin.de>

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
 *  $Log: ThreeDee.c,v $
 *  Revision 1.3  1999/10/18 22:37:57  cvs
 *   - Imakefile supports building a (static) library now.
 *   - all (resource etc.) strings go to strDefs.c if SEPARATE_STRINGS is defined.
 *
 *  Revision 1.2  1998/04/23 21:51:12  till
 *  type fixed
 *
 *  Revision 1.1  1998/04/23 20:28:02  till
 *  put under cvs control
 *

 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xaw3d/XawInit.h>
#include "ThreeDeeP.h"
#include <X11/Xosdefs.h>

#ifndef SEPARATE_STRINGS
/* include the automatically generated strings of resource names */
#ifdef __VMS
#include "ThreeDee_res_c."
#include "ThreeDee_class_c."
#include "ThreeDee_type_c."
#else
#include "ThreeDee_res_c"
#include "ThreeDee_class_c"
#include "ThreeDee_type_c"
#endif /* __VMS */
#endif


#define offset(field) XtOffsetOf(ThreeDeeRec, field)

static XtResource resources[] = {
    {XtNcallback, XtCCallback, XtRCallback, sizeof(XtPointer),
	offset(threeDee.callback), XtRCallback, (XtPointer) NULL},
    {XtNemboss, XtCEmboss, XtRBoolean, sizeof(Boolean),
	offset(threeDee.emboss), XtRImmediate, (XtPointer) True},
};

#undef offset

static void Redisplay();
static Boolean SetValues();

ThreeDeeClassRec threeDeeClassRec = {
    { /* core fields */
    /* superclass		*/	(WidgetClass) &threeDClassRec,
    /* class_name		*/	"ThreeDee",
    /* widget_size		*/	sizeof(ThreeDeeRec),
    /* class_initialize		*/	NULL,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	XawInitializeWidgetSet,
    /* initialize_hook		*/	NULL,
    /* realize			*/	XtInheritRealize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* resource_count		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	NULL,
    /* resize			*/	XtInheritResize,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
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
    /* shadow draw              */      XtInheritXaw3dShadowDraw
    },
    { /* threeDee fields */
    /* null 			*/      0
    }
};

WidgetClass threeDeeWidgetClass = (WidgetClass) &threeDeeClassRec;

/* ARGSUSED */
static void Redisplay (w, event, region)
    Widget w;
    XEvent *event;		/* unused */
    Region region;		/* unused */
{
((ThreeDeeWidgetClass)XtClass(w))->threeD_class.shadowdraw(w,event,region,
	((ThreeDeeWidget)w)->threeDee.emboss);
}

/* ARGSUSED */
static Boolean SetValues (gcurrent, grequest, gnew, args, num_args)
    Widget gcurrent, grequest, gnew;
    ArgList args;
    Cardinal *num_args;
{
    ThreeDeeWidget current = (ThreeDeeWidget) gcurrent;
    ThreeDeeWidget new = (ThreeDeeWidget) gnew;
    return current->threeDee.emboss != new->threeDee.emboss;
}

void ThreeDeeCallback(wid,cad)
 Widget wid;
 XtPointer cad;
{
 ThreeDeeWidget tw=(ThreeDeeWidget)wid;
 if (!XtIsSubclass(wid,threeDeeWidgetClass)) return;
 XtCallCallbackList(wid,tw->threeDee.callback,cad);
}
