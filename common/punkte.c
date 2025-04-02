#include <math.h>
#include <stdio.h>

#include "Malloc.h"
#include "punkte.h"
#include "basics.h"

void DestroyPolygon();

#define DestroyPoint Free





/* Ein Polygon erzeugen, und hinter dem selektierten
 * Polygon einf"ugen. falls kein solches existiert, gem"ass
 * der "ublichen Semantik als erstes Listenelement einf"ugen
 * Das selektierte Polygon zeigt auf das neu erzeugte!
 * Die selektierte Kante ist gleich NULL!
 */

int CreatePolygon(polist,sel,neu)

Polygonliste *polist;
Selection    *sel;
Polygon      **neu;

{
int rval;
if (polist==0) {return(-1);}
if (sel==0) {return(-2);}
if (0!=(rval=CreateEnqListEl(polist,sel->selpoly,neu,sizeof(Polygon))))
 return(rval);
/*neues Element initialisieren*/
PLIST_SETNULL(*neu);
ENDE_SETNULL(*neu);
sel->selpoly=*neu;
sel->selected=0;
return(0);
}

/* einen Punkt an der Stelle sel->selected in das Polygon
 * sel->selpoly einf"ugen. Falls sel->selected == 0
 * ein neues Polygon erzeugen.
 * Die Selektion zeigt auf den neu erzeugten Punkt
 * Falls der Punkt nicht erzeugt werden kann, wird 1 zur"uckgegeben,
 * -- die Selektion bleibt erhalten.
 */

int CreatePoint(polist,sel,neu)


Polygonliste *polist;
Selection    *sel;
Punkt       **neu;

{
int rval;
if (polist==0) {return(-1);}
if (sel==0) {return(-1);}

if (sel->selected==0) {
  if (0!=(rval=CreatePolygon(polist,sel,&sel->selpoly))){
   return(rval);
  }
  sel->selected=ENDE(sel->selpoly);
}

if (0!=(rval=CreateEnqListEl(&sel->selpoly->punkte,
		sel->selected,neu,sizeof(Punkt)))){
 return(rval);
}
sel->selected=*neu;
(*neu)->segs=0.;
(*neu)->len=0.;
(*neu)->speis=0.;
(*neu)->remove_proc=0;
(*neu)->ang=0.;

return(0);
}


void DestroyPolygon(poly)

Polygon * poly;

{
register Punkt *point,*tmp;

point=PLIST(poly); 
while (point) {
 tmp=point; point=NEXT(point);
 Free(tmp);
}
Free(poly);
}

/* Punkt l"oschen; es MUSS daf"ur gesorgt werden,
 * dass zugeh"orige Selektionen anderweitig gel"oscht
 * werden!!
 */

int RemovePoint(undel,anker,polygon,punkt)

UndelBuffer  *undel;
Polygonliste *anker;
Polygon      **polygon;
Punkt        **punkt;

{
int rval;

/* Entfernen-Prozedur aufrufen */

if ((*punkt)->remove_proc) (*punkt)->remove_proc(*punkt);


if (PLIST(*polygon)==ENDE(*polygon)) {
 /* nur ein Element in Polygon; ganzes Polygon
  * entfernen.
  */
 *punkt=0;
 return(RemovePolygon(undel,anker,polygon));
}else{
 rval=RemoveToUndel(undel,&((*polygon)->punkte.firstel),punkt,
                    CorrRicht,*polygon,DestroyPoint);

/* gcc complained this:
 rval=RemoveToUndel(undel,&PLIST(*polygon),punkt,
                    CorrRicht,*polygon,DestroyPoint);
*/

 /* Anpassen der Richtungsvektoren */
 CorrRicht(*polygon,*punkt);
 return(rval);
}
}

/* Polygon l"oschen; es MUSS daf"ur gesorgt werden,
 * dass zugeh"orige Selektionen anderweitig gel"oscht
 * werden!!
 */

int RemovePolygon(undel,anker,poly)

UndelBuffer  *undel;
Polygonliste *anker;
Polygon      **poly;

