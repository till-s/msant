/* $Id: SAccmeBSB.c,v 1.7 2000/09/24 19:38:44 till Exp $ */

/*
Copyright (c) 1999 Till Straumann
 */

/*
 * SAccmeBSB.c - Source code file for BSB Acc Menu Entry object.
 *
 * Date:    October, 1999
 *
 * By:      Till Straumann
 *
 * NOTE: The whole geometry management stuff doesn't work as soon as
 *		 the menu is realized (SimpleMenu's
 *		 Geo-manager doesn't treat setValues on rightMargin correctly).
 *		 Therefore, changing disp_sep disp_space do_display currently
 *		 shouldn't be done.
 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xos.h>

#ifdef XAW3D
#include <X11/Xaw3d/XawInit.h>
#else
#include <X11/Xaw/XawInit.h>
#endif

#include "SAccmeBSBP.h"
#include "SmplAccMnP.h"

#include <stdio.h>
#include <ctype.h>

#ifdef XKB
#include <X11/extensions/XKB.h>
#endif

#ifndef SEPARATE_STRINGS
/* include the automatically generated strings of resource names */
#ifdef __VMS
#include "SAccmeBSB_res_c."
#include "SAccmeBSB_class_c."
#include "SAccmeBSB_type_c."
#else
#include "SAccmeBSB_res_c"
#include "SAccmeBSB_class_c"
#include "SAccmeBSB_type_c"
#endif /* __VMS */
#endif

/* hmm, we have to guess the version of Xaw, i.e. we need to know
 * whether Simple has the 'international' and SmeBSB the 'fontset'
 * resource.
 * Unfortunately, libXaw does not defined a version number, so there's
 * no way to know (without e.g. doing a test-compilation).
 * Let's base our base upon the XtVersion number :-(, which will fail
 * if somebody uses an old Xaw together with a new Xt library :-( :-(
 */
#ifndef XAW_RELEASE_GUESS
#define XAW_RELEASE_GUESS	XT_REVISION
#endif

#define offset(field) XtOffsetOf(SAccmeBSBRec, sAccme_bsb.field)

#define FORMAT_DEFAULT "(%m%s)"

static XtResource resources[] = {
		{XtNaccelerators, XtCAccelerators, XtRString,
        sizeof(String), offset(accelerators),
        XtRString, (XtPointer)0},
		{XtNdoDisplayAccelerators, XtCDoDisplayAccelerators, XtRBoolean,
        sizeof(Boolean), offset(do_display),
        XtRImmediate, (XtPointer)True},
		{XtNaccDisplaySpace, XtCAccDisplaySpace, XtRDimension,
        sizeof(Dimension), offset(acc_disp_space),
        XtRImmediate, (XtPointer)0},
		{XtNaccDisplaySep, XtCAccDisplaySep, XtRDimension,
        sizeof(Dimension), offset(acc_disp_sep),
        XtRImmediate, (XtPointer)4},
		{XtNnoneDisplay,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(none_disp_str),XtRString, (XtPointer) "!-"},
		{XtNshiftDisplay,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[0]),XtRString, (XtPointer) "s-"},
		{XtNctrlDisplay,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[1]),XtRString, (XtPointer) "c-"},
		{XtNlockDisplay,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[2]),XtRString, (XtPointer) "l-"},
		{XtNmod1Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[3]),XtRString, (XtPointer) "m1-"},
		{XtNmod2Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[4]),XtRString, (XtPointer) "m2-"},
		{XtNmod3Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[5]),XtRString, (XtPointer) "m3-"},
		{XtNmod4Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[6]),XtRString, (XtPointer) "m4-"},
		{XtNmod5Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[7]),XtRString, (XtPointer) "m5-"},
		{XtNbtn1Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[8]),XtRString, (XtPointer) "b1-"},
		{XtNbtn2Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[9]),XtRString, (XtPointer) "b2-"},
		{XtNbtn3Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[10]),XtRString, (XtPointer) "b3-"},
		{XtNbtn4Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[11]),XtRString, (XtPointer) "b4-"},
		{XtNbtn5Display,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[12]),XtRString, (XtPointer) "b5-"},
		{XtNmetaDisplay,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[13]),XtRString, (XtPointer) "m-"},
		{XtNaltDisplay,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[14]),XtRString, (XtPointer) "a-"},
		{XtNhyperDisplay,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[15]),XtRString, (XtPointer) "h-"},
		{XtNsuperDisplay,XtCAcceleratorDisplay,XtRString,
		sizeof(String), offset(mod_disp_strs[16]),XtRString, (XtPointer) "su-"},
						/* !!! adjust the size in SAccmeBSBP.h if modifiers are added !!! */
		{XtNdisplayFormat,XtCDisplayFormat,XtRString,
		sizeof(String), offset(display_format_str),XtRString, (XtPointer) FORMAT_DEFAULT},
		{XtNaccDisplayJustify, XtCAccDisplayJustify, XtRJustify,
        sizeof(XtJustify), offset(acc_disp_justify),
        XtRImmediate, (XtPointer)XtJustifyLeft},
};   
#undef offset

