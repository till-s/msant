/* $Id: pinpop.c,v 1.1.1.1 2000/02/07 01:02:17 till Exp $ */

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

#include <stdio.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Shell.h>

#ifdef XAW3D
#include <X11/Xaw3d/ThreeD.h>
#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/Toggle.h>
#ifdef USE_FOCUSFORM
#include "widget/FocusForm.h"
#else
#include <X11/Xaw3d/Form.h>
#endif
#else
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>
#ifdef USE_FOCUSFORM
#include "widget/FocusForm.h"
#else
#include <X11/Xaw/Form.h>
#endif
#endif


#include "pix.h"

#include "pinpop.h"

/* Pixmap data */

typedef struct T_pix{
	Pixmap onpm,offpm;
} *Tpix;

static char *pin_bits[]=
#ifdef HASXPM
{
"16 16 3 1",
"  c #B2B2B2B2B2B2",
". c #FFFFFFFFFFFF",
"X c #000000000000",
"                ",
"        ...     ",
"     ...   .X   ",
"   .. .     X   ",
"  .  .       X  ",
"  .  .       X  ",
" .   .       X  ",
" .   ..     X   ",
" .    XX   XX   ",
" ..   XXXXXXX   ",
"  .     XXXX    ",
"  .XX    XXX    ",
" .XXXXXXXXX     ",
" XX  XXXX       ",
"                ",
"                ",
};
#else
{""};
#endif
                

static char *off_bits[]=
#ifdef HASXPM
{
"16 16 3 1",
"  c #CCCCCCCCCCCC",
". c #FFFFFFFFFFFF",
"X c #000000000000",
"                ",
"                ",
"  ............  ",
"  .          X  ",
"  .          X  ",
"  .          X  ",
"  .          X  ",
"  .          X  ",
"  .          X  ",
"  .          X  ",
"  .          X  ",
"  .          X  ",
"  .          X  ",
"  XXXXXXXXXXXX  ",
"                ",
"                ",
};
#else
{""};
#endif

static char *on_bits[]=
#ifdef HASXPM
{
"16 16 3 1",
"  c #CCCCCCCCCCCC",
". c #FFFFFFFFFFFF",
"X c #000000000000",
"                ",
"                ",
"  ........    X ",
"  .         XX  ",
"  .        XX   ",
"  .       XX    ",
"  .  X   XX  X  ",
"  . XXX XXX  X  ",
"  .XXXXXXX   X  ",
"  . XXXXXX   X  ",
"  .  XXXX    X  ",
"  .   XXX    X  ",
"  .    X     X  ",
"  XXXXXXXXXXXX  ",
"                ",
"                ",
};
#else
{""};
#endif


static void
CBPindown(
#if NeedFunctionPrototypes
	Widget, XtPointer, XtPointer
#endif
);

static void
CB_switchpm(
#if NeedFunctionPrototypes
	Widget, XtPointer, XtPointer
#endif
);

static void
CB_destroy(
#if NeedFunctionPrototypes
	Widget, XtPointer, XtPointer
#endif
);

Widget
#if NeedFunctionPrototypes
CreatePinpop(Widget top, String name, Widget *form)
#else
CreatePinpop(top,name,form)
	Widget top;
	String name;
	Widget *form;
#endif
{
	Widget pop,pin,mainf,wid;
	Pixmap pm_pin;

	pop=XtVaCreatePopupShell(
			name,
			transientShellWidgetClass,
			top,
			XtNallowShellResize,False,
			NULL);

	mainf=XtVaCreateManagedWidget(
			"pop_mainform",
#ifdef USE_FOCUSFORM
			focusFormWidgetClass,
#else
			formWidgetClass,
#endif
			pop,
			XtNborderWidth,0,
			NULL);

	pin=XtVaCreateManagedWidget(
			"Dismiss",
			commandWidgetClass,
			mainf,
			XtNhighlightThickness,2,
			XtNinternalWidth,2,
			XtNinternalHeight,2,
#ifdef XAW3D
			XtNborderWidth,0,
#endif
			XtNleft,XtChainLeft,
			XtNright,XtChainLeft,
			XtNtop,XtChainTop,
			XtNbottom,XtChainTop,
#ifdef XAW3D
			XtNshadowWidth,0,
#endif
			NULL);

	if (0!=(pm_pin=CreatePixmap(pin,pin_bits))) 
		XtVaSetValues(pin,XtNbitmap,pm_pin,NULL);

	XtAddCallback(pin,XtNcallback,CBPindown,(XtPointer)pop);

	wid=XtVaCreateManagedWidget(
			"pop_form",
#ifdef USE_FOCUSFORM
			focusFormWidgetClass,
#else
			formWidgetClass,
#endif
			mainf,
			XtNborderWidth,0,
			XtNtop,XtChainTop,
			XtNbottom,XtChainTop,
			XtNfromVert,pin,
			NULL);

	if (form) *form=wid;

	return pop;

}

