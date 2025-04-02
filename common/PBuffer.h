#ifndef PBuffer_h
#define PBuffer_h

typedef struct Punktbuffer * PBuffer;
typedef enum {line,point} PBMode;

PBuffer InitializePBuffer();

/*
 * PBuffer InitializePBuffer(disp,win,gc,len,mode)
 *  Display * disp;
 *  Drawable  win;
 *  GC        gc;
 *  unsigned  len;
 *  PBMode    mode;
 */


void     ResetPBuffer();

/* 
 * void ResetPBuffer(buf,win,gc,mode)
 *  Punktbuffer * buf;
 *  Drawable win;
 *  GC        gc;
 *  PBMode    mode;
 */


void     FreePBuffer();

/*
 * void FreePBuffer(buf)
 *  Punktbuffer * buf;
 */



void     FlushPBuffer();

/* 
 * void FlushPBuffer(buf)
 *  Punktbuffer * buf;
 */


void     PutPBuffer();

/* 
 * void PutPBuffer(buf,point)
 *  Punktbuffer * buf;
 *  XPoint point;
 */


unsigned PBufferLen();

/*
 * unsigned PBufferLen(buf)
 *  Punktbuffer * buf;
 */
#endif
