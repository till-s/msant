#include "Malloc.h"
#include "basics.h"
#include "punkte.h"
#include "widglist.h"
#include "popup.h"
#include "graf.h"
#include "CBstrom.h"
#include "nahkop.h"
#include "Functab.h"
#include "intr.h"
#include "msant_strings.h"
#include "feld.h"


#include <stdio.h>
#include <math.h>
#include <setjmp.h>
#include <assert.h>

#define NLINES 14
#define BITMAPLEN 32
#define DRAWFIELD_MAXZ 1.e12

/* Globale Variablen zur "Ubergabe von Information
 * an drawpatch()
 */

/* external fortran routines */
void gcontr_();

#define register 

static int   *work=NULL;
static float drawpatch_max,drawpatch_min;
static float *drawpatch_field;

static Pixmap draw_field_to_pixmap();


static jmp_buf env;


void Feld(wid,p1)

Widget wid;
Zeichnung  p1;

{

int m,mmax,n,nmax,j_index,l,speiseseg;
Bool bscratch;

Punkt * ptri;
Polygon *borderj;

int koppflag_local;

float fak1,fak,freq,epsi,mu,hsubst,repst;
float karho;

float BesselFak0,BesselFak1;

float *Vre,*Vim;
float minRe,maxRe,minIm,maxIm,maxAbs;


float segilen;
register float kL,ka,krix,kriy;

int   rix,riy;

Punkt *ptrj;
Polygon *speisePoly;
char  mess[128];

/* lokale Variablen initialisieren */

fak=p1->fak;
freq=p1->freq;
epsi=p1->epsi;
/* reciprocal of eps */
repst = 1.0/(EPSI_0*epsi);
mu=p1->mu;
hsubst=p1->hsubst;

if ( hsubst == 0. ) {
	/* dummy-Wert */
	hsubst = 1.;
}
	
kL=2.*PI*freq/C_VELOC/fak;
ka=kL*sqrt(epsi*mu);

/* For electrostatic calculation avoid ka==0 */
if ( p1->freq <= FREQLIM/2. )
	ka = 1./fak;
fak1= Z_0*sqrt(mu/epsi)*hsubst*ka*fak;

koppflag_local= (p1->jelek.raw != NULL);

speisePoly = getSpeisePoly(p1);

ptri=p1->speiseseg;

if (ISSTIFT(ptri)) { /* Stiftspeisung */

  karho=ka*ptri->len;
  if ( freq <= FREQLIM ) {
	BesselFak1= log(karho);
  } else {
  	BesselFak0= j0(karho);
  	BesselFak1=-2*PI*karho*j1(karho);
  }
}

speiseseg=p1->speiseindex;

segilen= (freq<=FREQLIM ? 2./getShortestSeg(p1) : freq*sqrt(epsi*mu)*40./fak/3.e8);

assert ( segilen > 0. );

/* altes Feldbild loswerden */
destroyfield(p1);

if (p1->pregion==0) PatchRegion(p1);
p1->mmax=mmax=(p1->rmax.x-p1->rmin.x)*segilen;
p1->nmax=nmax=(p1->rmax.y-p1->rmin.y)*segilen;
printf("MMAX/NMAX: %i %i\n",mmax,nmax);

/* Speicher f"ur das Feld anfordern */

       if (p1->Vim==NULL) Vim=(float *)Malloc(mmax*nmax*sizeof(float));
        else          Vim=(float *)Realloc(p1->Vim,mmax*nmax*sizeof(float));
       
       if ((p1->Vim=Vim)==NULL){
	 sprintf(mess,fNoMem,"'Vim'");
         printmessg(1,mess,0);
         goto errorret;}

       if (koppflag_local) {
        if (p1->Vre==NULL) Vre=(float *)Malloc(mmax*nmax*sizeof(float));
         else          Vre=(float *)Realloc(p1->Vre,mmax*nmax*sizeof(float));
       
        if ((p1->Vre=Vre)==NULL){
	  sprintf(mess,fNoMem,"Vre");
          printmessg(1,mess,0);
          goto errorret;}
       }



/* Berechnung des Feldes unter dem Patch */

/* FIXME: should use math.h magic to set these */
     minRe=minIm= 1.E38;
     maxRe=maxIm=-1.E38;
	 maxAbs=0.;
     m=0;


       printmessg(0,mBusyCalcField,
                  outputlist[MSSG_M].t_wid);

/* Falls gekoppelte Rechnung, und kein Speisestift, den
 * Speisestrom dem von aussen einfallenden H-Feld zu-
 * schlagen.
 */
        
if (koppflag_local && ! ISSTIFT(ptri)) {
 p1->jelek.co[speiseseg].re-=SPEISESTROM;
}

if (setjmp(env)) {
 /* Feld wurde unterbrochen */
 if (koppflag_local && ! ISSTIFT(ptri)) {
  p1->jelek.co[speiseseg].re+=SPEISESTROM;
 }
 goto errorret;
}

enable_interrupt(0,500000,wid,&env[0],0);
 


for (m=0;m<mmax;m++) {
  rix=p1->rmin.x+irint((float)(p1->rmax.x-p1->rmin.x)/(float)mmax*(m+0.5));
  krix=ka*rix;

for (n=0;n<nmax;n++) {
  register float vre,vim,ure,uim,ire,iim;
  
  riy= - (p1->rmin.y+irint((float)(p1->rmax.y-p1->rmin.y)/(float)nmax*(n+0.5)));
  kriy= ka*riy;
  /* spiegeln */
 

/* Berechnung des Feldes im Punkte rix,riy */
/* ptri, k zeigen aufs Speisetor */

j_index=0;
ire=iim=ure=uim=0;



block();
bscratch=XPointInRegion(p1->pregion,rix,-riy);
unblock(1);

if (bscratch)  { /* Y-Koord. im X_System */

for (borderj=(Polygon*)p1->borders.firstel;
     borderj;
     borderj=NEXTPOLY(borderj)) {
ptrj=PLIST(borderj);

if ( ISSTIFT(ptrj) ) { /* Stift */
  register float uu,hh,rij,krij,v,v2;

  if (ptrj!=p1->speiseseg) continue;

  rij=hypot((float)(rix-ptrj->ort.x),(float)(riy+ptrj->ort.y)); /* y-spiegeln */

  if (freq <= FREQLIM) {
#if 1
  	if ( freq <= FREQLIM/2. ) {
		uim+=uniform_chrg_static(rij, ptrj->len)*SPEISESTROM*repst;
	} else {
		uim+=uniform_chrg(ka*rij, karho)*SPEISESTROM*repst;
	}
#else
	krij = ka*rij;
 	if ( krij < karho ) {
		v = krij/karho;
		/*
		 * Approximation: constant charge distribution with radius 'sig'
		 *                (gaussian is more complicated). The goal is achieving
		 *                a finite field if we travel through the charge distribution.
		 *
		 * Series expansion of 
		uim += 2.*(j0(krij)*(y1(krho) - y1(krij)*v) + j1(krij)*y0(krij)*v)/karho*SPEISESTROM;

		 C = (-1+2*EulerGamma-2*log(2))/2/%pi = -0.1960570906
		 */
		v2   = v*v;
		uim += 2.*(-0.1960570906*(1-v2) + (BesselFak1 - log(krij)*v2)/PI )*SPEISESTROM;
		uim += v2*y0(krij)*SPEISESTROM;
	} else {
		uim += y0(krij) * SPEISESTROM;
	}
	uim *= repst;
#endif
  } else {
      krij = ka*rij;
	  if (krij<NAHKOP_GRENZE) {
		  if (krij<karho+RANDPUNKT) {
			  /* Randwert verwenden und nicht weiter integrieren */


			  if (koppflag_local) {
				  vim= p1->jmagn.co[j_index].im;
				  vre= p1->jmagn.co[j_index].re;
			  } else {
				  vim= p1->jmagn.re[j_index];
			  }

			  goto  skipintegration;

		  } else {
			  uu=hh=y0(krij);
			  uu*=BesselFak1;
			  hh*=BesselFak0;
		  }
	  } else {
		  LINTERPOL(uu,H0tab,krij);
		  hh=uu*BesselFak0;
		  uu*=BesselFak1;
	  }

	  if (koppflag_local) {
		  ure+=uu*p1->jmagn.co[j_index].re;
		  uim+=uu*p1->jmagn.co[j_index].im;

	  } else {
		  uim+=uu*p1->jmagn.re[j_index];
	  }
	  ire+=-hh*SPEISESTROM;

	  j_index++;
  }
  
  continue;
}
  
while (ptrj!=NULL) {
  register float krichtjx,krichtjy,krdelx,krdely;
  float ksegjlen;
 
  krichtjx=ka*ptrj->richtung.x/(float)ptrj->segs;
  krichtjy=-ka*ptrj->richtung.y/(float)ptrj->segs; /*spiegeln*/



  krdelx=  ka*ptrj->ort.x - krichtjx/2 -krix;
  krdely= -ka*ptrj->ort.y - krichtjy/2 -kriy; /* ort.y spiegeln */


  for (l=0;l<ptrj->segs;l++) {
    register float k2RdelNormj,krij;
    float uu,hh;

    krdelx+=krichtjx;
    krdely+=krichtjy;


    krij     =hypot(krdelx,krdely);
    k2RdelNormj=   (krdelx*krichtjy-krdely*krichtjx);


    if (krij<NAHKOP_GRENZE || freq<=FREQLIM) {

      if (krij<0) {
	  assert( "should never get here" == 0 );

       /* Randwert verwenden und nicht weiter integrieren */

       if (koppflag_local) {
        vim= p1->jmagn.co[j_index].im;
        vre= p1->jmagn.co[j_index].re;
       } else {
        vim= p1->jmagn.re[j_index];
       }
  
       goto  skipintegration;

      }
      
       ksegjlen=ka*ptrj->len/(float)ptrj->segs;

  	if ( freq <= FREQLIM/2. ) {
       /* ka is set to 1/fak on entrance [ugly, I know...] */
       nahkop_innen_static((krdelx*krichtjx+krdely*krichtjy)/ksegjlen,
 	            k2RdelNormj/ksegjlen,
 		    ksegjlen,&uu,&hh);
	} else {
       nahkop_innen((krdelx*krichtjx+krdely*krichtjy)/ksegjlen,
 	            k2RdelNormj/ksegjlen,
 		    ksegjlen,&uu,&hh);
	}


      if (koppflag_local) {
       ure+=uu*p1->jmagn.co[j_index].re;
       uim+=uu*p1->jmagn.co[j_index].im;

       ire+=hh*p1->jelek.co[j_index].re;
       iim+=hh*p1->jelek.co[j_index].im;
	  } else {
		  if ( freq <= FREQLIM ) {
			  ire+=-hh*p1->jmagn.re[j_index];
			  if ( borderj == speisePoly )
			  	uim+=uu*SPEISESTROM;
		  } else {
			  if (j_index==speiseseg) {
				  ire+=-hh*SPEISESTROM;
			  }
			  uim+=uu*p1->jmagn.re[j_index];
		  }
	  }

    } else {

	  assert( freq > FREQLIM );

      LINTERPOL(uu,H1tab,krij);
      uu*= k2RdelNormj/krij;
       
      if (koppflag_local) {
       LINTERPOL(hh,H0tab,krij); 

       ure+=uu*p1->jmagn.co[j_index].re;
       uim+=uu*p1->jmagn.co[j_index].im;

       ire+=hh*p1->jelek.co[j_index].re;
       iim+=hh*p1->jelek.co[j_index].im;
      } else {
       if (j_index==speiseseg) {
        LINTERPOL(hh,H0tab,krij); 
        ire+=-hh*SPEISESTROM;
       }
       uim+=uu*p1->jmagn.re[j_index];
      }
      
    }

  j_index++;
  }
  ptrj=NEXT(ptrj);

  } /* while ptrj */
 } /*for borderj*/

 if ( freq <= FREQLIM )
 	vim=1./(freq<=FREQLIM/2. ? ZEIGEN_0_LN : ZEIGEN_0_Y0) * ( repst*ire - uim );
 else
 	vim=1./ZEIGEN_0_Y0 * ( fak1*ire - uim );

 if (koppflag_local) {
  vre=1./ZEIGEN_0_Y0 * ( - fak1*iim - ure );
 }


skipintegration: ;
  /* Falls der Aufpunkt ein Randpunkt ist, wurden diese direkt eingesetzt! */

} else { /* if rix,riy in Region */
 
 vim=vre=0.;


}

 /* Ende Berechnung des Feldes im Punkte (rix,riy) */


/* Minima und Maxima bilden */
{ register float ftmp;

  if (vim>maxIm) maxIm=vim;
  if (vre>maxRe) maxRe=vre;
  if (vim<minIm) minIm=vim;
  if (vre<minRe) minRe=vre;
  if ((ftmp=hypot(vre,vim))>maxAbs) maxAbs=ftmp;
}


 Vim[n*mmax+m]=vim;
 if (koppflag_local) {
  Vre[n*mmax+m]=vre;
 }

} /*Ende for n */
} /*Ende for m */

p1->maxRe=maxRe;
p1->maxIm=maxIm;
p1->minRe=minRe;
p1->minIm=minIm;
p1->maxAbs=maxAbs;


disable_interrupt();


/* richtige Aussenstromverteilung wieder herstellen */

if (koppflag_local && (NEXT(ptri)!=LAST(ptri))) {
 p1->jelek.co[speiseseg].re+=SPEISESTROM;
}


printmessg(0,0,outputlist[MSSG_M].t_wid);
/* Feld fertig berechnet*/

if (0){ FILE *feil;
feil=popen("plotmtv_single -3d -fg black -bg gray","w");
fprintf(feil,"$DATA=CONTOUR\n");
fprintf(feil,"%%xmin=%g xmax=%g ymin=%g ymax=%g\n",
              (float)p1->rmin.x/(float)fak,(float)p1->rmax.x/(float)fak,
              (float)p1->rmin.y/(float)fak,(float)p1->rmax.y/(float)fak);
fprintf(feil,"%%nx=%i\n",mmax);
fprintf(feil,"%%ny=%i\n",nmax);
fprintf(feil,"%%contstyle=3\n");
fprintf(feil,"%%binary=True\n");
 fwrite((char*)Vim,sizeof(float),mmax*nmax,feil);
if (koppflag_local){
 fprintf(feil,"$DATA=CONTOUR\n");
 fprintf(feil,"%%xmin=%g xmax=%g ymin=%g ymax=%g\n",
              (float)p1->rmin.x/(float)fak,(float)p1->rmax.x/(float)fak,
              (float)p1->rmin.y/(float)fak,(float)p1->rmax.y/(float)fak);
 fprintf(feil,"%%nx=%i\n",mmax);
 fprintf(feil,"%%ny=%i\n",nmax);
 fprintf(feil,"%%contstyle=3\n");
 fprintf(feil,"%%binary=True\n");
 fwrite((char*)Vre,sizeof(float),mmax*nmax,feil);
}
fprintf(feil,"$END\n");
fclose(feil);
}

#ifdef QUARK
{
for (j_index=0;j_index<nmax;j_index++) {
 fprintf(feil,"{%g",(V[j_index*mmax]/max));
 for (i=1;i<mmax;i++) {
  if ((i%8)==0) fprintf(feil,"\n   ");
  fprintf(feil,",%g",( V[j_index*mmax+i]/max));
 }
 fprintf(feil,"},\n");
}
fprintf(feil,"}");
fclose(feil);
}
#endif
 


{ 
int *tmp;

if (work==NULL) tmp=(int *)Malloc(
        ( (2*mmax*nmax*(int)NLINES + BITMAPLEN -1)/BITMAPLEN)*sizeof(int));
else     tmp=(int *)Realloc(work,
        ( (2*mmax*nmax*(int)NLINES + BITMAPLEN -1)/BITMAPLEN)*sizeof(int));

if ((work=tmp)==NULL){
  sprintf(mess,fNoMem,"gcontr_work");
  printmessg(1,mess,0);
  goto errorret;
  }
}


SetCursor(none);

if (koppflag_local) {
  drawpatch_field=Vre;
  drawpatch_max=maxRe;
  drawpatch_min=minRe;
} else {
  drawpatch_field=Vim;
  drawpatch_max=maxIm;
  drawpatch_min=minIm;
}

destroyfield(p1); /*safety*/
p1->field_pix=draw_field_to_pixmap(p1);
NOTIFY_ON(p1,ST_FELD);
SetCursor(none);


CBExpose(0,0,0);

return; /* normal return */

errorret:

disable_interrupt();

if (p1->Vim) {Free((char*)p1->Vim); p1->Vim=(float *)0;}
if (p1->Vre) {Free((char*)p1->Vre); p1->Vre=(float *)0;}
if (work)    {Free((char*)work);   work=(int*)0;}
destroyfield(p1);

 
return;
}
  
