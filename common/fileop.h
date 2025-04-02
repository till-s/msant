#ifndef FILEOP_H
#define FILEOP_H

#include <stdio.h>
#include <widglist.h>

/* Parameter und Punkte aus feil in 
 * inputlist data->borders einlesen.
 */
void PunkteLesen(FILE *feil, TextFieldTabEl *inputlist, WidTabEl *flaglist);

/* Parameter und Punkte int feil
 * schreiben.
 */
void PunkteSchreiben(FILE *feil, TextFieldTabEl *inputlist, WidTabEl *flaglist);

#endif