/*
 * Semi Public function definitions. 
 */

#if 0
static XtGeometryResult QueryGeometry();
static void Resize();
#endif
static void Redisplay(), Destroy(), Initialize();
static void ClassInitialize();
static void ClassPartInitialize();
static Boolean SetValues();
static void DisplayAccelerator();

/* 
 * Private Function Definitions.
 */

static Dimension calcStringWidth();
static void		 reformatDisplayString();
    
SAccmeBSBClassRec sAccmeBSBClassRec = {
  {
    /* superclass         */    (WidgetClass) &smeBSBClassRec,
    /* class_name         */    "SAccmeBSB",
    /* size               */    sizeof(SAccmeBSBRec),
    /* class_initializer  */	ClassInitialize,
    /* class_part_initialize*/	ClassPartInitialize,
    /* Class init'ed      */	FALSE,
    /* initialize         */    Initialize,
    /* initialize_hook    */	NULL,
    /* realize            */    NULL,
    /* actions            */    NULL,
    /* num_actions        */    0,
    /* resources          */    resources,
    /* resource_count     */	XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    FALSE, 
    /* compress_exposure  */    FALSE,
    /* compress_enterleave*/ 	FALSE,
    /* visible_interest   */    FALSE,
    /* destroy            */    Destroy,
    /* resize             */    XtInheritResize,
    /* expose             */    Redisplay,
    /* set_values         */    SetValues,
    /* set_values_hook    */	NULL,
    /* set_values_almost  */	XtInheritSetValuesAlmost,  
    /* get_values_hook    */	NULL,			
    /* accept_focus       */    NULL,
    /* intrinsics version */	XtVersion,
    /* callback offsets   */    NULL,
    /* tm_table		  */    	NULL,
    /* query_geometry	  */    XtInheritQueryGeometry,
    /* display_accelerator*/    NULL,
    /* extension	  */    	NULL
  },{
    /* SimpleMenuClass Fields */
    /* highlight          */	XtInheritHighlight,
    /* unhighlight        */	XtInheritUnhighlight,
    /* notify             */	XtInheritNotify,
    /* extension	  */	NULL
#ifdef XAW3D
  }, {
    /* ThreeDClass Fields */
    /* shadowdraw         */    XtInheritXawSme3dShadowDraw
#endif
  }, {
    /* BSBClass Fields */  
    /* extension	  */    NULL
  }, {
    /* BSBClass Fields */  
    /* display_accelerator */  DisplayAccelerator,
  }
};

WidgetClass sAccmeBSBObjectClass = (WidgetClass) &sAccmeBSBClassRec;

/************************************************************
 *
 * Semi-Public Functions.
 *
 ************************************************************/

/*	Function Name: ClassInitialize
 *	Description: Initializes the SAccmeBSBObject. 
 *	Arguments: none.
 *	Returns: none.
 */

static void 
ClassInitialize()
{
    XawInitializeWidgetSet();
}

static void
ClassPartInitialize(wc)
WidgetClass wc;
{
	SAccmeBSBObjectClass sac  = (SAccmeBSBObjectClass) wc;
	SAccmeBSBObjectClass ssac = (SAccmeBSBObjectClass) sac->rect_class.superclass;


	if (XtInheritXaw3dDisplayAccelerator==sac->sAccme_bsb_class.display_accelerator)
		sac->sAccme_bsb_class.display_accelerator=
			ssac->sAccme_bsb_class.display_accelerator;
}

