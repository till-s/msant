#include <math.h>
#include "basics.h"
#include "nahkop.h"

#ifndef NAHKOP_GRENZE
float grenz=NAHKOP_MAX;
#endif

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

void eigenkop_innen(kw,speise)

float kw, *speise;

{
 register double kw2=kw*kw;
 *speise= -1.151695267781572 + 0.02841462205355762*kw2
         + (0.6366197723675814 - 0.01326291192432461*kw2)*log(kw);
}

void eigenkop_aussen(kh,kw,reteil,imteil)

float kh,kw,*reteil,*imteil;

{
 complex dummy,res;
/*
 diag=hypot(kh,kw);
 *reteil= kh*kw*2./3.;
 *imteil= kh * (- 8./kw/diag 
          +2.*asinh(kw/kh))
          +2.*asinh(kh/kw);
 */
 nahkop_aussen((double)0.,(double)0.,kh,kw,&res,&dummy);
 *reteil=res.re;
 *imteil=res.im;
}


/* N"aherungen f"ur die Integrale
 *  (der Vektor delij zeigt von Aufpunkt zu Quellpunkt, d.h. von j nach i)
 *
 * koppel= Integrate[ Edelij.Eni BesselY[1, krij], {kt,-kw/2,kw/2}]
 *       = Integrate[ c BesselY[1,krij]/krij, {u,uo-kw/2,uo+kw/2}]
 *
 * speise= 1/kw Integrate[ BesselY[0,krij], {u,uo-kw/2,uo+kw/2}]
 *
 */

void nahkop_innen(uo,co,kw,koppel,speise)

float uo,co,kw,*koppel,*speise;

{
register double c,u,delarctang,logar,u2,c2,resk,ress;


c=fabs(co);  

/* c kommt nur im atan vor. da ist es durch integration von
 *  
 *  1/(c^2 + u^2) 
 *
 * nach u hingekommen. Die Stammfunktion ist
 * 
 * atan(u/sqrt(c^2)); die Wurzel ist mit pos. Vorz. zu nehmen
 *
 * am Ende ist das Vorzeichen von c, welches das Vorzeichen des Integranden
 * bestimmt, auf das Integral zu "ubertragen!
 */

u=uo+kw/2.; /* Segmentende */


/* Auswertung des Arctan am oberen - unteren Intervallende */
u2=u*u; c2=c*c;

delarctang=atan2pi( c*kw, c2+uo*uo-kw*kw/4.);

logar  =log(u2+c2);

/* Dies f"ur die Koppelterme ( Y1 edel.en ) */

 resk  = (-delarctang*(2.-c2*(1.-c2/12.))+ 
  c*u*(logar*(0.159155 - 0.0198944 * c2 - 0.00663146 * u2)
        -0.514367 + 0.0808745 * c2 +0.0225372 * u2));
       
/* Dies f"ur die Speiseterme ( Y0 ) */

 ress = (delarctang*c*(2.-c2/3.)+
  u*(logar*(0.31831 - 0.0265258*(3.*c2+u2))
     -0.710424 + 0.283709 * c2 + 0.0768859 * u2));

/* Auswertung am untern Ende */
u-=kw; u2=u*u; /* Segmentanfang */
logar  =log(u2+c2);


*koppel= copysign(1.,co) * (
        resk - ( /* die Differenz der ArcTans wurde oben berechnet */
       c*u*(logar*(0.159155 - 0.0198944 * c2 - 0.00663146 * u2)
        -0.514367 + 0.0808745 * c2 +0.0225372 * u2)));
 
*speise  = (ress - ( /* die Differenz der ArcTans wurde oben berechnet */
  u*(logar*(0.31831 - 0.0265258*(3.*c2+u2))
     -0.710424 + 0.283709 * c2 + 0.0768859 * u2)))/kw;
}

void eigenkop_innen_static(w,speise)

float w, *speise;

