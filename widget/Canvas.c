#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#ifdef XAW3D
#include <X11/Xaw3d/XawInit.h>
#else
#include <X11/Xaw/XawInit.h>
#endif
#include "CanvasP.h"
#include <X11/Xosdefs.h>


#define offset(field) XtOffsetOf(CanvasRec, field)

static XtResource resources[] = {
    {XtNcallback, XtCCallback, XtRCallback, sizeof(XtPointer),
	offset(canvas.callback), XtRCallback, (XtPointer) NULL},
#ifndef XAW3D
    {XtNuserData, XtCUserData, XtRPointer, sizeof(XtPointer),
	offset(canvas.user_data), XtRPointer, (XtPointer) NULL},
#endif
};

#undef offset

CanvasClassRec canvasClassRec = {
    { /* core fields */
#ifdef XAW3D
    /* superclass		*/	(WidgetClass) &threeDClassRec,
#else
    /* superclass		*/	(WidgetClass) &simpleClassRec,
#endif
    /* class_name		*/	"Canvas",
    /* widget_size		*/	sizeof(CanvasRec),
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
    /* expose			*/	XtInheritExpose,
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
#ifdef XAW3D
    { /* threeD fields */
    /* shadow draw              */      XtInheritXaw3dShadowDraw
    },
#endif
    { /* canvas fields */
    /* null 			*/      0
    }
};

WidgetClass canvasWidgetClass = (WidgetClass) &canvasClassRec;

void CanvasCallback(wid,cad)
 Widget wid;
 XtPointer cad;
{
 CanvasWidget cw=(CanvasWidget)wid;
 if (!XtIsSubclass(wid,canvasWidgetClass)) return;
 XtCallCallbackList(wid,cw->canvas.callback,cad);
}
