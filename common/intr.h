#ifndef INTR_h
#define INTR_h

/* Periodische unterbrechung von Berechnungen, um die Event
 * Queue abzuarbeiten!
 */

/* Durch Setzen der Variablen critical geschieht beim
 * Timeout nichts, ausser dass interrupted incrementiert wird
 * Dadurch kann das Timeout verz"ogert behandelt werden.
 */


/* Periodischen Interrupt (sec, mikrosec) einschalten,
 * Ein ein Dialogfenster als Tochter von wid erscheint
 * mit der "Uberschrift message
 *
 * bei Abbruch im Dialogfenster wird kehrt enable_interrupt
 * mit dem dem gedr"uckten Button entsprechneden Wert 
 * zum Environment env zur"uck
 * (analog longjmp)
 *
 */

void enable_interrupt(/* long interval_sec, interval_usec,
                         Widget wid ,jmp_buf env, char * message*/);
 
/* Periodischen Interrupt ausschalten */

void disable_interrupt();

/* interrupts maskieren */

void block();

/* interrupt wieder erlauben; falls sync!=0 werden in der Zwischen-
 * zeit angefallene interrupts nachgeholt
 */

void unblock(/* int sync */);

#endif