static void
reformatDisplayString(sac)
SAccmeBSBObject sac;
{
String	disp=0;
int		modlen=0;
String	format=sac->sAccme_bsb.display_format_str;
String  modStr=0;
char	*idpos,*modpos;
String  fmtCopy=0;

	fmtCopy=XtNewString(format);

	idpos=strstr(fmtCopy,"%s");

	if ((modpos=strstr(fmtCopy,"%m"))) {
		/* replace '%m' by '%s' */
		*(modpos+1)='s';

		if (saccNone==sac->sAccme_bsb.modifiers) {
			modlen=strlen(sac->sAccme_bsb.none_disp_str);
		} else {
			int i;
			SaccModifier mod;
			for (i=0, mod=saccShift;
				i<XtNumber(sac->sAccme_bsb.mod_disp_strs);
				i++, mod=(mod<<1)) {
				if (sac->sAccme_bsb.modifiers & mod) {
					String str;
					modlen+=strlen(str=sac->sAccme_bsb.mod_disp_strs[i]);
					if (!modStr) {
						modStr=XtMalloc(modlen+1);
						modStr[0]=0;
					} else 
						modStr=XtRealloc(modStr,modlen+1);
					strcat(modStr,str);
				}
			}
		}
	}
	/* allocate space for the display string */
	disp = XtMalloc(strlen(fmtCopy)
					+ modlen
					+ (idpos ? strlen(sac->sAccme_bsb.keysym_string) : 0)
					+1);

	if (modpos && idpos) {
		if (modpos<idpos)
			sprintf(disp,fmtCopy,modStr?modStr:"",sac->sAccme_bsb.keysym_string);
		else
			sprintf(disp,fmtCopy,sac->sAccme_bsb.keysym_string,modStr?modStr:"");
	} else {
	  if (idpos)
		sprintf(disp,fmtCopy,sac->sAccme_bsb.keysym_string);
	  else if (modpos)
		sprintf(disp,fmtCopy,modStr?modStr:"");
	  else
		strcpy(disp,fmtCopy);
	}

	/* calculate the width of `disp' */
	sac->sAccme_bsb.disp_str_width=calcStringWidth(sac,disp);

	/* store the new display string */
	if (sac->sAccme_bsb.display_string)
		XtFree(sac->sAccme_bsb.display_string);
	sac->sAccme_bsb.display_string=disp;

	if (fmtCopy) XtFree(fmtCopy);
	if (modStr)  XtFree(modStr);
}

static Dimension
calcStringWidth(sac, str)
SAccmeBSBObject	sac;
String			str;
{
Dimension width;

#if XAW_RELEASE_GUESS > 5
	if ( sac->sme.international == True ) {
		width = XmbTextEscapement(sac->sme_bsb.fontset, str, strlen(str));
	}
	else
#endif
	{
 		width = XTextWidth(sac->sme_bsb.font, str, strlen(str));
	}

	return width;
}

/* dont' change the order! id must be the
 * same as
 *  - the SaccModifiers are defined
 *  - the display strings are stored in the resources
 */
static char *modPatterns[]={
	"Shift",
	"Ctrl",
	"Lock",
	"Mod1",
	"Mod2",
	"Mod3",
	"Mod4",
	"Mod5",
	"Button1",
	"Button2",
	"Button3",
	"Button4",
	"Button5",
	"Meta",
	"Alt",
	"Hyper",
	"Super",
};

