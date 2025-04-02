#include <stdio.h>

#include "basics.h"

#include "olnames.h"
#include "widglist.h"
#include "graf.h"
#include "msant_strings.h"

extern double strtod();
extern long   strtol();
static void   getradius();
static void   update();

/* Zentrale Tabelle aller 
 * Widgets. Die Prozeduren zum Auslesen von Werten
 * etc. sind z.T. implementationsabh"angig. Diese finden sich 
 * in 'inout'. Dort sind auch die Namen der Widgets
 * definiert, da auch diese leicht von der Implementierung
 * abh"angen k"onnen. (z.B. erhalten alle mit 'devguide'
 * erstellten Widgets automatisch den Modulnamen mit Underscore
 * als Pr"afix!
 */

/* Im ersten Feld einer Liste von TextFields wird die Adresse der
 * durch die Liste verwalteten Variablen
 * abgelegt!
 */

TextFieldTabEl   input_haupt[]={
	{{NULL				,NULL,NULL}
		,NULL	,0, False
		,NULL, NULL		},
	{{WID_NAME(TXTFLD_UNDELBUFSIZ)	,NULL,init_TextField}
		,getint	,XtOffset(Zeichnung,undel.max), True
		,NULL, NULL	},
	{{WID_NAME(TXTFLD_NSEGS)	,NULL,init_TextField}
		,getint	,XtOffset(Zeichnung,nsegs), True
		,NULL, NULL	},
	{{WID_NAME(TXTFLD_RAS)		,NULL,init_TextField}
		,getscaledint,XtOffset(Zeichnung,raster), True
		,NULL, NULL	},
	{{WID_NAME(TXTFLD_FREQ)		,NULL,init_TextField}
		,getfloat,XtOffset(Zeichnung,freq), False
		,NULL, NULL	},
	{{WID_NAME(TXTFLD_EPSI)		,NULL,init_TextField}
		,getfloat,XtOffset(Zeichnung,epsi), False
		,NULL, NULL	},
	{{WID_NAME(TXTFLD_HSUBST)	,NULL,init_TextField}
		,getfloat,XtOffset(Zeichnung,hsubst), False
		,NULL, NULL	},
	{{NULL				,NULL,NULL}
		,NULL	,0, False
		,NULL, NULL	},
       };


TextFieldTabEl   input_fname[]={
	{{NULL				,NULL,NULL}
		,NULL	,0, False
		,NULL	,NULL	},
	{{WID_NAME(TXTFLD_FNAME)	,NULL,NULL} 
		,NULL	,0, False
		,NULL	,NULL	},
	{{NULL				,NULL,NULL}
		,NULL	,0, False
		,NULL	,NULL	},
       };

TextFieldTabEl   input_radius[]={
	{{NULL				,NULL,NULL} 
		,NULL	,0, False
		,NULL	,NULL	},
	{{WID_NAME(TXTFLD_RADIUS)	,NULL,init_TextField}
		,getradius ,XtOffset(Punkt*,len),True
		,NULL	,NULL	},
	{{NULL				,NULL,NULL}
		,NULL	,0, False
		,NULL	,NULL	},
       };


TextFieldTabEl	input_koords[]={
	{{NULL				,NULL,NULL} 
		,NULL	,0, False
		,NULL	,NULL	},
	{{WID_NAME(TXTFLD_X), 	NULL, init_TextField}
		,getx,XtOffset(Zeichnung,input.x), True
		,update	,(XtPointer)TXTFLD_DX	},
	{{WID_NAME(TXTFLD_DX), 	NULL, init_TextField}
		,getdx,XtOffset(Zeichnung,input.x), True
		,update	,(XtPointer)TXTFLD_X	},
	{{WID_NAME(TXTFLD_Y), 	NULL, init_TextField}
		,gety,XtOffset(Zeichnung,input.y), True
		,update	,(XtPointer)TXTFLD_DY	},
	{{WID_NAME(TXTFLD_DY), 	NULL, init_TextField}
		,getdy,XtOffset(Zeichnung,input.y), True
		,update	,(XtPointer)TXTFLD_Y	},
	{{NULL				,NULL,NULL}
		,NULL	,0, False
		,NULL	,NULL	},
       };

