#include <sys/time.h>
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include "intr.h"
#include "popup.h"
#include "msant_strings.h"

/*
#define INTR_DEBUG
*/

/* Implementierung der M"oglichkeit zur
 * Unterbrechung l"angerer Berechnungen.
 * 
 * Vor einer solchen Berechnung wird enable_interrupt()
 * unter Angabe des Kontextes _vor_ der Berechnung auf-
 * gerufen ('setjmp()'). Falls die Berechnung vom Benutzer
 * unterbrochen wurde, wird wieder zu diesem Kontext zur"uck-
 * gesprungen.
 *
 * Durch enable_interrupt() wird ein timer gestartet, der
 * periodisch die Berechnung unterbricht, um die aufgelaufenen
 * Events abzuarbeiten. Nach einer gewissen Zeit erscheint ein
 * Popup, welches dem Benutzer die Gelegenheit gibt, die
 * seinen Abbruchwunsch zu signalisieren. (Eingabeorientierte
 * Events werden nat"urlich von diesem Popup 'gegrabt'.)
 * Das Popup kann mehrere Buttons aufweisen, um dem Benutzer
 * verschiedene Stati anbieten zu k"onnen. Der R"uckgabewert 
 * des Popups (she. popup.c) wird im Abbruchfalle um eins erh"oht
 * durch longjmp() zur"uckgegeben.
 * 
 * Am Ende der Berechnung wird disable_interrupt() aufgerufen, um
 * das Popup wieder zu entfernen.

 * Kritische Bereiche der Berechnung, welche nicht unterbrochen werden
 * sollen, sind mittels block() / unblock() zu Klammern.
 *
 * Eine Schachtelung von enable_interrupt()/disable_interrupt() ist
 * _NICHT_ vorgesehen.
 */

static void default_handler();

/* 
 * Da X-Applikationen single-threaded sind, ist nur ein einziger
 * Abbruch- und Event-Update Mechanismus sinnvoll. Sein Status kann
 * in globalen Variablen gehalten werden.
 */

static XtAppContext app;
static Display      *disp;

/* Der Status des assoziierten Popups
 */
static NoticeInstance ndp=0;

/* Status:
 *   critical:    Schachtelungstiefe
 *   interrupted: Unterbrechung im kritischen Bereich, ist
 *		  sp"ater zu vollziehen.
 */

static int critical,interrupted;

/* Kontext der an enable_interrupt() "ubergeben wird.
 */
static jmp_buf *jmp_env;

/* M"oglichkeit, den signal handler zu ersetzen
 * (nicht implementiert, z.Zt. private Variable)
 */
static void (*sig_handler)()=default_handler;


void enable_interrupt(interval_sec,interval_usec,wid,env,message)

long interval_sec,interval_usec;
Widget wid;
jmp_buf *env;
char *message;

{
struct itimerval itv;
Widget top;

if (message==NULL) message=mBusyCalc;

/* beim ersten Mal das Notice-Widget erzeugen!*/
if (ndp==0) {
  disp=XtDisplay(wid);
  app=XtDisplayToApplicationContext(disp);
  top=wid; 
  while( top && !XtIsTopLevelShell(top) ) top=XtParent(top);
  ndp=CreateNotice(top,nAbrtNticeName,message,nAbrtNticeName,0,0);
}
PlaceNotice(ndp,wid);

#ifdef INTR_DEBUG
fprintf(stderr,"intr: enable\n");
#endif

XFlush(XtDisplay(wid));
  
itv.it_value.tv_sec=itv.it_interval.tv_sec=interval_sec;
itv.it_value.tv_usec=itv.it_interval.tv_usec=interval_usec;
critical=interrupted=0;
jmp_env=env;
signal(SIGALRM,sig_handler);
setitimer(ITIMER_REAL,&itv,0);
}

void disable_interrupt()

{
sigset_t mask;
struct itimerval itv;

(void)sigemptyset(&mask);
(void)sigaddset(&mask,SIGALRM);
(void)sigprocmask(SIG_BLOCK,&mask,NULL);
#ifdef INTR_DEBUG
fprintf(stderr,"intr: disable\n");
#endif
itv.it_value.tv_sec=itv.it_value.tv_usec=0;
setitimer(ITIMER_REAL,&itv,0);
critical=interrupted=0;
if (ndp) NoticePopdown(ndp);
signal(SIGALRM,SIG_IGN);
(void)sigprocmask(SIG_UNBLOCK,&mask,NULL);
}

static void default_handler(sig)

int sig;

{
/* reinstall */
signal(SIGALRM,sig_handler);
#ifdef INTR_DEBUG
fprintf(stderr,"intr: ALRM(%i)...",sig);
#endif
if ( (critical>0) &&sig) {
#ifdef INTR_DEBUG
  fprintf(stderr,"...critical->leave.\n");
#endif
  interrupted++; return;
}
#ifdef INTR_DEBUG
  fprintf(stderr,"...processing events...\n");
#endif

NoticePopup(ndp);
XFlush(disp);
while (XtAppPending(app)) {
  XtAppProcessEvent(app,XtIMAll);
}
if (NoticeDone(ndp)) {
#ifdef INTR_DEBUG
 fprintf(stderr,"...got usr interrupt request.\n");
#endif
 disable_interrupt();
 longjmp(*jmp_env,NoticeRval(ndp)+1);
}
#ifdef INTR_DEBUG
 fprintf(stderr,"...done.\n");
#endif
}

void block()

{
#ifdef INTR_DEBUG
int i;
#endif

critical++;

#ifdef INTR_DEBUG
 for (i=0; i<critical; i++) fprintf(stderr,".");
 fprintf(stderr,"blocked.\n");
#endif
}

void unblock(sync)

int sync;

{
sigset_t mask;

#ifdef INTR_DEBUG
 int i;
 for (i=0; i<critical; i++) fprintf(stderr,".");
 fprintf(stderr,"unblocked.\n");
#endif

(void)sigemptyset(&mask);
(void)sigaddset(&mask,SIGALRM);
(void)sigprocmask(SIG_BLOCK,&mask,NULL);
if ( (--critical) <= 0) {
 if (critical<0) {
  fprintf(stderr,"Warning from unblock_intr: nothing to unblock\n");
 }
 if (sync&&interrupted) sig_handler(0);
 interrupted=critical=0; 
}
(void)sigprocmask(SIG_UNBLOCK,&mask,NULL);
}


#ifdef QUARK
FILE * open_plot(va_alist)

va_dcl

{ 

va_list argp;

FILE * peip;
char * argv[50];
int  argc, pid;

va_start(argp);
argc=0;

/* Argumentenvektor f"ur plotmtv zusammenstellen */
argv[0]=STRING(PLOTPATH/PLOTPROG); argc++;
argv[1]=STRING(PLOTPROG);          argc++;

while ( argv[argc++]=va_arg(argp,(char*)) );

va_end(argp);

peip=fopen;
}
#endif
