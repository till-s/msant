/* $XConsortium: ScrollByL.c,v 1.30 94/04/17 20:43:46 rws Exp $ */
/*

Copyright (c) 1987, 1988  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

Modified By T. Straumann:

	- added string search / highlight ability.
	- XAW3D support
	- bugfixes
	- methods to fill/free the buffer.
*/

#if ( !defined(lint) && !defined(SABER))
  static char rcs_version[] = "$Athena: ScrollByL.c,v 4.5 88/12/19 13:46:04 kit Exp $";
#endif

#include <stdio.h>

#include <stdlib.h>

#include <string.h>
#include <ctype.h>
#include <errno.h>

#define USE_MMAP
#ifdef USE_MMAP
#include <unistd.h>
#include <sys/mman.h>
#endif

#include <X11/IntrinsicP.h>
#include <sys/stat.h>		/* depends on IntrinsicP.h */
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/StdSel.h>

#include "ScrollByLP.h"

#ifdef XAW3D
#include <X11/Xaw3d/ScrollbarP.h>
#else
#include <X11/Xaw/ScrollbarP.h>
#endif

#include <X11/Xmu/Misc.h>


static Boolean ConvertSelection();
/* Default Translation Table */

static char defaultTranslations[] = 
  "<Btn1Down>,<Btn1Up>:  Select() \n\
   <Key>f:      Page(Forward) \n\
   <Key>b:      Page(Back) \n\
   <Key>1:      Page(Line, 1) \n\
   <Key>2:      Page(Line, 2) \n\
   <Key>3:      Page(Line, 3) \n\
   <Key>4:      Page(Line, 4) \n\
   <Key>\\ :    Page(Forward) \n\
   <Key>Return:	Find() \n\
   <Key>Up:		Page(Line, -1) \n\
   <Key>Down:	Page(Line,  1) \n";

      
/****************************************************************
 *
 * ScrollByLine Resources
 *
 ****************************************************************/

#define Offset(field) XtOffset(ScrollByLineWidget, scroll.field)
#define CoreOffset(field) XtOffset(ScrollByLineWidget, core.field)

static XtResource resources[] = {
	/* T. Straumann: if initial width is 0 we try to guess
	 *				 the width from the font dimensions
	 */
    {XtNwidth, XtCWidth, XtRDimension, sizeof(Dimension),
       CoreOffset(width), XtRImmediate, (caddr_t) 0},
	/* T. Straumann: if the initial height is 0 we set the
	 *				 height to font_height*nLines.
	 */
	{XtNnLines, XtCNLines, XtRInt, sizeof(int),
	   Offset(n_lines), XtRImmediate, (XtPointer) 40},
    {XtNheight, XtCHeight, XtRDimension, sizeof(Dimension),
       CoreOffset(height), XtRImmediate, (caddr_t) 0},
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
       Offset(foreground), XtRString, "XtDefaultForeground"},
    {XtNforceVert, XtCBoolean, XtRBoolean, sizeof(Boolean),
       Offset(force_vert), XtRImmediate, (caddr_t) FALSE},
    {XtNindent, XtCIndent, XtRDimension, sizeof(Dimension),
       Offset(indent), XtRImmediate, (caddr_t) 15},
    {XtNuseRight, XtCBoolean, XtRBoolean, sizeof(Boolean),
       Offset(use_right), XtRImmediate, (caddr_t) FALSE},
    {XtNmanualFontNormal, XtCFont, XtRFontStruct, sizeof(XFontStruct *),
       Offset(normal_font), XtRString, MANPAGE_NORMAL},
    {XtNmanualFontBold, XtCFont, XtRFontStruct, sizeof(XFontStruct *),
       Offset(bold_font), XtRString, MANPAGE_BOLD},
    {XtNmanualFontItalic, XtCFont, XtRFontStruct, sizeof(XFontStruct *),
       Offset(italic_font), XtRString, MANPAGE_ITALIC},
    {XtNmanualFontSymbol, XtCFont, XtRFontStruct, sizeof(XFontStruct *),
       Offset(symbol_font), XtRString, MANPAGE_SYMBOL},
    {XtNfile, XtCFile, XtRFile, sizeof(FILE *),
       Offset(file), XtRImmediate, (caddr_t) NULL},
    {XtNseparators, XtCSeparators, XtRString, sizeof(String),
       Offset(separators), XtRString, " ,\n\t()'"},
    {XtNselChngdCallback, XtCCallback, XtRCallback, sizeof(XtPointer),
       Offset(selectionChanged_cbl), XtRString, (XtPointer)NULL},
};

#undef Offset
#undef CoreOffset

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

static Boolean ScrollVerticalText();
static void MoveAndClearText(), LoadFile(), PrintText(), VerticalJump();
static void VerticalScroll(), SetThumbHeight(), PaintText(), Layout();

/* semi - public functions. */

static void Realize(), Initialize(), Destroy(), Redisplay(), Page(), Unselect(), Select();
static void ClassPartInitialize();
static int  FillBuffer();
static void FreeBuffer();
static void Find();
static Boolean SetValuesHook();

static XtActionsRec actions[] = {
  { "Page",   Page},
  { "Select", Select},
  { "Unselect", Unselect},
  { "Find",     Find},
};

#ifdef XAW3D
#define superclass		(&threeDClassRec)
#else
#define superclass		(&simpleClassRec)
#endif

ScrollByLineClassRec scrollByLineClassRec = {
  {
/* core_class fields      */
    /* superclass         */    (WidgetClass) superclass,
    /* class_name         */    "ScrollByLine",
    /* widget_size        */    sizeof(ScrollByLineRec),
    /* class_initialize   */    NULL,
    /* class_part_init    */    ClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize         */    Initialize,
    /* initialize_hook    */    NULL,
    /* realize            */    Realize,
    /* actions            */    actions,
    /* num_actions	  */	XtNumber(actions),
    /* resources          */    resources,
    /* num_resources      */    XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion	  */	TRUE,
    /* compress_exposure  */	FALSE,
    /* compress_enterleave*/    TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    Destroy,
    /* resize             */    Layout,
    /* expose             */    Redisplay,
    /* set_values         */    NULL,
    /* set_values_hook    */    SetValuesHook,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */    NULL,
    /* accept_focus       */    NULL,
    /* version            */    XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    defaultTranslations,
    /* query_geometry	  */	XtInheritQueryGeometry,
    /* display_accelerator*/	XtInheritDisplayAccelerator,
    /* extension	  */	NULL,
  },
  { /* simple fields */
    /* change_sensitive	  */	XtInheritChangeSensitive
  },
#ifdef XAW3D
  { /* threeD fields */
	/* shadowdraw */	XtInheritXaw3dShadowDraw
  },
#endif
  { /* scrollByLine fields*/
       /*  fillbuffer     */	FillBuffer,
       /*  freebuffer     */	FreeBuffer
  },
};
#undef superclass

WidgetClass scrollByLineWidgetClass = 
            (WidgetClass) &scrollByLineClassRec;


/****************************************************************
 *
 * Private Routines
 *
 ****************************************************************/

/*	Function Name: Layout
 *	Description: This function lays out the scroll widget.
 *	Arguments: w - the scroll widget.
 *                 key - a boolean: if true then resize the widget to the child
 *                                  if false the resize children to fit widget.
 *	Returns: TRUE if successful.
 */

static void
Layout(w)
Widget w;
{    
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;
  Dimension width, height;
  Widget bar;
  Position bar_bw;

/* 
 * For now always show the bar.
 */

  bar = sblw->scroll.bar;
  height = sblw->core.height;
  width = sblw->core.width;
  bar_bw = bar->core.border_width;

  /* Move child and v_bar to correct location. */

  if (sblw->scroll.use_right) {
    XtMoveWidget(bar, width - (bar->core.width + bar_bw), - bar_bw);
    sblw->scroll.offset = 0;
  }
  else {
    XtMoveWidget(bar, - bar_bw, - bar_bw);
    sblw->scroll.offset = bar->core.width + bar_bw;
  }

  /* resize the scrollbar to be the correct height or width. */

  XtResizeWidget(bar, bar->core.width, height, bar->core.border_width);

  SetThumbHeight(w);
}

