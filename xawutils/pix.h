#ifndef TILLS_PIX_H
#define TILLS_PIX_H
/* $Id: pix.h,v 1.1.1.1 2000/02/07 01:02:17 till Exp $ */
 
/* Purpose: create a pixmap from bitmap data using a widget's background
 *          color.
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

#include <X11/Intrinsic.h>

#ifdef __cplusplus
extern "C" {
#endif

Pixmap
CreatePixmap(
#if NeedFunctionPrototypes
	Widget widget, char *data[]
#endif
);

#ifdef __cplusplus
}
#endif

#endif
