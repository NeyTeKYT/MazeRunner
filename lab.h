#ifndef lab
#define lab

// Case du labyrinthe
struct Case {
	bool N,W,S,E;	// True si la porte est ouverte (possibilité d'y aller), False sinon.
	bool marq; 
	bool etat;	// True si la case a été visitée, False sinon.
};

// Labyrinthe
struct laby {
	int p;
	int q;
	Case ** tab;	// Le labyrinthe est un "tableau de tableau" / une matrice de cases.
};

// Couple de coordonnées
struct couple {
	int i;
	int j;
};

// Pile LIFO (Last-In, First-Out).
struct pile {
	int nbe;
	int taille;
	couple * T;
};

#endif