static void
DisplayAccelerator(saw,str)
SimpleAccMenuWidget saw;
String				str;
{
SAccmeBSBObject sac=saw->simple_acc_menu.display_child;
String			keysymStr;
char			*chpt,*rest,*evnt;
SaccModifier	modifiers=saccAny;
SaccModifier	excluded=saccAny;
SaccModifier	mask;
int				i;
Boolean			warn=False;

	/* don't do this twice */
	if (sac->sAccme_bsb.keysym_string) return;

	if (! ((evnt=strstr(str,"<KeyPress>")) || (evnt=strstr(str,"<KeyRelease>"))) ) {
		XtWarning("SAccmeBSBObject: DisplayAccelerator() -- sorry, only `Key' events supported");
		return;
	}

	rest=chpt=strchr(evnt,'>')+1;

	/* search for the modifiers
	 *
	 * Note that for the moment, we
	 * do not display explicitely excluded
	 * Modifiers ('~'Modifier).
	 * I.e. a `~Shift' is not displayed.
	 */
	for (i=0, mask=1; i<XtNumber(modPatterns); i++, mask=(mask<<1)) {
		if ((chpt=strstr(str,modPatterns[i])) && chpt<evnt) {
			if (chpt==str || '~'!=chpt[-1])
				modifiers |= mask;
			else
				excluded |= mask;
		}
	}

	/* assume that Xt does pass a `!' only if `None' was
	 * specified.
	 * E.g. `!Shift' is transformed by Xt into
	 * 		`Shift~Ctrl~Lock~Alt...'
	 */
	if ((chpt=strchr(str,'!')) && chpt<evnt ) {
			modifiers = saccNone;
			excluded  = (unsigned int) -1;
	}

	{ int restlen; char *ptr;

		evnt=rest+strlen(rest);
		/* look for next event with optional modifiers */
		if ((ptr=strchr(rest,'<')) && ptr<evnt) evnt=ptr;

		/* check if the rest is delimited by any modifiers */
		if ((ptr=strchr(rest,'!')) && ptr<evnt) evnt=ptr;
		if ((ptr=strchr(rest,'~')) && ptr<evnt) evnt=ptr;
		for (i=0; i<XtNumber(modPatterns); i++)
			if ((ptr=strstr(rest,modPatterns[i])) && ptr<evnt)
				evnt=ptr;
	
		restlen = evnt-rest;
		keysymStr=XtMalloc(restlen+1);
		strncpy(keysymStr,rest,restlen);
		keysymStr[restlen]=0;
	}

	/* Now do some efforts to convert the keysym name to a more
	 * decent string, if possible.
	 */
	if ((char)0 == *keysymStr) 
		/* do nothing */;
	else if ( (char)0 == keysymStr[1] && isalpha(keysymStr[0]) ) {
		/* if it's a letter, we try to convert it to uppercase
		 * without bothering to invoke XLookupString().
		 */
		switch ( modifiers & (ShiftMask||LockMask) ) {
			case ShiftMask:
				if (excluded & ShiftMask) warn=True;
				keysymStr[0]=(char)toupper(keysymStr[0]);
				modifiers &= ~ShiftMask;
				break;
			case LockMask:
				if (excluded & LockMask) warn=True;
				keysymStr[0]=(char)toupper(keysymStr[0]);
				modifiers &= ~LockMask;
				break;
			default: /* do nothing */
				break;
		}
	} else {
		/* previous attempts didn't work. We have to fake
		 * an event and call XLookupString (without that,
		 * an exclamation mark will be displayed as the
		 * string `exclam' - sigh...
		 */
		Display *di=XtDisplay((Widget)saw);
		KeyCode min,kc;
		int		ksPerKc;
		KeySym	sym=XStringToKeysym(keysymStr);
		KeySym	*symList=XtGetKeysymTable(di,&min,&ksPerKc);
		if (kc=XKeysymToKeycode(di,sym)) {
			int m=0,row=ksPerKc * (kc-min);
			while (m<ksPerKc) {
				if (sym == symList[row+m])
					break;
				m++;
			}
			if (m<ksPerKc) {
				/* found the keysym */
				char		buf[20];
				int			got;
				XKeyEvent	ke={0};
				unsigned int mod=0;
				KeySym		dummy;
				if (m%2) {
					mod |= ShiftMask;
					/* warn them if shift
					 * was excluded since it seems that
					 * it is needed to generate the
					 * keysym.
					 */
					if (excluded & ShiftMask)
						warn=True;
				}
#ifdef XKB
				/* set modifiers to the correct
				 * group
				 */
				mod |= XkbBuildCoreState(mod,m/2);
				/* TODO warn them if group shift modifier was excluded */
#endif
				/* build a fake key event
				 * (unfortunately, there seems to be no other way
				 * to convert a keysym to ascii.
				 */
				ke.type=KeyPress;
				ke.display=di;
				ke.keycode=kc;
				ke.state=mod;
				/* convert to ascii but use only if it's a printable symbol */
				if (0 < (got=XLookupString(&ke,buf,XtNumber(buf),&dummy,NULL))) {
					int n;
					/* look for any nonprintable char */
					for (n=got-1; n>=0; n--)
						if (!isprint(buf[n])) break;

					if (n<0) {
						/* uff -- found something printable */
						if (got > strlen(keysymStr)) {
							keysymStr=XtRealloc(keysymStr,got+1);
						}
						strncpy(keysymStr,buf,got);
						keysymStr[got]=0;

						/* don't display the modifier; i.e. `*' we
						 * display as `*' and not as S-8 nor S-`*'
						 */
						if (mod & ShiftMask) {
							/* reset lock and shift in the display */
							modifiers &= ~(ShiftMask);
						}
#ifdef XKB
						/* TODO: reset the group switch modifiers */
#endif

					}
				}
			}
		}
	}
	/* possibly warn them */
	if (warn) {
		String buf=XtMalloc(512);

		sprintf(buf,"%s %s `%s' %s",
			"SAccmeBSBObject: DisplayAccelerator()",
			"-- a modifier needed to generate keysym",
			keysymStr,
			"was excluded in the accelerator resource");
		XtWarning(buf);
		XtFree(buf);
	}

	sac->sAccme_bsb.keysym_string=keysymStr;
	sac->sAccme_bsb.modifiers=modifiers;

	reformatDisplayString(sac);
	if (sac->sAccme_bsb.acc_disp_space < sac->sAccme_bsb.disp_str_width) {
		sac->sAccme_bsb.acc_disp_space = sac->sAccme_bsb.disp_str_width;
	}
	if (sac->sAccme_bsb.do_display) {
		XtWidgetGeometry req;
		Dimension sp =  sac->sAccme_bsb.acc_disp_space
					  + sac->sAccme_bsb.acc_disp_sep;
		req.request_mode=CWWidth;
		req.width = sac->rectangle.width + sp;
		if (XtGeometryYes==XtMakeGeometryRequest((Widget)sac,&req,NULL)) {
			/* granted; expand the margin */
			sac->sme_bsb.right_margin += sp;
		}
	}
}

