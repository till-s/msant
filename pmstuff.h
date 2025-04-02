#ifndef PMSTUFF_H
#define PMSTUFF_H

#include "pan.h"

/*
 * set the background of the panner associated with canvas
 * to the clean pixmap
 */

void   PmSetCleanPixmap(/*Widget canvas*/);

/*
 * command codes to determine which pixmap to draw;
 * (passed as client_data, when registering the callback)
 */
#define PM_NEW_GRAF_PIXMAP	((XtPointer)1)
#define PM_NEW_STROM_PIXMAP	((XtPointer)2)
void   PmNewPixmap(/*Widget canvas, client_data, Zeichnung z*/);
#endif
