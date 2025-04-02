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
#include "datei_ui.h"

#define LAB_AKT	0
#define LAB_NAM	1

static WidTabEl flablist[]={
	{"*File",0,0},
	{"*New",0,0},
};


Widget CreateDatei(top)
 Widget top;
{
 Widget datei,wid,wid1,wid2,form;
 int    i,j,k;
 Arg	buttargs[10];
 Dimension w,h,maxw,maxh,overall,maxl;
 int	dist;

 datei=CreatePinpop(top,"File",&form);
 
 wid=0;
 j=0;
 XtSetArg(buttargs[j],XtNright,XtChainLeft); j++;
 XtSetArg(buttargs[j],XtNleft,XtChainLeft); j++;
#ifdef XAW3D
 XtSetArg(buttargs[j],XtNborderWidth,0); j++;
#endif
 XtSetArg(buttargs[j],XtNtop,XtChainTop); j++;
 XtSetArg(buttargs[j],XtNbottom,XtChainTop); j++;
 XtSetArg(buttargs[j],XtNfromVert,wid);j++;
 XtSetArg(buttargs[j],XtNfromHoriz,wid);j++;

 k=LAB_AKT;
 wid2=wid=flablist[k].t_wid=XtCreateManagedWidget(
	flablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j);

 buttargs[j-1].value=(XtArgVal)wid;

 k=MSSG_FN;
 wid1=outputlist[k].t_wid=XtCreateManagedWidget(
	outputlist[k].wid_name+1,
	textFieldWidgetClass,
	form,
	buttargs+1,j-1);

 XtVaSetValues(wid1,XtNreadOnly,True,XtNfocusInterest,False,NULL);

 XtVaGetValues(wid,XtNheight,&h,XtNwidth,&maxl,NULL);
 XtVaGetValues(wid1,XtNheight,&maxh,NULL);
 if (maxh>h) wid1=wid; else maxh=h;
 XtVaSetValues(wid1,XtNheight,maxh,NULL);

 buttargs[j-2].value=(XtArgVal)wid;
 buttargs[j-1].value=(XtArgVal)0;

 maxw=0;
 overall=0;
 for (i=BUTT_LOAD; i<=BUTT_SAVEAS; i++) {
   wid=buttonlist[i].t_wid=XtCreateManagedWidget(
	buttonlist[i].wid_name+1,
	commandWidgetClass,
	form,
	buttargs+2,j-2);
   XtVaGetValues(wid,XtNwidth,&w,XtNhorizDistance,&dist,NULL);
   overall+=(Dimension)dist;
   if (w>maxw) maxw=w;
   buttargs[j-1].value=(XtArgVal)wid;
 }

 for (i=BUTT_LOAD; i<=BUTT_SAVEAS; i++) {
   XtVaSetValues(buttonlist[i].t_wid,XtNwidth,maxw,NULL);
 }
 overall+=3*maxw;

 buttargs[j-2].value=(XtArgVal)wid;
 buttargs[j-1].value=(XtArgVal)0;
 k=LAB_NAM;
 wid=flablist[k].t_wid=XtCreateManagedWidget(
	flablist[k].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j);

 buttargs[j-1].value=(XtArgVal)wid;
 wid1=input_fname[TXTFLD_FNAME].wid.t_wid=XtCreateManagedWidget(
	input_fname[TXTFLD_FNAME].wid.wid_name+1,
	textFieldWidgetClass,
	form,
	buttargs+1,j-1);

 XtVaGetValues(wid,XtNheight,&h,XtNhorizDistance,&dist,XtNwidth,&w,NULL);

 if (w<maxl) wid2=wid;
 else	     maxl=w;
 XtVaSetValues(wid2,XtNwidth,maxl,NULL);

 overall-=(Dimension)dist+maxl;
 XtVaGetValues(wid1,XtNheight,&maxh,XtNhorizDistance,&dist,NULL);
 overall-=(Dimension)dist;
 XtVaSetValues(wid1,XtNwidth,overall,NULL);
 XtVaSetValues(outputlist[MSSG_FN].t_wid,XtNwidth,overall,NULL);
 
 if (maxh>h) wid1=wid; else maxh=h;
 XtVaSetValues(wid1,XtNheight,maxh,NULL);

 return datei;
}
