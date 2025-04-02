/*
 * $Id: SmplAccMnP.h,v 1.2 1999/10/16 02:15:50 cvs Exp $
 *
Copyright (c) 1999, Till Straumann

 */

/*
 * SmplAccMnP.h - Private Header file for SimpleAccMenu widget.
 *
 * Date:    April 3, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium
 *          kit@expo.lcs.mit.edu
 */

#ifndef _SimpleAccMenuP_h
#define _SimpleAccMenuP_h

#include "SmplAccMnu.h"
#ifdef XAW3D
#include <X11/Xaw3d/SimpleMenP.h>
#else
#include <X11/Xaw/SimpleMenP.h>
#endif

#include "SAccmeBSB.h"

typedef struct _SimpleAccMenuClassRec {
  CoreClassPart	          core_class;
  CompositeClassPart      composite_class;
  ShellClassPart          shell_class;
  OverrideShellClassPart  override_shell_class;
  SimpleMenuClassPart	  simpleMenu_class;
} SimpleAccMenuClassRec;

extern SimpleAccMenuClassRec simpleAccMenuClassRec;

typedef struct _SimpleAccMenuPart {
	/* resources */
	XtPointer		user_data;
	/* private state */
	SAccmeBSBObject	display_child;
} SimpleAccMenuPart;

typedef struct _SimpleAccMenuRec {
  CorePart			core;
  CompositePart 	composite;
  ShellPart 	    shell;
  OverrideShellPart override;
  SimpleMenuPart	simple_menu;
  SimpleAccMenuPart simple_acc_menu;
} SimpleAccMenuRec;

#endif /* _SimpleAccMenuP_h */
