/*
* $Id: VpFrameP.h,v 1.1 1998/05/02 15:06:41 till Exp $
*/
/*
 *  The VpFrame Widget
 *  ------------------
 *
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
 */

/*
 * $Log: VpFrameP.h,v $
 * Revision 1.1  1998/05/02 15:06:41  till
 * added many comments and usage info
 * renamed to VpFrameWidgetClass to avoid possible name clashes
 *
 */

#ifndef _VpFrameP_h
#define _VpFrameP_h

#include <X11/Xaw3d/ThreeDP.h>
#include "VpFrame.h"

typedef struct {
	/* resources */
	Boolean 	embossed; /* whether to draw the shadows in or out */
	XtCallbackList	unrealize_cbl;
	/* private fields */
    	Window		clipwin;
} VpFramePart;

/* Full instance record declaration */
typedef struct _VpFrameRec {
    CorePart	core;
    SimplePart	simple;
    ThreeDPart	threeD;
    VpFramePart	vpFrame;
} VpFrameRec;

typedef struct {
    char*	happy;
} VpFrameClassPart;

/* Full class record declaration. */
typedef struct _VpFrameClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    ThreeDClassPart	threeD_class;
    VpFrameClassPart	vpFrame_class;
  } VpFrameClassRec;

/* Class pointer. */
extern VpFrameClassRec vpFrameClassRec;

#endif /* _VpFrameP_h */
