#include "basics.h"

/* X-Windows Globals */

Display   *disp;
Widget   w_toplevel;
XtAppContext app;

/* Zeichen- und Darstellungsfenster */
Window   win ,win1;
int	 win_depth;

/* Das Icon-Window */
Window   icon_window;


/* Cursors */
Cursor curs_none, curs_cross, curs_mess, curs_move,
              curs_wait, curs_point, curs_readout;

/* GCs */
GC gc,gccoord,gcdotted,gcxor,gcthick,gcfield;

/* Farben */
unsigned long black,white;

/* Atoms */
Atom     A_wm_delete_window, A_wm_protocols;

/* Den Linker zufriedenstellen (kein FORTRAN MAIN) */

void MAIN_(argc,argv)
 int argc;
 char *argv[];
{
/* this should never be called! */
}
