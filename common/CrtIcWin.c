#include "XTutil.h"

#ifdef HASXPM
#include <xpm.h>

Window CreateIconWindow(disp,parent,fromfile,icon_fname,width,height)
  Display *disp;
  Window   parent;
  Bool     fromfile;
  char    *icon_fname;
  unsigned int *width;
  unsigned int *height;

{
  Pixmap        icon_pixmap,dum;
  Window	icon_window;
  XpmAttributes icon_attr;
  int		status;

  if (parent==0) parent=RootWindow(disp,DefaultScreen(disp));

  icon_window=XCreateSimpleWindow(disp,parent,
		0,0,1,1,
		CopyFromParent,
		BlackPixelOfScreen(DefaultScreenOfDisplay(disp)),
		BlackPixelOfScreen(DefaultScreenOfDisplay(disp)));

  icon_attr.valuemask=XpmCloseness;
  icon_pixmap=0;
  icon_attr.closeness=65536; /* allow any colors */

  
  status=(fromfile)? XpmReadFileToPixmap(disp, icon_window,
			icon_fname,
			&icon_pixmap, &dum, &icon_attr) :
  		     XpmCreatePixmapFromData(disp, icon_window, 
                        (char**)icon_fname,
		        &icon_pixmap, &dum, &icon_attr) ;

  if (status!=XpmSuccess) {

    XDestroyWindow(disp,icon_window);
    if(icon_pixmap) {
      XFreePixmap(disp,icon_pixmap);
      icon_pixmap=0;
    }


    switch (status) {
	case XpmColorError: 
		XtWarning("Icon Pixmap: color error\n");
		break;

	case XpmOpenFailed: 
		XtWarning("Icon Pixmap: file not found\n");
		break;

	case XpmFileInvalid: 
		XtWarning("Icon Pixmap: file invalid\n");
		break;
 
	case XpmNoMemory: 
		XtWarning("Icon Pixmap: no memory\n");
		break;
 
	case XpmColorFailed: 
		XtWarning("Icon Pixmap: color failed\n");
		break;
 
	default: 
		XtWarning("Icon Pixmap: unknown error\n");
		break;
    }

    return(0);

  } else {

   XResizeWindow(disp,icon_window,icon_attr.width,icon_attr.height);
   XSetWindowBackgroundPixmap(disp,icon_window,icon_pixmap);
   if (width)   *width=(unsigned int)icon_attr.width;
   if (height) *height=(unsigned int)icon_attr.height;

   XFreePixmap(disp,icon_pixmap);
  return(icon_window);
  }

}

#else

Window CreateIconWindow(disp,parent,fromfile,icon_fname,width,height)
  Display *disp;
  Window   parent;
  Bool     fromfile;
  char    *icon_fname;
  unsigned int *width;
  unsigned int *height;

{
  Pixmap        icon_pixmap,dum;
  Window	icon_window;
  XWindowAttributes atts;

  if (parent==0) parent=RootWindow(disp,DefaultScreen(disp));

  icon_window=XCreateSimpleWindow(disp,parent,
		0,0,1,1,
		CopyFromParent,
		BlackPixelOfScreen(DefaultScreenOfDisplay(disp)),
		BlackPixelOfScreen(DefaultScreenOfDisplay(disp)));

  if (fromfile || width==0 || height==0) {
	return(0);
  } else {
  XGetWindowAttributes(disp,icon_window,&atts);
  icon_pixmap=XCreatePixmapFromBitmapData(
	disp,icon_window, icon_fname,
	*width,*height,
	BlackPixelOfScreen(DefaultScreenOfDisplay(disp)),
	WhitePixelOfScreen(DefaultScreenOfDisplay(disp)),
	atts.depth);

   XResizeWindow(disp,icon_window,*width,*height);
   XSetWindowBackgroundPixmap(disp,icon_window,icon_pixmap);
   XFreePixmap(disp,icon_pixmap);
  return(icon_window);
  }

}
#endif
