#ifndef graf_h
#define graf_h


/* Exportierte Initialisierungsprozeduren */
void init_after_wids(/* Widget wid; int argc; char **argv; */);
void init_before_realize(/*Widget wid */);

/* Exportierte Callbacks */

void CBExpose();

/* exportierte Zeichenprozeduren */

/* Grafikfenster l"oschen, und Koordinaten zeichnen */

void ClearScr();

/*Punktliste, und einen noch nicht gespeicherten Punkt
 *zeichnen. Dieser Extrapunkt ist in die selektierte Kante
 *einzuf"ugen (insert/add-mode). Ein negativer x-Wert zeigt an,
 *dass kein solcher Punkt gezeichnet werden soll.
 */

void drawlist(/*Zeichnung z; int x,y*/);

/* Kantenbeschriftung zeichnen */

void drawnums(/* Zeichnung z;*/);

/* Die Segmentierungspunkte zeichnen */

void drawpoints(/* Zeichnung z;*/);

/* Operationen mit Zeichnungen */

/* Makro zum Rastern von Koordinaten */
#define RASTER(X,R) ((2*((X)%(R))<(R))?(X)-(X)%(R):(X)-(X)%(R)+(R))

/* eine Selektion l"oschen */
void unselect(/* Zeichnung z; Selection *sel */);

/* alle Selektionen l"oschen */
void unselectall(/* Zeichnung z */);

/* ist mind. eine Seletion vorhanden? 
 * gibt 1 zur"uck, falls nichts selektiert ist, 0 sonst
 */

int noselection(/*Zeichnung z*/);

/* eine Zeichnung als modifiziert markieren und alle
 * erforderlichen Anpassungen vornehmen
 */

void modified(/*Zeichnung z*/);

/* Cursor f"ur einen Grafikmodus setzten */
void SetCursor(/* modes mode */);

/* Gibt 1 zur"uck, falls Programmende zugelassen */
int CheckIfQuit(/*Widget w, Zeichnung p1*/);

/* actions registrieren */
void AddGrafActions(/* void */);

/* Defaultwerte */
#define ORIGX 30   /*Ursprung Zeichenfenster (in Pix.)*/
#define ORIGY 30

#define STUBH 500  /*Abmessungen Zeichenfenster (in Pix.)*/
#define STUBW 500 

#define OFFSET 20 /* Rand um Strom Diagramm */

/* eine dem patch entsprechende Region erzeugen
 * (mit num/denom skaliert).
 *
 */

Region ScaledRegion(/*Zeichnung p1, int num, int denom*/);

/* eine mit 1 skalierte Region erzeugen und in z->pregion 
 * speichern
 */
void PatchRegion(/*Zeichnung p1*/);
#endif  