WidTabEl	outputlist[]={
	{WID_NAME(MSSG_M), 	NULL, NULL},
	{WID_NAME(MSSG_FN), 	NULL, NULL},
	{WID_NAME(MSSG_KOPPVEK),NULL, NULL},
	{WID_NAME(MSSG_KOPPY), 	NULL, NULL},
	{WID_NAME(MSSG_VERSION),NULL, NULL},
        {NULL,			NULL, NULL}
       };

WidTabEl	canvaslist[]={
	{WID_NAME(CNVS_GRAFIKFENSTER), 	NULL, init_Canvas},
	{WID_NAME(CNVS_STROMFENSTER), 	NULL, init_Canvas},
        {NULL,			NULL, NULL}
       };

WidTabEl	chboxlist[]={
	{WID_NAME(CHBX_REFP_SET), 	NULL, NULL},
	{WID_NAME(CHBX_KOPPFLAG), 	NULL, NULL},
	{WID_NAME(CHBX_CLONESHOW), 	NULL, NULL},
        {NULL,			NULL, NULL}
       };

WidTabEl	buttonlist[]={
	{WID_NAME(BUTT_GRAF), 		NULL, NULL},
	{WID_NAME(BUTT_DATEI), 		NULL, NULL},
	{WID_NAME(BUTT_CALC), 		NULL, NULL},
	{WID_NAME(BUTT_USR1), 		NULL, NULL},
	{WID_NAME(BUTT_USR2), 		NULL, NULL},
	{WID_NAME(BUTT_QUIT), 		NULL, NULL},
	{WID_NAME(BUTT_NEWSTRUCT),	NULL, NULL},
	{WID_NAME(BUTT_INSPOINT),	NULL, NULL},
	{WID_NAME(BUTT_MOVEPOINT),	NULL, NULL},
	{WID_NAME(BUTT_DELPOINT),	NULL, NULL},
	{WID_NAME(BUTT_ADDPOL),		NULL, NULL},
	{WID_NAME(BUTT_COPYPOL),	NULL, NULL},
	{WID_NAME(BUTT_MOVEPOL),	NULL, NULL},
	{WID_NAME(BUTT_DELPOL),		NULL, NULL},
	{WID_NAME(BUTT_INSSTIFT),	NULL, NULL},
	{WID_NAME(BUTT_UNDEL),		NULL, NULL},
	{WID_NAME(BUTT_TOOLREF),	NULL, NULL},
	{WID_NAME(BUTT_TOOLDIST),	NULL, NULL},
	{WID_NAME(BUTT_LOAD),		NULL, NULL},
	{WID_NAME(BUTT_SAVE),		NULL, NULL},
	{WID_NAME(BUTT_SAVEAS),		NULL, NULL},
	{WID_NAME(BUTT_SEG),		NULL, NULL},
	{WID_NAME(BUTT_SEGALL),		NULL, NULL},
	{WID_NAME(BUTT_SPEIS),		NULL, NULL},
	{WID_NAME(BUTT_STROM),		NULL, NULL},
	{WID_NAME(BUTT_FELD),		NULL, NULL},
	{WID_NAME(BUTT_KOPP),		NULL, NULL},
	{WID_NAME(BUTT_CLONE),		NULL, NULL},
	{NULL,				NULL, NULL}
       };

/* float aus wid in zahl einlesen; falls
 * die zahl in wid ung"ultig ist, den alten
 * wert behalten, und ins wid schreiben
 */
void getfloat(wid,zahl,data)

Widget   wid;
float  *zahl;
char   *data;

