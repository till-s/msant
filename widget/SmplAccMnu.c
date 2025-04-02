/* $Id: SmplAccMnu.c,v 1.7 2000/10/06 03:08:09 cvs Exp $ */

/*
Copyright (c) 1999, Till Straumann

 */

/*
 * SmplAccMnu.c - Source code file for SimpleAccMenu widget.
 *
 * Date:    April 3, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium 
 *          kit@expo.lcs.mit.edu
 */

#include <stdio.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#ifdef XAW3D
#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/Cardinals.h>
#else
#include <X11/Xaw/XawInit.h>
#include <X11/Xaw/Cardinals.h>
#endif

#include "SAccmeBSBP.h"
#include "SmplAccMnP.h"

#include <X11/Xmu/Initer.h>
#include <X11/Xmu/CharSet.h>

#ifndef SEPARATE_STRINGS
/* include the automatically generated strings of resource names */
#ifdef __VMS
#include "SmplAccMnu_res_c."
#include "SmplAccMnu_class_c."
#include "SmplAccMnu_type_c."
#else
#include "SmplAccMnu_res_c"
#include "SmplAccMnu_class_c"
#include "SmplAccMnu_type_c"
#endif /* __VMS */
#endif

#define offset(field) XtOffsetOf(SimpleAccMenuRec, simple_acc_menu.field)

static XtResource resources[] = { 
  {XtNuserData,  XtCUserData, XtRPointer, sizeof(XtPointer),
     offset(user_data), XtRImmediate, NULL},
};  
#undef offset

/*
 * Semi Public function definitions. 
 */

static void ClassInitialize();

/*
 * Action Routine Definitions
 */

static void NotifyChild();

/* 
 * Private Function Definitions.
 */

static XtActionsRec actionsList[] =
{
  {"Notify",         NotifyChild},
};

#define superclass (&simpleMenuClassRec)
    
SimpleAccMenuClassRec simpleAccMenuClassRec = {
  {
    /* superclass         */    (WidgetClass) superclass,
    /* class_name         */    "SimpleAccMenu",
    /* size               */    sizeof(SimpleAccMenuRec),
    /* class_initialize   */	ClassInitialize,
    /* class_part_initialize*/	NULL,
    /* Class init'ed      */	FALSE,
    /* initialize         */    NULL,
    /* initialize_hook    */	NULL,
    /* realize            */    XtInheritRealize,
    /* actions            */    actionsList,
    /* num_actions        */    XtNumber(actionsList),
    /* resources          */    resources,
    /* resource_count     */	XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    TRUE, 
    /* compress_exposure  */    TRUE,
    /* compress_enterleave*/ 	TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    XtInheritResize,
    /* expose             */    XtInheritExpose,
    /* set_values         */    NULL,
    /* set_values_hook    */	NULL,
    /* set_values_almost  */	XtInheritSetValuesAlmost,  
    /* get_values_hook    */	NULL,			
    /* accept_focus       */    NULL,
    /* intrinsics version */	XtVersion,
    /* callback offsets   */    NULL,
    /* tm_table		  */    XtInheritTranslations,
    /* query_geometry	  */    XtInheritQueryGeometry,
    /* display_accelerator*/    NULL,
    /* extension	  */    NULL
  },{
    /* geometry_manager   */    XtInheritGeometryManager,
    /* change_managed     */    XtInheritChangeManaged,
    /* insert_child	  */	XtInheritInsertChild,
    /* delete_child	  */	XtInheritDeleteChild,
    /* extension	  */    NULL
  },{
    /* Shell extension	  */    NULL
  },{
    /* Override extension */    NULL
  },{
    /* Simple Menu extension*/  NULL
  }
	/* no Acc Menu class part */
};

WidgetClass simpleAccMenuWidgetClass = (WidgetClass)&simpleAccMenuClassRec;

#define ForAllChildren(smw, childP) \
  for ( (childP) = (SmeObject *) (smw)->composite.children ; \
        (childP) < (SmeObject *) ( (smw)->composite.children + \
				 (smw)->composite.num_children ) ; \
        (childP)++ )

