#ifndef NAHKOP_H
#define NAHKOP_H

/* Maximaler Abstand (k*r), f"ur den die Nahkopplung noch
 * anwendbar ist 
 */
#define NAHKOP_MAX 0.9
#define NAHKOP_GRENZE NAHKOP_MAX

#define FREQLIM 1.0

#ifndef NAHKOP_GRENZE
extern  float grenz;
#endif

/* Bei der Feldberechnung werden Aufpunkte, welche n"aher als
 * 0.05 Wellenzahlen von einem Quellpunkt liegen, als Randpunkte
 * betrachtet, und die (schon bekannten) Randwerte eingesetzt 
 */
#define RANDPUNKT     0.02

/* 'probe' integral around field/test point.
 *
 *   lim  integrate( dYo(kr)/dn r dphi, {phi=0..2Pi} )
 *   r->0
 *
 * for Psi = Yo 
 */
#define ZEIGEN_0_Y0	4.

/* self-coupling (limit where field/test point approaches source point) */
#define ZEIGEN_Y0  (4.-2.)


/* Nahkopplung unter dem Patch 
 */

/* In s"amtlichen zu berechnenden Integralen h"angen die Integranden
 * vom Betrag des Abstandsvektors
 *
 *   | k Rij | = k rij
 *
 *  (Vektoren weden Gross, Skalare klein geschrieben!)
 *
 * und vom Einheitsvektor Edelij in Richtung der Verbindungslinie zwischen
 * Punkt i und j ab. Bei Integration l"angs der g e r a d e n Linie
 * mit Richtungsvektor Egi sind nur rij und Edelij von der Integrations-
 * variablen  kt ( mit Wellenzahl skalierter Kurvenparameter ) ab.
 *
 * Es finden nun durchwegs die Substitutionen
 * 
 * u=k Rijo.Egi + kt = uo + kt
 *
 * c=k Rijo.Eni
 *
 * Verwendung. Rijo der Vektor vom Aufpunkt nach dem Mittelpunkt
 * des Quellsegments, welches mit i indiziert ist. Egi und Eni sind
 * Einheitsvektoren tangential, bzw. normal ("aussere Normale) zum
 * Quellsegment.
 *
 * --> krij = Sqrt[ u^2 + c^2 + kz^2]
 *
 * --> Edelij = (u Egi + c Eni + Ez kz)/krij
 *
 * Ez kz ist die z-Komponente (Integration "uber Segmentdicke)
 *
 */




/* N"aherung f"ur das Integral
 *
 * 1/kw Integrate[ BesselY[0,krij], {kt,-kw/2,kw/2}]
 *
 * =
 *
 * 1/kw Integrate[ BesselY[0,krij], {u,-kw/2,+kw/2}]
 *
 */

void eigenkop_innen(float kw, float *speise);


/* N"aherungen f"ur die Integrale
 *
 *
 * koppel= Integrate[ Edelij.Eni BesselY[1, krij], {kt,-kw/2,kw/2}]
 *       = Integrate[ c BesselY[1,krij]/krij, {u,uo-kw/2,uo+kw/2}]
 *
 * speise= 1/kw Integrate[ BesselY[0,krij], {u,uo-kw/2,uo+kw/2}]
 *
 */

void nahkop_innen(float u, float c, float kw, float *pkoppel, float *pspeise);

/* Electrostatic versions (g(r) = ln(r)) */
#define ZEIGEN_0_LN	(2.*PI)

/* self-coupling (limit where field/test point approaches source point) */
#define ZEIGEN_LN  (ZEIGEN_0_LN - PI)


void eigenkop_innen_static(float kw, float *speise);
void nahkop_innen_static(float u, float c, float kw, float *pkoppel, float *pspeise);

/* contribution of uniform charge at i to field at j
 *
 *  (integrate // log(r) qs(x,y) dx dy)
 *             A
 */
double
uniform_chrg_static(double rij, double sig);

/* contribution of uniform charge at i to field at j
 *
 *  (integrate // Yo(kr) qs(x,y) dx dy)
 */
double
uniform_chrg(double rij, double sig);

/* N"aherung f"ur das Integral
 *
 *
 * 1/kw Integrate[ I G(krij) ((Egj.Egi)(1+a) - 
 *
 *                            (Edelij.Egi)(Edelij.Egj)(1+3a)),
 *
 *                {kz,-kh/2,kh/2},{u,uo-kw/2,uo+kw/2}]
 *
 *
 *
 * = 1/kw Integrate[ I G(krij) ((Egj.Egi)( (1+a) - u^2/krij^2 (1+3a) )),
 *
 *                {kz,-kh/2,kh/2},{u,uo-kw/2,uo+kw/2}]
 *
 * - 1/kw Integrate[ I G(krij) ((Egj.Eni) c u /krij^2)
 *  
 *                {kz,-kh/2,kh/2},{u,uo-kw/2,uo+kw/2}]
 *
 * = Egj.Egi term1 + Egj.Eni term2
 *
 * mit G(krij) = E^(-I krij) / krij
 * 
 *     a       = -I/krij - 1/krij^2
 * 
 * term1= 1/kw   Integrate[ I G(krij) ((1+a) - (1+3a) u^2/krij^2),
 *
 *                {kz,-kh/2,kh/2},{u,uo-kw/2,uo+kw/2}]
 *
 * term2= - 1/kw Integrate[ I G(krij) (1+3a) c u /krij^2, 
 *
 *                {kz,-kh/2,kh/2},{u,uo-kw/2,uo+kw/2}]
 */

void nahkop_aussen(/* u,c,kh,kw,term1,term2*/);

  /* float u,c,kh,kw;
     complex *term1,*term2;
   */

/* ruft nahkop_aussen auf */

void eigenkop_aussen( /*kh,kw,reteil,imteil*/);

/* float kh,kw,*reteil,*imteil;
 */

#endif
