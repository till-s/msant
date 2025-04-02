#ifndef _XawPmPanner_h
#define _XawPmPanner_h

#include <X11/Xaw3d/Panner.h>

/*****************************************************************************
 * 
 * PmPanner Widget (subclass of ThreeD)
 * 
 * This widget is used to represent navigation in a 2d coordinate system.
 * 
 * Parameters:
 * 
 *  Name		Class		Type		Default
 *  ----		-----		----		-------
 * 
 *  resizeCallback	ResizeCallback	Callback	NULL
 * 
 *****************************************************************************/

					/* new instance and class names */

#ifndef XtNuserData
#define XtNuserData 		"userData"
#endif
#ifndef XtCUserData
#define XtCUserData 		"UserData"
#endif
#ifndef XtNtransparentSlider
#define XtNtransparentSlider 	"transparentSlider"
#endif
#ifndef XtCTransparentSlider
#define XtCTransparentSlider 	"TransparentSlider"
#endif
#ifndef XtNresizeCallback
#define XtNresizeCallback 	"resizeCallback"
#endif
#ifndef XtCResizeCallback
#define XtCResizeCallback 	"ResizeCallback"
#endif
#ifndef XtNemboss
#define XtNemboss	     	"emboss"
#endif
#ifndef XtCEmboss
#define XtCEmboss	     	"Emboss"
#endif
#ifndef XtNshadowWidth
#define XtNshadowWidth	     	"shadowWidth"
#endif
#ifndef XtCShadowWidth
#define XtCShadowWidth	     	"ShadowWidth"
#endif

	/* external declarations */
extern WidgetClass pmPannerWidgetClass;

typedef struct _PmPannerClassRec *PmPannerWidgetClass;
typedef struct _PmPannerRec      *PmPannerWidget;

#endif /* _XawPmPanner_h */