/* ARGSUSED */
static void 
GExpose(w,junk,event,cont)
Widget w;
XtPointer junk;
XEvent *event;
Boolean *cont;
{

/*
 * Graphics exposure events are not currently sent to exposure proc.
 */

  if (event->type == GraphicsExpose)
    Redisplay(w, event, NULL);

} /* ChildExpose */

/*
 * Repaint the widget's child Window Widget.
 */

/* ARGSUSED */
static void Redisplay(w, event, region)
Widget w;
XEvent *event;
Region region;
{
  int top, height;		/* the locations of the top and height
				   of the region that needs to be repainted. */

/*
 * This routine tells the client which sections of the window to 
 * repaint in his callback function which does the actual repainting.
 */

  if (event->type == Expose) {
    top = event->xexpose.y;
    height = event->xexpose.height;
  }
  else {
    top = event->xgraphicsexpose.y;
    height  = event->xgraphicsexpose.height;
  }
  
  PaintText(w, top, height);

} /* redisplay (expose) */

/*	Function Name: PaintText
 *	Description: paints the text at the give location for a given height.
 *	Arguments: w - the sbl widget.
 *                 y_loc, height - location and size of area to paint.
 *	Returns: none
 */

static void
PaintText(w, y_loc, height)
Widget w;
int y_loc, height;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;
  int start_line, num_lines, location;
  GC gc=sblw->scroll.normal_gc;

  start_line = y_loc / sblw->scroll.font_height + sblw->scroll.line_pointer;

  if (start_line >= sblw->scroll.lines)
    return;
  
  num_lines = height / sblw->scroll.font_height + 1;

/*
 * Only integer arithmetic makes this possible. 
 */

  location =  y_loc / sblw->scroll.font_height * sblw->scroll.font_height;

  PrintText(w, start_line, num_lines, location);

  if (sblw->scroll.hil_line>=start_line && sblw->scroll.hil_line<start_line+num_lines) {


      XFillRectangle(XtDisplay(w),XtWindow(w),sblw->scroll.hil_gc,
                     (sblw->scroll.hil_xloc>=0) ? sblw->scroll.hil_xloc:-sblw->scroll.hil_xloc,
                     (sblw->scroll.hil_line-sblw->scroll.line_pointer)*sblw->scroll.font_height,
                      sblw->scroll.hil_width,
                      sblw->scroll.font_height);
  }

#ifdef XAW3D
 (*((ScrollByLineWidgetClass)(XtClass((Widget)w)))->threeD_class.shadowdraw)(w,NULL,NULL,
		False);
#endif
     
} 

/*	Function Name: Page
 *	Description: This function pages the widget, by the amount it recieves
 *                   from the translation Manager.
 *	Arguments: w - the ScrollByLineWidget.
 *                 event - the event that caused this return.
 *                 params - the parameters passed to it.
 *                 num_params - the number of parameters.
 *	Returns: none.
 */

/* ARGSUSED */
static void 
Page(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal *num_params;
{
   ScrollByLineWidget sblw = (ScrollByLineWidget) w;
   Widget bar = sblw->scroll.bar;

   if (*num_params < 1)
     return;
/*
 * If no scroll bar is visible then do not page, as the entire window is shown,
 * of scrolling has been turned off. 
 */

   if (bar == (Widget) NULL)
     return;

   switch ( params[0][0] ) {
   case 'f':
   case 'F':
     /* move one page forward */
     VerticalScroll(bar, NULL, (int) bar->core.height);
     break;
   case 'b':
   case 'B':
     /* move one page backward */
     VerticalScroll(bar, NULL,  - (int) bar->core.height);
     break;
   case 'L':
   case 'l':
     /* move one line forward */
     VerticalScroll(bar, NULL, 
		    (int) atoi(params[1]) * sblw->scroll.font_height);
     break;
   default:
     return;
   }
}

/*	Function Name: CreateScrollbar
 *	Description: createst the scrollbar for us.
 *	Arguments: w - sblw widget.
 *	Returns: none.
 */

static void
CreateScrollbar(w)
Widget w;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;
  Arg args[5];
  Cardinal num_args = 0;

  if (sblw->scroll.bar != NULL) 
    return;

  XtSetArg(args[num_args], XtNorientation, XtorientVertical); num_args++;
  
  sblw->scroll.bar = XtCreateWidget("scrollbar", scrollbarWidgetClass, w, 
				    args, num_args);
  XtAddCallback(sblw->scroll.bar, XtNscrollProc, VerticalScroll, NULL);
  XtAddCallback(sblw->scroll.bar, XtNjumpProc, VerticalJump, NULL); 

}

/*	Function Name: ScrollVerticalText
 *	Description: This accomplished the actual movement of the text.
 *	Arguments: w - the ScrollByLine Widget.
 *                 new_line - the new location for the line pointer
 *                 force_redisplay - should we force this window to get 
 *                                   redisplayed?
 *	Returns: True if the thumb needs to be moved.
 */

static Boolean
ScrollVerticalText(w, new_line, force_redisp)
Widget w;
int new_line;
Boolean force_redisp;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;
  int num_lines = (int)w->core.height / sblw->scroll.font_height + 1;
  int max_lines, old_line;
  Boolean move_thumb = FALSE;

/*
 * Do not let the window extend out of bounds.
 */

  if ( new_line < 0) {
    new_line = 0;
    move_thumb = TRUE;
  }
  else {
    max_lines = sblw->scroll.lines - (int)w->core.height / sblw->scroll.font_height;
    AssignMax(max_lines, 0);

    if ( new_line > max_lines ) {
      new_line = max_lines;
      move_thumb = TRUE;
    }
  }

/* 
 * If forced to redisplay then do a full redisplay and return.
 */

  old_line = sblw->scroll.line_pointer;	
  sblw->scroll.line_pointer = new_line;	/* Set current top of page. */

  if (force_redisp) 
    MoveAndClearText(w, 0, /* cause a full redisplay */ 0, 0);

  if (new_line == old_line)
    return(move_thumb);

/*
 * Scroll forward.
 */

  else if (new_line < old_line) {
    int lines_to_scroll = old_line - new_line;
    MoveAndClearText(w, 0, num_lines - lines_to_scroll, lines_to_scroll);
  }

/* 
 * Scroll back.
 */

  else {
    int lines_to_scroll = new_line - old_line;
    MoveAndClearText(w, lines_to_scroll, num_lines - lines_to_scroll, 0);
  }

  return(move_thumb);
}

/*	Function Name: MoveAndClearText
 *	Description: Blits as much text as it can and clear the
 *                   remaining area with generate exposures TRUE.
 *	Arguments: w - the sbl widget.
 *                 old_y - the old y position.
 *                 height - height of area to move.
 *                 new_y - new y position.
 *	Returns: none
 */
	   
static void
MoveAndClearText(w, old_y, height, new_y)
Widget w;
int old_y, new_y, height;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;
  int from_left = sblw->scroll.indent + sblw->scroll.offset;
  int y_clear;
  int num_lines,old_lineptr,saved_hil_line;

  old_lineptr = (new_y > old_y) ? new_y : -old_y;
  old_lineptr += sblw->scroll.line_pointer;

  old_y *= sblw->scroll.font_height;
  new_y *= sblw->scroll.font_height;
  height *= sblw->scroll.font_height;

/*
 * If we are already at the right location then do nothing.
 * (height == 0).
 *
 * If we have scrolled more than a screen height then just clear
 * the window.
 */

  if (height <= sblw->scroll.font_height) { /* avoid rounding errors. */
    XClearArea( XtDisplay(w), XtWindow(w), from_left, 0, 
	       (unsigned int) 0, (unsigned int) 0, FALSE);    
    PaintText(w, 0, (int) sblw->core.height);
    return;
  }

