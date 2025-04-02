#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include "widget/Canvas.h"
#ifdef XAW3D
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/Command.h>
#ifdef USE_VIEWPORT
#include <X11/Xaw3d/Viewport.h>
#else
#include <X11/Xaw3d/Porthole.h>
#include "pan.h"
#include "pmstuff.h"
#endif
#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/SmeBSB.h>
#include <X11/Xaw3d/SmeLine.h>
#else
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#ifdef USE_VIEWPORT
#include <X11/Xaw/Viewport.h>
#else
#include <X11/Xaw/Porthole.h>
#include "pan.h"
#include "pmstuff.h"
#endif
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#endif

#ifdef XAW3D
#include "widget/ThreeDee.h"
#endif

#include "widget/FocusForm.h"
#include "widget/TextField.h"
#include "widget/SlPaned.h"

#include "msant_strings.h"
#include "olnames.h"
#include "widglist.h"
#include "haupt_ui.h"

#include "common/graf.h"
#include "common/CBstrom.h"

static String newss[]={
	nActNew,0};
static String insps[]={
	nActInsertPoint,0};
static String movps[]={
	nActMovePoint,0};
static String delps[]={
	nActDeletePoint,0};
static String addPs[]={
	nActAddPolygon,0};
static String movPs[]={
	nActPolygon,"M",0};
static String cpyPs[]={
	nActPolygon,"C",0};
static String delPs[]={
	nActPolygon,"D",0};
static String undls[]={
	nActUndelete,0};
static String refps[]={
	nActRefPoint,0};
static String dists[]={
	nActDistance,0};

static void GetCanvasDimensions();

static Dimension bw_hd(wid)
 Widget wid;
{
Dimension	w,bw;
int			d;
XtVaGetValues(wid,XtNheight,&w,XtNborderWidth,&bw,XtNvertDistance,&d,NULL);
return w+2*bw+(Dimension)d;
}


/* create a scrolling widget (either viewport or porthole)
 * with faked shadows, if possible (ifdef XAW3D). Because
 * porthole / viewport are subclasses of composite, they
 * can not have shadows.
 * We fake the shadows creating a form with two children;
 * a threeD child and the scrollable child which is layed on
 * top of the threeD widget but shifted down and right the
 * amount of shadowWidth.
 */

static Widget Create3DScrolled(name,parent)
  char		*name;
  Widget	parent;
{
 Widget 	form,scrolled;
 int		j;
 Arg		args[16];
 Dimension	w,h,bw=0;
#ifdef XAW3D
 int		i;
 Widget		threed;
 Dimension	shdw=0;
#endif

 form=XtVaCreateManagedWidget("shadow_form",
	focusFormWidgetClass,
	parent,
	XtNborderWidth,0,
	XtNdefaultDistance,0,
	NULL);

 j=0;
#ifdef USE_VIEWPORT
 XtSetArg(args[j],XtNuseRight,True); j++;
 XtSetArg(args[j],XtNuseBottom,True); j++;
 XtSetArg(args[j],XtNallowVert,True); j++;
 XtSetArg(args[j],XtNallowHoriz,True); j++;
#endif
#ifdef XAW3D
 i=j;
 XtSetArg(args[j],XtNborderWidth,0); j++;
 XtSetArg(args[j],XtNleft,XtChainLeft); j++;
 XtSetArg(args[j],XtNright,XtChainRight); j++;
 XtSetArg(args[j],XtNtop,XtChainTop); j++;
 XtSetArg(args[j],XtNbottom,XtChainBottom); j++;
#endif
 XtSetArg(args[j],XtNwidth,1);j++; /* give a minimal height/width */
 XtSetArg(args[j],XtNheight,1);j++;
 XtSetArg(args[j],XtNfocusInterest,True);j++;

 scrolled=XtCreateManagedWidget(
	name,
#ifdef USE_VIEWPORT
	viewportWidgetClass,
#else
	portholeWidgetClass,
#endif
	form,
	args,j);

 XtVaGetValues(scrolled,XtNwidth,&w,XtNheight,&h,XtNborderWidth,&bw,NULL);
 w+=2*bw; h+=2*bw;

#ifdef XAW3D
 XtSetArg(args[j],XtNemboss,False); j++;
 threed=XtCreateManagedWidget("graf_shadow",
	threeDeeWidgetClass,
	form,args+i,j-i);

 XtVaGetValues(threed,XtNshadowWidth,&shdw,NULL);
 w+=2*shdw; h+=2*shdw;
 XtVaSetValues(threed,XtNwidth,w,XtNheight,h,NULL);
 XtVaSetValues(scrolled,XtNhorizDistance,shdw,XtNvertDistance,shdw,NULL);
#endif

 return scrolled;

}

