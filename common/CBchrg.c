#include <math.h>
#include <stdio.h>
#include <setjmp.h>

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

/* Tabelleninterpolation , in Functab.h definiert

#define INTERPOL(mat,tab,kr) {\
  register int index; register float delta=kr*TABSEP;\
  index=(int)delta; delta-=index;\
  mat.re=(tab[index].re*(1-(delta))+ tab[index+1].re*delta);\
  mat.im=(tab[index].im*(1-(delta))+ tab[index+1].im*delta);\
 }

*/


#define CLOSEINDEX 1
#define FARINDEX   0

  
void Strom(wid,p1)

Widget    wid; /* where to popup notices */
Zeichnung p1;

{

	int k,l,lda,ipath,acc,speiseindex,speiseidim;

	/* Zeiger f"ur Speicherbl"ocke
	 * (Free)
	 */

	real_complex zmat,wk;
	int     *ipvt;
	float   *imat;


	/* Zeiger */
	real_complex uvek;
	float       *ivek;

	int spspeis,koppflag,ignore;
	static int ignorestiftwarn=0;

	float   zeigen,fak,freq,epsi,mu,hsubst,fakL;
	float   ieigen,geigenre,geigenim;
	float   temp;

	register float kL,ka,fak1FakL,fak1,segilen,kL2hsubst;

	register int dim,idim,i,j;

	Polygon *borderi, *borderj;

	register Punkt *ptri,*ptrj;

	char stri[100];


	zmat.raw=0;imat=0;wk.raw=0;ipvt=0;


	if (p1->speiseseg==NULL) {
		printmessg(1,mNoExcitation,wid);
		goto errorret ;
	}

	/* Parameter einlesen */
	getparams(input_haupt);
	koppflag=getflag(chboxlist[CHBX_KOPPFLAG].t_wid);
	spspeis=0;
	strominvalid(p1);
	destroyfield(p1);
	koppinvalid(p1);


	fak=p1->fak;
	freq=p1->freq;
	epsi=p1->epsi;
	mu=p1->mu;
	hsubst=p1->hsubst;

	/* Parameterpr"ufung */


	if (hsubst==0.) {
		if (koppflag) 
			switch( (int) Notice(wid,0,mWrnSubstHeight,mAckCancel,0,
						mAckNoCoupling,1,
						0)) {
				case 1: koppflag=0; 
						break;
				default: return;
			}
		hsubst=C_VELOC*sqrt(epsi*mu)/freq/100.; 
		/* dummy-Wert */
	}

	if (freq<=0.) {
		Notice(wid,0,mWrnFreqNeg,mAckCont,0,0);
		return;
	}
	if (epsi<1.) {
		Notice(wid,0,mErInvalid,mAckCont,0,0);
		return;
	}
	/* alle geometrischen L"angen sind
	 * mit fak skaliert!
	 */

	/* Konstanten berechnen */

	kL=2.*PI*freq/C_VELOC/fak;		/* Wellenzahl in Luft */

	ka=kL*sqrt(epsi*mu);			/* Wellenzahl im Substrat */

	zeigen=(4.-2.)/ka;			/* Eigenkopplung (innen) */
	fak1=-Z_0*sqrt(mu/epsi)*hsubst*fak;	/* Vorfaktor f"ur Stromvektor */

	kL2hsubst=kL*2*hsubst*fak;		/* Spiegelbild ber"ucksichtigen */

	fakL= 2./4./PI*kL*kL/Z_0;
	fak1FakL=fak1*fakL;			/* Vorfaktor f"ur Z-Matrix */


	ignore=0;

	j=i=0;


	dim=0;

	/*  Abz"ahlen der Segmente */

	for (borderi=(Polygon*)p1->borders.firstel;
			borderi; borderi=NEXTPOLY(borderi)){

		ptri=PLIST(borderi);

		for (ptri=PLIST(borderi);ptri;ptri=NEXT(ptri)) {

			if (ptri==p1->speiseseg) {
				speiseindex=dim + (int)(ptri->speis * ptri->segs);
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
				goto errorret;
			}

			if (!ignore && koppflag &&!ISSTIFT(ptri) && (ptri->len/ptri->segs/fak/hsubst < 1.)) {
				sprintf(stri,
						fWrnH_GT_len,
						j);

				switch ((int)Notice(wid,0,stri,mAckCancel,0,mAckNoCoupling,1,
							mAckIgnore,2,0)) {
					case 1: koppflag=0; break;
					case 2: ignore=1;   break;
					default: goto errorret;
				}
			}

			j++;
			dim+=ptri->segs;
		}
	}

	speiseidim=speiseindex*dim;

	i=sizeof(float)*dim;
	j=sizeof(float)*1;

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

	ivek=imat;
	uvek.re=zmat.re + speiseindex*dim;

	BusyButtons(buttonlist,True);
	if (setjmp(env)) {
		/* Strom wurde unterbrochen */
		strominvalid(p1); goto errorret;
	}

	sprintf(stri,fBusyZMatDim,dim);
	printmessg(0,stri,outputlist[MSSG_M].t_wid);
	enable_interrupt(0,500000,wid,&env[0],0);

	i=0;idim=0;
	for (borderi=(Polygon*) p1->borders.firstel;
			borderi; borderi=NEXTPOLY(borderi)){



		ptri=PLIST(borderi);

		if (ISSTIFT(ptri) && ptri!=p1->speiseseg) continue;

		while (ptri!=NULL) {
			register float richtix,richtiy,rix,riy;

			richtix=ptri->richtung.x/(float)ptri->segs;
			richtiy= - (ptri->richtung.y/(float)ptri->segs); 
			/* XWindows Y-Koordinaten sind gespiegelt */
			rix=ptri->ort.x-richtix/2.;
			riy= -ptri->ort.y-richtiy/2.; /* spiegeln */

			segilen=ptri->len/(float)ptri->segs;

			eigenkop_innen(ka*segilen,&ieigen);

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
					/* Vektor vom Auf- zum Quellpunkt Ri-Rj */
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
								borderj=NEXTPOLY(borderj);
							} while (ISSTIFT(ptrj) && ptrj!=p1->speiseseg);
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

					rij=rdelx*rdelx+rdely*rdely;

					EdelRichtiEdelRichtj=RdelRichti*RdelRichtj/rij;

					rij=sqrt(rij);

					EdelNormi=RdelNormi/rij;
					EdelNormj=RdelNormj/rij;


					kLrij=kL*rij;
					krij =ka*rij;


					{ /* kLrij ist sicher kleiner als krij */
						register float uo,c;
						float          res1,res2;
						complex        res1L,res2L;


						uo=RdelRichti/segilen;
						c= RdelNormi/segilen;


						nahkop_innen(ka*uo,ka*c,ka*segilen,&res1,&res2);


						{
							if (i==speiseindex) {
								uvek.re[j]=res1/ka;
								ivek[j]=res2;
							}else{
								zmat.re[idim+j]=res1/ka;
							}
						}


						/* symmetrisch dazu gelegenes Segment */


						/* bei Vertauschung von i mit j "andert Rdel das Vorzeichen! */

						uo= -RdelRichtj/segjlen; 
						c=  -RdelNormj/segjlen;

						nahkop_innen(ka*uo,ka*c,ka*segjlen,&res1,&res2);



						{
							if (j==speiseindex) {
								uvek.re[i]=res1/ka;
								ivek[i]=res2;
							}else{
								zmat.re[j*dim+i]=res1/ka;
							}
						}


					}
				} /* for j<i */


				/* Eigenkopplung */
				{

						if (i==speiseindex) {
							ivek[i]=ieigen;
							uvek.re[i]=zeigen;
						}else{
							zmat.re[idim+i]=zeigen;
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
		richtjx,richtjy,karij,tmp;

		J0=j0(ka*p1->speiseseg->len);
		r_J1=-2*PI*p1->speiseseg->len*j1(ka*p1->speiseseg->len);
		idim=0;
		borderj=(Polygon*)p1->borders.firstel;
		ptrj=0;
		l=-1;
		rix=p1->speiseseg->ort.x;
		riy=-p1->speiseseg->ort.y; /* spiegeln */

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

			/* Die Eigenkopplungen werden hier 
			 * mit den richtigen Werten "uberschrieben 
			 */
			zmat.re[speiseidim+i]=r_J1 * tmp;
			ivek[i]=J0*tmp;

			idim+=dim;
		}
		zmat.re[speiseidim+speiseindex]+=4./ka;

	}

	block();


	/* alte L"osung freigeben */

	if (p1->jmagn.raw!=NULL) {Free(p1->jmagn.raw); p1->jmagn.raw=0;}
	if (p1->jelek.raw!=NULL) {Free(p1->jelek.raw); p1->jelek.raw=0;}

	lda=dim;


	/* Matrixmultiplikation */
	{

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

		for (i=0; i<dim; i++) ivek[i]*=fak1;
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
	}

	disable_interrupt();

	if (imat){Free(imat); imat=0; ivek=0;}
	if (zmat.raw){Free(zmat.raw); zmat.raw=0;}

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

errorret:

	disable_interrupt();

	printmessg(0,0,outputlist[MSSG_M].t_wid);

	BusyButtons(buttonlist,False);

	/* allozierten Speicher freigeben */
	if (zmat.raw) {Free(zmat.raw);zmat.raw=0;}
	if (imat) {Free(imat);imat=0;}
	if (wk.raw) {Free(wk.raw);wk.raw=0;}
	if (ipvt) {Free(ipvt);ipvt=0;}

	return;
}

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

	XtVaGetValues(w,XtNwidth,&width,XtNheight,&height,0);
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
		if ((ptr=PLIST(border))==ENDE(border)) continue;
		while(ptr!=NULL) {
			ltot+=ptr->len; 
			if (koppflag_local) {
				seglen=ptr->len/(float)ptr->segs;

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
					if (tmp>max) max=tmp;
					i++;
				}
			}
			ptr=NEXT(ptr);
		}
	}

	sprintf(num,fMaxBorderVolt,max);
	if (koppflag_local) {
		ree=z->jmagn.co[z->speiseindex].re;
		imm=z->jmagn.co[z->speiseindex].im;
	} else {
		ree=0.0;
		imm=*(z->jmagn.re+z->speiseindex);
	}
	tmp=ree*ree+imm*imm;

	if (txty>0) {
		sprintf(num+strlen(num),fImpedance,ree,
				signbit(imm)?'-':'+',
				(float)fabs(imm));

		sprintf(num+strlen(num),fAdmittance, ree/tmp,
				signbit(imm)?'+':'-',
				(float)fabs(imm)/tmp);
		XDrawImageString(disp,drwbl,gc,txtx,txty,num,strlen(num));
	}

	delta=((float)width)/((float)ltot);

	n=i=0; 

	p1.x=p.x=origx;
	even=1;

	x_f=0.;


	for (border=(Polygon*)z->borders.firstel; 
			border; border=NEXTPOLY(border)){
		if ((ptr=PLIST(border))==ENDE(border)) continue;
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
					tmp=fabs( *(z->jmagn.re+i));
				}

				p.y=origy- (float)height*tmp/max;

				p.x+=dash/2;  /* Das Kurvenbildchen um ein halbes Segment nach rechts*/
				PutPBuffer(xbuf1,p);
				p.x-=dash/2;

				if (j==0) { /* senkrechte Linien nach jeder Kante; Kantennr.*/
					XDrawLine(disp,drwbl,gc,p.x,origy,p.x,origy-height);
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

