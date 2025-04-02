#ifndef TILLS_PINPOP_H
#define TILLS_PINPOP_H
/* $Id: pinpop.h,v 1.1.1.1 2000/02/07 01:02:17 till Exp $ */

/* Purpose: utilities to create / manage
 *
 *          - toggle widgets with `check marks' indicating the state.
 *          - popup shells with a `pin' for popdown (a la openlook).
 */

/* Author/Copyright: Till Straumann <Till.Straumann@TU-Berlin.de>, 2000 */

/*
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
 *  or consult
 *
 *  	http://www.gnu.org
 *  	
 * 	about more information and the full license text.
 */


#ifdef XAW3D
#include <X11/Xaw3d/Toggle.h>
#else
#include <X11/Xaw/Toggle.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

Widget
CreatePinpop(
#if NeedFunctionPrototypes
	Widget top,
	char *name,
	Widget *form /* returns a child form */
#endif
);

void
CBPinup(
#if NeedFunctionPrototypes
	Widget widget,
	XtPointer pop, /* Callback to popup widget passed as client_data */
	XtPointer cad
#endif
);

Widget
CreateToggle(
#if NeedFunctionPrototypes
	String name,
	Widget papi,
	ArgList args,
	Cardinal nargs
#endif
);

#ifdef __cplusplus
}
#endif

#endif
