#ifndef LISTS_H
#define LISTS_H

typedef struct listel_ {
 struct listel_ *nextel, *prevel;} listel;

typedef struct liste_ {
 listel *firstel, *lastel;} liste;

typedef struct UndelEl_ {
  listel zeigers;
  liste  *deleted_from;
  listel *position;
  listel *elem;
  void (*undo_side_effect)();
  char   *user_data;

 /* Funktion, welche beim undeleten mit den Argumenten
  *
  *  undo_side_effect(user_data,elem);
  *
  * aufgerufen wird, um Seiteneffekte r"uckg"angig 
  * machen zu k"onnen.
  */

  void  (*destroy)(); /* Zerst"orungsmethode */
} UndelEl; 


typedef struct UndelBuffer_ {
  liste buff;
  int   size,max;
} UndelBuffer;



#define NEXTUNDEL(undelptr) ((UndelEl *) ((undelptr)->zeigers).nextel)
#define LASTUNDEL(undelptr) ((UndelEl *) ((undelptr)->zeigers).prevel)


  /* Element neuel an der Stelle *pp in die Liste einf"ugen; 
   * Zeiger auf den 
   * letzten und ersten Punkt in der Liste angleichen;
   */

void getpo(/* liste * poly; listel *pp; listel *neuel */);


  /* Element auf das 'pp' zeigt aus der Liste entfernen
   * normalerweise zeigt 'pp' nach dem freigeben auf das nachfolgende
   * Element.
   
   * Falls das erste oder letzte Element entfernt wird, m"ussen
   * Listen-Anfang oder -Ende umgesetzt werden 
   * das entfernte Element zum freigeben desselben wird zur"uckgegeben.
   */


listel * frepo(/* liste *list; listel **pp; listel **alt; */);

/* Ein Element der Gr"osse size erzeugen, und an der Stelle pos
 * in die Liste list einf"ugen. Der Zeiger neu wird auf das
 * neue Element gesetzt.
 * ungen"ugender Speicher wird auf stderr angezeigt
 */

int CreateEnqListEl(/* liste *list,listel* pos, listel **neu, int size*/);

/* Ein Listenelement (beliebigen Typs) in eine
 * "Undel Liste" aufnehmen; Diese ist als Stack
 * implementiert; falls die maximale Tiefe erreicht ist,
 * wird das "alteste Element vernichtet.
 * Gibt -1 zur"uck, falls kein neues undo-Listenelement
 * erzeugt werden konnte, 1 falls der Undo Puffer voll war,
 * und 0 falls alles OK
 * wenn der zeiger auf den UndelBuffer ==0 ist, wird
 * 'hart' gel"oscht. In destroy_proc() wird eine Zerst"orungs
 * methode "ubergeben.
 */

int RemoveToUndel(/*undel,del_from,elem,undo_side_effect_proc,
                    user_data, destroy_proc*/);
/*
 * UndelBuffer *undel;
 * liste       *del_from;
 * listel      **elem;
 * void (*undo_side_effect_proc)();
 * char         *user_data;
 * void (*destroy_proc)();
 */

/* gibt 1 zur"uck, falls Undel m"oglich, 0 wenn der undelbuffer leer ist
 */
int Undel(/*UndelBuffer *undel*/);
#endif