{
return(RemoveToUndel(undel,anker,poly,(void (*)())0,
                     (char*)0,DestroyPolygon));
}


/* Den Richtungsvektor eines Punktes eintragen */

void Richtung(poly,ptr)

Polygon     *poly;
Punkt        *ptr;

{
  if(NEXT(ptr) == 0) {
    if (LAST(ptr)==0) { /* Stift! */
      ptr->richtung.x=ptr->richtung.y=0;
      return;
    }
    ptr->richtung.x=PLIST(poly)->ort.x - ptr->ort.x;
    ptr->richtung.y=PLIST(poly)->ort.y - ptr->ort.y;
    }
   else {
    ptr->richtung.x=NEXT(ptr)->ort.x - ptr->ort.x;
    ptr->richtung.y=NEXT(ptr)->ort.y - ptr->ort.y;
    }

  ptr->len=
     hypot((double)ptr->richtung.x,(double)ptr->richtung.y);
}

void dumpWink(Polygon *border)
{
	Polygon *l;
	Punkt   *p;

	while ( (l=LASTPOLY(border)) ) {
		border = l;
	}

	for (; border; border=NEXTPOLY(border)) {
		for (p=PLIST(border); p; p=NEXT(p)) {
			printf("X: %i, Y: %i, angle: %g\n",p->ort.x,-p->ort.y,p->ang/PI*180.);
		}
		printf("***********\n");
	}
}
/* Winkel eines Punktes eintragen */
void Winkel(poly,ptr)

Polygon     *poly;
Punkt        *ptr;

{
Punkt *pn,*pp;
koord r2,r1;
koord d; /* so we get the same type */

	if( (pn=NEXT(ptr)) == 0 ) {
		if (LAST(ptr)==0) { /* Stift oder letzter Punkt! */
			ptr->ang = 0.;
			return;
		}
		pn = PLIST(poly);
	}
	if ( (pp=LAST(ptr))==0 ) {
		pp = ENDE(poly);
  	}
	r2.x = pn->ort.x - ptr->ort.x;
	r2.y = - (pn->ort.y - ptr->ort.y); /* XWindows koordinaten sind gespiegelt */
	r1.x = ptr->ort.x - pp->ort.x;
	r1.y = - (ptr->ort.y - pp->ort.y);

	/* assume we're always on the outside of polygons with two edges */
	if ( (d.x = r2.y*r1.x - r1.y*r2.x) == 0 )
		ptr->ang = PI+PI;
	else
		ptr->ang = PI - atan2(d.x, r1.y*r2.y+r1.x*r2.x);
}

/* Die Richtungsvektoren eines Punktes, und seines
 * Vorg"angers anpassen.
 * Winkel eines Punktes sowie des Vorg"angers und Nachfolgers anpassen.
 */

void CorrRicht(poly,ptr)

Polygon     *poly;
Punkt       *ptr;

{
Punkt *pp, *pn;

	Richtung(poly,ptr);
	ptr->segs=0;
	Winkel(poly,ptr);

	if ( ! (pp=LAST(ptr)) ) {
		/* ein Element ist sicher vorhanden! */
		pp = ENDE(poly);
	}
	Richtung(poly,pp);
	pp->segs=0;
	Winkel(poly,pp);

	if ( ! (pn=NEXT(ptr)) ) {
		pn = PLIST(poly);
	}
	Winkel(poly,pn);
	dumpWink(poly);
}


/* Die Richtungsvektoren aller Punkte einer
 * Polygonliste eintragen
 */
void ClosePath(list)

Polygonliste *list;

{
 register Punkt *ptr;
 register Polygon *border;

 border=(Polygon*)list->firstel;

 while (border) {
  ptr=PLIST(border);
  while(ptr!=0) {
   Richtung(border,ptr);
   Winkel(border,ptr);
   ptr=NEXT(ptr);
  }
 border=NEXTPOLY(border);
}
  
	dumpWink((Polygon*)list->firstel);
}