{
/*
float dummy;
	nahkop_innen_static(0.,0.,w,&dummy,speise);
 */
 *speise = log(w/2.)-1.;
}

/* Compute the integrals
 *
 *               uo+w/2                         uo+w/2
 *    Koppel = - /        d ln(r)/dn   ds  = -  /     c/r^2  du
 *              uo-w/2                         u0-w/2
 *
 * NOTE: minus sign is so the caller can use the same
 *       signs for Bessel and Log green's functions:
 *
 *   4 Vp = 1/kw / Yo (-Ib) dks -  / Y1 c/r V dks
 *
 * 2pi Vp = 1/w / ln(r) (-Qb) ds - / - c/r^2 V ds
 *
 *                    ---------------^
 *
 * (Ib is the *outward* current; the caller actually uses
 * the *inward*-directed current)
 *
 *
 *             1  uo+w/2  
 *   Speise = --- /      ln(r) du
 *             w uo-w/2
 *
 */

void nahkop_innen_static(uo,c,w,koppel,speise)

float uo,c,w,*koppel,*speise;

{
register double u,delarctang,logar,u2,c2,ress;

u=uo+w/2.; /* Segmentende */

/* Auswertung des Arctan am oberen - unteren Intervallende */
u2=u*u; c2=c*c;

delarctang=atan2( c*w, c2+uo*uo-w*w/4.);

logar  =log(u2+c2);

/* Dies f"ur die Koppelterme ( Y1 edel.en ) */

/* see above for '-' sign here */
 *koppel = - delarctang;

/* Dies f"ur die Speiseterme ( Y0 ) */

 ress = delarctang*c + u*(logar*0.5 - 1.0);

/* Auswertung am untern Ende */
u-=w; u2=u*u; /* Segmentanfang */
logar  =log(u2+c2);

*speise  = (ress - /* die Differenz der ArcTans wurde oben berechnet */
  u*(logar*0.5-1.0))/w;
}

/* contribution of uniform charge at i to field at j
 *
 *  (integrate // log(r) qs(x,y) dx dy)
 *             A
 */
double
uniform_chrg_static(double rij, double sig)
{
double v2;
	if ( rij > sig )
		return log(rij);

	v2 = rij/sig;
	v2 = v2*v2;
	return ((v2-1.)*0.5 + log(sig));
}

/* contribution of uniform charge at i to field at j
 *
 *  (integrate // Yo(kr) qs(x,y) dx dy)
 */
