/*
 * $Id: SAccmeBSBP.h,v 1.2 1999/10/11 22:44:15 cvs Exp $
 *
Copyright (c) 1999, Till Straumann

 * Author:  Till Straumann.
 */

/* 
 * Private definitions for SAccmeBSB object
 * 
 */

#ifndef _XawSAccmeBSBP_h
#define _XawSAccmeBSBP_h

/***********************************************************************
 *
 * SAccme Object Private Data
 *
 ***********************************************************************/

#ifdef XAW3D
#include <X11/Xaw3d/SmeBSBP.h>
#else
#include <X11/Xaw/SmeBSBP.h>
#endif

#include "SAccmeBSB.h"

typedef struct _SAccmeBSBClassPart {
	void	(*display_accelerator)();	
} SAccmeBSBClassPart;

#define XtInheritXaw3dDisplayAccelerator ((void (*)())_XtInherit)

/* Full class record declaration */
typedef struct _SAccmeBSBClassRec {
    RectObjClassPart	rect_class;
    SmeClassPart		sme_class;
#ifdef XAW3D
    SmeThreeDClassPart	sme_threeD_class;
#endif
    SmeBSBClassPart		sme_bsb_class;
    SAccmeBSBClassPart	sAccme_bsb_class;
} SAccmeBSBClassRec;

extern SAccmeBSBClassRec sAccmeBSBClassRec;

typedef enum {
	saccAny   = (0),
	saccNone  = (-1),
	saccShift = (1<<0),
	saccCtrl  = (1<<1),
	saccLock  = (1<<2),
	saccMod1  = (1<<3),
	saccMod2  = (1<<4),
	saccMod3  = (1<<5),
	saccMod4  = (1<<6),
	saccMod5  = (1<<7),
	saccBtn1  = (1<<8),
	saccBtn2  = (1<<9),
	saccBtn3  = (1<<10),
	saccBtn4  = (1<<11),
	saccBtn5  = (1<<12),
	saccMeta  = (1<<13),
	saccAlt   = (1<<14),
	saccHyper = (1<<15),
	saccSuper = (1<<16)
} SaccModifier;

/* New fields for the Sme Object record */
typedef struct {
    /* resources */
	String			accelerators; 	/* the (uncompiled) accelerator table */
	Boolean			do_display;   	/* whether the accelerators should be shown */
	Dimension		acc_disp_space; /* space to reserve for the accelerator display */
	Dimension		acc_disp_sep;	/* space between label and acc display */
	String			none_disp_str;	/* how to print the `None' modifier */
								/* adjust the size if modifiers are added !!! */
	String			mod_disp_strs[17];/* how to print the modifiers */
	String			display_format_str; /* how to format the accelerator display */
	XtJustify		acc_disp_justify; /* how to adjust the accelerator display */

/* private resources. */
	XtTranslations	comp_accl;
	String			display_string;
	Dimension		disp_str_width;
	SaccModifier	modifiers;
	String			keysym_string;

} SAccmeBSBPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _SAccmeBSBRec {
    ObjectPart		object;
    RectObjPart		rectangle;
    SmePart			sme;
#ifdef XAW3D
    SmeThreeDPart	sme_threeD;
#endif
    SmeBSBPart		sme_bsb;
    SAccmeBSBPart	sAccme_bsb;
} SAccmeBSBRec;

/************************************************************
 *
 * Private declarations.
 *
 ************************************************************/

#endif /* _XawSAccmeBSBP_h */
