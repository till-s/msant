#ifndef XTutil_h
#define XTutil_h

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

/* Create Icon Window from  xpm-file
 * requires 'libXpm'
 * reads Pixmap from file or char_array according to fromfile
 * returns width, height of pixmap, if width,height!=NULL
 * uses RootWindow(disp,DefaultScreen(disp)) if parent=NULL
 */

Window CreateIconWindow(/*disp,parent,fromfile,icon_name,width,height*/);

/*
 *
 *Display *disp;
 *Window   parent;
 *Boolean  fromfile;
 *char    *icon_name; char *icon_name[];
 *unsigned int *width;
 *unsigned int *height;
 *
 */

#endif