/*      Function Name: Initialize
 *      Description: Initializes the simple menu widget
 *      Arguments: request - the widget requested by the argument list.
 *                 new     - the new widget with both resource and non
 *                           resource values.
 *      Returns: none.
 */

#ifndef __EMX__
#define ScanWhitespace(str) \
    while (*(str) == ' ' || *(str) == '\t') (str)++
#else
#define ScanWhitespace(str) \
    while (*(str) == ' ' || *(str) == '\t' || *(str) == '\r') (str)++
#endif


/* ARGSUSED */
static void
Initialize(request, new, args, num_args)
Widget request, new;
ArgList args;
Cardinal *num_args;
{
	String raw;
    SAccmeBSBObject entry = (SAccmeBSBObject) new;
	entry->sAccme_bsb.comp_accl=0;
	entry->sAccme_bsb.display_string=0;
	entry->sAccme_bsb.disp_str_width=0;
	entry->sAccme_bsb.modifiers=saccAny;
	entry->sAccme_bsb.keysym_string=0;
	if (entry->sAccme_bsb.do_display) {
		/* add disp_space and disp_sep to the right margin */
		entry->sme_bsb.right_margin+=entry->sAccme_bsb.acc_disp_space;
		entry->sme_bsb.right_margin+=entry->sAccme_bsb.acc_disp_sep;
	}
	if (raw=entry->sAccme_bsb.accelerators) {
		char	id[128];
		char	*chpt,*target=0;
		String	buf=0;
		int		total=1; /* reserve space for terminating \0 */
		int		idlen;

		/* fill in the widget id */
		sprintf(id,"%lx",(unsigned long)new);
		idlen = strlen(id);

		do {
			/* copy everything up to the first opening '(' to the buffer */
			if (!(chpt = strchr(raw,'('))) {
				XtWarning("SAccmeBSBObject: Initialize() - no action found");
				if (buf) XtFree(buf);
				return;
			} else {
				int		len = chpt - raw + 1;

				{
					char *obuf = buf;

					/* reserve buffer space */
					total += len + idlen + 2; /* space for ")\n" */
					buf=XtRealloc(buf,total); 

					/* adjust target pointer - buf may have moved in memory */
					target += buf - obuf;
				}

				/* copy everything up to '(' to the buffer */
				strncpy(target,raw,len);
				target+=len;

				/* append widget id and ") " */
				sprintf(target,"%s) ",id);
				target += idlen+2;

				/* warn them if there are arguments */
				chpt++;
				ScanWhitespace(chpt);
				if (')'!=*chpt) {
					XtWarning("SAccmeBSBObject: Initialize() - no action args allowed");
					if (buf) XtFree(buf);
					return;
				} else {
					do {
						chpt++;
						/* replace ") " by ")\n" if a \n was found */
						if ('\n'==*chpt) *(target-1)=*chpt;
					} while (*chpt==' ' || *chpt=='\t' || *chpt=='\n'
#ifndef __EMX__
							|| *chpt=='\r'
#endif
							);
				} 
				raw = chpt;
			}
		} while (*chpt);

		entry->sAccme_bsb.comp_accl=XtParseAcceleratorTable(buf);
		if (buf) XtFree(buf);
	}
}