#ifdef XAW3D
  if (old_y > new_y) {
	if ((int)new_y < (int)sblw->threeD.shadow_width) {
		old_y += sblw->threeD.shadow_width - new_y;
		new_y = sblw->threeD.shadow_width;
	}
  	if ((int)height + (int)old_y > (int)w->core.height - (int)sblw->threeD.shadow_width)
    	height = w->core.height - sblw->threeD.shadow_width - old_y;
  } else {
	if ((int)old_y < (int)sblw->threeD.shadow_width) {
		new_y += sblw->threeD.shadow_width - old_y;
		old_y = sblw->threeD.shadow_width;
	}
  	if ((int)height + (int)new_y > (int)w->core.height - (int)sblw->threeD.shadow_width)
    	height = w->core.height  - sblw->threeD.shadow_width - new_y;
  }
#else
  if (old_y > new_y) {
  	if ((int)height + (int)old_y > (int)w->core.height)
    	height = w->core.height - old_y;
  } else {
  	if ((int)height + (int)new_y > (int)w->core.height)
    	height = w->core.height - new_y;
  }
#endif

/* Before we move the image we have to undo the highlighting
 * because otherwise parts of the image may be painted twice
 * resulting in corrupted highlighted area (due to XOR).
 * E.g. highlighted area is moved and part of it is repainted
 * by PainText below.
 */

  num_lines  = sblw->core.height / sblw->scroll.font_height + 1;
  if (	 sblw->scroll.hil_line >= old_lineptr
	  && sblw->scroll.hil_line < old_lineptr+num_lines) {


      XFillRectangle(XtDisplay(w),XtWindow(w),sblw->scroll.hil_gc,
                     (sblw->scroll.hil_xloc>=0) ? sblw->scroll.hil_xloc:-sblw->scroll.hil_xloc,
                     (sblw->scroll.hil_line-old_lineptr)*sblw->scroll.font_height,
                      sblw->scroll.hil_width,
                      sblw->scroll.font_height);
  	 /* save hil line and disable highlighting */
	 saved_hil_line = sblw->scroll.hil_line;
	 sblw->scroll.hil_line = -1;
  } else {
	 saved_hil_line = -1;
  }

  XCopyArea(XtDisplay(w), XtWindow(w), XtWindow(w), sblw->scroll.move_gc,
	    from_left, old_y, 
	    (unsigned int) w->core.width - from_left, (unsigned int) height,
	    from_left, new_y);

  if (old_y > new_y)
    height -= sblw->scroll.font_height/2;  /* clear 1/2 font of extra space,
					      to make sure we don't lose or
					      gain decenders. */
  else
    height -= sblw->scroll.font_height;  /* clear 1 font of extra space,
					    to make sure we don't overwrite
					    with a last line in buffer. */

  if (old_y > new_y)
    y_clear = height;
  else
    y_clear = 0;

/*
 * We cannot use generate exposures, since that may allow another move and
 * clear before the area get repainted, this would be bad.
 */

  XClearArea( XtDisplay(w), XtWindow(w), from_left, y_clear,
	     (unsigned int) 0, (unsigned int) (w->core.height - height),
	     FALSE);

  /* paint text, possible with highlighting disabled */
  PaintText(w, (int) y_clear, (int) (w->core.height - height));

  /* now do the highligting */
  if (saved_hil_line >= 0) {
	 /* restore the hil_line */
	 sblw->scroll.hil_line = saved_hil_line;
     XFillRectangle(XtDisplay(w),XtWindow(w),sblw->scroll.hil_gc,
                     (sblw->scroll.hil_xloc>=0) ? sblw->scroll.hil_xloc:-sblw->scroll.hil_xloc,
                     (sblw->scroll.hil_line-sblw->scroll.line_pointer)*sblw->scroll.font_height,
                      sblw->scroll.hil_width,
                      sblw->scroll.font_height);
	
  }
}

/*	Function Name: SetThumbHeight
 *	Description: Set the height of the thumb.
 *	Arguments: w - the sblw widget.
 *	Returns: none
 */

static void
SetThumbHeight(w)
Widget w;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;
  float shown;

  if (sblw->scroll.bar == NULL)
    return;

  if (sblw->scroll.lines == 0) 
    shown = 1.0;
  else
    shown = (float) w->core.height / (float) (sblw->scroll.lines *
					      sblw->scroll.font_height);
  if (shown > 1.0)
    shown = 1.0;

  XawScrollbarSetThumb( sblw->scroll.bar, (float) -1, shown );
}

/*	Function Name: SetThumb
 *	Description: Set the thumb location.
 *	Arguments: w - the sblw.
 *	Returns: none
 */

static void
SetThumb(w) 
Widget w;
{
  float location;
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;

  if ( (sblw->scroll.lines == 0) || (sblw->scroll.bar == NULL) )
    return;

  location = (float) sblw->scroll.line_pointer / (float) sblw->scroll.lines; 
  XawScrollbarSetThumb( sblw->scroll.bar, location , (float) -1 );
}

/*	Function Name: VerticalJump.
 *	Description: This function moves the test
 *                   as the vertical scroll bar is moved.
 *	Arguments: w - the scrollbar widget.
 *                 junk - not used.
 *                 percent - the position of the scrollbar.
 *	Returns: none.
 */

/* ARGSUSED */
static void
VerticalJump(w, junk, percent_ptr)
Widget w;
XtPointer junk;
XtPointer percent_ptr;
{
  float percent = *((float *) percent_ptr);
  int new_line;			/* The new location for the line pointer. */
  ScrollByLineWidget sblw = (ScrollByLineWidget) XtParent(w);

  new_line = (int) ((float) sblw->scroll.lines * percent);
  if (ScrollVerticalText( (Widget) sblw, new_line, FALSE))
    SetThumb((Widget) sblw);
}

/*	Function Name: VerticalScroll
 *	Description: This function moves the postition of the interior window
 *                   as the vertical scroll bar is moved.
 *	Arguments: w - the scrollbar widget.
 *                 junk - not used.
 *                 pos - the position of the cursor.
 *	Returns: none.
 */

/* ARGSUSED */
static void
VerticalScroll(w, client_data, call_data)
Widget w;
XtPointer client_data;
XtPointer call_data;
{
  int pos = (int) call_data;
  int new_line;			/* The new location for the line pointer. */
  ScrollByLineWidget sblw = (ScrollByLineWidget) XtParent(w);

  new_line = sblw->scroll.line_pointer + (pos / sblw->scroll.font_height);
  (void) ScrollVerticalText( (Widget) sblw, new_line, FALSE);
  SetThumb( (Widget) sblw);
}

int h_width;			/* main font width */

static void
ClassPartInitialize(wc)
WidgetClass wc;
{
    ScrollByLineWidgetClass sblc  = (ScrollByLineWidgetClass) wc;
    ScrollByLineWidgetClass sblsc = (ScrollByLineWidgetClass) wc->core_class.superclass;
    if (XtInheritXawFillBuffer==sblc->scrolled_widget_class.fillbuffer)
	sblc->scrolled_widget_class.fillbuffer =
		sblsc->scrolled_widget_class.fillbuffer;
    if (XtInheritXawFreeBuffer==sblc->scrolled_widget_class.freebuffer)
	sblc->scrolled_widget_class.freebuffer =
		sblsc->scrolled_widget_class.freebuffer;
}

/* ARGSUSED */
static void 
Initialize(req, new, args, num_args)
Widget req, new;
ArgList args;
Cardinal *num_args;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) new;
  unsigned long figWidth;
  Atom atomNum;

  sblw->scroll.top_line = NULL;
  sblw->scroll.hil_line = -1;
  sblw->scroll.hil_xloc = 0;
  sblw->scroll.hil_srchstring = NULL;
  sblw->scroll.hil_string[0]=0;
  sblw->scroll.line_pointer = 0;
  sblw->scroll.nchars = 0;
#ifdef THIS_IS_A_BUG
  LoadFile(new);
  sblw->scroll.bar = (Widget) NULL;
#else
  /* T. Straumann: LoadFile tries to move the thumb;
   * will crash if bar not initialized.
   * Also, doing a resize before realization (old location
   * of CreateScrollbar()) resulted in a crash due to
   * deref of sblw->scroll.bar...
   *
   * Note that sblw->scroll.bar _must_ be initialized to 0
   * (CreateScrollbar() tests it).
   */
  sblw->scroll.bar = (Widget) NULL;
  CreateScrollbar(new);
  LoadFile(new);