/* some layout dimensions */

#define LEFT_SPACE 30
#define TF_SPACE    4
#define KOORD_SEP  30
#define INP_SEP    30
#define TOP_SEP	   15
#define MSSG_SEP   15
#define CNVS_SEP   15


 /* Labels */
#define LAB_FREQ 0
#define LAB_EPSI 1
#define LAB_HEIG 2
#define LAB_R 	 3
#define LAB_DR	 4

static WidTabEl lablist[]={
	{"*Frequency",0,0},
	{"*Permittivity",0,0},
	{"*Substrate Height",0,0},
	{"*X , Y",0,0},
	{"*dX , dY",0,0},
 };

/* Boxes */
#define FFW_R	0
#define FFW_DR	1
#define FFW_INP	2

static WidTabEl ffwlist[]={
	{"*ffw_r",0,0},
	{"*ffw_dr",0,0},
	{"*ffw_inp",0,0},
	{0,0,0},
};

/* Viewports */
#define VP_GRAFIK 0
#define VP_STROM  1

static WidTabEl vplist[]={
	{"*vp_grafik",0,0},
	{"*vp_strom",0,0},
};

Widget CreateHaupt(top)
 Widget top;
{
 /* The Form */
 Widget form;

 Widget wid,wid1,wid2,draw_menu;
#ifndef USE_VIEWPORT
 Widget spanner,gpanner;
#endif

 Widget vpaned,hpaned;

 Arg	args[16];
 int	nr,i,j,k,dist; /* use j only for setting args[] */
 Dimension w,h,maxw,overallw,overallh,maxtw,bw,maxh,maxh1[2];
 Dimension formw,gpanh;
 Pixel  bg;

 /* need a main focus form widget to manage focus traversal */
 wid=XtVaCreateManagedWidget("Mainffw",focusFormWidgetClass,top,
	XtNborderWidth,0,
	NULL);

 vpaned=XtVaCreateManagedWidget("Vpaned",slPanedWidgetClass,wid,
	XtNorientation,XtorientVertical,
	XtNborderWidth,0,
	NULL);

 hpaned=XtVaCreateManagedWidget("Hpaned",slPanedWidgetClass,vpaned,
	XtNorientation,XtorientHorizontal,
	XtNborderWidth,0,
	NULL);
 maxw=0;

 form=XtVaCreateManagedWidget("Haupt",focusFormWidgetClass,hpaned,
	XtNborderWidth,0,
	NULL);

 XtVaGetValues(form,XtNdefaultDistance,&dist,XtNbackground,&bg,NULL);

 XtVaSetValues(hpaned,XtNbackground,bg,XtNinternalBorderColor,bg,NULL);
 XtVaSetValues(vpaned,XtNbackground,bg,XtNinternalBorderColor,bg,NULL);

 /* Create the command widgets */
 overallh=TOP_SEP-dist;

 wid=0;
 for (nr=BUTT_GRAF;nr<=BUTT_QUIT;nr++) {
   buttonlist[nr].t_wid=XtVaCreateManagedWidget(
	buttonlist[nr].wid_name+1,
	commandWidgetClass,
	form,
	XtNfromVert,wid,
	XtNhorizDistance,LEFT_SPACE,
	NULL);
   wid=buttonlist[nr].t_wid;
   XtVaGetValues(wid,XtNwidth,&w,NULL);
   overallh+=bw_hd(wid);
   if (w>maxw) maxw=w;
 }

 gpanh=overallh-TOP_SEP;


 /* Now the Labels */
 for (nr=LAB_FREQ;nr<=LAB_DR;nr++) {
   lablist[nr].t_wid=XtVaCreateManagedWidget(
	lablist[nr].wid_name+1,
	labelWidgetClass,
	form,
	XtNfromVert,wid,
	XtNjustify,XtJustifyRight,
	XtNhorizDistance,LEFT_SPACE,
	XtNleft,XtChainLeft,
	XtNright,XtChainLeft,
	NULL);
   wid=lablist[nr].t_wid;
   XtVaGetValues(wid,XtNwidth,&w,NULL);
   if (w>maxw) maxw=w;
 }

 /* make everything the same width */
 j=0;
 XtSetArg(args[j],XtNwidth,maxw);j++;
 XtSetArg(args[j],XtNleft,XtChainLeft); j++;
 XtSetArg(args[j],XtNright,XtChainLeft); j++;
 XtSetArg(args[j],XtNtop,XtChainTop); j++;
 XtSetArg(args[j],XtNbottom,XtChainTop); j++;
#ifdef XAW3D
 XtSetArg(args[j],XtNborderWidth,0); j++;
#endif
 for (nr=BUTT_GRAF;nr<=BUTT_QUIT;nr++) 
  XtSetValues(buttonlist[nr].t_wid,args,j);

 wid=buttonlist[BUTT_QUIT].t_wid;
 wid1=ffwlist[FFW_INP].t_wid=XtVaCreateManagedWidget(
	ffwlist[FFW_INP].wid_name+1,
	focusFormWidgetClass,
	form,
	XtNleft,XtChainLeft,
	XtNright,XtChainRight,
	XtNtop,XtChainTop,
	XtNbottom,XtChainTop,
	XtNborderWidth,0,
	XtNhorizDistance,0,
	XtNvertDistance,0,
	XtNfromVert,wid,
	XtNfromHoriz,wid,
	XtNdefaultDistance,0,
	NULL);

 wid=0;
 /* create input fields */
 maxtw=0;
 for (nr=TXTFLD_FREQ;nr<=TXTFLD_HSUBST;nr++) {
  input_haupt[nr].wid.t_wid=XtVaCreateManagedWidget(
	input_haupt[nr].wid.wid_name+1,
	textFieldWidgetClass,
	wid1,
	XtNfromVert,wid,
	XtNvertDistance,dist,
	XtNleft,XtChainLeft,
	XtNright,XtChainRight,
	XtNtop,XtChainTop,
	XtNbottom,XtChainTop,
	XtNhorizDistance,TF_SPACE,
#ifdef XAW3D
	XtNborderWidth,0,
#endif
	NULL);
  wid=input_haupt[nr].wid.t_wid;
  XtVaGetValues(wid,
	XtNwidth,&w,
	XtNborderWidth,&bw,
	NULL);
  w+=TF_SPACE+2*bw;
  if (w>maxtw) maxtw=w;
 }

 overallh+=INP_SEP;

 /* adapt height and width of labels 
    and textfields */
 i=TXTFLD_FREQ;
 XtSetArg(args[j],XtNheight,0);j++;
 for (nr=LAB_FREQ;nr<=LAB_HEIG;nr++)  {
  XtVaGetValues(input_haupt[i].wid.t_wid,
	XtNborderWidth,&bw,
	XtNheight,&h,
	NULL);
  XtVaGetValues(lablist[nr].t_wid,
	XtNheight,&maxh,
	NULL);
  h+=2*bw;
  maxh=((maxh>h)?maxh:h);
  args[j-1].value=(XtArgVal)maxh;
  XtSetValues(lablist[nr].t_wid,args,j);
  w=maxtw-TF_SPACE-2*bw;
  XtVaSetValues(input_haupt[i].wid.t_wid,
	XtNwidth,w,
	XtNheight,maxh-2*bw,
	NULL);
  overallh+=dist+maxh;
 }


 wid=wid1;
 overallw=LEFT_SPACE+maxw+maxtw;
 /* create X, DX ,Y, DY*/
 i=LAB_R;
 k=FFW_R;
 maxw=0;
 maxh=0;
 maxh1[0]=maxh1[1]=0;
 for (nr=TXTFLD_X;nr<=TXTFLD_DX;nr++) {

     XtVaGetValues(lablist[i].t_wid,
	     XtNwidth,&w,
	     XtNheight,&h,
	     NULL);
     if (w>maxw) maxw=w;
     if (h>maxh1[nr-TXTFLD_X]) maxh1[nr-TXTFLD_X]=h;

     XtVaSetValues(lablist[i].t_wid,XtNfromVert,wid,NULL);
     ffwlist[k].t_wid=XtVaCreateManagedWidget(
        ffwlist[k].wid_name+1,
        focusFormWidgetClass,
	form,
	XtNleft,XtChainLeft,
	XtNright,XtChainRight,
	XtNtop,XtChainTop,
	XtNbottom,XtChainTop,
	XtNfromVert,wid,
	XtNfromHoriz,lablist[i].t_wid,
	XtNhorizDistance,0,
	XtNdefaultDistance,0,
	XtNborderWidth,0,
	NULL);

  wid1=input_koords[nr].wid.t_wid=XtVaCreateManagedWidget(
	input_koords[nr].wid.wid_name+1,
	textFieldWidgetClass,
	ffwlist[k].t_wid,
	NULL);

  XtVaGetValues(wid1,XtNheight,&h,XtNborderWidth,&bw,NULL);
  h+=2*bw;
  if (h>maxh1[nr-TXTFLD_X]) maxh1[nr-TXTFLD_X]=h;

  wid2=input_koords[nr+2].wid.t_wid=XtVaCreateManagedWidget(
	input_koords[nr+2].wid.wid_name+1,
	textFieldWidgetClass,
	ffwlist[k].t_wid,
	XtNfromHoriz,wid1,
	NULL);

  XtVaGetValues(wid2,XtNheight,&h,XtNborderWidth,&bw,NULL);
  h+=2*bw;
  if (h>maxh1[nr-TXTFLD_X]) maxh1[nr-TXTFLD_X]=h;

  wid=ffwlist[k].t_wid;
  i++; k++;
 }


 /* now adjust everything */

 XtVaSetValues(ffwlist[FFW_R].t_wid,XtNvertDistance,KOORD_SEP,NULL);
 XtVaSetValues(lablist[LAB_R].t_wid,XtNvertDistance,KOORD_SEP,NULL);

 overallh+=KOORD_SEP+maxh1[0]+dist+maxh1[1];

 XtVaSetValues(ffwlist[FFW_INP].t_wid,XtNvertDistance,INP_SEP,NULL);
 XtVaSetValues(lablist[LAB_FREQ].t_wid,XtNvertDistance,INP_SEP+dist,NULL);


 XtVaSetValues(buttonlist[BUTT_GRAF].t_wid,XtNvertDistance,TOP_SEP,NULL);

 w=(overallw-maxw-2*TF_SPACE-LEFT_SPACE);
 if (w&1) {
	w--;
	maxw+=2;
 }
 w=w>>1;
 j=0;
 XtSetArg(args[j],XtNtop,XtChainTop);j++;
 XtSetArg(args[j],XtNbottom,XtChainTop);j++;
#ifdef XAW3D
 XtSetArg(args[j],XtNborderWidth,0);j++;
#endif
 i=j;
 XtSetArg(args[j],XtNheight,0);j++;
 XtSetArg(args[j],XtNwidth,w);j++;
 XtSetArg(args[j],XtNhorizDistance,TF_SPACE); j++;
 for (nr=TXTFLD_X; nr<= TXTFLD_DY; nr++) {
	XtVaGetValues(input_koords[nr].wid.t_wid,XtNborderWidth,&bw,NULL);
	args[i].value=(XtArgVal)(maxh1[(nr-TXTFLD_X) & 1]-2*bw);
	args[i+1].value=(XtArgVal)(w-2*bw);
	XtSetValues(input_koords[nr].wid.t_wid,args,j);
 }
 j--;
 args[j-1].value=(XtArgVal)maxw;
 XtSetArg(args[j],XtNjustify,XtJustifyRight); j++;
 for (nr=LAB_R; nr<= LAB_DR; nr++) {
	args[i].value=(XtArgVal)maxh1[(nr-LAB_R) & 1];
	XtSetValues(lablist[nr].t_wid,args,j);
 }

 j=0;
 XtSetArg(args[j],XtNtop,XtChainTop); j++;
 XtSetArg(args[j],XtNbottom,XtChainTop); j++;
 XtSetArg(args[j],XtNhorizDistance,LEFT_SPACE); j++;
 XtSetArg(args[j],XtNleft,XtChainLeft); j++;
 XtSetArg(args[j],XtNright,XtChainRight); j++;
#ifdef XAW3D
 XtSetArg(args[j],XtNborderWidth,0); j++;
#endif
 XtSetArg(args[j],XtNresize,False); j++;
 XtSetArg(args[j],XtNfromVert,wid); j++;
 XtSetArg(args[j],XtNreadOnly,True); j++;

 wid=outputlist[MSSG_M].t_wid=XtCreateManagedWidget(
	outputlist[MSSG_M].wid_name+1,
	textFieldWidgetClass,
	form,
	args,j);

 XtVaGetValues(wid,XtNborderWidth,&bw,NULL);
 XtVaSetValues(wid,XtNwidth,overallw-LEFT_SPACE-2*bw,NULL);

 overallh+=bw_hd(wid);

 j-=1;

 args[j-1].value=(XtArgVal)wid;

 XtSetArg(args[j],XtNjustify,XtJustifyRight); j++;
 XtSetArg(args[j],XtNresize,False); j++;

 wid2=outputlist[MSSG_VERSION].t_wid=XtCreateManagedWidget(
	outputlist[MSSG_VERSION].wid_name+1,
	labelWidgetClass,
	form,
	args,j);

 XtVaGetValues(wid,XtNborderWidth,&bw,NULL);
 XtVaSetValues(wid,XtNwidth,overallw-LEFT_SPACE-2*bw,NULL);

 overallh+=bw_hd(wid2);

 formw=overallw;

 XtVaGetValues(hpaned,XtNinternalBorderWidth,&bw,NULL);
 overallw+=bw;


 wid=vplist[VP_GRAFIK].t_wid=Create3DScrolled(
	vplist[VP_GRAFIK].wid_name+1,
	hpaned);


 GetCanvasDimensions(top,&w,&h);

 wid=canvaslist[CNVS_GRAFIKFENSTER].t_wid=XtVaCreateManagedWidget(
	canvaslist[CNVS_GRAFIKFENSTER].wid_name+1,
	canvasWidgetClass,
	wid,
	XtNwidth,w,
	XtNheight,h,
	NULL);

#ifndef USE_VIEWPORT
 XtAddCallback(wid,XtNcallback,PmNewPixmap,PM_NEW_GRAF_PIXMAP);
#endif

 wid=vplist[VP_STROM].t_wid=Create3DScrolled(
	vplist[VP_STROM].wid_name+1,
	vpaned);


 wid=canvaslist[CNVS_STROMFENSTER].t_wid=XtVaCreateManagedWidget(
	canvaslist[CNVS_STROMFENSTER].wid_name+1,
	canvasWidgetClass,
	wid,
	NULL);

#ifndef USE_VIEWPORT
 XtAddCallback(wid,XtNcallback,PmNewPixmap,PM_NEW_STROM_PIXMAP);
#endif

 w=maxw=(formw-LEFT_SPACE-dist)/2;
 h=maxh=0;

#ifndef USE_VIEWPORT
 j=0;
 XtSetArg(args[j],XtNfromHoriz,0); j++;
 XtSetArg(args[j],XtNhorizDistance,LEFT_SPACE); j++;
 XtSetArg(args[j],XtNleft,XtChainLeft); j++;
 XtSetArg(args[j],XtNright,XtChainLeft); j++;
 XtSetArg(args[j],XtNtop,XtChainTop); j++;
 XtSetArg(args[j],XtNbottom,XtChainTop); j++;
 XtSetArg(args[j],XtNfromVert,wid2); j++;
 XtSetArg(args[j],XtNvertDistance,CNVS_SEP); j++;

 spanner=CreatePmPanner("pan_strom",form,
		canvaslist[CNVS_STROMFENSTER].t_wid,&maxw,&maxh,args,j);

 args[0].value=(XtArgVal)spanner;
 args[1].value=(XtArgVal)dist;

 gpanner=CreatePmPanner("pan_graf",form,
		canvaslist[CNVS_GRAFIKFENSTER].t_wid,&w,&h,args,j);
#endif

 if (h>maxh) maxh=h;


 overallh+=CNVS_SEP+maxh+dist;

 XtVaSetValues(form,XtNmin,formw,NULL);
 XtVaSetValues(hpaned,XtNmin,overallh,NULL);
 
 XtVaGetValues(vpaned,XtNinternalBorderWidth,&bw,NULL);
 overallh+=bw;

 draw_menu=XtVaCreatePopupShell(
	"menu_drawingTools",
	simpleMenuWidgetClass,
	top,
	NULL);

     XtVaCreateManagedWidget("line",smeLineObjectClass,draw_menu,NULL);

 wid=	XtVaCreateManagedWidget("New",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)newss);
 wid=	XtVaCreateManagedWidget("Insert Points",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)insps);
 wid=	XtVaCreateManagedWidget("Move Points",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)movps);
 wid=	XtVaCreateManagedWidget("Delete Points",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)delps);

     XtVaCreateManagedWidget("line1",smeLineObjectClass,draw_menu,NULL);

 wid=	XtVaCreateManagedWidget("Insert Polygon",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)addPs);
 wid=	XtVaCreateManagedWidget("Copy Polygon",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)cpyPs);
 wid=	XtVaCreateManagedWidget("Move Polygon",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)movPs);
 wid=	XtVaCreateManagedWidget("Delete Polygon",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)delPs);

     XtVaCreateManagedWidget("line2",smeLineObjectClass,draw_menu,NULL);

 wid=	XtVaCreateManagedWidget("Undelete",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)undls);
 wid=	XtVaCreateManagedWidget("Reference Point",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)refps);
 wid=	XtVaCreateManagedWidget("Distance",smeBSBObjectClass,draw_menu,NULL);
	 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)dists);

 return vpaned;
}

