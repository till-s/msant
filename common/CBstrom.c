#include <math.h>
#include <stdio.h>
#include <setjmp.h>
#include <assert.h>

/*
#define DIREKT
*/

#include "Malloc.h"

#include "basics.h"
#include "widglist.h"

#include "Functab.h"
#include "nahkop.h"

#include "CBstrom.h"
#include "feld.h"

#include "intr.h"

#include "msant_strings.h"


/* Fortran Importe (IMSL ua.) */
void l2nrg_();
void l2tcg_();
void lfscg_();
void cgefs_();
void sgefs_();

static Boolean errorcheck();

/*********************************
 *
 * Folgende Compilerflags k"onnen definiert werden:
 *
 *   DIREKT  : direkte L"osung des Problems   ([K] + j fak1fakL [Zi] [Ya]) U] = j fak1 iq]
 *            : sonst iterative L"osung, momentane Implementierung konvergiert aber nicht!
 *     												   -1
 *   VORMULT  : (nur wenn auch DIREKT definiert) ([Yi] + fakL[Ya]) U] = iq]  mit [Yi]=-j/fak1 [Zi]   [K]
 *
 *   NEVER    : darf nie definiert werden, dient zur Auskommentierung
 *
 *   NAHKOP_GRENZ: Abstand (kr), ab welchem die Integrationen approximativ durchgef"uhrt werden; sollte
 *		   nur zusammen mit den Approximationsfunktionen (nahkop.c) ver"andert werden. Falls dieses
 *		   Flag nicht definiert ist, wird (zu Testzwecken) die Variable 'grenz' herangezogen.
 *		   
 *
 *********************************
 */

/* globale Variablen */

	/* R"ucksprung nach Abbruch von Berechnungen */
static jmp_buf env;	

	/* Funktionstabellen */
complex H1tab[MAXTAB],H0tab[MAXTAB],Gtab[MAXTAB],G1tab[MAXTAB];
static int initflag=-1;

static int spspeis = 0;

/* Tabelleninterpolation , in Functab.h definiert

#define INTERPOL(mat,tab,kr) {\
  register int index; register float delta=kr*TABSEP;\
  index=(int)delta; delta-=index;\
  mat.re=(tab[index].re*(1-(delta))+ tab[index+1].re*delta);\
  mat.im=(tab[index].im*(1-(delta))+ tab[index+1].im*delta);\
 }

*/


/* Vorw"artsdeklarationen */
static void calc_jelek();

#define CLOSEINDEX 1
#define FARINDEX   0

#ifndef DIREKT
static void rMatVek();
static void cMatVek();
#endif

Polygon *getSpeisePoly(Zeichnung p1)
{
Polygon *borderi;
Punkt   *ptri;

	if ( !p1->speiseseg )
		return 0;

	for (borderi=(Polygon*)p1->borders.firstel;
			borderi; borderi=NEXTPOLY(borderi)) {

		for (ptri=PLIST(borderi);ptri;ptri=NEXT(ptri)) {

			if (ptri==p1->speiseseg)
				return borderi;
		}
	}
	return 0;
}

float getShortestSeg(Zeichnung p1)
{
Polygon *borderi;
Punkt   *ptri;
float   min = -1.0, l;

	for (borderi=(Polygon*)p1->borders.firstel;
			borderi; borderi=NEXTPOLY(borderi)) {

		for (ptri=PLIST(borderi);ptri;ptri=NEXT(ptri)) {
			if ( ISSTIFT(ptri) )
				continue;
			/* not segmented ? */
			if ( 0 == ptri->segs )
				return -1.;
			l = ptri->len / (float)ptri->segs;
			if ( l < min || min < 0. )
				min = l;
		}
	}
	return min;
}

int	countSegs(Widget wid, Zeichnung p1, int *pspeiseindex, int *pkoppflag)
{
int dim,j,ignore;
Polygon *borderi;
Punkt   *ptri;
static int ignorestiftwarn=0;
char   stri[100];

	dim    = 0;
	ignore = 0;
	/*  Abz"ahlen der Segmente */

	for (borderi=(Polygon*)p1->borders.firstel;
			borderi; borderi=NEXTPOLY(borderi)){

		ptri=PLIST(borderi);

		for (ptri=PLIST(borderi);ptri;ptri=NEXT(ptri)) {

			if (ptri==p1->speiseseg) {
				if ( ISSTIFT(ptri) && p1->freq <= FREQLIM ) {
					*pspeiseindex = 0xdeadbeef;
		continue;
				} else {
					*pspeiseindex=dim + (int)(ptri->speis * ptri->segs);
				}
			} else {
				if (ISSTIFT(ptri)) {
					if (!ignorestiftwarn) {
						ignorestiftwarn=(int)Notice(wid,0,
								mOnlyOneExcit,
								mAckCont,0,
								mNoMoreWarn,1,
								0);
					}
		continue;
				}
			}

			if (ptri->segs==0) {
				sprintf(stri,fNotSegmented,j);
				printmessg(1,stri,wid);
				return -1;
			}

			if (!ignore && *pkoppflag &&!ISSTIFT(ptri) && (ptri->len/ptri->segs/p1->fak/p1->hsubst < 1.)) {
				sprintf(stri,
						fWrnH_GT_len,
						j);

				switch ((int)Notice(wid,0,stri,mAckCancel,0,mAckNoCoupling,1,
							mAckIgnore,2,0)) {
					case 1: *pkoppflag=0; break;
					case 2: ignore=1;   break;
					return -1;
				}
			}

			j++;
			dim+=ptri->segs;
		}
	}
	return dim;
}

static float fakLCalc(Zeichnung p1)
{
float kL;
float fakL;
	kL   = 2.*PI*p1->freq/C_VELOC/p1->fak;		/* Wellenzahl in Luft */
	fakL = 2./4./PI*kL*kL/Z_0;
	return fakL;
}

/* 
 * Fill matrices zmat, imat (vector), gmat (latter only if koppflag != 0 [radiation coupling enabled])
 *
 * j: field/test point index
 * i: source point index
 *
 * matrix index is FORTRAN (A[m,n] = A[n*dim+m]).
 *
 * using 'res1,res2' results by 'nahkop_innen(..., &res1, &res2);
 *
 * 'a': 'probe-integral' around field point; a=4 if Psi=Yo(kr)
 *
 * a) Neumann-boundary problem; current feed:
 *    
 *
 *                                            kw
 *     imat[j]   = fak1 res2 = (-Zf D)  1/kw  / Yo(k r) dks
 *                                           0
 *
 *                                          1                 kw
 *     zmat[j,i] = a del(i,j)/k + res1/k = --- { a del(i,j) + / Y1(k r) c/r dks }
 *                                          k                0
 *                    
 *                     k    /         1                          c
 *    From BIM:  Vb = ---  o  j Zf D ---Yo(kr) (-Ib)  -  Y1(kr) --- Vb  ds
 *                     a  /           w                          r
 *
 *    Vb: voltage along boundary                                     ->  ->
 *    Ib: inward-directed current across boundary; because  -Ib = w (H * eg)
 *
 *    we have     [zmat] Vb] = imat] (j Ib)
 *
 * b) Dirichlet-boundary problem; voltage feed:
 *
 *      ~~         --                     kw                      --
 *     imat[j]   = >  a del(i,j)/k + 1/k  / Y1(k r) c/r dks = 1/k >  a del(i,j) + res1
 *                 --                    0                        --
 *                 i=source-polygon                               i=source-polygon
 *
 *      ~~                                   kw
 *     zmat[j,i] = fak1 res2 = (-Zf D) 1/kw  / Yo(k r)     dks
 *                                          0
 *
 *    From BIM: (see above)
 *
 *                   ~~            ~~
 *    it follows:  [zmat] jIb] = [imat] Vb
 *
 *                              ->  ->
 *    going back to  -j k Zf D (H * eg) = j k Zf D/w Ib = dV/dn
 *
 *    and using Dn = eps En = -dV/dn eps 
 *
 *               | D-field
 *               v
 *        =====[ qs ] =====> (boundary
 *            |
 *            v en (outward-directed vector normal to boundary
 *
 *    -> flux of D-field *out* of 'box' containing qs is D field
 *       in inward-direction across boundary, hence
 *
 *          qs = -Dn = dV/dn eps
 *
 *    
 *    Qb = w eps dV/dn  (line charge on boundary)   
 *
 *    Qb = w eps j k Zf D/w Ib = j k Zf eps D Ib
 *
 *    we use 
 *
 *         ~~
 *        zmat[j,i] jIb] = zmat[j,i] (-Zf D)/(k Zf eps D) Qb] = zmat[j,i] -1/(k eps) Qb]
 *
 *
 *    and
 *         ~~
 *        imat[i] = imat[i]/k
 *
 *    with                                --
 *        zmat[j,i] = res2    imat[i] =   >  a del(i,j) + res1
 *                                        --
 *                                        i=source-polygon
 *
 *        [zmat] Qb] = imat] (-V eps)
 *
 */

 
