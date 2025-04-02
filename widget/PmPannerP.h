/*
 * $XConsortium: PmPannerP.h,v 1.19 94/04/17 20:12:33 jim Exp $
 *
Copyright (c) 1989  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#ifndef _XawPmPannerP_h
#define _XawPmPannerP_h

#include "PmPanner.h"
#ifdef XAW3D
#include <X11/Xaw3d/PannerP.h>		/* parent */
#else
#include <X11/Xaw/PannerP.h>		/* parent */
#endif

typedef struct {			/* new fields in widget class */
    int dummy;
} PmPannerClassPart;

typedef struct _PmPannerClassRec {	/* PmPanner widget class */
    CoreClassPart core_class;
    SimpleClassPart simple_class;
    PannerClassPart panner_class;
    PmPannerClassPart pmPanner_class;
} PmPannerClassRec;

typedef struct {			/* new fields in widget */
    /* resources */
#ifdef XAW3D
    Dimension	 shadow_width;
#else
    XtPointer	 user_data;
#endif
    Boolean	 emboss;
    Boolean	 transparent_slider;
    XtPointer	 resize_callback;
    /* private data... */
    GC		 stipple_gc;		/* gc to stipple the bg pixmap */
    unsigned int pm_width,pm_height;
    Pixmap	 foreground_pixmap;
#ifdef XAW3D
    Widget	 threed;		/* 3d widget to draw the shadows */
#endif
} PmPannerPart;

typedef struct _PmPannerRec {
    CorePart core;
    SimplePart simple;
    PannerPart panner;
    PmPannerPart pmPanner;
} PmPannerRec;

/*
 * external declarations
 */
extern PmPannerClassRec pmPannerClassRec;

#endif /* _XawPmPannerP_h */
