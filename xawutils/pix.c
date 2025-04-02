/* $Id: pix.c,v 1.1.1.1 2000/02/07 01:02:17 till Exp $ */

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
#include <X11/StringDefs.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>

#ifdef HASXPM
#include <X11/xpm.h>
#endif

#ifdef XAW3D
#include <X11/Xaw3d/Toggle.h>
#else
#include <X11/Xaw/Toggle.h>
#endif

#include "pix.h"
                
/* creates a (white/black/bg)-Pixmap on the display of widget,
 * with the bg-colour of widget
 */
Pixmap
#if NeedFunctionPrototypes
CreatePixmap(Widget widget, char  *data[])
#else
CreatePixmap(widget,data)
	Widget widget;
	char  *data[];
#endif
{
#ifdef HASXPM
	XpmAttributes pm_attr;
	Pixmap	      pm,dum;
	int	      status;
	Pixel 	      background;
	Colormap      cmap;
	XColor	      color;
	Display	     *dpy=XtDisplay(widget);
	char	      buff[32],*save;

	pm_attr.valuemask=XpmCloseness;
	pm=0;
	pm_attr.closeness=65536; /*allow any colors*/

	XtVaGetValues(widget,
			XtNbackground,&background,
			XtNcolormap,&cmap,
			NULL);

	color.pixel=(unsigned long)background;
	color.flags=DoRed|DoGreen|DoBlue;
	XQueryColor(dpy,cmap,&color);
	sprintf(buff,"  c #%4.4X%4.4X%4.4X",(unsigned int)color.red,(unsigned int)color.green,(unsigned int)color.blue);
	save=data[1];
	data[1]=buff;

	status=XpmCreatePixmapFromData(
			dpy,
			RootWindowOfScreen(XtScreen(widget)),
			data,
			&pm,
			&dum,
			&pm_attr);

	if (status!=XpmSuccess) {
		XtWarning("Couldn't create PIN pixmap\n");
		XFreePixmap(XtDisplay(widget),pm);
		data[1]=save;
		return 0;
	}

	data[1]=save;
	return pm;
#else
	return 0;
#endif
}
