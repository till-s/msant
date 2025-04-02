#include "fft.h"
#include "malloc.h"

static unsigned  int swpbit(arg,bits)
 unsigned int arg;
 int  bits;
{
 register unsigned int i,rval=0,tmp;
 
 for (i=0;i<sizeof(int)*2;i++) {
  rval=rval<<4;
  switch(tmp=(arg&0xf)) {
	default:  rval+=tmp;  break;
	case 0x1: rval+= 0x8; break;
	case 0x2: rval+= 0x4; break;
	case 0x3: rval+= 0xc; break;
	case 0x4: rval+= 0x2; break;
	case 0x5: rval+= 0xa; break;
	case 0x7: rval+= 0xe; break;
	case 0x8: rval+= 0x1; break;
	case 0xa: rval+= 0x5; break;
	case 0xb: rval+= 0xd; break;
	case 0xc: rval+= 0x3; break;
	case 0xd: rval+= 0xb; break;
	case 0xe: rval+= 0x7; break;
  }
  arg=arg>>4;
 }
 return(rval>>((sizeof(int)<<3)-bits));
}

complex *fft_init(pot,W)
 int pot;
 complex *W;
{
 int N=1<<pot;
 int n;
 double wre0,wim0;
 sincospi((double)2./N,&wim0,&wre0);

 /* Arbeitsspeicher f"ur die Drehfaktoren */
 if (W==0) {
 if ((W=(complex*)malloc(sizeof(complex)*N/2))==0) {
  return(0);
 }
 }
 /* Berechnung der Drehfaktoren */

 W[0].re=1.;W[0].im=0.;

 for (n=1; n<N/2; n++) {
   W[n].re=W[n-1].re*wre0-W[n-1].im*wim0;
   W[n].im=W[n-1].re*wim0+W[n-1].im*wre0;
 }

 return(W);
}

/* Die Sache mit den Drehwinkeln kleiner als 2.Pi/N
 * funktioniert nicht! (Zerst"uckelung des Bildbereichs
 */

int fft(pot,F,f,W)
 int 	 pot;
 complex *F,*f,*W;
{
 int N=1<<pot;
 int error=0,mywk;
 int m,n;
 real   *Wre=0,*Wim=0;
 
 unsigned int delta,index,swind,drehind,drehinc;
 complex tmp;


 if (mywk=(W==0)) if((W=fft_init(pot,0))==0) {
   error=NO_MEM;
   goto errorret;
 }
 

 /* Vertauschen der Matrixelemente,umkopieren in Zielvektor */

  for (index=0; index<N; index++) {
   swind=swpbit(index,pot);
   if (swind>=index) {
	tmp=f[index]; F[index]=f[swind]; F[swind]=tmp;
   }
  }

	
  /* erste Drehung mit festem Faktor 1 */

  swind=1;
  for (index=0; index<N; index+=2) {
   tmp.re=F[index].re+F[swind].re;
   tmp.im=F[index].im+F[swind].im;
   F[swind].re=F[index].re-F[swind].re;
   F[swind].im=F[index].im-F[swind].im;
   F[index]=tmp;
   swind+=2;
  }

  delta=2;
  drehinc=N>>2;
  while (delta<N) {
 
/* ein Transformationsschritt von folgenl"ange delta zu 2*delta */


  n=0;
  while (n<N) {
  swind=delta+n;
  drehind=0;
  for (index=n; index<delta+n; index++) {
   real tmpre,tmpim;

   tmpre = W[drehind].re*F[swind].re-W[drehind].im*F[swind].im;
   tmpim = W[drehind].im*F[swind].re+W[drehind].re*F[swind].im;

   F[swind].re=F[index].re-tmpre;
   F[swind].im=F[index].im-tmpim;
   F[index].re+=tmpre;
   F[index].im+=tmpim;
   swind++;
   drehind+=drehinc;
  }
  n+=delta;
  n+=delta;
  }

  drehinc=drehinc>>1;
  delta=delta<<1;
 }

errorret:
 if (W && mywk) {free(W); W=0;}
 return(error);
}
