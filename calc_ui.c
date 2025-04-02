#include <stdio.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Shell.h>

#ifdef XAW3D
#include <X11/Xaw3d/ThreeD.h>
#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/Label.h>
#else
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Label.h>
#endif

#include "widget/FocusForm.h"
#include "widget/TextField.h"

#include "widglist.h"
#include "xawutils/pinpop.h"
#include "graf.h"

#include "calc_ui.h"

#define LAB_SEG	0
#define LAB_NUM	1
#define LAB_EXC 2
#define LAB_CPL 3
#define LAB_ANA 4
#define LAB_CLO 5
#define LAB_VIS 6
#define LAB_Y12 7

#define BUTTSEP 20

static WidTabEl clablist[]={
	{"*Segmentation",0,0},
	{"*Number",0,0},
	{"*Feed",0,0},
	{"*Radiation Coupling",0,0},
	{"*Analysis",0,0},
	{"*Clone",0,0},
	{"*visible",0,0},
	{"*Mutual Admittance",0,0},
};


Widget CreateCalc(top)
 Widget top;
{
 Widget calc,wid,wid1,wid2,form;
 int    i,j,k,l;
 Arg	buttargs[15];
 Arg    chargs[10];
 Dimension w,h,maxw,maxh;
 int	dist;

 calc=CreatePinpop(top,"Analysis",&form);
 
 wid=0;
 j=0;
 XtSetArg(buttargs[j],XtNright,XtChainLeft); j++;
 XtSetArg(buttargs[j],XtNleft,XtChainLeft); j++;
#ifdef XAW3D
 XtSetArg(buttargs[j],XtNborderWidth,0); j++;
#endif
 XtSetArg(buttargs[j],XtNjustify,XtJustifyLeft); j++;
 XtSetArg(buttargs[j],XtNtop,XtChainTop); j++;
 XtSetArg(buttargs[j],XtNbottom,XtChainTop); j++;
 XtSetArg(buttargs[j],XtNfromVert,wid);j++;
 XtSetArg(buttargs[j],XtNhorizDistance,BUTTSEP);j++;

 l=0;
 XtSetArg(chargs[l],XtNfromVert,wid); l++;
 XtSetArg(chargs[l],XtNhorizDistance,BUTTSEP); l++;
 XtSetArg(chargs[l],XtNleft,XtChainLeft); l++;
 XtSetArg(chargs[l],XtNright,XtChainLeft); l++;
 XtSetArg(chargs[l],XtNtop,XtChainTop); l++;
 XtSetArg(chargs[l],XtNbottom,XtChainTop); l++;

 k=LAB_SEG;
 wid2=wid=clablist[k].t_wid=XtCreateManagedWidget(
	clablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j-1);

 buttargs[j-2].value=(XtArgVal)wid;

 k=LAB_NUM;
 wid=clablist[k].t_wid=XtCreateManagedWidget(
	clablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j);

 wid1=wid;
 k=TXTFLD_NSEGS;
 j--;
 XtSetArg(buttargs[j],XtNfromHoriz,wid);j++;
 wid=input_haupt[k].wid.t_wid=XtCreateManagedWidget(
	input_haupt[k].wid.wid_name+1,
	textFieldWidgetClass,
	form,
	buttargs,j);
 j--;
 XtSetArg(buttargs[j],XtNhorizDistance,BUTTSEP);j++;

 XtVaGetValues(wid,XtNheight,&h,NULL);
 XtVaGetValues(wid1,XtNheight,&maxh,NULL);
 if (maxh>h) wid1=wid; else maxh=h;
 XtVaSetValues(wid1,XtNheight,maxh,NULL);

 buttargs[j-2].value=(XtArgVal)wid;

 maxw=0;
 for (i=BUTT_SEG; i<=BUTT_SEGALL; i++) {
   wid=buttonlist[i].t_wid=XtCreateManagedWidget(
	buttonlist[i].wid_name+1,
	commandWidgetClass,
	form,
	buttargs,j);
   XtVaGetValues(wid,XtNwidth,&w,NULL);
   if (w>maxw) maxw=w;
   buttargs[j-2].value=(XtArgVal)wid;
 }

 
 buttargs[j-2].value=(XtArgVal)wid;
 k=LAB_EXC;
 wid=clablist[k].t_wid=XtCreateManagedWidget(
	clablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j-1);

 buttargs[j-2].value=(XtArgVal)wid;
 k=BUTT_SPEIS;
 wid=buttonlist[k].t_wid=XtCreateManagedWidget(
	buttonlist[k].wid_name+1,
	commandWidgetClass,
	form,
	buttargs,j);

 XtVaGetValues(wid,XtNwidth,&w,NULL);
 if (w>maxw) maxw=w;

 buttargs[j-2].value=(XtArgVal)wid;
 k=LAB_ANA;
 wid=clablist[k].t_wid=XtCreateManagedWidget(
	clablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j-1);

 buttargs[j-2].value=(XtArgVal)wid;
 
 chargs[0].value=(XtArgVal)wid;
 k=CHBX_KOPPFLAG;
 wid=chboxlist[k].t_wid=CreateToggle(
	chboxlist[k].wid_name+1,
	form,
	chargs,l);


 j--;
 wid1=wid;
 XtSetArg(buttargs[j],XtNfromHoriz,wid);j++;
 k=LAB_CPL;
 wid=clablist[k].t_wid=XtCreateManagedWidget(
	clablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j);
 XtSetArg(buttargs[j-1],XtNhorizDistance,BUTTSEP);
 buttargs[j-2].value=(XtArgVal)wid;

 XtVaGetValues(wid,XtNheight,&h,XtNvertDistance,&dist,NULL);
 XtVaGetValues(wid1,XtNheight,&maxh,NULL);
 XtVaSetValues(wid1,XtNvertDistance,dist+(int)(h-maxh)/2,NULL);

 for (i=BUTT_STROM; i<=BUTT_KOPP; i++) {
   wid=buttonlist[i].t_wid=XtCreateManagedWidget(
	buttonlist[i].wid_name+1,
	commandWidgetClass,
	form,
	buttargs,j);
   XtVaGetValues(wid,XtNwidth,&w,NULL);
   if (w>maxw) maxw=w;
   buttargs[j-2].value=(XtArgVal)wid;
 }

 k=LAB_CLO;
 wid=clablist[k].t_wid=XtCreateManagedWidget(
	clablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j-1);
	
 buttargs[j-2].value=(XtArgVal)wid;
 k=BUTT_CLONE;
 wid=buttonlist[k].t_wid=XtCreateManagedWidget(
	buttonlist[k].wid_name+1,
	commandWidgetClass,
	form,
	buttargs,j);
 XtVaGetValues(wid,XtNwidth,&w,NULL);
 if (w>maxw) maxw=w;
 buttargs[j-2].value=(XtArgVal)wid;

 chargs[0].value=(XtArgVal)wid;
 k=CHBX_CLONESHOW;
 wid=chboxlist[k].t_wid=CreateToggle(
	chboxlist[k].wid_name+1,
	form,
	chargs,l);

 XtAddCallback(wid,XtNcallback,CBExpose,(XtPointer)NULL);

 j--;
 wid1=wid;
 XtSetArg(buttargs[j],XtNfromHoriz,wid);j++;
 k=LAB_VIS;
 wid=clablist[k].t_wid=XtCreateManagedWidget(
	clablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j);
 XtSetArg(buttargs[j-1],XtNhorizDistance,BUTTSEP);
 buttargs[j-2].value=(XtArgVal)wid;

 
 XtVaGetValues(wid,XtNheight,&h,XtNvertDistance,&dist,NULL);
 XtVaGetValues(wid1,XtNheight,&maxh,NULL);
 XtVaSetValues(wid1,XtNvertDistance,dist+(int)(h-maxh)/2,NULL);

 wid=outputlist[MSSG_KOPPVEK].t_wid=XtCreateManagedWidget(
	outputlist[MSSG_KOPPVEK].wid_name+1,
	textFieldWidgetClass,
	form,
	buttargs+1,j-1);
 XtVaSetValues(wid,XtNreadOnly,True,XtNwidth,maxw,XtNfocusInterest,False,NULL);
 buttargs[j-2].value=(XtArgVal)wid;

 k=LAB_Y12;
 wid=clablist[k].t_wid=XtCreateManagedWidget(
	clablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j-1);

 buttargs[j-2].value=(XtArgVal)wid;
 wid=outputlist[MSSG_KOPPY].t_wid=XtCreateManagedWidget(
	outputlist[MSSG_KOPPY].wid_name+1,
	textFieldWidgetClass,
	form,
	buttargs+1,j-1);

 XtVaSetValues(wid,XtNreadOnly,True,XtNwidth,maxw,XtNfocusInterest,False,NULL);
 buttargs[j-2].value=(XtArgVal)wid;

 XtVaGetValues(input_haupt[TXTFLD_NSEGS].wid.t_wid,
	XtNhorizDistance,&dist,
	NULL);
 XtVaGetValues(clablist[LAB_NUM].t_wid,XtNwidth,&w,NULL);

 XtVaSetValues(input_haupt[TXTFLD_NSEGS].wid.t_wid,
	XtNwidth,maxw-w-(Dimension)dist,
	NULL);

 for (i=BUTT_SEG; i<=BUTT_CLONE; i++) {
	XtVaSetValues(buttonlist[i].t_wid,XtNwidth,maxw,NULL);
 }
 return calc;
}