/*      Function Name: Destroy
 *      Description: Called at destroy time, cleans up.
 *      Arguments: w - the simple menu widget.
 *      Returns: none.
 */

static void
Destroy(w)
Widget w;
{
    SAccmeBSBObject entry = (SAccmeBSBObject) w;
	if(entry->sAccme_bsb.display_string)
		XtFree(entry->sAccme_bsb.display_string);
	if (entry->sAccme_bsb.keysym_string)
		XtFree (entry->sAccme_bsb.keysym_string);
}

#if 0
static void
Resize(w)
Widget w;
{
Dimension	sep,space,true_rm;
int			newsp;
SAccmeBSBObject sac=(SAccmeBSBObject)w;

	/* we were resized; adjust the display space */

	/* calculate the true right margin */
	true_rm =	sac->sme_bsb.right_margin;

	if (sac->sAccme_bsb.do_display) {
		true_rm -=  sac->sAccme_bsb.acc_disp_sep
				  + sac->sAccme_bsb.acc_disp_space;
	}

	if ( (newsp = w->core.width
		- sac->sme_bsb.left_margin
		- true_rm
#ifdef XAW3D
		- sac->sme_threeD.shadow_width * 2
#endif
		- calcStringWidth(sac,sac->sme_bsb.label)
		 ) < 0) {
		/* try to reduce the left margin as well */
		int lm=sac->sme_bsb.left_margin+newsp;
		if (lm<0) lm=0;
		sac->sme_bsb.left_margin=(Dimension)lm;
		newsp=0;
	}
fprintf(stderr,"Resizing `%s', new w %u, newsp %i, rm %i\n",
		XtName((Widget)sac),w->core.width,newsp,newsp+true_rm);
	sac->sme_bsb.right_margin=(Dimension)newsp + true_rm;	
	if (XtIsRealized(w))
		Redisplay(w,NULL,NULL);
}

#define super (w->core.widget_class->core_class.superclass)
static XtGeometryResult
QueryGeometry(w,req,rep)
Widget w;
XtWidgetGeometry *req,*rep;
{
XtGeometryResult	rval;
SAccmeBSBObject		sac=(SAccmeBSBObject)w;
XtWidgetGeometry	query;
Dimension			space;

	space =	sac->sAccme_bsb.acc_disp_space + sac->sAccme_bsb.acc_disp_sep;

	/* if they do want to see the accelerator display, reduce
 	 * the intended width by the space needed by the accelerator display.
 	 */
	if (sac->sAccme_bsb.do_display && req->request_mode&CWWidth) {
		req->width -= space;
	}
	/* call superclass method */
	rval = super->core_class.query_geometry(w,req,rep);

	/* restore the intended width */
	if (sac->sAccme_bsb.do_display && req->request_mode&CWWidth) {
		req->width += space;
	}

	/* add space to the preferred width */
	if (sac->sAccme_bsb.do_display && rep->request_mode&CWWidth) {
		rep->width += space;
	}

fprintf(stderr,"QG for `%s' returns %i\n",XtName(w),rep->request_mode&CWWidth ? rep->width : -1);
	return rval;
}

#undef super

#endif