#endif

  /* Try to set the default width (manpages seem to have 65 columns) */
  if (new->core.width == 0) {
  	new->core.width = 2 * sblw->scroll.indent +
					  65 * sblw->scroll.normal_font->max_bounds.width +
					  sblw->scroll.bar->core.width;
  }

  sblw->scroll.font_height = (sblw->scroll.normal_font->max_bounds.ascent + 
			      sblw->scroll.normal_font->max_bounds.descent); 

  atomNum = XInternAtom(XtDisplay(req), "FIGURE_WIDTH", False);

  if (XGetFontProperty(sblw->scroll.normal_font, atomNum, &figWidth))
      h_width = figWidth;
  else
    h_width = XTextWidth(sblw->scroll.normal_font, "$", 1);

 if (new->core.height==0) {
	new->core.height = sblw->scroll.font_height * sblw->scroll.n_lines;
 }


} /* Initialize. */

/*	Function Name: CreateGCs
 *	Description: Creates the graphics contexts that we need. 
 *	Arguments: w - the sblw.
 *	Returns: none
 */

static void
CreateGCs(w) 
Widget w;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;

  XtGCMask mask;
  XGCValues values;

  values.graphics_exposures = TRUE;
  sblw->scroll.move_gc = XtGetGC(w, GCGraphicsExposures, &values);

  mask = GCForeground | GCFont;
  values.foreground = sblw->scroll.foreground;
  
  values.font = sblw->scroll.normal_font->fid;
  sblw->scroll.normal_gc = XtGetGC(w, mask, &values);

  values.font = sblw->scroll.italic_font->fid;
  sblw->scroll.italic_gc = XtGetGC(w, mask, &values);

  values.font = sblw->scroll.bold_font->fid;
  sblw->scroll.bold_gc = XtGetGC(w, mask, &values);

  values.font = sblw->scroll.symbol_font->fid;
  sblw->scroll.symbol_gc = XtGetGC(w, mask, &values);

  values.plane_mask=sblw->scroll.foreground^sblw->core.background_pixel;
  values.function  = GXinvert;
  mask=GCFunction|GCPlaneMask|GCForeground;

  sblw->scroll.hil_gc=XtGetGC(w, mask, &values);
}

/*	Function Name: DestroyGCs
 *	Description: removes all gcs for this widget.
 *	Arguments: w - the widget.
 *	Returns: none
 */

static void
DestroyGCs(w)
Widget w;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;

  XtReleaseGC(w, sblw->scroll.normal_gc);
  XtReleaseGC(w, sblw->scroll.bold_gc);
  XtReleaseGC(w, sblw->scroll.italic_gc);
  XtReleaseGC(w, sblw->scroll.move_gc);
  XtReleaseGC(w, sblw->scroll.hil_gc);
}

static void
Realize(w, valueMask, attributes)
register Widget w;
Mask *valueMask;
XSetWindowAttributes *attributes;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;

  CreateGCs(w);
  Layout(w);
  (*XtSuperclass(w)->core_class.realize) (w, valueMask, attributes);
  XtRealizeWidget(sblw->scroll.bar); /* realize scrollbar. */
  XtMapWidget(sblw->scroll.bar); /* map scrollbar. */

  XtAddEventHandler(w, 0, TRUE, GExpose, NULL); /* Get Graphics Exposures */
} /* Realize */

/*	Function Name: Destroy
 *	Description: Cleans up when we are killed.
 *	Arguments: w - the widget.
 *	Returns: none
 */

static void
Destroy(w)
Widget w;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;

  if (sblw->scroll.bar != NULL)
    XtDestroyWidget(sblw->scroll.bar); /* Destroy scrollbar. */
  /* T. Straumann: resource manager opens 'file' only once
   * and keeps it in cache
  if (sblw->scroll.file != NULL)
    fclose(sblw->scroll.file);
   */
  /* T. Straumann: release memory */
  if ( sblw->scroll.top_line != NULL) {
	  ScrollByLineWidgetClass sblwc=(ScrollByLineWidgetClass)XtClass(w);
	  /* free characters. */
	  sblwc->scrolled_widget_class.freebuffer(sblw,*(sblw->scroll.top_line),sblw->scroll.nchars);
	  XtFree((char *)(sblw->scroll.top_line)); /* free lines. */
  }
  DestroyGCs(w);
}

/*
 *
 * Set Values
 *
 */

/* ARGSUSED */
static Boolean 
SetValuesHook( w, args, num_args)
Widget w;
ArgList args;
Cardinal *num_args;
{
  Boolean ret = TRUE;
  int i;

  for (i = 0; i < *num_args; i++) {
    if (strcmp(XtNfile, args[i].name) == 0) {
      LoadFile(w);
      ret = TRUE;
    }
  }

/*
 * Changing anthing else will have strange effects, I don't need it so
 * I didn't code it.
 */

  return(ret);

} /* Set Values */

/* Fill Buffer
 *
 * returns: buffer size (maybe 0)
 *	    -1 on error
 */

#ifndef USE_ZLIB
static int
FillBuffer(sblw,buffer)
ScrollByLineWidget sblw;
char **buffer;
{
FILE		*file = sblw->scroll.file;
struct stat	fileinfo;	/* file information from fstat. */
char		*page;

    if (!file) return 0;

    /*
     * Get file size and allocate a chunk of memory for the file to be 
     * copied into.
     */

    if (fstat(fileno(file), &fileinfo))
	XtAppError(XtWidgetToApplicationContext((Widget)sblw), 
		"SBLW FillBuffer: Failure in fstat.");
    
    if (fileinfo.st_size==0) {
	*buffer=NULL;
	return 0;
    }

#ifdef USE_MMAP
    fprintf(stderr,"mapping\n");
    *buffer = (char*)mmap(0,fileinfo.st_size,PROT_READ,MAP_SHARED,fileno(file),0);
	   
    if (MAP_FAILED==*buffer) {
	*buffer=0;
	XtAppError(XtWidgetToApplicationContext((Widget)sblw), 
		sys_errlist[errno]);
	return -1;
    }	
#else
    /* leave space for the NULL */
    *buffer = page = XtMalloc(fileinfo.st_size + 1);

    if (!page) return -1;

    /*
     * Copy the file into memory. 
     */

    /* T. Straumann, rewind the stream before reading; file
     * might have been used by others
     */
    rewind(file);
    if (fread(page, sizeof(char), fileinfo.st_size, file) == 0)
	XtAppError(XtWidgetToApplicationContext((Widget)sblw), 
		"SBLW FillBuffer: Failure in fread.");

    /* put NULL at end of buffer. */

    *(page + fileinfo.st_size) = '\0';
#endif

    sblw->scroll.hil_line=-1;

    return fileinfo.st_size+1;
}/* Fill Buffer */

#else

#endif

/* Free Buffer */
static void
FreeBuffer(sblw, buf, size)
ScrollByLineWidget	sblw;
char				*buf;
int					size;
{
    if (size>0) {
#ifdef USE_MMAP
	munmap(buf,size);
#else
	XtFree(buf);
#endif
    }
}

/* 
 * A little design philosophy is probabally wise to include at this point.
 *
 * One of the things that I has hoped to achieve with xman is to make the
 * viewing of manpage not only easy for the nieve user, but also fast for
 * the experienced user, I wanted to be able to use it too.  To achieve this
 * I end up sacrificing a bit of start up time for the manual data structure.
 * As well as, the overhead of reading the entire file before putting it up 
 * on the display.  This is actually hardly even noticeable since most manual
 * pages are shots, one to two pages - the notable exception is of course csh,
 * but then that should be broken up anyway. 
 *
 * METHOD:
 *
 * I allocate a chunk of space that is the size of the file, plus a null for
 * debugging.  Then copiesthe file into this chunk of memory.  I then allocate
 * an array of char*'s that are assigned to the beginning of each line.  Yes,
 * this means that I have to read the file twice, and could probabally be more
 * clever about it, but once it is in memory the second read is damn fast.
 * There are a few obsucrities here about guessing the number of lines and
 * reallocing if I guess wrong, but other than that it is pretty straight 
 * forward.
 *
 *                                         Chris Peterson
 *                                         1/27/88
 */

