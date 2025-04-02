/*
 * SlPanedP.h - SlPaned Composite Widget's private header file.
 *
 * Updated and significantly modified from the Athena VSlPaned Widget.
 *
 * Date:    March 1, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium
 *          kit@expo.lcs.mit.edu
 */

#ifndef _XawSlPanedP_h
#define _XawSlPanedP_h

#ifdef XAW3D
#include <X11/Xaw3d/PanedP.h>
#else
#include <X11/Xaw/PanedP.h>
#endif

#include "SlPaned.h"

/*********************************************************************
 *
 * SlPaned Widget Private Data
 *
 *********************************************************************/

/* New fields for the SlPaned widget class record */

typedef struct _SlPanedClassPart {
    int foo;			/* keep compiler happy. */
} SlPanedClassPart;

/* Full Class record declaration */
typedef struct _SlPanedClassRec {
    CoreClassPart       core_class;
    CompositeClassPart  composite_class;
    ConstraintClassPart constraint_class;
    PanedClassPart      paned_class;
    SlPanedClassPart    slPaned_class;
} SlPanedClassRec;

extern SlPanedClassRec slPanedClassRec;

typedef struct _SlPanedConstraintsRec {
    PanedConstraintsPart Paned;
} SlPanedConstraintsRec, *SlPanedConstraints;

/*
 * The Pane Stack Structure.
 */

/* New Fields for the SlPaned widget record */
typedef struct {
    /* resources */
    Pixel	foreground;
    int		line_width;

    /* private */
    GC          linegc;                    /* GC to use when animating
					      borders */
} SlPanedPart;

/**************************************************************************
 *
 * Full instance record declaration
 *
 **************************************************************************/

typedef struct _SlPanedRec {
    CorePart       core;
    CompositePart  composite;
    ConstraintPart constraint;
    PanedPart      paned;
    SlPanedPart    slPaned;
} SlPanedRec;

#endif /* _XawSlPanedP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