{
float  rval;
char   str[80],*ptr;

if (zahl==0) return;
getmessg(wid,str);
rval=(float)strtod(str,&ptr);
if (str==ptr) {rval=*zahl;}
*zahl=rval;
sprintf(str,"%g",rval);
printmessg(0,str,wid);
}

/* mit data->fak skaliertes float einlesen
 */

void getscaled(wid,zahl,data)

Widget wid;
float *zahl;
Zeichnung data;

{
*zahl/=data->fak;
getfloat(wid,zahl,data);
*zahl*=data->fak;
}

void getrasterscaled(wid,zahl,data)
Widget wid;
koord_el *zahl;
Zeichnung data;
{
float	 val;
char	 str[80],*ptr;
koord_el rval;

if (zahl==0) return;

val=(float)*zahl/data->fak;

getmessg(wid,str);
rval=(koord_el)(strtod(str,&ptr)*data->fak);
if (str!=ptr) {
  *zahl=RASTER(rval,data->raster);
  val=(float)*zahl/data->fak;
}
sprintf(str,"%g",val);
printmessg(0,str,wid);
}

/* int aus wid in zahl einlesen; falls
 * die zahl in wid ung"ultig ist, den alten
 * wert behalten, und ins wid schreiben
 */
void getint(wid,zahl,data)

Widget   wid;
int    *zahl;
Zeichnung   data;

{
int    rval;
char   str[80],*ptr;

if (zahl==0) return;
getmessg(wid,str);
rval=(int)strtol(str,&ptr,10);
if (str==ptr) {rval=*zahl;}
*zahl=rval;
sprintf(str,"%i",rval);
printmessg(0,str,wid);
}


/* double aus wid in zahl einlesen; falls
 * die zahl in wid ung"ultig ist, den alten
 * wert behalten, und ins wid schreiben
 */
void getdouble(wid,zahl,data)

Widget   wid;
double  *zahl;
char   *data;

{
double  rval;
char   str[80],*ptr;

if (zahl==0) return;
getmessg(wid,str);
rval=strtod(str,&ptr);
if (str==ptr) {rval=*zahl;}
*zahl=rval;
sprintf(str,"%g",rval);
printmessg(0,str,wid);
}

void getscaledint(wid,zahl,z)
Widget wid;
int   *zahl;
Zeichnung z;

{
int rval;
float tmpf;
char   str[80],*ptr;

if (zahl==0) return;
getmessg(wid,str);
tmpf=(float)strtod(str,&ptr);

if (str==ptr) {rval=*zahl;}
else {rval=(int)(tmpf*z->fak);}

if (rval==0) rval=1;

*zahl=rval;
sprintf(str,"%.4g",rval/z->fak);
printmessg(0,str,wid);
}

void getparams(inputlist)

TextFieldTabEl inputlist[];

{
int i;
char *structp;

structp=inputlist[0].wid.wid_name;
for (i=1; (inputlist[i].wid.wid_name!=(char*)0); i++) {
 if (inputlist[i].converter) {   /* Wenn ein Converter registriert, */
  inputlist[i].converter(inputlist[i].wid.t_wid,
  		 	 structp+inputlist[i].offset,
			 zeichnung);
 }
}
}

void storeparam(inputlist,index,z)


TextFieldTabEl  inputlist[];
int		index;
Zeichnung	z;

{
 if (inputlist[index].converter &&    /* Wenn ein Converter registriert, */
     inputlist[0].wid.wid_name ) {    /* und der Zeiger auf die struct !=0 ist */

     inputlist[index].converter(inputlist[index].wid.t_wid,
  	    inputlist[0].wid.wid_name+inputlist[index].offset,
	    z);

     if (inputlist[index].propagate)
	    inputlist[index].propagate(inputlist,index,z);
 }
 return;
}


char *register_to_widgetlist(wlist,incr)

char  * wlist;
int	 incr;

