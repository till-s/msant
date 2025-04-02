#ifndef SOMMOD_H
#define SOMMOD_H

#include "../common/basics.h"

/* Oeffentliche Deklarationen */

typedef struct diel_ *Diel;

/* Zeiger auf Gtab holen */
complex *dielGtab(/*Diel dp*/);

/* Zeiger auf G1tab holen */
complex *dielG1tab(/*Diel dp*/);

/* Anz. Eintr"age in Gtab */
int dielGtabSize(/*Diel dp*/);

/* berechnete Daten ausdrucken */
void print_diel(/* Diel dp */);

/* Dielektrikum erzeugen */
Diel init_diel(/* float kD, er */);

/* Dielektrikum vernichten */
void destroy_diel(/* Diel dp */);

/* Nahkopplung */
void somkop_nahe(/*u,c,dp,kw,term1,term2*/);
/*
  float u,c,kw;
  Diel dp;
  complex *term1,*term2;
*/

/* Wegintegration */
int alloc_work(/*int neqs, float **rwork, int **iwork, char *extension*/);

/* Wenn der Integrator einen Fehler meldet ist der R"uckgabewert
 * !=0 und ein Fehlercode wird in 'error' "ubergeben
 */

int pathint(/*diel,y,krho,neqs,tend,alf,rwork,iwork,error*/);
/*
  Diel	  diel;
  float   *y,krho,tend;
  complex alf;
  int     neqs;
  float	  *rwork;
  int	  *iwork;
  int	  *error;
*/

/* Oberfl"achenwellenanteil im Fernfeld berechnen
 * in 'result' werden 2 komplexe Koeffizienten abgelegt,
 * aus denen das H-Feld berechnet werden kann.
 *
 * ->			       ->                    ->
 * H(krho) = faktor ( I1(krho) ex + I2(krho) (x/rho) erho ) jmagn
 *
 * mit faktor = ko/(j Zo) 1/(4 Pi)
 *
 * und 	result[0]=Re{I1}, result[1]=Im{I1},
 *	result[2]=Re{I2}, result[3]=Im{I2}
 */
void SFW(/* float *result, Diel diel, float krho */);

/* add 'size' new entries to Gtab/G1tab of 'dp'
 * 'krho_proc' has to be supplied to map table_index to
 * krho values.
 */

int dielGetChunk(/* dp, size, krho_proc*/);
/*
   Diel dp;
   int  size;
   float (*krho_proc)(int);
*/

#endif
