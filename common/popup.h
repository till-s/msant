#ifndef popup_h
#define popup_h

#include <stdarg.h>

typedef struct notice_data_ *NoticeInstance;

/* Anzahl der momentan aufgepoppten Notices 
 * (soll nicht beschrieben werden)
 */

extern int notices_up;

int Notice(Widget top,...);

/* Popup widget: Aufruf mit:
 *
 *   Notice( Widget w, "name", "Text", "Button1text",button1val, ... , 0);
 *
 *   schellt beim Popup in der N"ahe des Widget w!
 */

/* ein Notice_Widget erzeugen (gleiche Parameter wie Notice) */

NoticeInstance CreateNotice(Widget top,...);

/* und Vernichten */

void DestroyNotice(/* NoticeInstance ndp */);

/* Hochbringen/Niederlegen; gleichzeitig wird notices_up um eins
 * erh"oht bzw. erniedrigt und erlaubt somit einen globalen Test, ob
 * irgendwo Notices aktiv sind.
 */

void NoticePopup(/*NoticeInstance ndp;*/);
void NoticePopdown(/*NoticeInstance ndp;*/);

void PlaceNotice(/* NoticeInstance ndp,emanate; */);

/* Test des R"uckgabewertes */

Boolean NoticeDone(/* NoticeInstance ndp */);
int	NoticeRval(/* NoticeInstance ndp */);

#endif
