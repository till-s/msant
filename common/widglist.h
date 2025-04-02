#ifndef WIDGLIST_H
#define WIDGLIST_H

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include "inout.h"

typedef struct {
  char *  wid_name;	/* Name des TextField-Widgets 
			 */
  Widget  t_wid;	/* Widget-Id; soll beim Generieren des
			 * Widget mit register_to_widgetlist()
			 * hier eingetragen werden 
			 */
  void   (*init_proc)(/*Widget w; char *wid_list*/);
			/* Prozedur, welche beim Initialisieren,
			 * nach dem Realisieren der Widgethierarchie
			 * aufgerufen wird (in register_to_widgetlist)
			 */
  } WidTabEl;

typedef struct {
  WidTabEl  wid;
  void (*converter)(/* Widget w, XtPointer var, Zeichnung z */);
  int 	    offset;	/* Variable, welche mit dem TextField verkn"upft ist
			 * Diese wird als offset spezifiziert (zB. index in eine
			 * Struktur. Die Bezugsadresse wird als Namenszeiger (wid_name)
			 * im ersten Tabellenelement gespeichert (Da keine statische
			 * Initialisierung m"oglich!)
			 */
  Boolean   update;     /* Soll das dem Text-Field zugeordnete Feld
		         * bei jedem Focus-Verlust automatisch
	                 * aktualisiert werden?
			 */
  void (*propagate)(/* TextFieldTabEl *list, int index, Zeichnung z */);
			/* Wenn eine 'propagate' Prozedur vorhanden ist,
			 * kann sie Seiteneffekte behandeln
			 */
  XtPointer data;	/* Beliebige Daten */
  } TextFieldTabEl;


/* Die zu den Widgetnamen aus wlist geh"origen Widgets suchen,
 * und in die Tabelle eintragen. Gibt 0 zur"uck, falls alles ok,
 * einen Zeiger auf den nicht gefundenen Namen im Fehlerfalle.
 * Die einzelen Tabelleneintr"age haben die Gr"osse incr
 * (zB. sizeof(TextFieldTabEl))
 */

char *register_to_widgetlist(/* WidTabEl *wlist ; int incr; */);

/* Der Konversionsprozedur werden drei Parameter "ubergeben:
 * - das Widget, mit welchem sie kommunizieren muss
 *   (dies geschieht mit den implementierungsabh"angigen
 *   Routinen getmessg() und printmessg() aus inout
 * - ein Zeiger auf die Zahl; 
 * - ein Zeiger auf eine hdata-Struktur, da zum Konvertieren
 *   evtl. weitere Information erforderlich ist (Rasterung)
 */

void getfloat(/*Widget wid, float/double/int *zahl, Zeichnung zeichnung*/);

/* mit zeichnung->fak zu skalierendes float einlesen */
void getscaled(/*Widget wid, float/double/int *zahl, Zeichnung zeichnung*/);

void getint(/*Widget wid, float/double/int *zahl, Zeichnung zeichnung*/);

void getdouble(/*Widget wid, float/double/int *zahl, Zeichnung zeichnung*/);

/* skaliertes int einlesen */
void getscaledint(/*Widget wid, float/double/int *zahl, Zeichnung zeichnung*/);

/* skalierte und gerasterte Koordinate eingeben */
void getrasterscaled(/*Widget wid, koord_el *zahl, Zeichnung zeichnung*/);

/* x,y-Koordinaten bezgl. Ursprung/Referenzpunkt */
void getx(/*Widget wid, koord_el* zahl, Zeichnung z*/);
void gety(/*Widget wid, koord_el* zahl, Zeichnung z*/);
void getdx(/*Widget wid, koord_el* zahl, Zeichnung z*/);
void getdy(/*Widget wid, koord_el* zahl, Zeichnung z*/);

/* Parameter der Widgets aus inputlist (bis zu deren Ende) in die
 * Variable mit der Adresse structptr lesen.
 */