void drawpatch(p1)
Zeichnung  p1;

{
if (!ST_VALID(p1,ST_FELD)) {
  if (p1->pregion==(Region)0 || XEmptyRegion(p1->pregion)) return;
  XSetForeground(disp,gcfield,p1->patch_colour);
  XFillRectangle(disp,win,gcfield,
	p1->rmin.x,
	p1->rmin.y,
	(unsigned int)(p1->rmax.x-p1->rmin.x),
	(unsigned int)(p1->rmax.y-p1->rmin.y));
} else {
  XCopyArea(disp,p1->field_pix,win,gcfield,
	0,
	0,
	(unsigned int)(p1->rmax.x-p1->rmin.x),
	(unsigned int)(p1->rmax.y-p1->rmin.y),
	p1->rmin.x,
	p1->rmin.y);
}
}

static void gcline(float*, float*, int*);

static Pixmap draw_field_to_pixmap(p1)
Zeichnung  p1;
{
float cv[NLINES];
int   ncv;
unsigned int wid,hig;
float zmax;
Pixmap pix;

ncv=0;
while (ncv<NLINES) {
 cv[ncv]=drawpatch_min + (float)ncv/(float)(NLINES+1)*(drawpatch_max-drawpatch_min); ncv++;
}

/* 
printf("zmax? \n");
#ifdef DOPPELT
scanf("%lg",&zmax);
#else
scanf("%g",&zmax);
#else
#endif
zmax*=max;
*/

zmax=DRAWFIELD_MAXZ;

/*Dieser Trick sorgt daf"ur, dass Punkte ausserhalb der Strukturen die
  Konturlinien nicht in Verlegenheit bringen, da das Feld dort Null ist.
  Mit unserer Methode werden solche Punkte von gcontr nicht ber"ucksichtigt */

wid=(unsigned int) (p1->rmax.x-p1->rmin.x); 
hig=(unsigned int) (p1->rmax.y-p1->rmin.y); 

pix=XCreatePixmap(disp,win,wid,hig,win_depth);





/* move the clip mask */
XSetClipOrigin(disp,gcfield,-p1->rmin.x,-p1->rmin.y);


/* clear this; alas there's no direct Call */

/* erase everything to the canvas background color */
XSetForeground(disp,gc,p1->canvas_colour);
XFillRectangle(disp,pix,gc,0,0,wid,hig);
XSetForeground(disp,gc,p1->foreground);

/* now draw the patch */
XSetForeground(disp,gcfield,p1->patch_colour);
XFillRectangle(disp,pix,gcfield,0,0,wid,hig);
XSetForeground(disp,gcfield,p1->field_colour);

ResetPBuffer(p1->xbuf1,pix,gcfield);
printmessg(0,mBusyDrawField,
             outputlist[MSSG_M].t_wid);

#ifndef DOPPELT
gcontr_(drawpatch_field,&p1->mmax,&p1->mmax,&p1->nmax,cv,&ncv,&zmax,work,gcline);
#endif

printmessg(0,0,outputlist[MSSG_M].t_wid);
FlushPBuffer(p1->xbuf1);

/*
XSetClipOrigin(disp,gcfield,0,0);
*/
/* No more need for the clip mask as long as the
 * field pixmap is drawn first
 */
XSetClipMask(disp,gcfield,None);
if (p1->pregion) {XDestroyRegion(p1->pregion); p1->pregion=(Region)0;}
return pix;
}