int fill_matrix(Widget wid, Zeichnung p1, real_complex zmat, complex *gmat, float *imat, int dim, int speiseindex, int koppflag)
{
int            k,l,ldim;
Polygon        *borderi, *borderj, *speisePoly;
int            borderjIsSpeise;
register Punkt *ptri,*ptrj;
register int   i,j,idim;
register float segilen;
register float kL,ka,fak1FakL,fak1,kL2hsubst;
int            speiseidim;
float          zeigen,fak,freq,epsi,epst,mu,hsubst,fakL;
float          ieigen,geigenre,geigenim;
real_complex   uvek;
float          *ivek;

	fak=p1->fak;
	freq=p1->freq;
	epsi=p1->epsi;
	mu=p1->mu;
	hsubst=p1->hsubst;

	if ( hsubst == 0. ) {
		/* dummy-Wert */
		hsubst = 1.;
	}

	/* Konstanten berechnen */

	kL=2.*PI*freq/C_VELOC/fak;		/* Wellenzahl in Luft */

	ka=kL*sqrt(epsi*mu);			/* Wellenzahl im Substrat */

	zeigen=ZEIGEN_Y0/ka;			/* Eigenkopplung (innen) */
	fak1=-Z_0*sqrt(mu/epsi)*hsubst*fak;	/* Vorfaktor f"ur Stromvektor */

	kL2hsubst=kL*2*hsubst*fak;		/* Spiegelbild ber"ucksichtigen */

	fakL    = fakLCalc(p1);
	fak1FakL=fak1*fakL;			/* Vorfaktor f"ur Z-Matrix */
	epst    =EPSI_0*epsi;

	speiseidim=speiseindex*dim;

	switch (2*koppflag+spspeis) {
		case 0: /* ungekoppelt, stromspeisung */
			ivek=imat;
			uvek.re=zmat.re + speiseindex*dim;
			break;
		case 1: /* ungekoppelt, spannungsspeisung */
			uvek.re=imat;
			ivek=zmat.re + speiseindex*dim;
			break;
		case 2: /* gekoppelt, stromspeisung */
			ivek=imat+speiseindex*dim;
		case 3: /* gekoppelt, spannungsspeisung */
			break;
		default: spspeis=0; break; /*nicht implementiert*/
	}

	if ( freq <= FREQLIM ) {
		for ( i=0; i<dim; i++ )
			ivek[i] = 0.;
	}
	speisePoly = getSpeisePoly(p1);

	i=0;idim=0;
	for (borderi=(Polygon*) p1->borders.firstel;
			borderi; borderi=NEXTPOLY(borderi)){



		ptri=PLIST(borderi);

		if (ISSTIFT(ptri) && (ptri!=p1->speiseseg || freq <= FREQLIM)) continue;

		while (ptri!=NULL) {
			register float richtix,richtiy,rix,riy;

			richtix=ptri->richtung.x/(float)ptri->segs;
			richtiy= - (ptri->richtung.y/(float)ptri->segs); 
			/* XWindows Y-Koordinaten sind gespiegelt */
			rix=ptri->ort.x-richtix/2.;
			riy= -ptri->ort.y-richtiy/2.; /* spiegeln */

			segilen=ptri->len/(float)ptri->segs;

			if ( freq <= FREQLIM/2. )
				eigenkop_innen_static(segilen,&ieigen);
			else
				eigenkop_innen(ka*segilen,&ieigen);

			if (koppflag) {
				eigenkop_aussen(kL2hsubst,kL*segilen,&geigenre,&geigenim);
				/* mit L"ange von Auf- und Quellpunktsegment multiplizieren (i==j) */
				geigenim*=segilen*segilen/kL2hsubst;
				geigenre*=segilen*segilen/kL2hsubst;
			}



			for(k=0;k<ptri->segs;k++) {
				rix+=richtix;
				riy+=richtiy;


				/* Schleife "uber j initialisieren */
				borderj=(Polygon*)p1->borders.firstel;
				ptrj=0;
				l=-1;

				/* Die Schleife "uber j l"auft solange j<i */

				for (j=0; j<i; j++) {
					register float segjlen,
					/* Segmentvektor (Egj*segjlen) */
					richtjx,richtjy,
					/* Vektor vom Auf (j) - zum Quellpunkt (i): Ri-Rj */
					rdelx,rdely,          
					/* Betrag von Rij */
					krij,kLrij,rij,
					/* Skalarprodukte Edelij.Richti etc. = Edelij.Egi segilen ...
					 */
					RdelRichti,RdelRichtj,
					RdelNormi, RdelNormj,
					RichtiRichtj,RichtjNormi,
					EdelRichtiEdelRichtj,  /* (Edelij.Egi)(Edelij.Egj) */
					EdelNormi,EdelNormj;


					/* n"achsten Punkt auf einer Kante holen */

					/* wenn kein Punkt mehr da, neue Kante holen */
					if (l<=0) {

						if (ptrj!=NULL) ptrj=NEXT(ptrj);
						if (ptrj==NULL) { 
							/* wenn keine Kante mehr da, n"achste Struktur suchen */

							do {
								ptrj=PLIST(borderj);
								borderjIsSpeise = ( borderj == speisePoly );
								borderj=NEXTPOLY(borderj);
							} while (ISSTIFT(ptrj) && (ptrj!=p1->speiseseg || freq <= FREQLIM));
						}

						richtjx= ptrj->richtung.x/(float)ptrj->segs;
						richtjy= -  (ptrj->richtung.y/(float)ptrj->segs); /* spiegeln */
						rdelx= rix - (  ptrj->ort.x  - richtjx/2.);
						rdely= riy - (- ptrj->ort.y  - richtjy/2.); /* ort.y spiegeln */

						RdelRichti=rdelx*richtix+rdely*richtiy;
						RdelRichtj=rdelx*richtjx+rdely*richtjy;

						RichtiRichtj=richtix*richtjx+richtiy*richtjy;
						RichtjNormi =richtjx*richtiy-richtjy*richtix;

						RdelNormi =rdelx*richtiy-rdely*richtix;
						RdelNormj =rdelx*richtjy-rdely*richtjx;

						segjlen=ptrj->len/(float)ptrj->segs;

						l=ptrj->segs;
					}

					l--;
					rdelx-=richtjx;
					rdely-=richtjy;

					RdelRichti-=RichtiRichtj;
					RdelRichtj-=segjlen*segjlen;

					RdelNormi -=RichtjNormi;
					/* RdelNormj+=RichtjNormj==0 ! */


					/* 
					   es wurde "uber das mit i indizierte Segment integriert! 

					   Dies m"usste im zu l"osenden Gleichungssystem eigentlich der zweite
					   Index sein; die Transposition geschieht implizit, da der FORTRAN 
					   Gleichungsl"oser die Indexreihenfolge umgekehrt interpretiert */

					rij=hypotf(rdelx, rdely);

					EdelRichtiEdelRichtj=RdelRichti*RdelRichtj/(rij*rij);

					EdelNormi=RdelNormi/rij;
					EdelNormj=RdelNormj/rij;


					kLrij=kL*rij;
					krij =ka*rij;


					if (krij > (MAXTAB-1)/TABSEP ) {
						printmessg(1,mGeoTooBig,wid);
						return -1;
					}

#ifndef NAHKOP_GRENZE
					if (krij<grenz) { /* kLrij ist sicher kleiner als krij */
#else
						if (krij<NAHKOP_GRENZE) { /* kLrij ist sicher kleiner als krij (} f"ur vi) */
#endif
							register float uo,c;
						float          res1,res2;
						complex        res1L,res2L;


						uo=RdelRichti/segilen;
						c= RdelNormi/segilen;


						if ( freq <= FREQLIM/2. )
							nahkop_innen_static(uo,c,segilen,&res1,&res2);
						else
							nahkop_innen(ka*uo,ka*c,ka*segilen,&res1,&res2);


						if (koppflag) {
							zmat.re[idim+j]=res1/ka;
							imat[idim+j]=res2;


							nahkop_aussen(kL*uo,kL*c,kL2hsubst,kL*segilen,&res1L,&res2L);
							gmat[idim+j].re=1/kL2hsubst*(  RichtiRichtj * res1L.re 
									+ RichtjNormi  * res2L.re);
							gmat[idim+j].im=1/kL2hsubst*(  RichtiRichtj * res1L.im 
									+ RichtjNormi  * res2L.im);
						} else {
							if ( freq <= FREQLIM ) {
								zmat.re[idim+j]=res2;
								if ( borderi == speisePoly )
									ivek[j] += res1;
							} else {
								if (i==speiseindex) {
									uvek.re[j]=res1/ka;
									ivek[j]=res2;
								}else{
									zmat.re[idim+j]=res1/ka;
								}
							}
						}


						/* symmetrisch dazu gelegenes Segment */


						/* bei Vertauschung von i mit j "andert Rdel das Vorzeichen! */

						uo= -RdelRichtj/segjlen; 
						c=  -RdelNormj/segjlen;

						if ( freq <= FREQLIM/2. )
							nahkop_innen_static(uo,c,segjlen,&res1,&res2);
						else
							nahkop_innen(ka*uo,ka*c,ka*segjlen,&res1,&res2);

						if (koppflag) {
							zmat.re[j*dim+i]=res1/ka;
							imat[j*dim+i]=res2;

							/* Bei Vertauschung von i mit j "andert ist das Skalarprodukt
							 *
							 *  - RichtjNormi == RichtiNormj
							 *
							 * zu Verwenden!
							 */

							nahkop_aussen(kL*uo,kL*c,kL2hsubst,kL*segjlen,&res1L,&res2L);
							gmat[j*dim+i].re=1/kL2hsubst*(  RichtiRichtj * res1L.re 
									- RichtjNormi  * res2L.re);
							gmat[j*dim+i].im=1/kL2hsubst*(  RichtiRichtj * res1L.im 
									- RichtjNormi  * res2L.im);
						} else {
							if ( freq <= FREQLIM ) {
								zmat.re[j*dim+i]=res2;
								if ( borderjIsSpeise )
									ivek[i] += res1;
							} else {
								if (j==speiseindex) {
									uvek.re[i]=res1/ka;
									ivek[i]=res2;
								}else{
									zmat.re[j*dim+i]=res1/ka;
								}
							}
						}


					} else {

						assert( freq > FREQLIM );

						if (koppflag) {
#ifndef NAHKOP_GRENZE
							if (kLrij<grenz) { 
#else
								if (kLrij<NAHKOP_GRENZE) {  /* } (f"ur vi) */
#endif
									register float uo,c;
								complex        res1L,res2L;


								uo=RdelRichti/segilen;
								c= RdelNormi/segilen;


								nahkop_aussen(kL*uo,kL*c,kL2hsubst,kL*segilen,&res1L,&res2L);
								gmat[idim+j].re=1/kL2hsubst*(  RichtiRichtj * res1L.re 
										+ RichtjNormi  * res2L.re);
								gmat[idim+j].im=1/kL2hsubst*(  RichtiRichtj * res1L.im 
										+ RichtjNormi  * res2L.im);

								/* symmetrisch dazu gelegenes Segment */


								/* bei Vertauschung von i mit j "andert Rdel das Vorzeichen! */
								uo=-RdelRichtj/segjlen;
								c= -RdelNormj/segjlen;

								nahkop_aussen(kL*uo,kL*c,kL2hsubst,kL*segjlen,&res1L,&res2L);

								/* Bemerkung zum Vorzeichen von RichtjNormi s.o. 
								 */

								gmat[j*dim+i].re=1/kL2hsubst*(  RichtiRichtj * res1L.re 
										- RichtjNormi  * res2L.re);
								gmat[j*dim+i].im=1/kL2hsubst*(  RichtiRichtj * res1L.im 
										- RichtjNormi  * res2L.im);

							} else {
								/* hier werden die "ausseren Kopplungen f"ur entfernte Elemente berechnet */
								register float tmpre,tmpim,a,sumre,sumim;

								a=( RichtiRichtj - EdelRichtiEdelRichtj); 
								INTERPOL2(tmpre,tmpim,Gtab,kLrij,a);

								a-=EdelRichtiEdelRichtj;
								a-=EdelRichtiEdelRichtj;

								INTERPOL2(sumre,sumim,G1tab,kLrij,a);

								sumre=(sumre+tmpre);
								sumim=(sumim+tmpim);

								gmat[idim+j].re=sumre;  gmat[idim+j].im=sumim;
								gmat[j*dim+i].re=sumre; gmat[j*dim+i].im=sumim;
							}
						} /* if (koppflag) */


						/* hier werden die inneren Kopplungen f"ur entfernte Elemente berechnet */
						{ register float tmpre; 
							LINTERPOL(tmpre,H1tab,krij);

							if (koppflag) {
								zmat.re[idim+j] =tmpre*EdelNormi;
								/* Bei Vertauschung i<>j "andert Edel das Vorzeichen!*/
								zmat.re[j*dim+i]=-tmpre*EdelNormj;

								LINTERPOL(tmpre,H0tab,krij);
								imat[j*dim+i]=tmpre;
								imat[idim+j] =tmpre;
							} else {
								if (i==speiseindex) {
									/* Bei Vertauschung i<>j "andert Edel das Vorzeichen!*/
									zmat.re[j*dim+i]=-tmpre*EdelNormj;
									uvek.re[j]=tmpre*EdelNormi;

									LINTERPOL(tmpre,H0tab,krij);
									ivek[j]=tmpre;
								}else if (j==speiseindex) {
									zmat.re[idim+j] =tmpre*EdelNormi;
									/* Bei Vertauschung i<>j "andert Edel das Vorzeichen!*/
									uvek.re[i]=-tmpre*EdelNormj;

									LINTERPOL(tmpre,H0tab,krij);
									ivek[i]=tmpre;
								}else{
									zmat.re[idim+j] =tmpre*EdelNormi;
									/* Bei Vertauschung i<>j "andert Edel das Vorzeichen!*/
									zmat.re[j*dim+i]=-tmpre*EdelNormj;
								}


							}
						}
					} /* if krij<RMIN */
				} /* for j<i */


				/* Eigenkopplung */
				{

					if (koppflag) {
						imat[idim+i]=ieigen;
						zmat.re[idim+i]=zeigen; 
						gmat[idim+i].re=geigenre;
						gmat[idim+i].im=geigenim;
					} else {
						if ( freq <= FREQLIM ) {
							zmat.re[idim+i]=ieigen;
							if ( borderi == speisePoly ) {
								/* loop over j iterated only while j<i; hence
								 * we must add the contribution of j==i:
								 */
								ivek[i] += (freq <= FREQLIM/2.) ? ZEIGEN_LN : ZEIGEN_Y0;
							}
						} else {
							if (i==speiseindex) {
								ivek[i]=ieigen;
								uvek.re[i]=zeigen;
							}else{
								zmat.re[idim+i]=zeigen;
							}
						}
					}
				}

				i++;
				idim+=dim;

			} /* for k */
			ptri=NEXT(ptri);
		}
	} /*for borderi*/


	/* Falls das Speisesegment ein Stift ist, stimmen die
	 * zugeh"origen Matrixelemente nicht! Sie werden an dieser
	 * Stelle neu berechnet!
	 */

	if (ISSTIFT(p1->speiseseg)) {
		register float J0,r_J1,rix,riy,rdelx,rdely,
		richtjx,richtjy,karij,rij,tmp;

		idim=0;
		borderj=(Polygon*)p1->borders.firstel;
		ptrj=0;
		l=-1;
		rix=p1->speiseseg->ort.x;
		riy=-p1->speiseseg->ort.y; /* spiegeln */

		if ( freq <= FREQLIM ) {
		/*
		 * For the electrostatic DIRICHLET problem we have
		 *
		 * //  Psi (/\ + k^2) V = - q/eps
		 * A
		 *                               Q              1           2
		 * If Psi = -Yo(k r) and q = ---------- exp( - --- (rho/sig)  )
		 *                           2 pi sig^2         2 
		 *
		 * using Yo( k sqrt(ro^2 + rho^2 - 2 ro rho cos(fi)) ) which is
		 * valid for rho < ro
		 *
		 *
		 *         +inf
		 *         --
		 *       = >   Jn(k rho) Yn(k ro) exp( -j n fi)
		 *         --
		 *         n=-inf
		 *                  inf 2 pi
		 * integrating over /   /     dfi r dr leaves only the n==0 term
		 *                 0   0
		 *
		 * of the series so that
		 *
		 *                  ~         Q/eps      inf
		 *  // Psi (-q/eps) =  2 pi ----------   / exp(-1/2 (r/sig)^2) Jo(k r) r dr  Yo(k ro)
		 *  A                       2 pi sig^2  0
		 *
		 *                  =  Q/eps Yo(k ro) exp( - 1/2 (k sig)^2 ) 
		 *
		 *                  ~  Q/eps Yo(k ro)
		 *
		 *  CAVEAT: series only is valid if ro > rho, i.e., if we don't look 'into' the
		 *          charge distribution. Hence, the above expression is an approximation
		 *          for ro >> sig. 'Inside' the series is reversed, i.e.,
		 *          we obtain  (rho > ro)
		 *                        --
		 *               Yo(kr) = >  Jn(k ro) Yn(k rho) exp( -j n fi)
		 *                        --
		 *
		 *          The exact expression is
		 *
		 *                  ro                                      ro
		 *        Yo(k ro)  /  q(rho) Jo(k rho) rho drho + Jo(k ro) / q(rho) Yo(k rho) rho drho
		 *                 0                                       0
		 *
		 *          which AFAIK cannot be integrated in closed form for gaussian q(rho).
		 *          [But it works for a constant distribution: Q/(pi sig^2) for rho<sig
		 *          and 0 if rho>sig:
		 *
		 *          2 pi Q/(pi sig^2) 
		 *          	{ Yo(k ro) J1(k ro) - Jo(k ro) Y1(k ro) + sig Y1(k sig) }/k
		 *
		 *        OTOH, if we deal with very small k then we might be back in business...
		 *
		 *
		 *  ELECTROSTATIC CASE (charge distro q(r,fi,sig)):
		 *
		 *       gs(ro,r,fi) = 1/2 log(r0^2 + r^2 - 2 r ro cos(fi))
		 *
		 *       Source term 
		 *
		 *                    inf 2pi
		 *       S(ro,sig) =  /   / q(r,fi,sig) gs(ro,r,sig) r dr dfi
		 *                   0   0
		 *
		 *       gs(ro,r,fi) is periodic in fi (period 2pi) and even in fi
		 *       -> can be expanded in fourier series
		 *
		 *                     inf
		 *                     --                        1
		 *       gs(ro,r,fi) = >   An(ro,r) cos(n fi) + --- Ao(ro,r)
		 *                     --                        2
		 *                     n=1
		 *
		 *                         2pi
		 *       with An =  1/(pi) /  gs(ro,r,fi) cos(n fi) dfi
		 *                        0
		 *
		 *       For isotropic charge distro q(r,fi,sig) = qi(r,sig)
		 *       only Ao matters and from 
		 *
		 *          pi                                           log(ro)  ro >= r  > 0
		 *          / log(a^2 + b^2 - 2 ab cos(fi)) dfi = 2 pi {                             }*vi*
		 *         0                                             log(r)   r  >= ro > 0
		 *
		 *       we obtain ( intg(gs,0..2 pi) = 2 intg(gs,0..pi) [note that gs contains 1/2 log]
		 *
		 *        1         log(ro)  ro >= r  > 0 
		 *       --- Ao = {                                                                  }*vi*
		 *        2         log(r)   r  >= ro > 0
		 *
		 *
		 *       For uniform charge distribution:
		 *
		 *                      Q/(pi sig^2)   sig >= r >= 0
		 *         q(r,sig) = {                                                              }*vi*
		 *                      0              r > sig
		 *
		 *       we calculate S(ro,sig):
		 *
		 *                   |               Q              sig             
		 *         S(ro,sig) |          = ----------  2 pi /  log(ro) r dr = Q log(ro)
		 *                   |ro > sig     pi sig^2       0
		 *
		 *                   
		 *                   |               Q                ro                sig
		 *         S(ro,sig) |          = ----------  2 pi ( /  log(ro) r dr +  /  log(r) r dr )
		 *                   |ro < sig     pi sig^2         0                  ro
		 *
		 *                              
		 *                                       ro^2
		 *                              =  Q ( -------- + log(sig) - 1/2 )
		 *                                     2 sig^2
		 */
			for (i=0; i<dim; i++){
				if (l<=0) {

					if (ptrj!=NULL) ptrj=NEXT(ptrj);
					if (ptrj==NULL) { 
						/* wenn keine Kante mehr da, n"achste Struktur suchen */

						do {
							ptrj=PLIST(borderj);
							borderj=NEXTPOLY(borderj);
						} while (ISSTIFT(ptrj) && (ptrj!=p1->speiseseg || freq<=FREQLIM));
					}
					l=ptrj->segs;
					richtjx= ptrj->richtung.x/(float)l;
					richtjy= -  (ptrj->richtung.y/(float)l); /* spiegeln */
					rdelx= rix - (  ptrj->ort.x  - richtjx/2.);
					rdely= riy - (- ptrj->ort.y  - richtjy/2.); /* ort.y spiegeln */
				}

				l--;
				rdelx-=richtjx;
				rdely-=richtjy;

				rij=hypot(rdelx,rdely);

				/* ivek contains voltage; convert charge to volts... */
				if ( freq <= FREQLIM/2. ) {
					ivek[i] = uniform_chrg_static(rij, p1->speiseseg->len)/epst;
				} else {
					ivek[i] = uniform_chrg(ka*rij, ka*p1->speiseseg->len)/epst;
				}
			}
		} else {

			assert( freq > FREQLIM );

			J0=j0(ka*p1->speiseseg->len);
			r_J1=-2*PI*p1->speiseseg->len*j1(ka*p1->speiseseg->len);
			for (i=0; i<dim; i++){
				if (l<=0) {

					if (ptrj!=NULL) ptrj=NEXT(ptrj);
					if (ptrj==NULL) { 
						/* wenn keine Kante mehr da, n"achste Struktur suchen */

						do {
							ptrj=PLIST(borderj);
							borderj=NEXTPOLY(borderj);
						} while (ISSTIFT(ptrj) && ptrj!=p1->speiseseg);
					}
					l=ptrj->segs;
					richtjx= ptrj->richtung.x/(float)l;
					richtjy= -  (ptrj->richtung.y/(float)l); /* spiegeln */
					rdelx= rix - (  ptrj->ort.x  - richtjx/2.);
					rdely= riy - (- ptrj->ort.y  - richtjy/2.); /* ort.y spiegeln */

				}

				l--;
				rdelx-=richtjx;
				rdely-=richtjy;

				karij=(i!=speiseindex) ? ka*hypot(rdelx,rdely) : ka*ptrj->len;;

				zmat.re[idim+speiseindex]*=J0;

				tmp=(karij>=NAHKOP_GRENZE) ? ivek[i] : y0(karij);

				if (koppflag) {
					gmat[speiseidim+i].re=0.;
					gmat[speiseidim+i].im=0.;
					gmat[idim+speiseindex].re=0.;
					gmat[idim+speiseindex].im=0.;

					imat[idim+speiseindex]*=J0;
				}

				/* Die Eigenkopplungen werden hier 
				 * mit den richtigen Werten "uberschrieben 
				 */
				zmat.re[speiseidim+i]=r_J1 * tmp;
				ivek[i]=J0*tmp;

				idim+=dim;
			}
			zmat.re[speiseidim+speiseindex]+=ZEIGEN_0_Y0/ka;
		}
	}

	if ( koppflag ) {
		char     stri[100];

		ldim=(dim-1)*dim;

		j=0;

#ifdef VORMULT
		wk.re=(float*)Malloc(sizeof(float)*dim*(dim+2)/2);
		ipvt=(int*)Malloc(sizeof(int)*dim);
		l2nrg_(&lda,imat,&lda,imat,&lda,wk.re,ipvt);
		Free(wk.raw);wk.raw=(char*)0;
		Free(ipvt);ipvt=0;
#endif

		sprintf(stri,fBusyMatMult);
		printmessg(0,stri,outputlist[MSSG_M].t_wid);
		unblock(1);

#ifdef VORMULT
		for (l=dim-1;l>=0;l--) {
			register int j;
			for (j=dim-1;j>=0;j--) {
				register double sumre,sumim;
				register float   *imatp;
				register float *gmatp;

				register int i;
				sumre=sumim=0;

				gmatp=& zmat.re[ldim /* + 0*/];

				for ( imatp=& imat[/*0 * dim + */ j];
						imatp<& imat[dim*dim+j];
						imatp+=dim){
					sumre+=*imatp * *gmatp;

					gmatp+=1;
				}
				zmat.co[ldim+j+dim].re=gmat[ldim+j].re*fakL;
				zmat.co[ldim+j+dim].im=gmat[ldim+j].im*fakL-(float)sumre/fak1;

			}
			ldim-=dim;
		}/*for l*/

#else 

		for (l=dim-1;l>=0;l--) {
			register int j;
			for (j=dim-1;j>=0;j--) {
				register double sumre,sumim;
				register float   *imatp;
				register complex *gmatp;
				sumre=sumim=0;

				gmatp=& gmat[ldim /* + 0*/];

				for ( imatp=& imat[/*0 * dim + */ j];
						imatp<& imat[dim*dim+j];
						imatp+=dim){
					sumre+=*imatp * gmatp->re;
					sumim+=*imatp * gmatp->im;

					gmatp+=1;
				}
				zmat.co[ldim+j+dim].re=zmat.re[ldim+j] - (float)sumim*fak1FakL;
				zmat.co[ldim+j+dim].im=(float)sumre*fak1FakL;

			}
			ldim-=dim;
		}/*for l*/

#endif

		for (j=0; j<dim; j++) {
			p1->jmagn.co[j].re=0.;
#ifdef VORMULT
			p1->jmagn.co[j].im=0.;
#else
			p1->jmagn.co[j].im=fak1*ivek[j];
#endif
		}

#ifdef VORMULT
		p1->jmagn.co[speiseindex].re=1.;
#endif

	} else {
		if ( freq > FREQLIM ) {
			for (j=0; j<dim; j++) ivek[j]*=     fak1 * SPEISESTROM;
		} else {
			/* well - shouldn't be a current but a voltage;
			 *
			 * -epst appears here because we multiply
			 *
			 *             -                            -
			 *  2 pi Vp + / (- dln(r)/dn) ds Vp  = 1/w / log(r) (-Q/eps) 
			 *           -                            -
			 *
			 *  on both sides with (-eps); ivek then contains -eps * Vp
			 *  and we are solving for 'Q'.
			 */
			for (j=0; j<dim; j++) ivek[j]*= (- epst) * SPEISESTROM;
		}
	}
	return 0;
}

#define COMPARE
#ifdef COMPARE
void theStrom(wid,p1)
#else
void Strom(wid,p1)
#endif

Widget    wid; /* where to popup notices */
Zeichnung p1;

{

	int lda,ipath,acc,speiseindex,speiseidim;

	/* Zeiger f"ur Speicherbl"ocke
	 * (Free)
	 */

	real_complex zmat,wk;
	complex *gmat;
	int     *ipvt;
	float   *imat, *jmag0, *fac;
	complex *delta;
	float   *ivek;


	/* Zeiger */
#ifndef DIREKT
	complex     *jptr;
	complex     *gptr; /* Iteration der L"osung */
#endif


	int koppflag;

	float   temp;


	register int dim,idim,i,j;

	char stri[100];


	zmat.raw=0;imat=0;gmat=0;wk.raw=0;ipvt=0;delta=0;
	fac=0; jmag0=0;


	if (p1->speiseseg==NULL) {
		printmessg(1,mNoExcitation,wid);
		goto errorret ;
	}

	/* Parameter einlesen */
	getparams(input_haupt);
	koppflag=getflag(chboxlist[CHBX_KOPPFLAG].t_wid);
	strominvalid(p1);
	destroyfield(p1);
	koppinvalid(p1);


	/* Parameterpr"ufung */

	/* alle geometrischen L"angen sind
	 * mit fak skaliert!
	 */
	if (p1->hsubst==0.) {
		if (koppflag) 
			switch( (int) Notice(wid,0,mWrnSubstHeight,mAckCancel,0,
						mAckNoCoupling,1,
						0)) {
				case 1: koppflag=0; 
						break;
				default: return;
			}
	}

	if (p1->freq<=0.) {
		Notice(wid,0,mWrnFreqNeg,mAckCont,0,0);
		return;
	}
	if (p1->freq <= FREQLIM) {
		if (koppflag) 
			switch( (int) Notice(wid,0,mWrnFreqLow,mAckCancel,0,
						mAckNoCoupling,1,
						0)) {
				case 1: koppflag=0; 
						break;
				default: return;
		}
	}
	if (p1->epsi<1.) {
		Notice(wid,0,mErInvalid,mAckCont,0,0);
		return;
	}

	if ( (dim=countSegs(wid, p1, &speiseindex, &koppflag)) < 0 )
		goto errorret;

	if (koppflag) {
		/* Speicher allozieren f"ur die Aussenverkopplung (gmat) */
		if ((gmat=(complex *) Malloc(sizeof(complex)*dim*dim)) == NULL) {
			sprintf(stri,fNoMem,"'gmat'");
			printmessg(1,stri,0);
			goto errorret;}

			i=sizeof(complex)*(dim+1); /* extra vektor als Zwischenspeicher f"ur Matrixmult. */
			j=sizeof(float)*dim;
	} else {
		i=sizeof(float)*dim;
		j=sizeof(float)*1;
	}

	/* Speicher allozieren f"ur die Innenverkopplung (zmat f"ur Spannungen) */
	if ((zmat.raw= Malloc(i*dim)) == NULL) {
		sprintf(stri,fNoMem,"'zmat'");
		printmessg(1,stri,0);
		goto errorret;
	}


	/* Speicher allozieren f"ur die Innenverkopplung (imat f"ur Str"ome) */
	if ((imat=(float *) Malloc(j*dim)) == NULL) {
		sprintf(stri,fNoMem,"'imat'");
		printmessg(1,stri,0);
		goto errorret;
	}

	switch (2*koppflag+spspeis) {
		case 0: /* ungekoppelt, stromspeisung */
			ivek=imat;
			break;
		case 1: /* ungekoppelt, spannungsspeisung */
			ivek=zmat.re + speiseindex*dim;
			break;
		case 2: /* gekoppelt, stromspeisung */
			ivek=imat+speiseindex*dim;
		case 3: /* gekoppelt, spannungsspeisung */
			break;
		default: spspeis=0; break; /*nicht implementiert*/
	}


	if (initflag) {
		/* Beim ersten Aufruf die Funktionstabellen initialisieren */

		/* Diese Tabellenwerte sollten nie ben"otigt werden! */

		H0tab[0].re=0;
		H1tab[0].re=0;
		Gtab[0].re =1.;
		G1tab[0].re =0;
		H0tab[0].im=1;
		H1tab[0].im=0;
		Gtab[0].im=1.e10;
		G1tab[0].im=-1e20;

		for(initflag=1;initflag<MAXTAB;initflag++) {
			double arg;
			arg=(double)initflag/TABSEP;

			/* In die Tabelle wird -1/j * H(2)(kr) = Y(kr)+ j J(kr)
			   geschrieben (wegen Faktor 1/2j) */

			H0tab[initflag].re=y0(arg);
			H1tab[initflag].re=y1(arg);
#ifndef MODIF1
			H0tab[initflag].im=j0(arg);
			H1tab[initflag].im=j1(arg);
			Gtab[initflag].re =sin(arg)/arg;
			Gtab[initflag].im =cos(arg)/arg;
			G1tab[initflag].re=Gtab[initflag].im/arg-Gtab[initflag].re/arg/arg;   
			G1tab[initflag].im=-Gtab[initflag].re/arg-Gtab[initflag].im/arg/arg;   
#else
			H0tab[initflag].im=0;
			H1tab[initflag].im=0;
			Gtab[initflag].re =0;
			Gtab[initflag].im =0;
			G1tab[initflag].re =0;
			G1tab[initflag].im =0;
#endif

		}

		initflag=0;
	}

	BusyButtons(buttonlist,True);
	if (setjmp(env)) {
		/* Strom wurde unterbrochen */
		strominvalid(p1); goto errorret;
	}

	/* alte L"osung freigeben */

	if (p1->jmagn.raw!=NULL) {Free(p1->jmagn.raw); p1->jmagn.raw=0;}
	if (p1->jelek.raw!=NULL) {Free(p1->jelek.raw); p1->jelek.raw=0;}

	if ( koppflag && (p1->jmagn.raw=Malloc(sizeof(complex)*lda))==NULL) {
			sprintf(stri,fNoMem,"'jmagn'");
			printmessg(1,stri,0);
			goto errorret;
	}

	sprintf(stri,fBusyZMatDim,dim);
	printmessg(0,stri,outputlist[MSSG_M].t_wid);
	enable_interrupt(0,500000,wid,&env[0],0);


	if ( fill_matrix(wid,p1,zmat,gmat,imat,dim,speiseindex,koppflag) )
		goto errorret;

	block();

	lda=dim;

	/* Matrixmultiplikation */
#ifdef DIREKT
	if (koppflag) { 

		if ((wk.raw= Malloc(sizeof(complex)*lda)) == NULL) {
			sprintf(stri,fNoMem,"'wk'");
			printmessg(1,stri,0);
			goto errorret;
		}

		if ((ipvt=(int *) Malloc(sizeof(int)*lda)) == NULL) {
			sprintf(stri,fNoMem,"'ipvt'");
			printmessg(1,stri,0);
			goto errorret;
		}

		unblock(1);

		printmessg(0,mBusySolveEqn,outputlist[MSSG_M].t_wid);

#ifdef IMSL
		ipath=1;
		l2lcg_(&lda,zmat.co+lda,&lda,p1->jmagn.co,&ipath,
				p1->jmagn.co,zmat.co+lda,ipvt,wk.co);
#else
		ipath=1;
		cgefs_(zmat.co+lda,&lda,&lda,p1->jmagn.co,&ipath,&acc,wk.co,ipvt);
		block();
		if (errorcheck(wid,nEqnSolCGEFS,acc,stri)) {
			disable_interrupt();
			Notice(wid,0,stri,mAckCont,0,0);
			goto errorret;
		}
		unblock(0);
#endif

		/* Falls Faktorisierung gew"unscht: */
		/*
		   l2tcg_(&lda,zmat.co+lda,&lda,zmat.co+lda,&lda,ipvt,wk.co);
		   lfscg_(&lda,zmat.co+lda,&lda,ipvt,p1->jmagn.co,&ipath,p1->jmagn.co);
		 */
		block();

		p1->jelek.co=wk.co;  wk.raw=0; /* umfunktionieren von wk zum Stromvektor */
		Free(zmat.raw); zmat.raw=0;
		Free(ipvt); ipvt=0;
		Free(imat); imat=0; ivek=0;

		unblock(0);
		calc_jelek(p1,gmat,dim);

	} else {

		if ((wk.raw= Malloc(sizeof(float)*lda)) == NULL) {
			sprintf(stri,fNoMem,"'wk'");
			printmessg(1,stri,0);
			goto errorret;
		}

		if ((ipvt=(int *) Malloc(sizeof(int)*lda)) == NULL) {
			sprintf(stri,fNoMem,"'ipvt'");
			printmessg(1,stri,0);
			goto errorret;
		}


		printmessg(0,mBusySolveEqn,outputlist[MSSG_M].t_wid);
		unblock(1);

#ifdef IMSL
		ipath=1;
		l2lrg_(&lda,zmat.re,&lda,ivek,&ipath,ivek,zmat.re,ipvt,wk.re);
#else
		ipath=1;
		sgefs_(zmat.re,&lda,&lda,ivek,&ipath,&acc,wk.re,ipvt);
		block();
		if (errorcheck(wid,nEqnSolSGEFS,acc,stri)) {
			disable_interrupt();
			Notice(wid,0,stri,mAckCont,0,0);
			goto errorret;
		}
		unblock(0);
#endif


		block();


		Free(wk.raw);   wk.raw=0;
		Free(ipvt);     ipvt=0;
		Free(zmat.raw); zmat.raw=0;
		p1->jmagn.re=ivek; ivek=imat=0;
		/* TSILL */
		{
		double tot=0.;
		for (i=0; i<dim; i++)
			tot+=p1->jmagn.re[i];	
		printf("TSILL tot charge %10.10g\n",tot);
		}
	}
#else


	/* solve for uncoupled solution */

	/* workspace */
	if ((wk.raw= Malloc(sizeof(complex)*lda)) == NULL) {
		sprintf(stri,fNoMem,"'wk'");
		printmessg(1,stri,0);
		goto errorret;
	}

	if ((ipvt=(int *) Malloc(sizeof(int)*lda)) == NULL) {
		sprintf(stri,fNoMem,"'ipvt'");
		printmessg(1,stri,0);
		goto errorret;
	}

	/* solution */
	if ((jmag0= (float*)Malloc(sizeof(float)*lda)) == NULL) {
		sprintf(stri,fNoMem,"'jmag0'");
		printmessg(1,stri,0);
		goto errorret;
	}

	for (i=0; i<lda; i++) jmag0[i]=fak1*ivek[i];

	/* compute start vector */
	ipath=1;
	sgefs_(zmat.re,&lda,&lda,jmag0,&ipath,&acc,wk.re,ipvt);

	if (! koppflag) {
		/* We're already done, there will be no iteration */
		p1->jmagn.re=jmag0; jmag0=0;
		p1->nstrom=dim;
		NOTIFY_ON(p1,ST_STRM);

		Free(ipvt); ipvt=0;
		Free(wk.raw); wk.raw=0;
	} else {

		/* compute [zmat]^-1 * [imat] (goes to [imat])*/
		idim=0;
		ipath=0;
		for (i=0; i<dim; i++) {
			sgesl_(zmat.re,&lda,&lda,ipvt,&(imat[idim+0]),&ipath);
			idim+=dim;
		}

		Free(ipvt); ipvt=0;
		Free(zmat.raw); zmat.raw=0;

		if ((p1->jmagn.raw=Malloc(sizeof(complex)*lda))==NULL) {
			sprintf(stri,fNoMem,"'jmagn'");
			printmessg(1,stri,0);
			goto errorret;
		}

		if ((delta=(complex*) Malloc(sizeof(complex)*lda))==NULL) {
			sprintf(stri,fNoMem,"'delta'");
			printmessg(1,stri,0);
			goto errorret;
		}

		/* initial value of solution */
		for (i=0; i<dim; i++) {
			p1->jmagn.co[i].re=0.;
			p1->jmagn.co[i].im=jmag0[i];
		}

		p1->nstrom=dim;
		p1->jelek=wk.co; wk.raw=0; 


		/*
		 * L"osung von  [Zmat] U] + j fak1fakL [Imat] [Gmat] U] =j fak1 [Imat] Iq] =j fak1 Ivek]
		 *
		 * Oben wurde die L"osung des ungekoppelten Systems berechnet.
		 *
		 *                            -1			        -1
		 *  jmagn] + j fak1fakL [Zmat]   [Imat] [Gmat] jmagn] = j [Zmat]   Ivek] = j jmag0]
		 *
		 * Initialisierung der Iteration
		 *
		 * jmagn]|    = jmagn]| + ee [B0] (j jmag0] - [A] jmagn]|)
		 *       |n+1         |n			      |n
		 *
		 * mit    [A]  = ([1] + j fak1fakL [P] [Gmat])
		 *
		 *                     -1
		 *        [P]  = [Zmat]   [Imat]
		 *
		 *                  T			       -1
		 *     ee [B0] = [A]    als Startwert f"ur  [A]
		 *
		 *			                 T    T
		 *        [B0] = ([1] + j fak1fakL [Gmat]  [P] )
		 *
		 */


		XClearWindow(disp,win1);

		while (1) {

			printf ("iter, ee?\n");
			scanf ("%i%f",&ipath,&ee);

			if (ipath==0) break;

			while (ipath--) {

				/* Ein Iterationsschritt */

				/* Multiplikation mit [Gmat] (indices wg. Fortran vertauscht */
				cMatVek(dim,gmat,p1->jmagn.co,NULL,p1->jelek.co,FARINDEX);

				/* Multiplikation mit j fak1fakL */
				jptr=p1->jelek.co;
				for (i=0; i<dim; i++) {
					temp= -jptr->im*fak1FakL;
					jptr->im= jptr->re*fak1FakL;
					jptr->re=temp;
					jptr++;
				}

				/* Multiplikation mit [Zmat]^-1 [Imat] , Addition von jmagn] */
				rMatVek(dim,imat,p1->jelek.co,p1->jmagn.co,delta,FARINDEX);

				/*
				 * Berechnung von 0 +j jmag0 - delta 
				 *
				 * dieses Ergebnis muss sp"ater mit der adjungierten Matrix
				 *  
				 *				  T*
				 * ( [1] + j fak1fakL [P] [Gmat] )
				 *
				 * multipliziert werden. Da
				 *
				 *       T*            T   *  *
				 *  ( [M]   u]) = ( [M]  u]  )
				 *					*
				 * berechnen wir also   -j jmag0 - delta 
				 *
				 */
				for (i=0; i<dim; i++) {
					delta[i].re= -delta[i].re;
					delta[i].im= -(float)(((double)jmag0[i]) - ((double)delta[i].im));
				}

				/* 							     T
				 * Multiplikation mit der Transponierten ( [Zmat]^-1 [Imat] )
				 */

				rMatVek(dim,imat,delta,NULL,p1->jelek.co,CLOSEINDEX);

				/* Multiplikation mit  j fak1fakL */
				jptr=p1->jelek.co;
				for (i=0; i<dim; i++) {
					temp=     -jptr->im*fak1FakL;
					jptr->im=  jptr->re*fak1FakL;
					jptr->re=temp;
					jptr++;
				}

				/* 			    T			      *
				 * Multiplikation mit [Gmat]   und Addition von delta]
				 */

				cMatVek(dim,gmat,p1->jelek.co,delta,delta,CLOSEINDEX);

				/* 
				 * Schliesslich der neue Wert von jmagn]
				 * das Ergebnis der ganzen Multiplikationen muss nun wieder
				 *			      *
				 * konjugiert werden  ( delta]  )
				 */

				for (i=0; i<dim; i++) {
					p1->jmagn.co[i].re+=  ee*delta[i].re;
					p1->jmagn.co[i].im+= -ee*delta[i].im;
				}

				calc_jelek(p1,gmat,dim);
				/* Zeichnen */

				CBExpStrom(canvaslist[CNVS_STROMFENSTER].t_wid);
				XFlush(disp);

				/* Abbruch der Iteration? */

				unblock(1);

				block();

			} /* while iter-- */
		} /* Loop */

		/* Ende Iterationsschritt 
		 * Beim Abbruch der Schleife enthalten
		 *
		 * p1->jmagn, p1->jelek
		 * 
		 * g"ultige Werte; deltaV kann freigegeben werden!
		 */
		if (jmag0)  {Free(jmag0); jmag0=0;}
		if (delta)  {Free(delta); delta=0;}



	}

#endif

	disable_interrupt();

	if (imat){Free(imat); imat=0; ivek=0;}
	if (zmat.raw){Free(zmat.raw); zmat.raw=0;}
	if (gmat) {Free(gmat); gmat=0;}

	{ float p,q,psp,qsp;
		int j;
		p=q=0;
		if (koppflag) {
			for (j=0; j<lda;j++) {
				p+=p1->jmagn.co[j].re*p1->jelek.co[j].re - p1->jmagn.co[j].im*p1->jelek.co[j].im;
				q+=p1->jmagn.co[j].re*p1->jelek.co[j].im + p1->jmagn.co[j].im*p1->jelek.co[j].re;
			}
			j=speiseindex;
			psp=p1->jmagn.co[j].re*p1->jelek.co[j].re - p1->jmagn.co[j].im*p1->jelek.co[j].im;
			qsp=p1->jmagn.co[j].re*p1->jelek.co[j].im + p1->jmagn.co[j].im*p1->jelek.co[j].re;
#ifdef VERBOSE
			printf("Scheinleistung %g +j %g; am Speisetor %g +j %g\n",p,q,psp,qsp);
#endif
		}
	}

	/* der Strom ist nun g"ultig und der Interrupt ist
	 * ausgeschaltet!
	 */  
	p1->nstrom=dim;
	p1->speiseindex=speiseindex;
	XClearWindow(disp,win1);
	CBExpStrom(canvaslist[CNVS_STROMFENSTER].t_wid);
	NOTIFY_ON(p1,ST_STRM);
	XFlush(disp);
#ifdef VERBOSE
	printf("nachher\n");
#endif

	if (!spspeis) {
		if (koppflag) {
#ifdef VERBOSE
			printf("Speisespannung %g +j %g\n",p1->jmagn.co[speiseindex].re,
					p1->jmagn.co[speiseindex].im);
#endif
			temp=hypot(p1->jmagn.co[speiseindex].re,
					p1->jmagn.co[speiseindex].im);
			temp*=temp;
#ifdef VERBOSE
			printf("Speiseadmittanz %g +j %g\n", p1->jmagn.co[speiseindex].re/temp,
					-p1->jmagn.co[speiseindex].im/temp);
#endif
		} else {
#ifdef VERBOSE
			printf("Speisespannung 0 +j %g\n",*(p1->jmagn.re+speiseindex));
#endif
		}
	}


errorret:

	disable_interrupt();

	printmessg(0,0,outputlist[MSSG_M].t_wid);

	BusyButtons(buttonlist,False);

	/* allozierten Speicher freigeben */
	if (fac) {if (fac!=zmat.re) Free(fac); fac=0;}
	if (zmat.raw) {Free(zmat.raw);zmat.raw=0;}
	if (gmat) {Free(gmat);gmat=0;}
	if (jmag0) {if (jmag0!=imat) Free(jmag0); jmag0=0;}
	if (imat) {Free(imat);imat=0;}
	if (wk.raw) {Free(wk.raw);wk.raw=0;}
	if (delta) {Free((char*)delta); delta=0;}
	if (ipvt) {Free(ipvt);ipvt=0;}

	return;
}

#ifdef COMPARE
void Strom(wid,p1)
Widget    wid; /* where to popup notices */
Zeichnung p1;
{
float *s1=0;
float forig=p1->freq;
double diff;
double magn;
float  x;
int   dim,d1,d2;
	/* doesn't work! theStrom uses getparams to read from the widgets;
	 * I set a breakpoint in gdb and changed freq from there...
	 */
	p1->freq = 0.7;
	theStrom(wid,p1);
	s1 = p1->jmagn.re;
	p1->jmagn.raw=0;
	p1->freq = 0.2;
	theStrom(wid,p1);
	if ( s1 && p1->jmagn.re ) {
		dim = countSegs(wid,p1,&d1,&d2);
		magn = diff = 0.;
		for ( d1=0; d1<dim; d1++ ) {
			x = s1[d1]-p1->jmagn.re[d1];
			diff += x*x;
			magn += x*(s1[d1]+p1->jmagn.re[d1]);
		}
		printf("Diff: %10.10g, Magn: %10.10g, Rel: %10.10g\n", diff, magn, diff/magn);
	}
	free(s1);
}
#endif


void strominvalid(p1)
 Zeichnung p1;  
{
	if (! ST_VALID(p1,ST_STRM)) return;

	if (p1->jelek.raw) {
		Free(p1->jelek.raw); p1->jelek.raw=0;
	}

	if (p1->jmagn.raw) {
		Free(p1->jmagn.raw); p1->jmagn.raw=0;
	}

	XClearWindow(disp,win1);
	p1->nstrom=0;

	NOTIFY_OFF(p1,ST_STRM);
}

void CBExpStrom(w,cld,cad)

Widget w;
XtPointer cld,cad;

{
	Dimension width,height;

	XtVaGetValues(w,XtNwidth,&width,XtNheight,&height,NULL);
	drawstrom(zeichnung,XtWindow(w),
			width-40,height-50,
			20,height-20,
			20,15);
}


void drawstrom(z,drwbl,width,height,origx,origy,txtx,txty)
Zeichnung z;
Drawable  drwbl;
int	  width,height;
int	  origx,origy;  
int	  txtx, txty;

{
	/* void drawcoords(); */
	float max,maxe,tmp,tmpe,ltot,delta,seglen;
	float ree,imm;
	int   i,j,even,n;
	Punkt * ptr;
	char dash,num[BUFSIZ];
	XPoint p,p1,firstp,firstp1;
	Polygon * border;
	int koppflag_local;
	PBuffer xbuf1,xbuf2;
	int	x_i;
	float	dashf,x_f;

	if (!ST_VALID(z,ST_STRM)) return;

	koppflag_local=(z->jelek.raw!=NULL);

	if ( ! koppflag_local ) {
		/* draw signed */
		height/=2;
		origy -=height;
	}

	xbuf1=z->xbuf1;
	xbuf2=z->xbuf2;

	ResetPBuffer(xbuf1,drwbl,gc);

	XSetLineAttributes(disp,gcxor,0,LineOnOffDash,CapButt,JoinRound);
	XSetFunction(disp,gcxor,GXcopy);
	XSetDashes(disp,gcxor,0,"\3\2",2);
	ResetPBuffer(xbuf2,drwbl,gcxor);


	ltot=0;
	max=maxe=0;
	i=0;

	for (border=(Polygon*)z->borders.firstel; 
			border; border=NEXTPOLY(border)){
		ptr = PLIST(border);
		if ( ISSTIFT(ptr) )
			continue;
		while(ptr!=NULL) {
			ltot+=ptr->len; 
			seglen=ptr->len/(float)ptr->segs;
			if (koppflag_local) {

				for (j=0;j<ptr->segs;j++) {

					switch (z->nsegs) {
						case 1: 
							tmp= z->jmagn.co[i].re ;
							tmpe= z->jelek.co[i].re ;break;
						case 2: 
							tmp=z->jmagn.co[i].im;
							tmpe=z->jelek.co[i].im; break;
						default:
							tmp=hypot(z->jmagn.co[i].re,z->jmagn.co[i].im);
							tmpe=z->jmagn.co[i].re*z->jelek.co[i].re + z->jmagn.co[i].im*z->jelek.co[i].im;
							break;
					}
					tmp=fabs(tmp);
					tmpe=fabs(tmpe/seglen);
					if (tmp>max) max=tmp;
					if (tmpe>maxe) maxe=tmpe;
					i++;
				}
			} else {
				for (j=0;j<ptr->segs;j++) {
					tmp=fabs(*(z->jmagn.re+i));
					if ( z->freq <= FREQLIM )
						tmp/=seglen;
					if (tmp>max) max=tmp;
					i++;
				}
			}
			ptr=NEXT(ptr);
		}
	}

	i = 0;
	if ( koppflag_local )
		i+= sprintf(num+i,mModulusShown);

	if ( z->freq <= FREQLIM ) {
		i+= sprintf(num+i,mChrgDensShown);
	} else {
		i+= sprintf(num+i,fMaxBorderVolt,max);
		if (koppflag_local) {
			ree=z->jmagn.co[z->speiseindex].re;
			imm=z->jmagn.co[z->speiseindex].im;
		} else {
			ree=0.0;
			imm=*(z->jmagn.re+z->speiseindex);
		}
		tmp=ree*ree+imm*imm;

		if (txty>0) {
			i+=sprintf(num+i,fImpedance,ree,
					signbit(imm)?'-':'+',
					(float)fabs(imm));

			i+=sprintf(num+i,fAdmittance, ree/tmp,
					signbit(imm)?'+':'-',
					(float)fabs(imm)/tmp);
		}
	}

	if ( txty > 0 )
		XDrawImageString(disp,drwbl,gc,txtx,txty,num,i);

	delta=((float)width)/((float)ltot);

	n=i=0; 

	p1.x=p.x=origx;
	even=1;

	x_f=0.;


	for (border=(Polygon*)z->borders.firstel; 
			border; border=NEXTPOLY(border)){
		ptr = PLIST(border);
		if ( ISSTIFT(ptr) )
			continue;
		firstp.x=-1; /* flag  f"ur erste Kante */
		while(ptr) {
			char dashes[2];
			int offset;

			seglen=ptr->len/(float)ptr->segs;
			dashf=delta*(float)seglen;
			dash=(char)irint(dashf);
			if (dash==0) dash=1;
			dashes[0]=dashes[1]=dash;
			offset = (even) ? 0:dash-1;
			XSetDashes(disp,gccoord,offset,dashes,2);
			x_i=irint(p.x+ptr->segs*dashf);
			XDrawLine(disp,drwbl,gccoord,p.x,origy,x_i,origy);

			even=!(even * (ptr->segs%2));


			for (j=0;j<ptr->segs;j++) {

				if (koppflag_local) {
#ifdef NEVER
					tmp=(float) atan2pi((double) z->jmagn.co[i].im,
							(double) z->jmagn.co[i].re)/2.;
					if (tmp<0) tmp+=1;
#endif
					switch (z->nsegs) {
						case 1: 
							tmp=z->jmagn.co[i].re;
							tmpe=z->jelek.co[i].re;
							break;
						case 2: 
							tmp=z->jmagn.co[i].im;
							tmpe=z->jelek.co[i].im;
							break;
						default:
							tmp=hypot(z->jmagn.co[i].re,z->jmagn.co[i].im);
							tmpe=z->jmagn.co[i].re*z->jelek.co[i].re + z->jmagn.co[i].im*z->jelek.co[i].im;
							break;
					}

					tmp=fabs(tmp);
					tmpe=fabs(tmpe)/maxe/seglen; /* Dichte zeichnen! */

					p1.y=origy-(float) height * tmpe;
					p1.x+=dash/2;  /* Das Kurvenbildchen um ein halbes Segment nach rechts*/
					PutPBuffer(xbuf2,p1);
					p1.x-=dash/2;

				} else {
					/* if real then draw signed quantity */
					tmp=*(z->jmagn.re+i);
					/* in electrostatic case we want the charge density */
					if ( z->freq <= FREQLIM ) {
						tmp/=seglen;
					}
				}

				p.y=origy- (float)height*tmp/max;

				p.x+=dash/2;  /* Das Kurvenbildchen um ein halbes Segment nach rechts*/
				PutPBuffer(xbuf1,p);
				p.x-=dash/2;

				if (j==0) { /* senkrechte Linien nach jeder Kante; Kantennr.*/
					if ( koppflag_local ) {
						XDrawLine(disp,drwbl,gc,p.x,origy,p.x,origy-height);
					} else {
						/* signed picture; draw lines across whole area */
						XDrawLine(disp,drwbl,gc,p.x,2*height,p.x,0);
					}
					if (txty>0) {
						sprintf(num,"%u",n);
						x_i=irint(p.x+ptr->segs*dashf/2.);
						XDrawString(disp,drwbl,gc,x_i,origy+10,
								num,strlen(num));
					}
					if (firstp.x==-1) {
						firstp=p; firstp1=p1;
					}
				}
				x_f+=dashf;
				p.x=p1.x=origx+(short)irint(x_f);
				i++;
			}
			n++;
			ptr=NEXT(ptr);
		}
		if (firstp.x!=-1) { /* existiert die Struktur? */
			/* Eine Struktur ist zuende; die Stromkurve schliessen */
			p.y=(p.y+firstp.y)/2;
			PutPBuffer(xbuf1,p);
			FlushPBuffer(xbuf1);
			XDrawLine(disp,drwbl,gc,firstp.x,p.y,firstp.x+dash/2,firstp.y);
			if (koppflag_local) {
				p1.y=(p1.y+firstp1.y)/2;
				PutPBuffer(xbuf2,p1);
				FlushPBuffer(xbuf2);
				XDrawLine(disp,drwbl,gcxor,firstp1.x,p1.y,firstp1.x+dash/2,firstp1.y);
			}


		}
	} /* for bordernr*/

	/*p.y=ORIG1Y- (float)(ORIG1Y-30)*
	  hypot(z->jmagn.co[0].re , z->jmagn.co[0].im)/max;
	  tmp=(float) atan2pi((double) z->jmagn.co[0].im,
	  (double) z->jmagn.co[0].re)/2;
	  if (tmp<0) tmp+=1;
	  p1.y=ORIG1Y-(float)(ORIG1Y-30) * tmp;
	 */


	XDrawLine(disp,drwbl,gc,p.x,origy,p.x,origy-height);

	XSetLineAttributes(disp,gcxor,0,LineSolid,CapButt,JoinRound);
	XSetFunction(disp,gcxor,GXinvert);
}

/* Elektrischen Strom aus dem magnetischen berechnen */

static void calc_jelek(p1,gmat,dim)

Zeichnung p1;
complex *gmat;
int dim;

{
	register int i;
	register complex *jmag, *jelek;
	register double Ibet2=0.;
	int dim_dim;
	float fakL = fakLCalc(p1);

	dim_dim=dim*(dim-1);
	jmag=p1->jmagn.co;
	jelek=p1->jelek.co;

	for (i=0; i<dim; i++){
		register complex *gmatp;
		register double sumre,sumim;
		register complex *uptr;

		sumre=sumim=0.;
		gmatp=&gmat[dim_dim+i];
		for (uptr=&jmag[dim-1];uptr>=jmag; uptr--) {
			sumre+=uptr->re * gmatp->re - uptr->im * gmatp->im;
			sumim+=uptr->re * gmatp->im + uptr->im * gmatp->re;
			gmatp-=dim;
		}
		jelek[i].re=-sumre*fakL;
		jelek[i].im=-sumim*fakL;
		Ibet2+=sumre*sumre+sumim*sumim;
	}

	p1->Ibet2=Ibet2*fakL*fakL;
}

void Kopp(wid,p1)

Widget    wid; /* where to popup notices */
Zeichnung p1;

{

	int k,l,speiseindex;

	float   fak,freq,epsi,mu,hsubst;
	float   translen,min;

	register float kL,fakL,segilen,kL2hsubst;
	register double Yre=0.,Yim=0.;

	register int dim,i,j,koppflag_local=(p1->jelek.raw!=0);

	Polygon *borderi, *borderj;

	register Punkt *ptri,*ptrj;

	char stri[100];


	if (!ST_VALID(p1,ST_STRM)) {
		printmessg(1,mNeedBordVolt,wid);
		goto errorret ;
	}

	koppinvalid(p1);

	fak=p1->fak;
	freq=p1->freq;
	epsi=p1->epsi;
	mu=p1->mu;
	hsubst=p1->hsubst;

	/* alle geometrischen L"angen sind
	 * mit fak skaliert!
	 */

	/* Konstanten berechnen */

	kL=2.*PI*freq/C_VELOC/fak;		/* Wellenzahl in Luft */


	kL2hsubst=kL*2*hsubst*fak;		/* Spiegelbild ber"ucksichtigen */

	fakL= 2./4./PI*kL*kL/Z_0;

	j=i=0;


	dim=p1->nstrom;
	speiseindex=p1->speiseindex;

	/*  Bereichskontrolle f"ur Translationsvektor */

	translen=hypot((double)p1->koppdist.x,(double)p1->koppdist.y);
	min=translen+1.;

	for (borderi=(Polygon*)p1->borders.firstel;
			borderi; borderi=NEXTPOLY(borderi)){

		ptri=PLIST(borderi);

		for (ptri=PLIST(borderi);ptri;ptri=NEXT(ptri)){
			if (ISSTIFT(ptri)) {
				continue;
			}
			if (ptri->len<min) min=ptri->len;

		}
	}

	if (translen<min) {
		switch( Notice(wid,0,
					mWrnOverlap,
					mAckCont,0,
					mAckIgnore,1,
					0)){
			case 1:  break;
			default: goto errorret;
		}
	}


	BusyButtons(buttonlist,True);
	if (setjmp(env)) {
		/* Kopp wurde unterbrochen */
		koppinvalid(p1);
		goto errorret;
	}

	sprintf(stri,fBusyCplAdm);
	printmessg(0,stri,outputlist[MSSG_M].t_wid);
	enable_interrupt(0,500000,wid,&env[0],0);


	i=0;
	for (borderi=(Polygon*) p1->borders.firstel;
			borderi; borderi=NEXTPOLY(borderi)){



		ptri=PLIST(borderi);

		if (ISSTIFT(ptri)) {
			if (ptri==p1->speiseseg) i++;
			continue; /* Speisestifte strahlen nicht */
		}

		while (ptri!=NULL) {
			register float richtix,richtiy,rix,riy;

			richtix=ptri->richtung.x/(float)ptri->segs;
			richtiy= - (ptri->richtung.y/(float)ptri->segs); 
			/* XWindows Y-Koordinaten sind gespiegelt */
			rix=(ptri->ort.x+p1->koppdist.x)-richtix/2.;
			riy= -(ptri->ort.y+p1->koppdist.y)-richtiy/2.; /* spiegeln */

			segilen=ptri->len/(float)ptri->segs;

			for(k=0;k<ptri->segs;k++) {
				float	Uire,Uiim;

				rix+=richtix;
				riy+=richtiy;

				if (koppflag_local) {
					Uire=p1->jmagn.co[i].re;
					Uiim=p1->jmagn.co[i].im;
				} else {
					Uire=0;
					Uiim=p1->jmagn.re[i];
				}

				/* Die Schleife "uber j l"auft solange j<i */
				j=0;
				for (borderj=(Polygon*) p1->borders.firstel;
						borderj; borderj=NEXTPOLY(borderj)){

					ptrj=PLIST(borderj);

					if (ISSTIFT(ptrj)) {
						if (ptrj==p1->speiseseg) j++;
						continue; /* Speisestifte strahlen nicht */
					}

					while (ptrj!=NULL) {
						register float segjlen,
						/* Segmentvektor (Egj*segjlen) */
						richtjx,richtjy,
						/* Vektor vom Auf- zum Quellpunkt Ri-Rj */
						rdelx,rdely,          
						/* Skalarprodukte Edelij.Richti etc. = Edelij.Egi segilen ...
						 */
						RdelRichti,RdelRichtj,
						RdelNormi, 
						RichtiRichtj,RichtjNormi;



						richtjx= ptrj->richtung.x/(float)ptrj->segs;
						richtjy= -  (ptrj->richtung.y/(float)ptrj->segs); /* spiegeln */
						rdelx= rix - (  ptrj->ort.x  - richtjx/2.);
						rdely= riy - (- ptrj->ort.y  - richtjy/2.); /* ort.y spiegeln */

						RdelRichti=rdelx*richtix+rdely*richtiy;
						RdelRichtj=rdelx*richtjx+rdely*richtjy;

						RichtiRichtj=richtix*richtjx+richtiy*richtjy;
						RichtjNormi =richtjx*richtiy-richtjy*richtix;

						RdelNormi =rdelx*richtiy-rdely*richtix;

						segjlen=ptrj->len/(float)ptrj->segs;

						for (l=0; l<ptrj->segs; l++) {
							register float kLrij,rij,EdelRichtiEdelRichtj;
							register float gmatijre,gmatijim,Qre,Qim;
							/* (Edelij.Egi)(Edelij.Egj) */

							rdelx-=richtjx;
							rdely-=richtjy;

							RdelRichti-=RichtiRichtj;
							RdelRichtj-=segjlen*segjlen;

							RdelNormi -=RichtjNormi;

							/* 
							   es wurde "uber das mit i indizierte Segment integriert! 
							 */

							rij=hypotf(rdelx,rdely);

							EdelRichtiEdelRichtj=RdelRichti*RdelRichtj/(rij*rij);

							kLrij=kL*rij;


#ifndef NAHKOP_GRENZE
							if (kLrij<grenz) { 
#else
								if (kLrij<NAHKOP_GRENZE) {  /* } (f"ur vi) */
#endif
									register float uo,c;


								complex        res1L,res2L;


								uo=RdelRichti/segilen;
								c= RdelNormi/segilen;


								nahkop_aussen(kL*uo,kL*c,kL2hsubst,kL*segilen,&res1L,&res2L);
								gmatijre=
									1/kL2hsubst*(  RichtiRichtj * res1L.re 
											+ RichtjNormi  * res2L.re);
								gmatijim=
									1/kL2hsubst*(  RichtiRichtj * res1L.im 
											+ RichtjNormi  * res2L.im);


								/*  Y=U] [Y] U]
								 *   = Uj Yij Ui
								 *  da ij vertauscht waren (Fortran in Strom())
								 *
								 * --> (Uj Ui) Yij + (Ui Uj) Yji = Q (Yij+Yji)
								 *
								 *  mit Q=Uj Ui.
								 */



							}
							else {
								/* hier werden die "ausseren Kopplungen f"ur entfernte Elemente berechnet */
								register float tmpre,tmpim,a;

								a=( RichtiRichtj - EdelRichtiEdelRichtj); 
								INTERPOL2(tmpre,tmpim,Gtab,kLrij,a);

								a-=EdelRichtiEdelRichtj;
								a-=EdelRichtiEdelRichtj;

								INTERPOL2(gmatijre,gmatijim,G1tab,kLrij,a);

								gmatijre=(gmatijre+tmpre);
								gmatijim=(gmatijim+tmpim);


								/*  Y=U] [Y] U]
								 *   = Uj Yij Ui
								 *  da ij vertauscht waren (Fortran in Strom())
								 *
								 */

							}

							if (koppflag_local) {
								Qre=p1->jmagn.co[j].re*Uire-p1->jmagn.co[j].im*Uiim;
								Qim=p1->jmagn.co[j].re*Uiim+p1->jmagn.co[j].im*Uire;
								Yre+=Qre*gmatijre - Qim*gmatijim;
								Yim+=Qre*gmatijim + Qim*gmatijre;

							} else {
								Qre =  -Uiim*p1->jmagn.re[j];
								Yre+=   Qre*gmatijre;
								Yim+=   Qre*gmatijim;
							}

							j++;
						} /* for l */

						ptrj=NEXT(ptrj);
					}

				} /* for borderj */


				i++;

			} /* for k */
			ptri=NEXT(ptri);
		}
	} /*for borderi*/



	disable_interrupt();

	/* das Reaktionsintegral ist berechnet
	 */  
	{
		double kwre,kwim,yre,yim,zbet2,y2re,y2im,g11;

		if (koppflag_local) {
			yre=p1->jmagn.co[speiseindex].re;
			yim=p1->jmagn.co[speiseindex].im;
		} else {
			yre=0.;
			yim=p1->jmagn.re[speiseindex];
		}
		zbet2=yre*yre+yim*yim;

		/* Berechnung von 1/Uspeis */
		yre/=  zbet2;
		yim/= -zbet2;

		g11=yre;
		/* 1/Uspeis^2 */
		y2re=yre*yre-yim*yim;
		y2im=2*yre*yim;



		kwre=(float)Yre*fakL;
		kwim=(float)Yim*fakL;

		yre=kwre*y2re-kwim*y2im;
		yim=kwre*y2im+kwim*y2re;

		if (g11!=0.){
			float k=(float)hypot((double)yre,(double)yim)/2.0/g11;
			sprintf(stri,"%.3e %cj %.3e\nK=%.1fdB",
					yre,signbit(yim)?'-':'+',(float)fabs(yim),
					20*log10(k));
		}else{
			sprintf(stri,"%.3e %cj %.3e",yre,signbit(yim)?'-':'+',(float)fabs(yim));
		}
		printmessg(0,stri,outputlist[MSSG_KOPPY].t_wid);
		p1->koppY.re=yre;
		p1->koppY.im=yim;
		NOTIFY_ON(p1,ST_KOPP);
#ifdef VERBOSE
		printf("Reaktionsintegral: %g+j%g\n",kwre,kwim);
		printf("Y12: %s\n",stri);
		if (g11!=0.){
			printf("K = |Y21|/(2*Re{Y11}): %g\n",k,20*log10(k));
		}
#endif

	}


errorret:

	disable_interrupt();


	printmessg(0,0,outputlist[MSSG_M].t_wid);

	BusyButtons(buttonlist,False);


	return;
}

void koppinvalid (p1)
 Zeichnung p1;
{
	if (! ST_VALID(p1,ST_KOPP)) return;
	printmessg(0,0,outputlist[MSSG_KOPPY].t_wid);
	NOTIFY_OFF(p1,ST_KOPP);
}

/* Check the return code of cgefs_ , sgefs_ */
static Boolean errorcheck(wid,name,code,mess)
 Widget wid;
 char *name;
 int code;
 char *mess;
{
	char *form;
	if (code>0) return False;

	switch (code) {
		case -1:
		case -2: form=fWrnDimension; break;

		case -3: form=fWrnPars; break;

		case -4: form=fWrnEqnSolSngErr; break;

		case -10: form=fWrnEqnSolInacc; break;

		default: form=fWrnUnknown;	break;
	}
	sprintf(mess,form,name);
	return True;
}

#ifndef DIREKT
/* multiply real Matrix with complex vector and add an optional offset
 * ( if off != NULL )
 * 
 * The inner product is calculated over the closer spaced
 * index, if closeindex==True, and over the wider spaced one
 * otherwise (the matrix must lie in a contiguous memory area)
 */

static void rMatVek(dim,mat,vek,off,result,closeindex)
int      dim;
float   *mat;
complex *vek, *off, *result;
int	 closeindex;

{
	complex tmp;
	complex czero={0.,0.};
	int     i,j,iinc,jinc,imax,jmax;
	complex *vekp;
	float   *matp;

	if (closeindex) { iinc=dim; jinc=1; }
	else 	 	 { iinc=1; jinc=dim; }
	imax=iinc*dim; jmax=jinc*dim;

	for (i=0; i<imax; i+=iinc) {
		matp=mat+i;
		vekp=vek;
		tmp= (off ? *off++ : czero);
		for (j=jmax; j>0; j-=jinc) {
			tmp.re+= *matp * vekp->re;
			tmp.im+= *matp * vekp->im;
			vekp++;
			matp+=jinc;
		}
		*result++ = tmp;
	}
}

/* the same for a complex Matrix */

static void cMatVek(dim,mat,vek,off,result,closeindex)
int      dim;
complex *mat;
complex *vek, *off, *result;
int	 closeindex;

{
	complex tmp;
	complex czero={0.,0.};
	int     i,j,iinc,jinc,imax,jmax;
	float	 buf1,buf2;
	complex *vekp;
	complex *matp;

	if (closeindex) { iinc=dim; jinc=1; }
	else 	 	 { iinc=1; jinc=dim; }
	imax=iinc*dim; jmax=jinc*dim;

	for (i=0; i<imax; i+=iinc) {
		matp=mat+i;
		vekp=vek;
		tmp= (off ? *off++ : czero);
		for (j=jmax; j>0; j-=jinc) {
			buf1 = matp->re * vekp->re;
			buf2 = matp->im * vekp->im;
			tmp.re+= buf1 - buf2;
			tmp.im+= (matp->re+matp->im)*(vekp->re+vekp->im)-buf1-buf2;
			vekp++;
			matp+=jinc;
		}
		*result++ = tmp;
	}
}

#endif
