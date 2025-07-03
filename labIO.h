#ifndef labIO
#define labIO
#include "lab.h"

void Show_Lab(laby & L);    // Affiche le labyrinthe
void Verif_Lab(laby & L);   // Vérifie le labyrinthe
void Show_Case(int i , int j, Case c);  // Affiche les bords de la case 
void Mark_Case(int li, int col);    // Affiche une étoile sur la case
void Mark_Case_Pile(pile & p);
void UMark_Case(int li, int col);   // Efface la case
int LireCommande();
void InitCurses();
void EndCurses();

#endif