#define ADD_MORE_MEM 100	/* first guesses for allocations. */
#define CHAR_PER_LINE 40

/*	Function Name: LoadFile
 *	Description: Loads the current file into the internal memory.
 *	Arguments: w - the sblw.
 *	Returns: none
 */

static void
LoadFile(w)
Widget w;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;

  char *page;
  char **line_pointer,**top_line; /* pointers to beginnings of the 
				     lines of the file. */
  int nlines;			/* the current number of allocated lines. */

  if ( sblw->scroll.top_line != NULL) {
	  ScrollByLineWidgetClass sblwc=(ScrollByLineWidgetClass)XtClass(w);
	  /* free characters. */
	  sblwc->scrolled_widget_class.freebuffer(sblw,*(sblw->scroll.top_line),sblw->scroll.nchars);
	  XtFree((char *)(sblw->scroll.top_line)); /* free lines. */
  }
  sblw->scroll.top_line = NULL;

  if ((sblw->scroll.nchars =
		((ScrollByLineWidgetClass)XtClass(w))->scrolled_widget_class.fillbuffer(sblw,&page))<=0)
      return;

/* 
 * Allocate a space for a list of pointer to the beginning of each line.
 */

  if ( (nlines = sblw->scroll.nchars/CHAR_PER_LINE) == 0)
    return;

  top_line = line_pointer = (char**) XtMalloc( nlines * sizeof(char *) );


/*
 * Go through the file setting a line pointer to the character after each 
 * new line.  If we run out of line pointer space then realloc that space
 * with space for more lines.
 */

  *line_pointer++ = page;	/* first line points to first char in buffer.*/
  while (*page != '\0') {

    if ( *page == '\n' ) {
      *line_pointer++ = page + 1;

      if (line_pointer >= top_line + nlines) {
	top_line = (char **) XtRealloc( (char *)top_line, (nlines +
					  ADD_MORE_MEM) * sizeof(char *) );
	line_pointer = top_line + nlines;
	nlines += ADD_MORE_MEM;
      }
    }
    page++;
  }
   
/*
 *  Realloc the line pointer space to take only the minimum amount of memory
 */

  sblw->scroll.lines = nlines = line_pointer - top_line - 1;
  top_line = (char **) XtRealloc((char *)top_line, nlines * sizeof(char *));

/*
 * Store the memory pointers
 */

  sblw->scroll.top_line = top_line;
  sblw->scroll.line_pointer = 0;
  SetThumbHeight(w);
  SetThumb(w);
}

#define NLINES  66		/* This is the number of lines to wait until
				   we boldify the line again, this allows 
				   me to bold the first line of each page.*/
#define BACKSPACE 010		/* I doubt you would want to change this. */

#define NORMAL	0
#define BOLD	1
#define ITALIC	2
#define SYMBOL	3
#define WHICH(italic, bold)	((bold) ? BOLD : ((italic) ? ITALIC : NORMAL))
				/* Choose BOLD over ITALICS.  If neither */
				/* is chosen, use NORMAL. */

static int DumpText();
static int ScanLine();
static void ShowSelection();
static char * StringInBuf();
static char * AnyStringInBuf();
static Boolean Boldify();	

/*	Function Name: PrintText
 *	Description: This function actually prints the text.
 *	Arguments: w - the ScrollByLine widget.
 *                 start_line - line to start printing,
 *                 num_lines - the number of lines to print.
 *                 location - the location to print the text.
 *	Returns: none.
 */

/* ARGSUSED */

static void
PrintText(w, start_line, num_lines, location)
Widget w;
int  start_line, num_lines, location;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;

  register char *bufp, *c;	/* Generic char pointers */
  int current_line;		/* the number of the currrent line */
  char buf[BUFSIZ];		/* Misc. characters */
  int width;			/* Width of a tab stop. */
  Boolean italicflag = FALSE;	/* Print text in italics?? */
  Boolean first = TRUE;	        /* First line of a manual page??? */
  int x_loc, y_loc;		/* x and y location of text. */

/*
 * For table hack
 * To get columns to line up reasonably in most cases, make the
 * assumption that they were lined up using lots of spaces, where
 * lots is greater than two. Use a space width of 70% of the
 * widest character in the font.
 */
  XFontStruct * h_font;
  int h_col, h_fix;
  char * h_c;

  h_font = sblw->scroll.normal_font;

/*
 * Nothing loaded, take no action.
 */

  if (sblw->scroll.top_line == NULL || num_lines == 0)
    return;

  current_line = start_line;

/* Set the first character to print at the first line. */

  c = *(sblw->scroll.top_line + start_line);

 /* Width of a tab stop. */
  width = 8 * h_width;

/*
 * Because XDrawString uses the bottom of the text as a position
 * reference, add the height from the top of the font to the baseline
 * to the ScollByLine position reference.
 */

  y_loc = location + sblw->scroll.normal_font->max_bounds.ascent;

/*
 * Ok, here's the more than mildly heuristic man page formatter.
 * We put chars into buf until either a font change or newline
 * occurs (at which time we flush it to the screen.)
 */


  bufp = buf;
  x_loc = sblw->scroll.offset + sblw->scroll.indent;
  h_col = 0;
  
