#ifndef FFT_H
#define FFT_H

#include "util.h"

#define OK 0
#define NO_MEM 1

/* FFT einer 1D Matrix f der Dimension 2^^pot
 * in den Array F (f==F ist m"oglich).
 * In W werden die Drehfaktoren "ubergeben, welche
 * durch fft_init() initialisiert werden m"ussen.
 * Wird W==0 "ubergeben, besorgt fft() das Allozieren
 * und Initialisieren selber.
 *
 * Der Returnwert entspricht einem Fehlercode
 * 
 * fft_init() kann die Drehfaktoren in selber allozierten
 * Speicher (W==0), oder in bereitgestellten Speicher
 * (sizeof(complex)*2^^(N-1)) ablegen.
 * 
 * Der urspr"unglich vorgesehene Trick mit den Drehfaktoren
 *  a<2Pi/N funktioniert nicht!
 */


complex *fft_init(/*pot,W*/);
 /* int     pot;
  * complex *W;
  */

int fft(/*pot,F,f,W*/);
 /* int     pot;
  * complex *F,*f;
  * complex *W;
  */

#endif
