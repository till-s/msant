#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include "basics.h"

#include "widglist.h"
#include "graf.h"
#include "haupt_ui.h"
#include "grafik_ui.h"
#include "datei_ui.h"
#include "calc_ui.h"

#include "xawutils/pinpop.h"

#include "popup.h"
#include "common/msant_strings.h"

#include <X11/Shell.h>

/* externals */

extern char**environ;

/* forward declarations */

static XrmOptionDescRec opts[]={
        {"-label",  ".label",  XrmoptionSepArg,NULL}, 
       };

/* Global var defs */

static String fbr[]={
     "*background: 		grey80",
        NULL};

int main(argc,argv)
 int   argc;
 char *argv[];

{
  Widget top,pop_grafik,pop_datei,pop_calc;


  XtSetLanguageProc(0,0,0);

  top=XtVaAppInitialize(&app,"Msant",
                     opts, 		/* options */
                     XtNumber(opts),    /* nopts   */
		     &argc,argv,
                     fbr, 		/* fallback-resources */
                     NULL);		/* Va-args */

  AddGrafActions(app);

  init_before_realize(top);

  CreateHaupt(top);
  
  XtVaSetValues(top,XtNiconWindow,icon_window,NULL);

 
  /*
  Create_pops
   */
  pop_grafik=CreateGrafik(top);
  XtAddCallback(buttonlist[BUTT_GRAF].t_wid,XtNcallback,CBPinup,(XtPointer)pop_grafik);

  pop_datei =CreateDatei(top);
  XtAddCallback(buttonlist[BUTT_DATEI].t_wid,XtNcallback,CBPinup,(XtPointer)pop_datei);

  pop_calc =CreateCalc(top);
  XtAddCallback(buttonlist[BUTT_CALC].t_wid,XtNcallback,CBPinup,(XtPointer)pop_calc);

  XtRealizeWidget(top);
  init_after_wids(top,argc,argv);

  XtAppMainLoop(app);
  return 0;
}