{
int i;
WidTabEl *ptr;
for (i=0; (ptr=(WidTabEl *)(wlist +i))->wid_name; i+=incr) {
 if ((ptr->t_wid=XtNameToWidget(w_toplevel,ptr->wid_name))==0) {
  break;
 }else{
 if (ptr->init_proc) {ptr->init_proc(ptr->t_wid,(char*)wlist);}
 }
}
return(ptr->wid_name);
}

/* dies ist ein bisschen wirr!
 * beim "Andern des Radius muss auch der 
 * Richtungsvektor des Stiftes (zum Zeichnen der Nummer
 * angepasst werden. Da der Konversionsprozedur dessen Adresse
 * nicht "Ubergeben wird, muss sie berechnet werden 
 */

static void getradius(wid,zahl,z)

Widget wid;
float *zahl;
Zeichnung z;

{
float rad;
char str[50];
Punkt *punkt;

punkt=(Punkt*)((char*)zahl-XtOffset(Punkt*,len));
getfloat(wid,&rad,z);

if (rad<=0.) {
 sprintf(str,"%g",*zahl/z->fak);
 XBell(disp,RING);
 printmessg(0,str,wid);
} else {
 *zahl=rad*z->fak;
 Richtung(0,punkt);
 CBExpose(0,0,0);
}
}
 
/* Koordinaten bezgl. Ursprung od. Referenzpunkt */
void getx(wid,zahl,z)
Widget wid;
koord_el *zahl;
Zeichnung z;
{
koord_el val;
val=*zahl-(koord_el)z->orig.x;
getrasterscaled(wid,&val,z);
*zahl=val+(koord_el)z->orig.x;
}

void gety(wid,zahl,z)
Widget wid;
koord_el *zahl;
Zeichnung z;
{
koord_el val;
val=-*zahl+(koord_el)z->orig.y;
getrasterscaled(wid,&val,z);
*zahl=-val+(koord_el)z->orig.y;
}

void getdx(wid,zahl,z)
Widget wid;
koord_el *zahl;
Zeichnung z;
{
koord_el val;
val=*zahl-(koord_el)z->orig.x-(koord_el)z->off.x;
getrasterscaled(wid,&val,z);
*zahl=val+(koord_el)z->orig.x+(koord_el)z->off.x;
}

void getdy(wid,zahl,z)
Widget wid;
koord_el *zahl;
Zeichnung z;
{
koord_el val;
val=-*zahl+(koord_el)z->orig.y-(koord_el)z->off.y;
getrasterscaled(wid,&val,z);
*zahl=-val+(koord_el)z->orig.y-(koord_el)z->off.y;
}

/* Wenn ein Koordinatenfeld (zB. 'X') ver"andert wird,
 * muss das zugeordnete (z.B. 'DX') nachgef"uhrt werden.
 */
static void update(inputlist,index,z)
TextFieldTabEl	inputlist[];
int		index;
Zeichnung	z;
{
printort(inputlist,&z->input,z);
}

void printort(koord_inputlist,ort,z)
TextFieldTabEl	koord_inputlist[];
koord		*ort;
Zeichnung	z;
{
char str[128];
sprintf(str,fPosData,((ort->x-z->orig.x)/z->fak));
printmessg(0,str,koord_inputlist[TXTFLD_X].wid.t_wid);
sprintf(str,fPosData,((-ort->y+z->orig.y)/z->fak));
printmessg(0,str,koord_inputlist[TXTFLD_Y].wid.t_wid);
sprintf(str,fPosData,((ort->x-z->orig.x-z->off.x)/z->fak));
printmessg(0,str,koord_inputlist[TXTFLD_DX].wid.t_wid);
sprintf(str,fPosData,((-ort->y+z->orig.y-z->off.y)/z->fak));
printmessg(0,str,koord_inputlist[TXTFLD_DY].wid.t_wid);
}

