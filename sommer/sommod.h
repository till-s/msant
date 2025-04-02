#ifndef SOMMOD_H
#define SOMMOD_H

/* Oeffentliche Deklarationen */

typedef struct diel_ *diel;

/* berechnete Daten ausdrucken */
void print_diel(/* diel dp */);

/* Dielektrikum erzeugen */
diel init_diel(/* real kD, er */);

/* Dielektrikum vernichten */
void destroy_diel(/* diel dp */);

/* Nahkopplung */
void somkop_nahe(/*u,c,dp,kw,term1,term2*/);
/*
  real u,c,kw;
  diel dp;
  complex *term1,*term2;
*/

#endif