/************************************************************
 *
 * Semi-Public Functions.
 *
 ************************************************************/

/*      Function Name: ClassInitialize
 *      Description: Class Initialize routine, called only once.
 *      Arguments: none.
 *      Returns: none.
 */

static void
ClassInitialize()
{
  XawInitializeWidgetSet();
}

/* ARGSUSED */
static void
NotifyChild(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal * num_params;
{
SimpleAccMenuWidget samw=(SimpleAccMenuWidget)w;
SmeObject			entry;
SmeObjectClass		class;
int					i;
unsigned long		par;

	if (*num_params < 1 || 1!=sscanf(params[0],"%lx",&par)) {
		XtWarning("SimpleAccMenu: NotifyChild() - need a numerical parameter");
		return;
	}
	entry=(SmeObject)par;
	for (i=samw->composite.num_children-1; i>=0; i--) {
		if (samw->composite.children[i]==(Widget)entry) break;
	}
	if (i<0) {
		XtWarning("SimpleAccMenu: NotifyChild() - may only be used for my children");
		return;
	}
	entry = (SmeObject)samw->composite.children[i];
    
    if ( (entry == NULL) || !XtIsSensitive((Widget) entry ) ) return;
    
    class = (SmeObjectClass) entry->object.widget_class;
    (class->sme_class.notify)( (Widget) entry );
}

typedef void (*DisplayAccProc)(
#ifdef NeedFunctionPrototypes
	Widget /* source */,
	String /* accelerator_table */
#endif
);

void
Xaw3dInstallAllSmnuAccelerators(dest,source)
Widget dest;
Widget source;
{
SimpleAccMenuWidget sam=(SimpleAccMenuWidget)source;
XtTranslations		save;
DisplayAccProc		saveDAcc,*pdac;
int					i;
Dimension			widest=0;
Arg					args[5];
Cardinal			nargs=0;
	if (!XtIsSubclass(source,simpleAccMenuWidgetClass)) {
		XtWarning("Xaw3dInstallAllSmnuAccelerators(): source must be a simpleAccMenu");
		return;
	}
	/* we hack around the fact that 
	 * object can not have accelerators (ok, they can not have tranlations since
	 * they receive no events. However, they could be the source for accelerators
	 * and define actions, in theory...)
	 */
	/* save our accelerator table */
	save=source->core.accelerators;
	/* save our class'es display_accelerator method */
	pdac = & source->core.widget_class->core_class.display_accelerator;
	saveDAcc=*pdac;
	for (i=0; i<sam->composite.num_children; i++) {
		Widget chld=sam->composite.children[i];
		if (XtIsSubclass(chld,sAccmeBSBObjectClass) &&
			XtIsManaged(chld)) {
			SAccmeBSBObject sac=(SAccmeBSBObject)chld;
			/* install the child's accelerators as if it were ours */
			if (source->core.accelerators=sac->sAccme_bsb.comp_accl) {
				SAccmeBSBObjectClass scl=(SAccmeBSBObjectClass)sac->object.widget_class;
				/* install the child's display_accelerator method as if it were ours;
				 * store the child pointer to our display_child field, so the child's
				 * method may get the child identity.
				 */
				*pdac = scl->sAccme_bsb_class.display_accelerator;
				sam->simple_acc_menu.display_child = sac;
				XtInstallAccelerators(dest,source);
				/* get the width of the accelerator string */
				if (sac->sAccme_bsb.disp_str_width > widest)
					widest=sac->sAccme_bsb.disp_str_width;
			}
		}
	}
	/* restore our original table and method */
	source->core.accelerators=save;
	*pdac=saveDAcc;
	nargs=0;
	XtSetArg(args[nargs],XtNaccDisplaySpace,0); nargs++;
	/* set the display space for all children to `widest' */
	for (i=0; i<sam->composite.num_children; i++) {
		Widget chld=sam->composite.children[i];
		if (XtIsSubclass(chld,sAccmeBSBObjectClass) &&
			XtIsManaged(chld)) {
			args[0].value=(XtArgVal)widest;
			XtSetValues(chld,args,nargs);
		}
	}
}