static void gcline(u,v,flag)

float *u, *v;
int   *flag;


{
XPoint p;
short lintyp;
static short acttyp=-1;
static char dashes[]={5,0,
                      6,1,
                      5,1,
                      4,1,
                      3,1,
                      2,1,
                      2,2,
                      1,1,
		      1,2,
                      1,3,
		      1,4,
                      1,5,
		      1,6,
                      1,8};

/* we're drawing relative to the pixmap origin */
p.x=(float)(zeichnung->rmax.x-zeichnung->rmin.x)*(*u-0.5)/(float)zeichnung->mmax;
p.y=(float)(zeichnung->rmax.y-zeichnung->rmin.y)*(*v-0.5)/(float)zeichnung->nmax;

if ((lintyp=(*flag/10-1)*2)<0) return;
if (lintyp!=acttyp) {
 XSetDashes(disp,gcfield,0,&dashes[lintyp],2);
 acttyp=lintyp;
}

switch (*flag % 10) {
  case 3:
  case 2: FlushPBuffer(zeichnung->xbuf1);
  case 1: PutPBuffer(zeichnung->xbuf1,p); break;
  case 4:
  case 5: PutPBuffer(zeichnung->xbuf1,p);
          FlushPBuffer(zeichnung->xbuf1); break;
}
}

void destroyfield(p1)
Zeichnung p1;
{
 if (!ST_VALID(p1,ST_FELD)) return;
 if (p1->field_pix) {
   XFreePixmap(disp,p1->field_pix);
   p1->field_pix=None;
   NOTIFY_OFF(p1,ST_FELD);
   SetCursor(none);
 }
}
