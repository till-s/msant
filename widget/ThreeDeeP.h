/*
 * $Id: ThreeDeeP.h,v 1.2 1998/04/23 21:51:12 till Exp $
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
 *  $Log: ThreeDeeP.h,v $
 *  Revision 1.2  1998/04/23 21:51:12  till
 *  type fixed
 *
 *  Revision 1.1  1998/04/23 20:28:03  till
 *  put under cvs control
 *

 */

#ifndef _ThreeDeeP_h
#define _ThreeDeeP_h

#include "ThreeDee.h"
#include <X11/Xaw3d/ThreeDP.h>

typedef struct {
    XtCallbackList callback;
    Boolean	   emboss;
  } ThreeDeePart;

/* Full instance record declaration */
typedef struct _ThreeDeeRec {
    CorePart		core;
    SimplePart		simple;
    ThreeDPart		three;
    ThreeDeePart	threeDee;
  } ThreeDeeRec;

typedef struct {
    int	nix;
  } ThreeDeeClassPart;

/* Full class record declaration. */
typedef struct _ThreeDeeClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    ThreeDClassPart	threeD_class;
    ThreeDeeClassPart	threeDee_class;
  } ThreeDeeClassRec;

/* Class pointer. */
extern ThreeDeeClassRec threeDeeClassRec;


#endif /* _ThreeDeeP_h */