/*      Function Name: Redisplay
 *      Description: Redisplays the contents of the widget.
 *      Arguments: w - the simple menu widget.
 *                 event - the X event that caused this redisplay.
 *                 region - the region the needs to be repainted. 
 *      Returns: none.
 */

/* ARGSUSED */
static void
Redisplay(w, event, region)
Widget w;
XEvent * event;
Region region;
{
SAccmeBSBObject sac=(SAccmeBSBObject)w;
Pixmap			right;
GC				gc;
String			str=sac->sAccme_bsb.display_string;

	/* get the correct GC */
	if (XtIsSensitive(w) && XtIsSensitive( XtParent(w) ) ) {
		if ( w == XawSimpleMenuGetActiveEntry(XtParent(w)) ) {
			gc = sac->sme_bsb.rev_gc;
		} else
			gc = sac->sme_bsb.norm_gc;
	} else
		gc = sac->sme_bsb.norm_gray_gc;


	/* don't let them draw the right pixmap */
	right=sac->sme_bsb.right_bitmap;
	sac->sme_bsb.right_bitmap=None;

	/* draw the parent stuff */
	w->core.widget_class->core_class.superclass->core_class.expose(w,event,region);

	/* restore the bitmap and draw it here */
	if (None!=(sac->sme_bsb.right_bitmap=right)) {
		int x_loc,y_loc;

		/* draw the right bitmap */
		x_loc = sac->rectangle.width
#ifdef XAW3D
			- sac->sme_threeD.shadow_width
#endif
			- (int)(sac->sme_bsb.right_margin 
					- sac->sAccme_bsb.acc_disp_space
					- sac->sAccme_bsb.acc_disp_sep
					+ sac->sme_bsb.right_bitmap_width) / 2;

		y_loc = sac->rectangle.y +
			(int)(sac->rectangle.height -
				  sac->sme_bsb.right_bitmap_height) / 2;

		XCopyPlane(	XtDisplayOfObject(w),
				sac->sme_bsb.right_bitmap,
				XtWindowOfObject(w), gc, 0, 0,
				sac->sme_bsb.right_bitmap_width,
				sac->sme_bsb.right_bitmap_height,
				x_loc, y_loc, 1);
	}

	if (str && sac->sAccme_bsb.do_display) {
		int y_loc=sac->rectangle.y;
#if XAW_RELEASE_GUESS > 5
		int fontset_ascent=0,fontset_descent=0;
#endif
		int font_ascent=0,font_descent=0;
		int x_loc=sac->rectangle.width - sac->sme_bsb.right_margin
				  + sac->sAccme_bsb.acc_disp_sep
#ifdef XAW3D
				  + sac->sme_threeD.shadow_width
#endif
				  ;
		int	len=strlen(str);

		/* if the menu string is left adjusted of centered, center
		 * the display strings.
		 */
		switch (sac->sAccme_bsb.acc_disp_justify) {
			case XtJustifyCenter:
				x_loc += ((int)(	sac->sAccme_bsb.acc_disp_space
						  - sac->sAccme_bsb.disp_str_width))/2;
				break;

			case XtJustifyRight:
				x_loc += (	sac->sAccme_bsb.acc_disp_space
						  - sac->sAccme_bsb.disp_str_width);
				break;

			default: /* left justified */
				break;
		}

#if XAW_RELEASE_GUESS > 5
		if ( sac->sme.international == True ) {
			XFontSetExtents *ext = XExtentsOfFontSet(sac->sme_bsb.fontset);
			fontset_ascent = abs(ext->max_ink_extent.y);
			fontset_descent = ext->max_ink_extent.height - fontset_ascent;
		}
		else
#endif
		{   /*else, compute size from font like R5*/
			font_ascent = sac->sme_bsb.font->max_bounds.ascent;
			font_descent = sac->sme_bsb.font->max_bounds.descent;
		}


#if XAW_RELEASE_GUESS > 5
		if ( sac->sme.international==True ) {
			y_loc += ((int)sac->rectangle.height -
					(fontset_ascent + fontset_descent)) / 2 + fontset_ascent;

			XmbDrawString(XtDisplayOfObject(w), XtWindowOfObject(w),
					sac->sme_bsb.fontset, gc, x_loc , y_loc, str,len);
		}
		else
#endif
		{
			y_loc += ((int)sac->rectangle.height -
					(font_ascent + font_descent)) / 2 + font_ascent;

			XDrawString(XtDisplayOfObject(w), XtWindowOfObject(w), gc,
					x_loc , y_loc, str, len);
		}

	}
}