double
uniform_chrg(double krij, double ksig)
{
double v2;

	if ( krij > ksig )
		return y0(krij);

	v2 = krij/ksig;
	v2 = v2*v2;
	/*
	 * Approximation: constant charge distribution with radius 'sig'
	 *                (gaussian is more complicated). The goal is achieving
	 *                a finite field if we travel through the charge distribution.
	 *
	 * Series expansion of 
	uim += 2.*(j0(krij)*(y1(krho) - y1(krij)*v) + j1(krij)*y0(krij)*v)/karho*SPEISESTROM;

	 C = (-1+2*EulerGamma-2*log(2))/2/%pi = -0.1960570906
	 */

	return 2.*(-0.1960570906*(1-v2) + (log(ksig) - log(krij)*v2)/PI ) + v2*y0(krij);
}


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
 * - 1/kw Integrate[ I G(krij) ((Egj.Eni) c u /krij^2 (1+3a))
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


void nahkop_aussen(uo,c,kh,kw,term1,term2)

 float uo,c,kh,kw;
 complex *term1, *term2;

{
double wa,we,ua,ue,logarhe,logarha,kh2,logaru,arctang,ue2,ua2,c2,c4;
double res1im,res2im,we2,wa2,koeff,term,koeff2;

term2->re=term2->im=c/kw;
c=fabs(c);  /* siehe Kommentar bei nahkop_innen */

ue=uo+kw/2.; ua=ue-kw;
ue2=ue*ue; ua2=ua*ua;
c2=c*c; c4=c2*c2; kh2=kh*kh;


we2=  kh2 + 4.*c2;
wa2=  we2 + 4.*ua2;
we2+= 4.*ue2;
wa= sqrt(wa2);
we= sqrt(we2);


arctang=atan2(c*kh*(ue*wa-ua*we), c2*we*wa+kh2*ue*ua);

koeff =1./2. - 3./16.*c2 + 5./384.*c4;
koeff2=1./2. - 1./16.*c2;

/* Probleme bei kleinen kh 
 * logaru=log((2.*ue+we)/(2.*ua+wa));
 */

logaru=asinh(2.*(ue*wa-ua*we)/(4.*c2+kh2));
logarhe=log1p(-2.*kh/(we+kh));
logarha=log1p(-2.*kh/(wa+kh));


term = 2/ (we*(ue2+c2));

term1->re=kh*(   (2./3. - 2./15.*c2 - 1./90.*kh2)
           - (1./45. + 1./630.*c2) * (ue*ue2 - ua*ua2)/kw);

res1im= kh*(  ( 1. - 1./4.*c2 + 5./384.*c4) * logaru 
        + ue*we*( we2 + 11.*c2 - 120.)/2304.
        - ue*term )
        - c*( 2. - 1./3.*c2 + 1./72.*c4) * arctang
        - ue * (koeff - 1./48.*ue2 + 5./576.*c2*ue2) * logarhe;

term2->re*=kh*((14. - c2)*(ue2-ua2) - (ue2*ue2-ua2*ua2)/2.)/420.; /* schon alles (ue2-ua2) */

res2im=kh*( 1./32.*we 
            - term)
        + (koeff2 - 1./16.*ue2)*logarhe;

/*Auswertung am Segmentanfang*/


term = 2./(wa*(ua2+c2));
term1->im=(res1im - ( kh * (
                         ua*wa*( wa2 + 11.*c2 - 120.)/2304.
                       - ua*term)
           - ua * (koeff - 1./48.*ua2 + 5./576.*c2*ua2) * logarha
                   ))/kw;

term2->im*=(res2im - ( kh* (
                         1./32.*wa 
                         - term )
           + (koeff2 - 1./16.*ua2)*logarha
                   ));


}

#ifdef NEVER
/* N"aherung f"ur das Integral
 *
 *
 *  Integrate[ Edelij.Eni I G(krij) (1 - I/krij), {u,uo-kwi/2,uo+kwi/2},{kz,-kh,kh}]
 *
 *  = Integrate[ kL*Rdelij.Eni/krij I G(krij) (1-I/krij) ...
 *  = Integrate[ c/krij I G(krij) (1-I/krij) ...
 *
 * 
 */

void nahkopE_aussen(uo,c,kh,kw,res)
 float uo,c,kh,kw;
 complex *res;

{
double cabs,wa,we,ua,ue,kh2,arctang,ue2,ua2,c2;
double resim,res2im,we2,wa2,koeff,term,koeff2;

cabs=fabs(c);  /* siehe Kommentar bei nahkop_innen */

res->im=c*kw*kh/2.;
ue=uo+kw/2.; ua=ue-kw;
ue2=ue*ue; ua2=ua*ua;
c2=cabs*cabs; kh2=kh*kh;


we2=  kh2 + c2;
wa2=  we2 + ua2;
we2+= ue2;
wa= sqrt(wa2);
we= sqrt(we2);


arctang=atan2(cabs*kh*(ue*wa-ua*we), c2*we*wa+kh2*ue*ua);
resim=  -(ue*asinh(kh/sqrt(ue2+c2)) - ua*asinh(kh/sqrt(ua2+c2)));
resim-=  (kh*asinh(ue*wa-ua*we));
resim+=  (cabs-2./cabs)*arctang;
resim*=c/2.;
res->im=resim;
}

void eigenkopE_aussen(kh,kw,res)
 float kh,kw,*res;
{
 *res=PI;
}
#endif