/*
 * A fix:
 * Assume that we are always starting to print on or before the
 * first line of a page, and then prove it if we aren't.
 */
  for (h_fix = 1; h_fix <= (start_line % NLINES); h_fix++)
    if (**(sblw->scroll.top_line + start_line - h_fix) != '\n')
      {
	first = FALSE;
	break;
      }

  while(TRUE) {
    if (current_line % NLINES == 0)
      first = TRUE;

/* 
 * Lets make sure that we do not run out of space in our buffer, making full
 * use of it is not critical since no window will be wide enough to display
 * nearly BUFSIZ characters.
 */

    if ( (bufp - buf) > (BUFSIZ - 10) )
      /* Toss everything until we find a <CR> or the end of the buffer. */
      while ( (*c != '\n') && (*c != '\0') ) c++;

    switch(*c) {

    case '\0':		      /* If we reach the end of the file then return */
      DumpText(w, x_loc, y_loc, buf, bufp - buf, WHICH(italicflag, first));
      return;

    case '\n':
      if (bufp != buf) {
	Boolean bold;
	*bufp = '\0';		/* for Boldify. */
	bold = ( (first) || ((x_loc == (sblw->scroll.offset +
			      sblw->scroll.indent)) && Boldify(buf)) );

	(void) DumpText(w, x_loc, y_loc, buf, bufp - buf,
			WHICH(italicflag, bold));

	if (bold)
	  first = FALSE;
      }

/* 
 * If we have painted the required number of lines then we should now return.
 */
      if (++current_line == start_line + num_lines ) 
	return;

      bufp = buf;
      italicflag = FALSE;
      x_loc = sblw->scroll.offset + sblw->scroll.indent;
      h_col = 0;
      y_loc += sblw->scroll.font_height;
      break;

/*
 * This tab handling code is not very clever it moves the cursor over
 * to the next boundry of eight (8) spaces, as calculated in width just
 * before the printing loop started.
 */

    case '\t':			/* TAB */
      x_loc = DumpText(w, x_loc, y_loc, buf, bufp - buf,
		       WHICH(italicflag, first));
      h_col += bufp - buf;
      bufp = buf; 
      italicflag = FALSE;
      x_loc = sblw->scroll.offset + sblw->scroll.indent;
      h_col = h_col + 8 - (h_col%8);
      x_loc += h_width * h_col;
      break;

    case ' ':
      h_c = c + 1;
      while (*h_c == ' ') h_c++;

      if (h_c - c < 4)
	{
	  *bufp++ = *c;
	  break;
	}

      x_loc = DumpText(w, x_loc, y_loc, buf, bufp - buf,
		       WHICH(italicflag, first));
      h_col += bufp - buf;
      bufp = buf; 
      italicflag = FALSE;

      x_loc = sblw->scroll.offset + sblw->scroll.indent; 
      h_col += (h_c - c);
      x_loc += h_width * h_col; 
      c = h_c - 1;
      break;

    case '\033':		/* ignore esc sequences for now */
      c++;			/* should always be esc-x */
      break;

/* 
 * Overstrike code supplied by: cs.utexas.edu!ut-emx!clyde@rutgers.edu 
 * Since my manual pages do not have overstrike I couldn't test this.
 */

    case BACKSPACE:		/* Backspacing for nroff bolding */
      if (c[-1] == c[1] && c[1] != BACKSPACE) {	/* overstriking one char */
	bufp--;		/* Zap 1st instance of char to bolden */
	if (bufp > buf) {
	  x_loc = DumpText(w, x_loc, y_loc, buf, bufp - buf,
			   WHICH(italicflag, FALSE));
	  h_col += bufp - buf;
	  bufp = buf;
	}
	/*
	 *     Nroff bolding looks like:
	 *	 	     C\bC\bC\bCN...
	 * c points to ----^      ^
	 * it needs to point to --^
	 */
	while (*c == BACKSPACE && c[-1] == c[1]) {
	  *bufp++ = c[1];
	  c += 3;
	}
	c-=2;		/* Back up to previous char */
	x_loc = DumpText(w, x_loc, y_loc, buf, bufp - buf, BOLD);
	h_col += bufp - buf;
	bufp = buf;
	first = FALSE;

      }
      else {
	if ((c[-1] == 'o' && c[1] == '+')          /* Nroff bullet */
	    || (c[-1] == '+' && c[1] == 'o')) {	   /* Nroff bullet */
				/* If we run into a bullet, print out */
				/* everything that's accumulated to this */
				/* point, then the bullet, then resume. */
	  bufp--;
	  x_loc = DumpText(w, x_loc, y_loc, buf, bufp - buf,
			   WHICH(italicflag, FALSE));
	  h_col += bufp - buf;
	  bufp = buf;
	  *bufp = (char)183;
	  x_loc = DumpText(w, x_loc, y_loc, buf, 1, SYMBOL);
	  h_col++;
	  c++;
	}
	else {		/* 'real' backspace - back up output ptr */
	  bufp--;
	}
      }
      break;

/* End of contributed overstrike code. */
  
   case '_':			/* look for underlining [italicize] */
      if(*(c + 1) == BACKSPACE) {
	if(!italicflag) {	/* font change? */
	  if (bufp != buf) {
	    x_loc = DumpText(w, x_loc, y_loc, buf, bufp - buf, NORMAL);
	    h_col += bufp - buf;
	    bufp = buf;
	  }
	  italicflag = TRUE;
	}
	c += 2;
	*bufp++ = *c;
	break;
      }
      /* else fall through to default, because this was a real underscore. */

    default:
      if(italicflag) { 			/* font change? */
	if (bufp != buf) {
	  x_loc = DumpText(w, x_loc, y_loc, buf, bufp - buf,
			   WHICH(italicflag, FALSE));
	  h_col += bufp - buf;
	  bufp = buf;
	}
	italicflag = FALSE;
      }
      *bufp++ = *c;
      break;
    }
    c++;
  }
}

/*	Function Name: DumpText
 *	Description: Dumps text to the screen.
 *	Arguments: w - the widget.
 *                 x_loc - to dump text at.
 *                 y_loc - the y_location to draw_text.
 *                 buf - buffer to dump.
 *                 italic, bold, boolean that tells us which gc to use.
 *	Returns: x_location of the end of the text.
 */

static int
DumpText(w, x_loc, y_loc, buf, len, format)
Widget w;
int x_loc, y_loc;
char * buf;
int len;
int format;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;
  GC gc;
  XFontStruct * font;
  
  if (len==0) return(x_loc);
  buf[len]=0;

  switch(format) {

  case ITALIC:
    gc = sblw->scroll.italic_gc;
    font = sblw->scroll.italic_font;
    break;

  case BOLD:
      gc = sblw->scroll.bold_gc;
      font = sblw->scroll.bold_font;
    break;

  case SYMBOL:
    gc = sblw->scroll.symbol_gc;
    font = sblw->scroll.symbol_font;
    break;

  default:
      gc = sblw->scroll.normal_gc;
      font = sblw->scroll.normal_font;
    break;
    }


  XDrawString(XtDisplay(w), XtWindow(w), gc, x_loc, y_loc, buf, len);
  return(x_loc + XTextWidth(font, buf, len));
}

/*	Function Name: Boldify
 *	Description: look for keyword.
 *	Arguments: sp - string pointer.
 *	Returns: 1 if keyword else 0.
 */

static Boolean
Boldify(sp)
register char *sp;
{
  register char *sp_pointer;
  int length,count;

/* 
 * If there are not lower case letters in the line the assume it is a
 * keyword and boldify it in PrintManpage.
 */

  length = strlen(sp);
  for (sp_pointer = sp, count = 0; count < length; sp_pointer++,count++) 
    if ( !isupper(*sp_pointer) && !isspace(*sp_pointer) )
      return(0);
  return(1);
}


#define UPPER_C(ch) ((ch>='a'&&ch<='z')?(ch-'a'+'A'):(ch))

static char * AnyStringInBuf(w,bufptr)
  Widget w;
  char *bufptr;

{
 ScrollByLineWidget sblw=(ScrollByLineWidget)w;
  register char *srchptr=sblw->scroll.hil_srchstring;
  static char *buf;
  register char *found;
  if (strlen(srchptr)==0) { /*erster Durchlauf*/
    buf=bufptr;
    strcpy(srchptr,bufptr);
    strtok(srchptr,sblw->scroll.separators);
    return(bufptr);
  } else {
    found=strtok(NULL,sblw->scroll.separators);
    if (found==NULL) {
      srchptr[0]=0;
      return(0);
    }
    strcpy(srchptr,found);
    return( bufptr + (found - srchptr) - (bufptr-buf) );
  }

}

static char *StringInBuf(w,bufptr)
  char *bufptr;

Widget w;

{
 ScrollByLineWidget sblw=(ScrollByLineWidget)w;
 register char  *scan, *sscan;
 register char *srchptr=sblw->scroll.hil_srchstring;
 register int i;

 int len=strlen(srchptr);

 bufptr--;
 sscan=srchptr;
 srchptr[len]=04;

  do {

  bufptr++;
  if(*srchptr==UPPER_C(*bufptr)) {

     scan=bufptr+1;
     sscan=srchptr+1;

     while(*sscan==UPPER_C(*scan)) {sscan++; scan++;}

   }
  } while (*bufptr!=(char)0 && *sscan!=04); 

 if (*sscan==04) {
   *sscan=0;
   return(bufptr);
 }else{
   srchptr[len]=0;
   return(0);
 }
}

int
HighlightString(w, srch_str, caseless, ptr_cause)
  Widget w;
  char  *srch_str;
  int caseless,ptr_cause;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;

  register char *bufp, *c;	/* Generic char pointers */
  int current_line,start_line;	/* the number of the currrent line */
  char buf[BUFSIZ];		/* Misc. characters */
  char str[BUFSIZ];   
  int width;			/* Width of a tab stop. */
  Boolean italicflag = FALSE;	/* Print text in italics?? */
  Boolean first = TRUE;	        /* First line of a manual page??? */
  int x_loc;		        /* x and y location of text. */

  int i,len;
/*
 * For table hack
 * To get columns to line up reasonably in most cases, make the
 * assumption that they were lined up using lots of spaces, where
 * lots is greater than two. Use a space width of 70% of the
 * widest character in the font.
 */
  XFontStruct * h_font;
  int h_col, h_fix;
  char * h_c;

  h_font = sblw->scroll.normal_font;

/*
 * Nothing loaded, take no action.
 */
  strcpy(str,srch_str);
  len=strlen(str);

  if (caseless) 
   for (i=0; i<len; i++) str[i]=UPPER_C(str[i]);

  sblw->scroll.hil_srchstring=str;

  if (sblw->scroll.top_line == NULL || str == 0)
    return(0);

  if(!ptr_cause && sblw->scroll.hil_xloc<0) 
     sblw->scroll.hil_xloc=-sblw->scroll.hil_xloc;

  start_line = sblw->scroll.hil_line;
  if (start_line==-1) start_line=0;
  current_line= start_line;