/*      Function Name: SetValues
 *      Description: Relayout the menu when one of the resources is changed.
 *      Arguments: current - current state of the widget.
 *                 request - what was requested.
 *                 new - what the widget will become.
 *      Returns: none
 */

#define SACCME ((entry)->sAccme_bsb)
#define SACCMO ((old_entry)->sAccme_bsb)

/* ARGSUSED */
static Boolean
SetValues(current, request, new, args, num_args)
Widget current, request, new;
ArgList args;
Cardinal *num_args;
{
Boolean rval=False;
SAccmeBSBObject entry = (SAccmeBSBObject) new;
SAccmeBSBObject old_entry = (SAccmeBSBObject) current;
Dimension	deltaWidth=0;
int	i;

	if ( SACCME.display_string &&
#if XAW_RELEASE_GUESS > 5
		 (	entry->sme.international != old_entry->sme.international||
			((old_entry->sme_bsb.font != entry->sme_bsb.font) &&
    		(entry->sme.international == False )              )		||
 			((old_entry->sme_bsb.fontset != entry->sme_bsb.fontset) &&
            (entry->sme.international == True )) 
		 )
#else
		 (old_entry->sme_bsb.font != entry->sme_bsb.font)
#endif
		) {
		SACCME.disp_str_width=
			calcStringWidth(entry,SACCME.display_string);
		rval=True;
	}

	/* Now check for anything affecting the right margin */

	if (SACCME.do_display != SACCMO.do_display) {
		if (SACCME.do_display) {
			/* they just switched the display on */
			
		} else {
		}
		rval=True;
	}

	/* subtract the old display space if 
	 * - do_display stays on && right_margin unchanged && (sep or space changed)
	 * - do_display changed from on to off && right_margin unchanged.
	 *
	 */
	if ( (entry->sme_bsb.right_margin == old_entry->sme_bsb.right_margin)
		&& SACCMO.do_display
		&&	(	SACCME.acc_disp_space != SACCMO.acc_disp_space
		 	 || SACCME.acc_disp_sep != SACCMO.acc_disp_sep
			 || ! SACCME.do_display ) ) {
			/* remove old space from the margin */
			Dimension sp = SACCMO.acc_disp_space + SACCMO.acc_disp_sep;
			entry->sme_bsb.right_margin -= sp;
			deltaWidth -= sp;
			rval=True;
	}

	/* add the display space if doDisplay and
	 * - do Display was off
	 * - right margin was changed.
	 * - sep/space was changed.
	 */
	if (SACCME.do_display &&
		( ! SACCMO.do_display
		 || entry->sme_bsb.right_margin != old_entry->sme_bsb.right_margin
		 || SACCME.acc_disp_space != SACCMO.acc_disp_space
		 || SACCME.acc_disp_sep != SACCMO.acc_disp_sep ) ) {
		Dimension sp;

		/* restrict disp_space */
		if (SACCME.acc_disp_space < SACCME.disp_str_width)
				SACCME.acc_disp_space = SACCME.disp_str_width;
		sp = SACCME.acc_disp_space + SACCME.acc_disp_sep;
		entry->sme_bsb.right_margin += sp;
		deltaWidth += sp;
		rval = True;
	}

	if (SACCME.accelerators != SACCMO.accelerators) {
		XtWarning("SAccmeBSBObject: setValues() - changing accelerators rejected");
		SACCME.accelerators = SACCMO.accelerators;
	}

	for (i=(XtNumber(SACCME.mod_disp_strs)-1); i>=0; i--) {
		if (strcmp(SACCME.mod_disp_strs[i],SACCMO.mod_disp_strs[i]))
			break;
	}

	if ( i>=0 || 
		 strcmp(SACCME.display_format_str,SACCMO.display_format_str) ||
		 strcmp(SACCME.none_disp_str,SACCMO.none_disp_str)) {
		reformatDisplayString(entry);
		rval=True;
	}
	new->core.width += deltaWidth;
    return rval;
}

#undef SACCME
#undef SACCMO
