#ifndef PUNKTE_H
#define PUNKTE_H

#include <X11/Xlib.h>

#include "lists.h"

/* Verwaltung von Punkten und Polygonen durch verkettete
 * Listen
 */


/* ein einzelnes Element von koord */
typedef short  koord_el;
typedef XPoint koord;


typedef struct node {
 listel zeigers;
 koord ort,richtung;
 int   segs;
 float len;
 float speis;
 float ang;             /* inner angle of vertex                        */		
 void (*remove_proc)(); /* wird beim Entfernen eines Punktes aufgerufen */
 } Punkt;

#define NEXT(pktptr) ((Punkt*) ((pktptr)->zeigers).nextel)
#define LAST(pktptr) ((Punkt*) ((pktptr)->zeigers).prevel)

typedef struct {
 listel zeigers;
 liste  punkte;} Polygon;

#define NEXTPOLY(polyptr) ((Polygon*) ((polyptr)->zeigers).nextel)
#define LASTPOLY(polyptr) ((Polygon*) ((polyptr)->zeigers).prevel)

#define PLIST(polyptr)    ((Punkt *) ((polyptr)->punkte).firstel)
#define ENDE(polyptr)     ((Punkt *) ((polyptr)->punkte).lastel)

#define PLIST_SETNULL(polyptr)	\
	do { (polyptr)->punkte.firstel = 0 ; } while (0)
#define ENDE_SETNULL(polyptr)	\
	do { (polyptr)->punkte.lastel = 0 ; } while (0)


typedef liste Polygonliste;

typedef struct {
  listel zeigers;
  Polygon *selpoly;
  Punkt   *selected;} Selection;

typedef liste Selectionliste;

#define NEXTSEL(selptr)  ((Selection *) ((selptr)->zeigers).nextel)
#define LASTSEL(selptr)  ((Selection *) ((selptr)->zeigers).prevel)
#define SELPOLY(selptr)  (((selptr)->selpoly))
#define SELECTED(selptr) (((selptr)->selected))

/* einen Punkt an der Stelle sel->selected in das Polygon
 * sel->selpoly einf"ugen. Falls sel->selected == 0
 * ein neues Polygon erzeugen.
 * Die Selektion zeigt auf den neu erzeugten Punkt
 * Falls der Punkt nicht erzeugt werden kann, wird 1 zur"uckgegeben,
 * -- die Selektion bleibt erhalten.
 * Returnval: 0: normal, 1: Element konnte nicht erzeugt werden 
 * (von CreateEnqListEl), -1: -2: polist, oder sel == 0;
 */

int CreatePoint(/*polist,sel,neu*/);

/*
 * Polygonliste *polist;
 * Selection    *sel;
 * Punkt       **neu;
 */


/* Punkt aus Polygon l"oschen; es MUSS daf"ur gesorgt werden,
 * dass zugeh"orige Selektionen anderweitig gel"oscht
 * werden!!
 * ist der undel-Puffer == 0, so wird der Punkt 'hart'
 * gel"oscht. 
 * Falls es sich um den letzten Punkt in einem Polygon handelt,
 * wird dieses auch aus anker entfernt.
 */

int RemovePoint(/*undel,anker,polygon,punkt*/);

/*
 * UndelBuffer  *undel;
 * Polygonliste *anker;
 * Polygon      **polygon;
 * Punkt        **punkt;
 */


/* Polygon l"oschen; es MUSS daf"ur gesorgt werden,
 * dass zugeh"orige Selektionen anderweitig gel"oscht
 * werden!!
 * Return-val: siehe RemovePoint()
 */

int RemovePolygon(/*undel,anker,poly*/);
/*
 * UndelBuffer  *undel;
 * Polygonliste *anker;
 * Polygon      **poly;
 */

/* Richtungsvektor und L"ange einer Kante eintragen */

void Richtung(/* Polygon *poly; Punkt *punkt;*/);

/* Richtungsvektoren, L"angen einer kante und ihres Vorg"angers
 * anpassen, segmentzahl auf null setzen
 */

void CorrRicht(/* Polygon *poly; Punkt *punkt;*/);

/* Die Richtungsvektoren aller Punkte einer
 * Polygonliste eintragen
 */
void ClosePath(/* Polygonliste * list*/);

/* Ist ein punkt ein Stift? */
#define ISSTIFT(punkt)((punkt) && (NEXT(punkt)==LAST(punkt)))
#endif