/* Set the first character to print at the first line. */

  c = *(sblw->scroll.top_line + start_line);

 /* Width of a tab stop. */
  width = 8 * h_width;

/*
 * Because XDrawString uses the bottom of the text as a position
 * reference, add the height from the top of the font to the baseline
 * to the ScollByLine position reference.
 */


/*
 * Ok, here's the more than mildly heuristic man page formatter.
 * We put chars into buf until either a font change or newline
 * occurs (at which time we flush it to the screen.)
 */


  bufp = buf;
  x_loc = sblw->scroll.offset + sblw->scroll.indent;
  h_col = 0;
  
/*
 * A fix:
 * Assume that we are always starting to print on or before the
 * first line of a page, and then prove it if we aren't.
 */
  for (h_fix = 1; h_fix <= (start_line % NLINES); h_fix++)
    if (**(sblw->scroll.top_line + start_line - h_fix) != '\n')
      {
	first = FALSE;
	break;
      }

  while(TRUE) {
    if (current_line % NLINES == 0)
      first = TRUE;

/* 
 * Lets make sure that we do not run out of space in our buffer, making full
 * use of it is not critical since no window will be wide enough to display
 * nearly BUFSIZ characters.
 */

    if ( (bufp - buf) > (BUFSIZ - 10) )
      /* Toss everything until we find a <CR> or the end of the buffer. */
      while ( (*c != '\n') && (*c != '\0') ) c++;

    switch(*c) {

    case '\0':		      /* If we reach the end of the file then return */
      ScanLine(w, x_loc, current_line , buf, bufp - buf, WHICH(italicflag, first));
      ShowSelection(w);
      return( (sblw->scroll.hil_srchstring==0)? -1:0);

    case '\n':
      if (bufp != buf) {
	Boolean bold;
	*bufp = '\0';		/* for Boldify. */
	bold = ( (first) || ((x_loc == (sblw->scroll.offset +
			      sblw->scroll.indent)) && Boldify(buf)) );

	ScanLine(w, x_loc, current_line, buf, bufp - buf,
			WHICH(italicflag, bold));
        if (sblw->scroll.hil_srchstring==NULL) {
          ShowSelection(w);return(-1);
        }

	if (bold)
	  first = FALSE;
      }

/* 
 * If we have painted the required number of lines then we should now return.
 */
      if (sblw->scroll.hil_srchstring==NULL) {
         ShowSelection(w);return(-1);
      }

      current_line++;
      bufp = buf;
      italicflag = FALSE;
      x_loc = sblw->scroll.offset + sblw->scroll.indent;
      h_col = 0;
      break;

/*
 * This tab handling code is not very clever it moves the cursor over
 * to the next boundry of eight (8) spaces, as calculated in width just
 * before the printing loop started.
 */

    case '\t':			/* TAB */
      x_loc = ScanLine(w, x_loc, current_line, buf, bufp - buf,
		       WHICH(italicflag, first));
      if (sblw->scroll.hil_srchstring==NULL) {
         ShowSelection(w);return(-1);
      }
      h_col += bufp - buf;
      bufp = buf; 
      italicflag = FALSE;
      x_loc = sblw->scroll.offset + sblw->scroll.indent;
      h_col = h_col + 8 - (h_col%8);
      x_loc += h_width * h_col;
      break;

    case ' ':
      h_c = c + 1;
      while (*h_c == ' ') h_c++;

      if (h_c - c < 4)
	{
	  *bufp++ = *c;
	  break;
	}

      x_loc = ScanLine(w, x_loc, current_line, buf, bufp - buf,
		       WHICH(italicflag, first));
      if (sblw->scroll.hil_srchstring==NULL) {
         ShowSelection(w);return(-1);
      }
      h_col += bufp - buf;
      bufp = buf; 
      italicflag = FALSE;

      x_loc = sblw->scroll.offset + sblw->scroll.indent; 
      h_col += (h_c - c);
      x_loc += h_width * h_col; 
      c = h_c - 1;
      break;

    case '\033':		/* ignore esc sequences for now */
      c++;			/* should always be esc-x */
      break;

/* 
 * Overstrike code supplied by: cs.utexas.edu!ut-emx!clyde@rutgers.edu 
 * Since my manual pages do not have overstrike I couldn't test this.
 */

    case BACKSPACE:		/* Backspacing for nroff bolding */
      if (c[-1] == c[1] && c[1] != BACKSPACE) {	/* overstriking one char */
	bufp--;		/* Zap 1st instance of char to bolden */
	if (bufp > buf) {
	  x_loc = ScanLine(w, x_loc, current_line, buf, bufp - buf,
			   WHICH(italicflag, FALSE));
          if (sblw->scroll.hil_srchstring==NULL) {
             ShowSelection(w);return(-1);
          }
	  h_col += bufp - buf;
	  bufp = buf;
	}
	/*
	 *     Nroff bolding looks like:
	 *	 	     C\bC\bC\bCN...
	 * c points to ----^      ^
	 * it needs to point to --^
	 */
	while (*c == BACKSPACE && c[-1] == c[1]) {
	  *bufp++ = c[1];
	  c += 3;
	}
	c-=2;		/* Back up to previous char */
	x_loc = ScanLine(w, x_loc, current_line, buf, bufp - buf, BOLD);
        if (sblw->scroll.hil_srchstring==NULL) {
           ShowSelection(w);return(-1);
    }
	h_col += bufp - buf;
	bufp = buf;
	first = FALSE;

      }
      else {
	if ((c[-1] == 'o' && c[1] == '+')          /* Nroff bullet */
	    || (c[-1] == '+' && c[1] == 'o')) {	   /* Nroff bullet */
				/* If we run into a bullet, print out */
				/* everything that's accumulated to this */
				/* point, then the bullet, then resume. */
	  bufp--;
	  x_loc = ScanLine(w, x_loc, current_line, buf, bufp - buf,
			   WHICH(italicflag, FALSE));
          if (sblw->scroll.hil_srchstring==NULL) {
             ShowSelection(w);return(-1);
          }
	  h_col += bufp - buf;
	  bufp = buf;
	  *bufp = (char)183;
	  x_loc = ScanLine(w, x_loc, current_line, buf, 1, SYMBOL);
          if (sblw->scroll.hil_srchstring==NULL) {
             ShowSelection(w);return(-1);
          }
	  h_col++;
	  c++;
	}
	else {		/* 'real' backspace - back up output ptr */
	  bufp--;
	}
      }
      break;

/* End of contributed overstrike code. */
  
   case '_':			/* look for underlining [italicize] */
      if(*(c + 1) == BACKSPACE) {
	if(!italicflag) {	/* font change? */
	  if (bufp != buf) {
	    x_loc = ScanLine(w, x_loc, current_line, buf, bufp - buf, NORMAL);
            if (sblw->scroll.hil_srchstring==NULL) {
               ShowSelection(w);return(-1);
            }
	    h_col += bufp - buf;
	    bufp = buf;
	  }
	  italicflag = TRUE;
	}
	c += 2;
	*bufp++ = *c;
	break;
      }
      /* else fall through to default, because this was a real underscore. */

    default:
      if(italicflag) { 			/* font change? */
	if (bufp != buf) {
	  x_loc = ScanLine(w, x_loc, current_line, buf, bufp - buf,
			   WHICH(italicflag, FALSE));
          if (sblw->scroll.hil_srchstring==NULL) {
             ShowSelection(w);return(-1);
          }
	  h_col += bufp - buf;
	  bufp = buf;
	}
	italicflag = FALSE;
      }
      *bufp++ = *c;
      break;
    }
    c++;
  }
}

