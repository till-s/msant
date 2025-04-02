#ifndef _XawCanvas_h
#define _XawCanvas_h

/***********************************************************************
 *
 * Canvas Widget
 *
 ***********************************************************************/

#ifdef XAW3D
#include <X11/Xaw3d/ThreeD.h>
#else
#include <X11/Xaw/Simple.h>
#endif

/* Parameters:

 Name                 Class              RepType         Default Value
 ----                 -----              -------         -------------
 callback	      Callback           Callback	 NULL
#ifndef XAW3D
 userData	      UserData           Pointer	 NULL
#endif

*/

#ifndef XAW3D
#ifndef XtNuserData
#define XtNuserData	"userData"
#endif
#ifndef XtCUserData
#define XtCUserData	"UserData"
#endif
#endif
#ifndef XtNcallback
#define XtNcallback	"callback"
#endif
#ifndef XtCCallback
#define XtCCallback	"Callback"
#endif

typedef struct _CanvasRec *CanvasWidget;
typedef struct _CanvasClassRec *CanvasWidgetClass;

extern WidgetClass canvasWidgetClass;

void CanvasCallback(/*Widget canvas, XtPointer call_data*/);

#endif /* _XawCanvas_h */
