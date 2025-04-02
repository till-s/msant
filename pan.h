#ifndef PAN_H
#define PAN_H

typedef struct PmPannerPixmap_{
	Pixmap 	  clean_pm;
	Dimension width,height;
	Widget	  panner;
	int	  depth;
	Boolean	  isclean;
} PmPannerPixmapRec, *PmPannerPixmap;

/* create a PmPanner widget using the given arglist
 * callbacks are installed to communicate with the
 * porthole parent of canvas; the canvasWidth and
 * canvasHeight resources of pmPanner are set.
 *
 * a bounding box for the panner must be given
 * in w,h. The actual dimensions are returned.
 *
 * an empty pixmap is created and a PmPannerPixmap struct
 * stuck to the userData resource of the canvas.
 */

Widget CreatePmPanner(/*
  char 		*name,
  Widget	parent,
  Widget 	canvas,
  Dimension 	*w,
  Dimension 	*h,
  Arg		*args,
  int		nargs*/);

/* get a copy of the clean pixmap
 */

Pixmap PmCopyCleanPixmap(/*PmPannerPixmap pmdata*/);


#endif
