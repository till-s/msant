#ifndef INOUT_H
#define INOUT_H

#include "popup.h"

/* exportierte Prozeduren */

/* Einen String in ein Widget schreiben (welches ein und dieselbe resource 
 * unterst"utzt).
 * wenn bell!=0 ist, wird ein t"onendes popup-Widget aufgerufen.
 * Message==0 erzeugt den leeren String.
 */

void printmessg(/* int bell; char * Message; Widget w; */);

/* Die string resource aus wid in str einlesen */

void getmessg(/*Widget wid, char *str*/);

/* Einlesen von numerischen Parametern aus einer parlist in 
 * zeichnung. (Automatische Typ-Differenzierung.
 * liest bis das Ende der parlist erreicht ist. Der zu lesende
 * Block muss deshalb am Ende der Liste zu finden sein!
 */


void getparams(/* TextFieldTabEl *parlist, struct hdata *zeichnung*/);

/* Flag status eines Checkbox-Widget lesen */
int getflag(/* Widget */);
void setflag(/* Widget wid; int value;*/);

/* Die Daten sollen nicht explizit aus dem Widget gelesen werden, sondern
 * automatisch auf den neusten Stand gebracht werden. Dies leistet dieser
 * Event-handler, der beim FocusOut Event eine Datenspeicherung einleitet.
 * Er wird durch init_TextField() installiert.
 */

void init_TextField(/*Widget wid, char* w_list*/);

void init_Canvas(/*Widget wid, char* w_list*/);


/* Aktivieren bzw. Desaktivieren aller Buttons in einer Buttonlist
 */

void BusyButtons(/* WidTabEl *buttonlist, Boolean busy */);

/* Aktivieren und desaktivieren eines Widget, umleiten des Focus
 * nach focuswidget
 */
void sens_desens(/* Widget wid, Boolean sensitive, Widget focuswidget*/);


/* Sichtbaren Ausschnitt so justieren, dass der Punkt (x,y) sichtbar ist */
void AdjustView(/*Widget canvas; Position *x,*y*/);

/* Teilnahme am wm_delete_window protokoll */
void deal_with_wmproto(/* Widget toplevel, Boolean quitNotPopdown*/);

/* Das TextField Widget aus dem Widget, in welchem
 * Keyboard-Events verarbeitet werden (Textedit),
 * ermitteln.
 */


Widget GetTxtFldWid(/* Widget wid */);

/* get the scrollable ancestor of canvas (widget set dependent) */
Widget GetScrollable(/*Widget wid*/);
/* get the clipping ancestor of canvas */
Widget GetClip(/*Widget wid*/);

/* this is called to notify (GUI-dependend) widgets that
 * something has changed (and probably settings in the GUI
 * have to be updated).
 */
void   NotifyStatusChanged(/*Zeichnung z*/);

/* some convenience macros */

#define NOTIFY_ON(z,bits) NotifyStatusChanged((z),(((z)->status)|((int)(bits))))
#define NOTIFY_OFF(z,bits) NotifyStatusChanged((z),(((z)->status)&~((int)(bits))))

#endif
