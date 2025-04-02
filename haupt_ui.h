#ifndef HAUPT_UI_H
#define HAUPT_UI_H

Widget CreateHaupt( /*Widget parent*/ );

/* maps a callback to an Action
 * cld is a (String *) pointing to
 * String cld[]={
 *	"ActionName",
 *	"param1",
 *	"etc",
 *	0,
 * };
 */

void   CB_To_Action(/* Widget wid, XtPointer cld,cad */);

#endif
