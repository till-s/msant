#ifndef _CanvasP_h
#define _CanvasP_h

#include "Canvas.h"
#ifdef XAW3D
#include <X11/Xaw3d/ThreeDP.h>
#else
#include <X11/Xaw/SimpleP.h>
#endif

typedef struct {
    XtCallbackList callback;
#ifndef XAW3D
    XtPointer	   user_data;
#endif
  } CanvasPart;

/* Full instance record declaration */
typedef struct _CanvasRec {
    CorePart		core;
    SimplePart		simple;
#ifdef XAW3D
    ThreeDPart		three;
#endif
    CanvasPart		canvas;
  } CanvasRec;

typedef struct {
    int	nix;
  } CanvasClassPart;

/* Full class record declaration. */
typedef struct _CanvasClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
#ifdef XAW3D
    ThreeDClassPart	threeD_class;
#endif
    CanvasClassPart	canvas_class;
  } CanvasClassRec;

/* Class pointer. */
extern CanvasClassRec canvasClassRec;


#endif /* _CanvasP_h */