static int
ScanLine(w, x_loc, thisLine, buf, len, format)
Widget w;
int x_loc, thisLine;
char * buf;
int len;
int format;
{
  ScrollByLineWidget sblw = (ScrollByLineWidget) w;
  XFontStruct * font;
  register int loc_len;
  register char * found;
  unsigned int width;
  char *(*compare)();

  if (len==0) return(x_loc);
  buf[len]=0;

  compare = (strlen(sblw->scroll.hil_srchstring)==0) ? AnyStringInBuf : StringInBuf;

  switch(format) {

  case ITALIC:
    font = sblw->scroll.italic_font;
    break;

  case BOLD:
      font = sblw->scroll.bold_font;
    break;

  case SYMBOL:
    font = sblw->scroll.symbol_font;
    break;

  default:
      font = sblw->scroll.normal_font;
    break;
    }

  while ((found=compare(w,buf))!=NULL &&
          thisLine>=sblw->scroll.hil_line) {


    loc_len=found-buf;
    if (loc_len>0) { /* unhighlighted part */
      x_loc+=XTextWidth(font,buf,loc_len);
      buf+=loc_len;
      len-=loc_len;
    }

     loc_len=strlen(sblw->scroll.hil_srchstring);
     width=XTextWidth(font,buf,loc_len);

    if (thisLine!=sblw->scroll.hil_line || 
         ((sblw->scroll.hil_xloc >=0) ? (x_loc > sblw->scroll.hil_xloc) :
                                       (x_loc+sblw->scroll.hil_xloc <=0  && 
                                        x_loc + (int) width + sblw->scroll.hil_xloc >=0)) ) {
      strncpy(sblw->scroll.hil_string,buf,loc_len);
      sblw->scroll.hil_string[loc_len]=0;
      sblw->scroll.hil_srchstring=NULL;
      sblw->scroll.hil_xloc=(sblw->scroll.hil_xloc>=0)?x_loc:-x_loc;
      sblw->scroll.hil_line=thisLine;
      sblw->scroll.hil_width=width;
      x_loc+=width;
      buf+=loc_len;
      len-=loc_len;
    
      break;
    }

    x_loc+=width;
    buf+=loc_len;
    len-=loc_len;

 }
 return((len>0)?x_loc + XTextWidth(font, buf, len) : x_loc);
}

static void
doUnselect(sblw)
ScrollByLineWidget sblw;
{
 sblw->scroll.hil_line=-1;
 sblw->scroll.hil_xloc= 0;
 XClearArea( XtDisplay((Widget)sblw), XtWindow((Widget)sblw),
			 sblw->scroll.indent + sblw->scroll.offset, 
             0, (unsigned int) 0, (unsigned int) (sblw->core.height), FALSE);
 PaintText((Widget)sblw,0,sblw->core.height);
}

/* ARGSUSED */
static void 
Unselect(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal *num_params;
{
 ScrollByLineWidget sblw=(ScrollByLineWidget)w;
 doUnselect(sblw);
 XtCallCallbackList(w,sblw->scroll.selectionChanged_cbl,(XtPointer)0);
}

/* ARGSUSED */
static void 
Find(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal *num_params;
{
Boolean retryFromTop = False;
String	srch;
if (0==(srch = *num_params>0 ? params[0] : GetHighlightString(w)))
	return;
if ( (retryFromTop = !HighlightString(w,srch,1,0)) ) {
  /* wrap around buffer to search from top */
  doUnselect((ScrollByLineWidget)w);
  HighlightString(w,srch,1,0);
}
}

/* ARGSUSED */
static void 
Select(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal *num_params;
{
int i,max;
Atom a_sels[10];
Display *dpy=XtDisplay(w);
Boolean success;
ScrollByLineWidget sblw=(ScrollByLineWidget)w;
int line= sblw->scroll.line_pointer + event->xbutton.y/sblw->scroll.font_height;

switch(event->type) {
   case KeyPress:
   case KeyRelease:
   case ButtonPress:
   case ButtonRelease:
 			break;

   default: return;
}

max=(*num_params>10)?10:(int)*num_params;

if (max) {
 for (i=0; i<max; i++) {
  a_sels[i]=XInternAtom(dpy,params[i],True);
 }
} else {
 max=1;
 a_sels[0]=XA_PRIMARY;
}

if (event->type==KeyPress || event->type==KeyRelease) {
  if (sblw->scroll.hil_line==-1) return;
} else {
    
  sblw->scroll.hil_line=line;
  sblw->scroll.hil_xloc= - event->xbutton.x;

  HighlightString(w,"",1,1);
}

success=False;
for (i=0;i<max;i++) {
 if (a_sels[i]!=None) {
  success = success || XtOwnSelection(w,a_sels[i],CurrentTime,
                      ConvertSelection,NULL,NULL);
 }
}

if (!success) doUnselect(sblw, event, params, num_params);
XtCallCallbackList(w,
			sblw->scroll.selectionChanged_cbl,
			(XtPointer)(success ? GetHighlightString(w) : 0));

}

char *GetHighlightString(w)
Widget w;

{
 ScrollByLineWidget sblw=(ScrollByLineWidget)w;
 if (sblw->scroll.hil_line!=-1) 
   return(sblw->scroll.hil_string);
 else
   return(0);
}

static void
ShowSelection(w)
 Widget w;
{
    ScrollByLineWidget sblw = (ScrollByLineWidget) w;
    int thisLine=sblw->scroll.hil_line;
    if (sblw->scroll.line_pointer<=thisLine &&
        thisLine<sblw->scroll.line_pointer+w->core.height/sblw->scroll.font_height + 1) {

           XClearArea( XtDisplay(w), XtWindow(w), sblw->scroll.indent + sblw->scroll.offset, 
                       0, (unsigned int) 0, (unsigned int) (w->core.height), FALSE);
           PaintText(w,0,sblw->core.height);

     } else {

           ScrollVerticalText( (Widget) sblw, (thisLine>9)? thisLine-10:0, True);
           SetThumb((Widget) sblw);

     }
}

/* ARGSUSED */
char * GetStartOfFile(w)
  Widget w;    
{
    ScrollByLineWidget sblw = (ScrollByLineWidget) w;
    return(sblw->scroll.top_line[0]);
}
/* ARGSUSED */
char * GetEndOfFile(w)
  Widget w;    
{
    ScrollByLineWidget sblw = (ScrollByLineWidget) w;
    return(strchr(sblw->scroll.top_line[sblw->scroll.lines - 1],(char)0));
}

static Boolean ConvertSelection(w, selection, target,
				type, value, length, format)
    Widget w;
    Atom *selection, *target, *type;
    XtPointer *value;
    unsigned long *length;
    int *format;
{
    ScrollByLineWidget sblw=(ScrollByLineWidget)w;
    Display* d = XtDisplay(w);
    XSelectionRequestEvent* req =
	XtGetSelectionRequest(w, *selection, (XtRequestId)NULL);

    if (*target == XA_TARGETS(d)) {
	Atom* targetP;
	Atom* std_targets;
	unsigned long std_length;
#ifdef __VMS
	XmuConvertStandardSelection(w, req->time, selection, target, type,
				  (void*)&std_targets, &std_length, format);
#else
	XmuConvertStandardSelection(w, req->time, selection, target, type,
				  (XPointer*)&std_targets, &std_length, format);
#endif
	*value = XtMalloc(sizeof(Atom)*(std_length + 5));
	targetP = *(Atom**)value;
	*targetP++ = XA_STRING;
	*targetP++ = XA_TEXT(d);
	*length = std_length + (targetP - (*(Atom **) value));
	bcopy( (char*)std_targets, (char*)targetP, sizeof(Atom)*std_length);
	XtFree((char*)std_targets);
	*type = XA_ATOM;
	*format = 32;
	return True;
    }

    
    
    if (*target == XA_STRING ||
      *target == XA_TEXT(d) ||
      *target == XA_COMPOUND_TEXT(d))
    {
    	if (*target == XA_COMPOUND_TEXT(d))
	    *type = *target;
    	else
	    *type = XA_STRING;
    	*value =  XtNewString(GetHighlightString(sblw));
        if (*value) *length = strlen(sblw->scroll.hil_string);
        else *length=0;
    	*format = 8;
    	return True;
    }
    
#ifdef __VMS
    if (XmuConvertStandardSelection(w, req->time, selection, target, type,
				    (void *)value, length, format))
#else
    if (XmuConvertStandardSelection(w, req->time, selection, target, type,
				    (XPointer *)value, length, format))
#endif
	return True;

    return False;
}

