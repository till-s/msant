/*
 * SlPaned.h - SlPaned Composite Widget's public header file.
 *
 */

#ifndef _XawSlPaned_h
#define _XawSlPaned_h

#ifdef XAW3D
#include <X11/Xaw3d/Paned.h>
#else
#include <X11/Xaw/Paned.h>
#endif

/****************************************************************
 *
 * Vertical SlPaned Widget (SubClass of CompositeClass)
 *
 ****************************************************************/

/* RESOURCES:

 Name		         Class		   RepType	    Default Value
 ----		         -----		   -------	    -------------
 foreground		Foreground	   Pixel	    XtDefaultForeground
 lineWidth		LineWidth	   Int		    1

 Draw the lines while moving the grip usin foreground color 'foreground' 
 and line width 'lineWidth'.
 For best performance set pane's 'internalBorderColor' to 'background'
 and 'internalBorderWidth' to width/height of grip.

*/

/* New Fields */
#ifndef XtNforeground
#define XtNforeground	"foreground"
#endif
#ifndef XtCForeground
#define XtCForeground	"Foreground"
#endif
#ifndef XtNlineWidth
#define XtNlineWidth	"lineWidth"
#endif
#ifndef XtCLineWidth
#define XtCLineWidth	"LineWidth"
#endif

/* Class record constant */
extern WidgetClass slPanedWidgetClass;

typedef struct _SlPanedClassRec	*SlPanedWidgetClass;
typedef struct _SlPanedRec	*SlPanedWidget;

#endif /* _XawSlPaned_h */