void getparams(/*inputlist*/);
/* TextFieldTabEl inputlist[];
 */

/* Das Widget wid in der inputlist suchen, seinen String
 * konvertieren und in der "Programm-internen" Variablen
 * speichern
 */
void storeparam(/*TextFieldTabEl *inputlist, int index, Zeichnung z*/);

#if (__STDC__ && !defined(UnixCpp)) || defined(AnsiCpp)
#define WID_NAME(wid)wid##_STR
#else
#define WID_NAME(wid)wid/**/_STR
#endif

void printort(/*TextFieldTabEl *koord_inputlist, koord ort, Zeichnung z*/);

/* Die folgenden Variablen werden exportiert
 *
 * extern WidTabEl *outputlist, *buttonlist;
 * extern TextFieldTabEl	      *inputlist;
 */

/*
 *
 * !!! WICHTIG !!!
 *
 * die Widgets m"ussen in widglist.c
 * IN DER SELBEN REIHENFOLGE
 * in die Tabellen eingetragen werden
 */

/* Text-Field (oder "aquivalente Widgets)
 */


/* Das erste Array-Element enth"alt einen Zeiger
 * auf die durch das Array verwaltete struct
 */
/* Felder von input_haupt */
#define TXTFLD_UNDELBUFSIZ	1
#define TXTFLD_NSEGS		2
#define TXTFLD_RAS		3
#define TXTFLD_FREQ		4
#define TXTFLD_EPSI		5
#define TXTFLD_HSUBST		6

#define N_TXTFLD_HAUPT		7


/* Felder von input_fname */
#define TXTFLD_FNAME		1

#define NTXTFLD_FNAME		2

/* Felder von input_koords */
#define TXTFLD_X		1
#define TXTFLD_DX		2
#define TXTFLD_Y		3
#define TXTFLD_DY		4

#define NTXTFLD_KOORDS		5

/* Felder von input_radius */
#define TXTFLD_RADIUS		1

#define NTXTFLD_RADIUS		2

/* Meldungen (Message) */
#define MSSG_M			0
#define MSSG_FN			1
#define MSSG_KOPPVEK		2
#define MSSG_KOPPY		3
#define MSSG_VERSION		4

#define N_MSSG			5



/* Oblong Buttons */
#define BUTT_GRAF		0
#define BUTT_DATEI		1
#define BUTT_CALC		2
#define BUTT_USR1		3
#define BUTT_USR2		4
#define BUTT_QUIT		5

#define BUTT_NEWSTRUCT		6
#define BUTT_INSPOINT		7
#define BUTT_MOVEPOINT		8
#define BUTT_DELPOINT		9
#define BUTT_ADDPOL		10
#define BUTT_COPYPOL		11
#define BUTT_MOVEPOL		12
#define BUTT_DELPOL		13
#define BUTT_INSSTIFT		14
#define BUTT_UNDEL		15
#define BUTT_TOOLREF		16
#define BUTT_TOOLDIST		17

#define BUTT_LOAD		18
#define BUTT_SAVE		19
#define BUTT_SAVEAS		20

#define BUTT_SEG		21
#define BUTT_SEGALL		22
#define BUTT_SPEIS		23
#define BUTT_STROM		24
#define BUTT_FELD		25
#define BUTT_KOPP		26

#define BUTT_CLONE		27

#define N_BUTT			28

/* Canvas Widgets */
#define CNVS_GRAFIKFENSTER	0
#define CNVS_STROMFENSTER	1

#define N_CNVS			2

/* Check-Boxes */
#define CHBX_REFP_SET		0
#define CHBX_KOPPFLAG		1
#define CHBX_CLONESHOW		2

/* exportierte Variablen */

extern TextFieldTabEl	input_haupt[],input_fname[],input_radius[],input_koords[];
extern WidTabEl		buttonlist[],outputlist[],canvaslist[],
			chboxlist[];

#endif
