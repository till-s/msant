#ifndef CBSTROM_H
#define CBSTROM_H

#include "basics.h"
/* exportierte Prozeduren */


void Strom();
void strominvalid();
void CBExpStrom();
void Kopp();
void koppinvalid();
void drawstrom();
Polygon *getSpeisePoly(Zeichnung p1);
float    getShortestSeg(Zeichnung p1);

/* exportierte Variablen */

extern complex H0tab[],H1tab[],Gtab[];

#endif
