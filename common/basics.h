#ifndef BASICS_H
#define BASICS_H
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <math.h>

#ifdef _POSIX_SOURCE
#include "divfuncs.h"
#endif

/* einige mehreren Orts verwendete Datenstrukturen
 */

#include "PBuffer.h"
#include "punkte.h"

#define VERSIONFORMAT "MsantVersion %u\n"
#define VERSION   12

#define RING	   0 /* Lautst"arke f"ur XBell() */

/* einige Konstanten */

#define C_VELOC 299792458.
#define EPSI_0  8.85418781700E-12
#define Z_0     376.730313475

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

#ifndef PI
#define PI M_PI
#endif

#define SPEISESTROM 1.

/* N"utzliche Makros */
#define STR(zeichenfolge)"zeichenfolge"

/* Datentypen */
typedef struct { float re,im;} complex;
typedef enum {none,add,ins,move,dist,speise,speisestift,movepol,copypol,refpnt,klon} modes;
typedef struct hdata *Zeichnung;

typedef union _re_co { float *re; complex *co; char *raw;} real_complex;

#define ST_KOPP	(1<<0)
#define ST_STRM (1<<1)
#define ST_FELD (1<<2)
#define ST_FILE (1<<3)
#define ST_GEOM (1<<4)
#define ST_REGI (1<<5)

#define ST_VALID(zeichnung,mode)((((zeichnung)->status) & (mode))!=0)

struct hdata{
 Boolean resources_found; /* Wurde eine App-resources Datei gefunden? */
 String  raw_strings;     /* Die Message-strings */
 modes mode;

 float fak,freq,epsi,mu,hsubst;

 koord distort,off,orig; /* verstellbarer Referenzpunkt */
 koord input;		 /* wird "uber Eingabefelder eingestellt */

 koord translate; 	/* translationsvektor f. move, copypol, movepol,klon */

 int   nsegs;
 int   raster;

 Polygonliste borders;

 Selectionliste sel;

 UndelBuffer  undel;

 Punkt *speiseseg;
 int    speiseindex;

 struct _stift{
  koord  ort;
  float  radius;} stift;
 

 real_complex jmagn, jelek;
 float		Ibet2;
			/* Integral "uber Betragsquadrat des elekt.
	 		 * Stroms l"angs der Konturen
			 */
 int   nstrom;

/* Felddaten */
 XPoint rmax, rmin;
 float *Vre, *Vim,
       minRe,maxRe,minIm,maxIm,maxAbs;
 Pixmap field_pix;     /* field_pix == 0 bedeutet, dass kein g"ultiges 
			* Feld vorhanden ist.
			*/
 int   mmax,nmax;
 Region pregion;

/* Kopplung zwischen identischen Strukturen 
 * ('Ueberlappungsintegral')
 */
 koord   koppdist;         /* verschiebungsvektor */
 complex koppY;

 PBuffer xbuf1,xbuf2;
/* Gr"osse des Grafikfensters */
 Dimension width,height;
 Pixel	foreground;
 Pixel	field_colour;
 Pixel	coord_colour;
 Pixel	patch_colour;
 Pixel  canvas_colour;

/* status */
 int     status;
}; 


/* exportierte globale Variablen */
#define zeichnung (*(Zeichnung*)&input_haupt[0].wid.wid_name)

/* X-Windows Globals */
extern Display   *disp;

extern Widget    w_toplevel;

extern XtAppContext app;


/* Zeichen- und Darstellungsfenster */
extern Window   win ,win1;
extern int	win_depth; /* depth of win */

/* Das icon */
extern Window   icon_window;

/* Cursors */
extern Cursor curs_none, curs_cross, curs_mess, curs_move,
              curs_wait, curs_point, curs_readout;

/* GCs */
extern GC gcdotted,gcthick;  /* These are allocated with */
			     /* XtGetGC and should not be modified */

extern GC gc,gccoord,gcxor,gcfield;

/* Farben */
extern unsigned long black,white;

/* Atoms */
extern Atom     A_wm_delete_window, A_wm_protocols;

#endif