static void
#if NeedFunctionPrototypes
CBPindown(Widget w, XtPointer cld, XtPointer cad)
#else
CBPindown(w,cld,cad)
	Widget w;
	XtPointer cld,cad;
#endif
{
	Widget pop=(Widget)cld;
	XtPopdown(pop);
}

void
#if NeedFunctionPrototypes
CBPinup(Widget w, XtPointer cld, XtPointer cad)
#else
CBPinup(w,cld,cad)
	Widget w;
	XtPointer cld,cad;
#endif
{
	Widget pop=(Widget)cld;
	XtPopup(pop,XtGrabNone);
	XRaiseWindow(XtDisplay(pop),XtWindow(pop));
}

static void
#if NeedFunctionPrototypes
CB_destroy(Widget w, XtPointer cld, XtPointer cad)
#else
CB_destroy(w,cld,cad)
	Widget w;
	XtPointer cld,cad;
#endif
{
	struct T_pix *pp=(struct T_pix *)cld;
	if (pp->onpm)  XFreePixmap(XtDisplay(w),pp->onpm);
	if (pp->offpm) XFreePixmap(XtDisplay(w),pp->offpm);
	XtFree((char*)pp);
}

static void
#if NeedFunctionPrototypes
CB_switchpm(Widget w, XtPointer cld, XtPointer cad)
#else
CB_switchpm(w,cld,cad)
	Widget w;
	XtPointer cld,cad;
#endif
{
	struct T_pix *pp=(struct T_pix *)cld;
	Boolean st;
	XtVaGetValues(w,XtNstate,&st,NULL);

	if (pp->onpm==0 || pp->offpm==0)
		XtVaSetValues(w,XtNlabel,st?"X":"O",NULL);
	else
		XtVaSetValues(w,XtNbitmap,st?pp->onpm:pp->offpm,NULL);
}


Widget
#if NeedFunctionPrototypes
CreateToggle(String name, Widget papi, ArgList args, Cardinal nargs)
#else
CreateToggle(name,papi,args,nargs)
	String name;
	Widget papi;
	ArgList args;
	Cardinal nargs;
#endif
{
	Widget tt;
	Boolean st;
	Arg  targs[10];
	ArgList new;
	Tpix pp;
	int  j;

	j=0;
	XtSetArg(targs[j],XtNinternalHeight,NULL);j++;
	XtSetArg(targs[j],XtNinternalWidth,NULL);j++;
	XtSetArg(targs[j],XtNhighlightThickness,NULL);j++;
#ifdef XAW3D
	XtSetArg(targs[j],XtNshadowWidth,NULL);j++;
#endif

	new=XtMergeArgLists(targs,j,args,nargs);
	tt=XtCreateManagedWidget(name?name:"toggle",toggleWidgetClass,papi,new,nargs+j);
	XtFree((char*)new);
	pp=(Tpix)XtMalloc(sizeof(struct T_pix));
	pp->onpm=CreatePixmap(tt,on_bits);
	pp->offpm=CreatePixmap(tt,off_bits);
	XtAddCallback(tt,XtNcallback,CB_switchpm,(XtPointer)pp);
	XtAddCallback(tt,XtNdestroyCallback,CB_destroy,(XtPointer)pp);
	XtVaGetValues(tt,XtNstate,&st,NULL);
	if (pp->onpm==0 || pp->offpm==0)
		XtVaSetValues(tt,XtNlabel,st?"X":"O",NULL);
	else
		XtVaSetValues(tt,XtNbitmap,st?pp->onpm:pp->offpm,NULL);

	return tt;
}
