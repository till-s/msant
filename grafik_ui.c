#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Shell.h>

#ifdef XAW3D
#include <X11/Xaw3d/ThreeD.h>
#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/SmeBSB.h>
#include <X11/Xaw3d/SmeLine.h>
#include <X11/Xaw3d/MenuButton.h>
#else
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/MenuButton.h>
#endif

#include "widget/FocusForm.h"
#include "widget/TextField.h"

#include "widglist.h"
#include "xawutils/pinpop.h"
#include "haupt_ui.h"

#include "common/msant_strings.h"

#define BUTTSEP 20

#define LAB_STRUCT	0
#define LAB_POINTS	1
#define LAB_POLYGON	2
#define LAB_RADIUS	3
#define LAB_TOOLS	4
#define LAB_RASTER	5
#define LAB_ITEMS	6
#define LAB_AUTO	7

static char *undelMenu_name="menu_undel";

static WidTabEl glablist[]={
	{"*Structure",0,0},
	{"*Points",0,0},
	{"*Polygons",0,0},
	{"*Radius",0,0},
	{"*Tools",0,0},
	{"*Raster",0,0},
	{"*Undelete Items",0,0},
	{"*automatic",0,0},
};

static char *undls[]={
	nActUndelete,0};


Widget CreateGrafik(top)
 Widget top;
{
 Widget grafik,wid,wid1,form,menu;
 Widget udf,undel_pop;
 int    i,j,k,l;
 Arg    args[15];
 Arg	buttargs[15];
 Arg    chargs[10];
 Dimension w,h,maxw,maxh,bw;
 int	dist;

 grafik=CreatePinpop(top,"Drawing Tools",&form);
 undel_pop=CreatePinpop(top,"Undelete-Buffer Size",&udf);
 

 menu=XtVaCreatePopupShell(
	undelMenu_name,
	simpleMenuWidgetClass,
	top,
	NULL);

 XtVaCreateManagedWidget("line",smeLineObjectClass,menu,NULL);
 wid=XtVaCreateManagedWidget("Undelete",smeBSBObjectClass,menu,NULL);
 XtAddCallback(wid,XtNcallback,CB_To_Action,(XtPointer)undls);
 wid=XtVaCreateManagedWidget("Buffer Size",smeBSBObjectClass,menu,NULL);
 XtAddCallback(wid,XtNcallback,CBPinup,(XtPointer)undel_pop);

 XtVaGetValues(menu,XtNwidth,&maxw,NULL);

 wid=0;
 j=0;
#ifdef XAW3D
 XtSetArg(buttargs[j],XtNborderWidth,0); j++;
#endif
 XtSetArg(buttargs[j],XtNtop,XtChainTop); j++;
 XtSetArg(buttargs[j],XtNbottom,XtChainTop); j++;
 XtSetArg(buttargs[j],XtNleft,XtChainLeft); j++;
 XtSetArg(buttargs[j],XtNright,XtChainLeft); j++;
 XtSetArg(buttargs[j],XtNjustify,XtJustifyLeft); j++;
 XtSetArg(buttargs[j],XtNfromVert,wid); j++;
 XtSetArg(buttargs[j],XtNhorizDistance,BUTTSEP); j++;

 k=LAB_STRUCT;
 for (i=BUTT_NEWSTRUCT; i<=BUTT_INSSTIFT; i++) {
   if (i==BUTT_NEWSTRUCT || i== BUTT_INSPOINT || i==BUTT_ADDPOL) {
	wid=glablist[k].t_wid=XtCreateManagedWidget(
		glablist[k].wid_name+1,
		labelWidgetClass,
		form,
		buttargs,j-1);
  	buttargs[j-2].value=(XtArgVal)wid;
	k++;
   }
   
   wid=buttonlist[i].t_wid=XtCreateManagedWidget(
	buttonlist[i].wid_name+1,
	commandWidgetClass,
	form,
	buttargs,j);
   XtVaGetValues(wid,XtNwidth,&w,NULL);
   if (w>maxw) maxw=w;
   buttargs[j-2].value=(XtArgVal)wid;
 }

 k=LAB_RADIUS;
 wid=glablist[k].t_wid=XtCreateManagedWidget(
		glablist[k].wid_name+1,
		labelWidgetClass,
		form,
		buttargs,j);

 wid1=wid;
 i=0;
 XtSetArg(args[i],XtNtop,XtChainTop); i++;
 XtSetArg(args[i],XtNbottom,XtChainTop); i++;
 XtSetArg(args[i],XtNleft,XtChainLeft); i++;
 XtSetArg(args[i],XtNjustify,XtJustifyLeft); i++;
#ifdef XAW3D
 XtSetArg(args[i],XtNborderWidth,0); i++;
#endif
 XtSetArg(args[i],XtNfromHoriz,wid); i++;
 XtSetArg(args[i],XtNfromVert,(Widget)buttargs[j-2].value); i++;
 wid=input_radius[TXTFLD_RADIUS].wid.t_wid=XtCreateManagedWidget(
		input_radius[TXTFLD_RADIUS].wid.wid_name+1,
		textFieldWidgetClass,
		form,
		args,i);
 XtVaGetValues(wid,XtNheight,&h,NULL);
 XtVaGetValues(wid1,XtNheight,&maxh,NULL);
 
 if (maxh>h) wid1=wid; else maxh=h;
 XtVaSetValues(wid1,XtNheight,maxh,NULL);

 wid1=glablist[LAB_ITEMS].t_wid=XtCreateManagedWidget(
		glablist[LAB_ITEMS].wid_name+1,
		labelWidgetClass,
		udf,
		buttargs,j-2);

 args[i-2].value=(XtArgVal)wid1;
 input_haupt[TXTFLD_UNDELBUFSIZ].wid.t_wid=XtCreateManagedWidget(
 		input_haupt[TXTFLD_UNDELBUFSIZ].wid.wid_name+1,
		textFieldWidgetClass,
		udf,
		args,i-1);

 buttargs[j-2].value=(XtArgVal)wid;
 
 wid=glablist[LAB_TOOLS].t_wid=XtCreateManagedWidget(
		glablist[LAB_TOOLS].wid_name+1,
		labelWidgetClass,
		form,	
		buttargs,j-1);

 buttargs[j-2].value=(XtArgVal)wid;
 XtSetArg(buttargs[j],XtNmenuName,undelMenu_name);j++;
 wid=buttonlist[BUTT_UNDEL].t_wid=XtCreateManagedWidget(
		buttonlist[BUTT_UNDEL].wid_name+1,
		menuButtonWidgetClass,
		form,	
		buttargs,j);
 j--;

 XtVaGetValues(wid,XtNwidth,&w,NULL);
 if (w>maxw) maxw=w;

 buttargs[j-2].value=(XtArgVal)wid;
 wid=buttonlist[BUTT_TOOLREF].t_wid=XtCreateManagedWidget(
		buttonlist[BUTT_TOOLREF].wid_name+1,
		commandWidgetClass,
		form,	
		buttargs,j);
 
 
 wid1=wid;
 l=0;
 XtSetArg(chargs[l],XtNfromVert,wid); l++;
 XtSetArg(chargs[l],XtNhorizDistance,BUTTSEP); l++;
 XtSetArg(chargs[l],XtNleft,XtChainLeft); l++;
 XtSetArg(chargs[l],XtNright,XtChainLeft); l++;
 XtSetArg(chargs[l],XtNtop,XtChainTop); l++;
 XtSetArg(chargs[l],XtNbottom,XtChainTop); l++;
 wid=chboxlist[CHBX_REFP_SET].t_wid=CreateToggle(
	chboxlist[CHBX_REFP_SET].wid_name+1,
	form,
	chargs,l);

 buttargs[j-2].value=(XtArgVal)wid1;
 wid1=wid;
 j--;
 XtSetArg(buttargs[j],XtNfromHoriz,wid);j++;
 wid=glablist[LAB_AUTO].t_wid=XtCreateManagedWidget(
	glablist[LAB_AUTO].wid_name+1,
	labelWidgetClass,
	form,
	buttargs,j);
 XtSetArg(buttargs[j-1],XtNhorizDistance,BUTTSEP);

 buttargs[j-2].value=(XtArgVal)wid;

 XtVaGetValues(wid,XtNheight,&h,XtNvertDistance,&dist,NULL);
 XtVaGetValues(wid1,XtNheight,&maxh,NULL);
 XtVaSetValues(wid1,XtNvertDistance,dist+(int)(h-maxh)/2,NULL);
	
 wid=buttonlist[BUTT_TOOLDIST].t_wid=XtCreateManagedWidget(
		buttonlist[BUTT_TOOLDIST].wid_name+1,
		commandWidgetClass,
		form,	
		buttargs,j);
 
 buttargs[j-2].value=(XtArgVal)wid;

 wid1=glablist[LAB_RASTER].t_wid=XtCreateManagedWidget(
		glablist[LAB_RASTER].wid_name+1,
		labelWidgetClass,
		form,
		buttargs,j);

 XtVaGetValues(wid1,XtNwidth,&w,NULL);
 if (w>maxw) maxw=w;

 args[i-2].value=(XtArgVal)wid1;
 args[i-1].value=(XtArgVal)wid;
 wid=input_haupt[TXTFLD_RAS].wid.t_wid=XtCreateManagedWidget(
 		input_haupt[TXTFLD_RAS].wid.wid_name+1,
		textFieldWidgetClass,
		form,
		args,i);

 XtVaGetValues(wid,XtNheight,&h,XtNhorizDistance,&dist,XtNborderWidth,&bw,NULL);
 XtVaGetValues(wid1,XtNheight,&maxh,NULL);
 
 if (maxh>h) wid1=wid; else maxh=h;
 XtVaSetValues(wid1,XtNheight,maxh,NULL);

 /* adapt the width of the textFields */
 w=maxw-w-2*bw-(Dimension)dist;
 XtVaSetValues(wid,XtNwidth,w,NULL);

 XtVaGetValues(glablist[LAB_RADIUS].t_wid,XtNwidth,&w,NULL);
 
 wid=input_radius[TXTFLD_RADIUS].wid.t_wid;

 XtVaGetValues(wid,XtNheight,&h,XtNhorizDistance,&dist,XtNborderWidth,&bw,NULL);
 w=maxw-w-2*bw-(Dimension)dist;
 XtVaSetValues(wid,XtNwidth,w,NULL);

 for (i=BUTT_NEWSTRUCT; i<=BUTT_TOOLDIST; i++) {
	XtVaSetValues(buttonlist[i].t_wid,XtNwidth,maxw,NULL);
 }
 XtVaSetValues(menu,XtNwidth,maxw,NULL);

 return grafik;
}
