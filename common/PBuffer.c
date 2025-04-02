#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>

#include "Malloc.h"
#include "PBuffer.h"

#define PBUFLEN 100


typedef struct {
  Display * PBufferDisp;
  Drawable  PBufferDraw;
  GC        PBufferGC;
  int       max,iptr;
  XPoint    * PBuffer;
  PBMode    mode;
 } Punktbuffer;


PBuffer  InitializePBuffer(disp,win,gc,len,mode)

Display * disp;
Drawable  win;
GC        gc;
unsigned  len;
PBMode    mode;

{
 unsigned maxreq;
 Punktbuffer *  buf;
 unsigned bytes;


 maxreq=(XMaxRequestSize(disp)-3)/2;
 maxreq=(len > maxreq) ? maxreq:len;
 bytes=maxreq*sizeof(XPoint);;
 /*bytes=maxreq*sizeof(XPoint)+sizeof(Display *)
      +sizeof(Drawable)+sizeof(GC)+2*sizeof(int);*/

 if ((buf= (Punktbuffer *) Malloc(sizeof(Punktbuffer)))!=NULL) {
  if ((buf->PBuffer=(XPoint *) Malloc(bytes)) == NULL) {
   Free(buf); return(NULL);}
  buf->PBufferDisp=disp;
  buf->PBufferDraw=win;
  buf->PBufferGC=gc;
  buf->max=maxreq;
  buf->iptr=0;
  buf->mode=mode;
 }
 return((PBuffer)buf);
}

void FreePBuffer(buf)

Punktbuffer * buf;

{
 Free((char *)buf);
}

void ResetPBuffer(buf,win,gc,mode)

Punktbuffer * buf;
Drawable win;
GC        gc;
PBMode   mode;

{
 buf->PBufferDraw=win;
 buf->PBufferGC=gc;
 buf->iptr=0;
 buf->mode=mode;
}

void FlushPBuffer(buf)

Punktbuffer * buf;
{
XDrawLines(buf->PBufferDisp,buf->PBufferDraw,buf->PBufferGC,
           buf->PBuffer,buf->iptr,CoordModeOrigin);
buf->iptr=0;
}

void PutPBuffer(buf,point)

Punktbuffer * buf;
XPoint point;

{
 if (buf->iptr>=buf->max) {
  FlushPBuffer(buf);
  buf->PBuffer[0]=buf->PBuffer[buf->max-1];
  buf->iptr=1;}
 buf->PBuffer[buf->iptr++]=point;
}

unsigned PBufferLen(buf)

Punktbuffer * buf;

{
return(buf->max);
}