void CB_To_Action(widget, cld, cad)
 Widget    widget;
 XtPointer cld,cad;
{
 String *actp=(String*)cld;
 Cardinal nargs;
 XEvent ev;

 ev.type=ClientMessage;
 nargs=0;
 while(actp[nargs+1])nargs++;

 while (widget && !XtIsSubclass(widget,coreWidgetClass)) widget=XtParent(widget);
 XtCallActionProc(widget,actp[0],&ev,actp+1,nargs);
}

/* This is a horrible thing -- 
 * for sake of compatibility with 
 * the semi-automatically generated
 * GUI using Sun's 'Devguide' tool.
 *
 * This tool forces widget dimensions
 * to be hardcoded. Therefore app_resources
 * 'canvasWidth' / 'canvasHeight' exist, which
 * are used to resize the canvas after creation.
 *
 * This routine retrieves these app_resources.
 *
 */

typedef struct dims_{
	Dimension width,height;
} dims;

static void GetCanvasDimensions(top,w,h)
 Widget top;
 Dimension *w,*h;
{
dims	d;
static XtResource r[]={
 {XtNcanvasWidth,XtCWidth,XtRDimension,sizeof(Dimension),
  XtOffsetOf(dims,width),XtRImmediate,(XtPointer)STUBW},
 {XtNcanvasHeight,XtCHeight,XtRDimension,sizeof(Dimension),
  XtOffsetOf(dims,height),XtRImmediate,(XtPointer)STUBH},
};

XtGetApplicationResources(top,&d,r,XtNumber(r),NULL,0);
*w=d.width; *h=d.height;
}
