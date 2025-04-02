#include <stdio.h>

#include "Malloc.h"

#include "lists.h"
#include "msant_strings.h"

#define BELLCHR 7


/* neuel hinter pp in die liste data einf"ugen; wenn pp==0
   an den listenanfang einf"ugen */

void getpo(data,pp,neuel)

liste * data;
listel *pp, *neuel;

{
if (pp==NULL){ /* am Anfang anf"ugen */
  neuel->nextel=data->firstel;
  data->firstel=neuel;
}
else {
 neuel->nextel=pp->nextel;
 pp->nextel=neuel;
}
 neuel->prevel=pp;
 if (data->lastel==pp) {
  data->lastel=neuel;
 } else { /* sicher mind. ein El. hinter nextel */
  neuel->nextel->prevel=neuel;
 }
}


/* Element auf das 'pp' zeigt, in die 'frei' Liste aufnehmen 
   normalerweise zeigt 'pp' nach dem freigeben auf das vorhergehende
   Element. Gibt es jedoch kein solches, so zeigt 'pp' auf das 
   NACHFOLGENDE. 
   
   Falls das erste oder letzte Element entfernt wird, m"ussen
   Listen-Anfang oder -Ende umgesetzt werden */


listel * frepo(data,pp)

liste *data;
listel **pp;

{
listel *tmp;

tmp=*pp;

if (tmp==0) return(0);

/* letztes element? */
if (tmp->nextel!=NULL) tmp->nextel->prevel=tmp->prevel;
 else data->lastel=tmp->prevel; /*Listenende umsetzten*/

/* erstes Element? */
if (tmp->prevel!=NULL) {
  tmp->prevel->nextel=tmp->nextel;
  *pp=tmp->prevel;}
 else{
  *pp=tmp->nextel;
  data->firstel=tmp->nextel;  /*Listenanfang umsetzten */
 }
return(tmp);
} 

int CreateEnqListEl(list,pos,neu,size)
liste *list;
int size;
listel *pos, **neu;
{
if ((*neu=(listel*)Malloc(size))==0) {
  /* Es hat keinen Sinn, ein Notice aufzurufen, 
   * da dieses auch Speicher anfordert
   */
  fprintf(stderr,"%c",BELLCHR);
  fprintf(stderr,fNoMem,"CreateEnqListEl");
  fprintf(stderr,"\n");
  return(1);
}
getpo(list,pos,*neu);
return(0);
}

/* Ein Listenelement (beliebigen Typs) in eine
 * "Undel Liste" aufnehmen; Diese ist als Stack
 * implementiert; falls die maximale Tiefe erreicht ist,
 * wird das "alteste Element vernichtet.
 * Gibt -1 zur"uck, falls kein neues undo-Listenelement
 * erzeugt werden konnte, 1 falls der Undo Puffer voll war,
 * und 0 falls alles OK
 * wenn der zeiger auf den UndelBuffer ==0 ist, wird
 * 'hart' gel"oscht
 */

int RemoveToUndel(undel,del_from,elem,undo_side_effect_proc,
                  user_data, destroy_proc)

UndelBuffer *undel;
liste       *del_from;
listel      **elem;
void (*undo_side_effect_proc)();
char    *user_data;
void (*destroy_proc)();

{
UndelEl *neu;
int rval;

rval=0;
if (undel==0) {
  destroy_proc(frepo(del_from,elem));
  return(rval);
}

/* alles bis zum maximum l"oschen*/
while (undel->size>=undel->max && undel->size!=0) {
  ((UndelEl*)undel->buff.firstel)->destroy(((UndelEl*)undel->buff.firstel)->elem);
  Free(frepo(&undel->buff,&undel->buff.firstel));
  rval=1;
  undel->size--;
}
/* hier ist entweder size<max, oder size==0 (oder beides)
 * der erste Fall ist normal zu behandeln, der Fall
 * size==0 und max==0 erfordert hartes l"oschen
 */

if (undel->max==0) {
  destroy_proc(frepo(del_from,elem));
  return(rval);
}
  
if (CreateEnqListEl(&undel->buff,undel->buff.lastel,
                &neu,sizeof(UndelEl))) return(-1);

neu->deleted_from=del_from;
neu->position=(*elem)->prevel;
neu->elem=frepo(del_from,elem);
(undel->size)++;
neu->undo_side_effect=undo_side_effect_proc;
neu->user_data=user_data;
neu->destroy=destroy_proc;
return(rval);
}

int Undel(udb)

UndelBuffer *udb;

{
UndelEl *undel;
if (udb->size==0) {
 if (udb->buff.firstel!=0 || udb->buff.lastel!=0) {
  fprintf(stderr,"Fatal error: undel-list scrambled!\n");
  exit(-1);
 }
 return(0);
}
undel=(UndelEl*)frepo(&(udb->buff),&udb->buff.lastel);
(udb->size)--;
getpo(undel->deleted_from,undel->position,undel->elem);

if (undel->undo_side_effect)
 undel->undo_side_effect(undel->user_data,undel->elem);

Free(undel);
return(1);
}

