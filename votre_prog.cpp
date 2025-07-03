#include <ncurses.h>
#include "labIO.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <thread>
#include <chrono>
using namespace std;

/** 
 * @brief 	Initialise une pile.
 *
 * @param p 	La pile à initialiser.
 * @param taille 	Taille à donner pour créer dynamiquement la pile.
 *
*/
void init(pile & p, int taille) {
	p.taille = taille;
	p.nbe = 0;	// Nombre d'éléments dans la pile, par défaut à 0.
	p.T = new couple[taille];	// Création dynamique de la pile.
}

/** 
 * @brief	Empile un couple dans une pile.
 *
 * @param p	La pile sur laquelle on va empiler le couple.
 * @param couple	Le couple à empiler dans la pile.
 *
 * @return	True si le couple a bien été empilé dans pile, False sinon.
*/
bool empiler(pile & p, couple c) {
	if(p.nbe < p.taille) {	// Cas où il est encore possible d'empiler 
		p.T[p.nbe] = c;
		(p.nbe)++;
		return true;
	}
	else {	// Cas où il n'est plus possible d'empiler
		cerr << "La pile est pleine !" << endl;
		exit(-1);
	}
}

/** 
 * @brief 	Dépile un couple de la pile en suivant la logique LIFO (Last-In, First-Out).
 *
 * @param p 	La pile dans laquelle on va y dépiler un couple.
 *
 * @return	Le couple dépilé de la pile.
*/
couple depiler(pile & p) {
	if(p.nbe > 0) {	// Cas où il est encore possible de dépiler un couple de la pile.
		(p.nbe)--;
		return(p.T[p.nbe]);
	}
	else {	// Cas où il n'est plus possible de dépiler (la pile est vide).
		cerr << "La pile est vide !" << endl;
		exit(-1);
	}
}

/** 
 * @brief	Savoir si la pile est vide.
 *
 * @param p	La pile sur laquelle agir.
 *
 * @return	True si la pile est vide, False sinon.
*/
bool vide(pile & p) {
	if(p.nbe == 0) return true; 
	else return false;	// Il y a au moins un couple dans la pile.
}

/** 
 * @brief	Initialise une case.
 *
 * @param c	La case à initialiser.	
 *
*/
void init(Case & c) {
	c.N = c.S = c.E = c.W = false; // Toutes les portes sont fermées.
	c.etat = false; // La case n'a pas encore été visitée.
}

/** 
 * @brief	Initialise un labyrinthe.
 *
 * @param L	Le labyrinthe à initialiser.	
 * @param p
 * @param q
 *
*/
void init(laby & L, int p, int q) {
	L.p = p; 
	L.q = q; 
	L.tab = new Case * [p]; 
	for(int i = 0; i < p; i++) L.tab[i] = new Case[q]; 
	for(int i = 0; i < p; i++) {
		for(int j = 0; j < q; j++) {
			init(L.tab[i][j]);
		}
	}
}

/** 
 * @brief	Lit un labyrinthe à partir d'un fichier.
 *
 * @param filename	Fichier contenant les données pour créer le labyrinthe correspondant.	
 * 
 * @return Le labyrinthe intialisé avec les valeurs du fichier.
 *
*/
laby * lire_Laby(const char * filename) {
	ifstream file;
	file.open(filename);

	laby * L;
	L = new laby;
	file >> (*L).p >> (*L).q;	// Les deux premiers entiers du fichier forment la taille du labyrinthe.
	init((*L) ,(*L).p, (*L).q);

	for(int i = 0; i < (*L).p; i++) {
		for(int j = 0; j < (*L).q; j++) {
			// Pour chaque case, on a 4 entiers correspondants dans l'ordre à la porte Ouest, Nord, Sud, Est.
			file >> (*L).tab[i][j].W >> (*L).tab[i][j].N >> (*L).tab[i][j].S >> (*L).tab[i][j].E;
		}
	}

	return L;
}

/** 
 * @brief	Se déplacer dans le labyrinthe.
 *
 * @param L	Le labyrinthe sur lequel se déplacer.	
 *
 * @return True si le joueur a trouvé la sortie du labyrinthe, False sinon.
 *
*/
bool deplacement(laby & L) {
	bool win = true;
	int i = 0, j = 0;	// Coordonnées de la case de départ
	Mark_Case(i, j);	// Marque la case sur laquelle on se trouve.
	int id = L.p-1, jd = L.q-1; // Coordonnées de la case d'arrivée

	// Boucle tant que le joueur n'a pas trouvé la case d'arrivée.
	while((i != id) || (j != jd)) { 
		
		int x = LireCommande();	// On stocke dans x la touche appuyée par l'utilisateur.
		
		// Touche F1 = Le joueur veut quitter la partie.
		if(x == -1) { 
			win = false;
			break;
		}
		
		// Cas où le joueur veut aller à droite / à l'est.
		if(x == 3) {
			// Si la porte de l'est est ouverte
			if(L.tab[i][j].E) {
				UMark_Case(i, j);	// On enlève la marque de la case actuelle car on va avancer vers la droite.
				j = j + 1;	// Réajustement des coordonnées.
				Mark_Case(i, j);	// Marque la nouvelle case.
			}
			// Cas où le joueur fonce dans un mur.
			else {
				win = false;
				break;
			}
		}
		
		// Cas où le joueur veut aller en bas / vers le sud.
		if(x == 6) {
			// Si la porte vers le sud est ouverte.
			if(L.tab[i][j].S) {
				UMark_Case(i, j);	// On enlève la marque de la case actuelle car on va avancer vers le sud.
				i = i + 1;	// Réajustement des coordonnées.
				Mark_Case(i, j);	// Marque la nouvelle case.
			}
			// Cas où le joueur fonce dans un mur.
			else {
				win = false;
				break;
			}
		}
		
		// Cas où le joueur veut aller vers la gauche / l'ouest.
		if(x == 9) {
			// Si la porte de l'ouest est ouverte
			if(L.tab[i][j].W) {
				UMark_Case(i, j);	// Enlève la marque de la case actuelle car on va avancer vers la gauche.
				j = j - 1;	// Réajustement des coordonnées.
				Mark_Case(i, j);	// Marque la nouvelle case.
			}
			// Cas où le joueur fonce dans un mur.
			else {
				win = false;
				break;
			}
		}
		
		// Cas où le joueur veut aller vers le haut / le nord.
		if(x == 12) {
			// Si la porte de l'ouest est ouverte
			if(L.tab[i][j].N) {
				UMark_Case(i, j);	// Enlève la marque de la case actuelle car on va avancer vers le nord.
				i = i - 1;	// Réajustement des coordonnées.
				Mark_Case(i,j);	// Marque la nouvelle case.
			}
			// Cas où le joueur fonce dans un mur.
			else {
				win = false;
				break;
			}
		}
		
		// Cas où le joueur a appuyé sur une touche qui n'est pas reconnue par le jeu.
		if((x != 3) && (x != 6) && (x!= 9) && (x != 12)) {
			win = false;
			break;
		}
	}

	return win;
}

/** 
 * @brief	Création d'un labyrinthe.
 *
 * @param p
 * @param q
 *
 * @return Le labyrinthe crée.
 *
*/
laby * My_Creation(int p, int q) {
	
	// Création dynamique d'un labyrinthe
	laby * L = new laby; 
	init(* L, p, q);
	
	// Création d'une pile
	pile * P = new pile;
	init(* P, p*q);	// On donne une taille suffisante à la pile 
	
	// Empile la case de départ (0, 0)
	couple * case_depart = new couple; 
	case_depart->i = 0;
	case_depart->j = 0;
	empiler(* P, * case_depart); 
	
	L->tab[case_depart->i][case_depart->j].etat = true;	// Marque la case de départ comme visitée
	
	delete case_depart;
	
	while(!vide(* P)) {
		couple c = depiler(* P);	// Dépile un couple de la pile que nous mettons dans c.
		
		char * T = new char[4]; // Tableau avec les directions possibles à partir de c (maximum 4)
		int nb_directions = 0; // Nombre de directions possibles
		
		// Cas où il est encore possible d'aller vers le nord.
		if(c.i > 0) { 
			if(!L->tab[c.i-1][c.j].etat) {	// Si la case nord n'est pas encore visitée.
				// Alors la case nord est accessible depuis c.
				T[nb_directions] = 'N';
				nb_directions++; 
			}
		}

		// Cas où il est encore possible d'aller vers l'ouest.
		if(c.j > 0) { 
			if(!L->tab[c.i][c.j-1].etat) {	// Si la case ouest n'est pas encore visitée.
				// Alors la case ouest est accessible depuis c.
				T[nb_directions] = 'W';
				nb_directions++; 
			}
		}
		
		// Cas où il est encore possible d'aller vers le sud.
		if(c.i < (L->q-1)) { 
			if(!L->tab[c.i+1][c.j].etat) {	// Si la case sud n'est pas encore visitée.
				// Alors la case sud est accessible depuis c.
				T[nb_directions] = 'S';
				nb_directions++; 
			}
		}
		
		// Cas où il est encore possible d'aller vers l'est.
		if(c.j < (L->p-1)) { 
			if(!L->tab[c.i][c.j+1].etat) {	// Si la case est n'est pas encore visitée.
				// Alors la case est est accessible depuis c.
				T[nb_directions] = 'E';
				nb_directions++; 
			}
		}
		
		// Cas où il y a AU MOINS une direction possible.
		if(nb_directions != 0) { 
			
			int random_direction = rand() % nb_directions;	// Tirage d'une direction au hasard.
			
			// Cas où la direction tirée au sort est le nord.
			if(T[random_direction] == 'N') {
				L->tab[c.i-1][c.j].etat = true;	// Marque la nouvelle case comme visitée.
				
				// Ouverture des portes entre la case actuelle et la nouvelle case.
				L->tab[c.i][c.j].N = true;
				L->tab[c.i-1][c.j].S = true;
				
				empiler(* P, c);	// Réempile la case de départ.
				c.i = c.i-1;	// Ajustement des coordonnnées pour pouvoir empiler la nouvelle case.
				empiler(* P, c);	// Empile la nouvelle case.
			}
			
			// Cas où la direction tirée au sort est l'ouest.
			else if(T[random_direction] == 'W') {
				L->tab[c.i][c.j-1].etat = true;	// Marque la nouvelle case comme visitée.
				
				// Ouverture des portes entre la case actuelle et la nouvelle case.
				L->tab[c.i][c.j].W = true;
				L->tab[c.i][c.j-1].E = true;
				
				empiler(* P, c);	// Réempile la case de départ.
				c.j = c.j-1;	// Ajustement des coordonnnées pour pouvoir empiler la nouvelle case.
				empiler(* P, c);	// Empile la nouvelle case.
			}
			
			// Cas où la direction tirée au sort est le sud.
			else if(T[random_direction] == 'S') {
				L->tab[c.i+1][c.j].etat = true;	// Marque la nouvelle case comme visitée.
				
				// Ouverture des portes entre la case actuelle et la nouvelle case.
				L->tab[c.i][c.j].S = true;
				L->tab[c.i+1][c.j].N = true;
				
				empiler(* P, c);	// Réempile la case de départ.
				c.i = c.i+1;	// Ajustement des coordonnnées pour pouvoir empiler la nouvelle case.
				empiler(* P, c);	// Empile la nouvelle case.
			}
			
			// Cas où la direction tirée au sort est l'est.
			else if(T[random_direction] == 'E') { 
				L->tab[c.i][c.j+1].etat = true;	// Marque la nouvelle case comme visitée.
				
				// Ouverture des portes entre la case actuelle et la nouvelle case.
				L->tab[c.i][c.j].E = true;
				L->tab[c.i][c.j+1].W = true;
				
				empiler(* P, c);	// Réempile la case de départ.
				c.j = c.j+1;	// Ajustement des coordonnnées pour pouvoir empiler la nouvelle case.
				empiler(* P, c);	// Empile la nouvelle case.
			}

			delete [] T;
			nb_directions = 0;
		}

		else c = depiler(* P);
	}

	delete P;
	return L;
}

/** 
 * @brief	Recherche un chemin dans le labyrinthe.
 *
 * @param L Le labyrinthe sur lequel on va rechercher un chemin.
 *
 * @return La pile contenant le chemin.
 *
*/
pile * explorer(laby & L) {
	// Initialisation de toutes les cases comme non marquées car init() ne le fait pas.
	for(int i = 0; i < L.p; i++) {
		for(int j = 0; j < L.q; j++) {
			L.tab[i][j].marq = false;
		}
	}
	
	// Création d'une pile
	pile * P = new pile; 
	init(* P, L.p*L.q);
	
	// Empile la case de départ dans la pile qui vient d'être crée.
	couple * c = new couple;
	c->i = 0;
	c->j = 0;
	empiler(* P, * c);
	L.tab[c->i][c->j].marq = true;	// On marque la case de départ comme visitée.
	
	// Tant que le chemin vers la case d'arrivée n'a pas été trouvé.
	while((c->i != L.p-1) || (c->j != L.q-1)) {
		* c = depiler(*P);	// Dépile une case appellée c(i, j).
		
		// Si il existe encore des cases accessibles à partir de c non encore explorées.
		int cases_accessibles = 0;
		char T[4];	// Tableau contenant toutes les directions possibles à partir de c.
		
		// Cas où il est possible d'aller vers le nord.
		if(c->i > 0) { 
			if(!L.tab[c->i-1][c->j].marq) {	// Si la case nord n'est pas encore marquée.
				if(L.tab[c->i][c->j].N) {	// Si la porte nord est ouverte.
					// Alors la case au nord de c est accessible.
					T[cases_accessibles] = 'N';
					cases_accessibles++;	
				}
			}
		}
		
		// Cas où il est possible d'aller vers l'ouest.
		if(c->j > 0) {
			if(!L.tab[c->i][c->j-1].marq) {	// Si la case ouest n'est pas encore marquée.
				if(L.tab[c->i][c->j].W) {	// Si la porte ouest est ouverte.
					// Alors la case à l'ouest de c est accessible.
					T[cases_accessibles] = 'W';
					cases_accessibles++; 
				}
			}
		}
		
		// Cas où il est possible d'aller vers le sud.
		if(c->i < L.q-1) {
			if(!L.tab[c->i+1][c->j].marq) {	// Si la case sud n'est pas encore marquée.
				if(L.tab[c->i][c->j].S) {	// Si la porte sud est ouverte.
					// Alors la case au sud de c est accessible.
					T[cases_accessibles] = 'S';
					cases_accessibles++;
				}
			}
		}
		
		// Cas où il est possible d'aller vers l'est.
		if(c->j < L.q-1) {
			if(!L.tab[c->i][c->j+1].marq) {	// Si la case est n'est pas encore marquée.
				if(L.tab[c->i][c->j].E) {	// Si la porte est est ouverte.
					// Alors la case à l'est de c est accessible.
					T[cases_accessibles] = 'E';
					cases_accessibles++;
				}
			}
		}
		
		if(cases_accessibles > 0) {
			int random_direction = rand() % cases_accessibles;	// Tirage d'une direction au hasard.
			
			// Cas où la direction tirée est le nord.
			if(T[random_direction] == 'N') {
				empiler(* P, * c);	// Réempile c.
				L.tab[c->i-1][c->j].marq = true;	// Marque la nouvelle case visitée.
				c->i = c->i-1;	// Réajustement des coordonnées de c.
				empiler(* P, * c);	// Empile la nouvelle case.
			}
			
			// Cas où la direction tirée est l'ouest.
			else if(T[random_direction] == 'W') {
				empiler(* P, * c);	// Réempile c.
				L.tab[c->i][c->j-1].marq = true;	// Marque la nouvelle case visitée.
				c->j = c->j-1;	// Réajustement des coordonnées de c.
				empiler(* P, * c);	// Empile la nouvelle case.
			}
			
			// Cas où la direction tirée est le sud.
			else if(T[random_direction] == 'S') {
				empiler(* P, * c);	// Réempile c.
				L.tab[c->i+1][c->j].marq = true;	// Marque la nouvelle case visitée.
				c->i = c->i+1;	// Réajustement des coordonnées de c.
				empiler(* P, * c);	// Empile la nouvelle case.
			}
			
			// Cas où la direction tirée est l'est.
			else if(T[random_direction] == 'E') {
				empiler(* P, * c);	// Réempile c.
				L.tab[c->i][c->j+1].marq = true;	// Marque la nouvelle case visitée.
				c->j = c->j+1;	// Réajustement des coordonnées de c.
				empiler(* P, * c);	// Empile la nouvelle case.
			}
		}
	}

	return P;
}

/** 
 * @brief	Création d'une pile dans l'ordre inverse pour pouvoir afficher le chemin dans le sens départ -> arrivée.
 *
 * @param P Pile à inverser.
 * 
 * @return	La pile inversée.
 *
*/
pile inversionPile(pile & P) {
	pile newPile;
	init(newPile, P.taille);
	while(!vide(P)) {
		couple c = depiler(P);
		empiler(newPile, c);
	}
	return newPile;
}

/** 
 * @brief	Affichage d'un chemin sur un labyrinthe.
 *
 * @param P Pile contenant le chemin.
 * @param L Labyrinthe correspondant au chemin.
 *
*/
void affiche_chemin(pile & P, laby & L) {
	pile newPile = inversionPile(P);	// Création d'une nouvelle pile pour avoir le chemin départ -> arrivée.
	while(!vide(newPile)) {
		couple c = depiler(newPile);
		Mark_Case(c.i, c.j);
		// Affichage de la solution du labyrinthe de manière progressive
		refresh();
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

/** 
 * @brief	Permet au joueur de choisir la difficulté de la partie.
 *
 * @return	La taille du labyrinthe en fonction de la difficulté choisie par le joueur.
 *
*/
int chooseDifficulty() {
	int taille = 0;
	char difficulty;

	cout << "Choisissez une difficulté :\n\n";
	cout << "1. Facile (10x10)\n";
	cout << "2. Moyen  (15x15)\n";
	cout << "3. Difficile (20x20)\n";
	cout << "\nVotre choix : ";
	cin >> difficulty;

	switch(difficulty) {
		case '1':	taille = 10; break;
		case '2':	taille = 15; break;
		case '3':	taille = 20; break;
		default:	taille = 15;	// Par défaut, difficulté moyenne.
	}

	return taille;
}

/** 
 * @brief	Attend que le joueur appuie sur ENTER.
 *
*/
void waitForEnter() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
	cout << "\nAppuyez sur Entrée pour continuer...";
    cin.get();
}

/** 
 * @brief	Lance une partie.
 *
 * @param taille Taille du labyrinthe.
 *
*/
void game(int taille) {
	int games = 0, wins = 0;
	int userInput = -2;	// -2 est la valeur pour rejouer une partie.
	
	while(userInput == -2) {	// Boucle pour pouvoir rejouer une partie avec la même difficulté.
		games++;
		clear();
		laby * L = My_Creation(taille, taille);
		InitCurses();
		Show_Lab(*L);
		mvprintw(2 * L->q + 1, 2 * L->p - 20, "Pour quitter le jeu, appuyez sur F1.");

		auto start = chrono::steady_clock::now();	// Lancement du chronomètre.
		int ch = wgetch(stdscr);
		bool won = deplacement(*L);
		// Arrête le chronomètre et récupération du temps mis par le joueur pour résoudre le labyrinthe.
		auto end = chrono::steady_clock::now();
        int elapsedTime = chrono::duration_cast<chrono::seconds>(end - start).count();
				
		clear();
		Show_Lab(*L);

		if(won) {
			mvprintw(2 * L->q + 1, 2 * L->p - 20, "VOUS AVEZ GAGNE LA PARTIE !");
			wins++;
		}
		else mvprintw(2 * L->q + 1, 2 * L->p - 20, "VOUS AVEZ PERDU !");
			
		mvprintw(2 * L->q + 2, 2 * L->p - 20, "Temps écoulé : %ds", elapsedTime);
		mvprintw(2 * L->q + 3, 2 * L->p - 20, "F1: Quitter | F2: Rejouer");
			
		// Affichage de la solution.
		pile * P = new pile;
		P = explorer(*L);
		affiche_chemin(*P, *L);
		delete P;
		delete L;
		userInput = LireCommande();
	}

	EndCurses();
	
	system("clear");
	if((games == wins) && (games >= 2)) cout << "BRAVO !!! VOUS AVEZ FAIT UN SANS FAUTE !" << endl;
	else if((wins == 0) && (games >= 2)) cout << "Qu'est-ce qu'il s'est passé ? Vous avez perdu vos " << games << " parties." << endl;
	cout << "Statistiques : " << wins << " victoire(s) / " << games << " partie(s) jouée(s)." << endl;

	waitForEnter();
}

/** 
 * @brief	Affiche les règles du jeu.
 *
*/
void rules() {
	cout << "=== RÈGLES DU JEU ===\n\n";
    cout << "🎯	Atteindre la case en bas à droite du labyrinthe.\n\n";
    cout << "⚠️	Vous perdez si vous heurtez un mur ou entrez une commande invalide.\n\n";
    cout << "➡️	Utilisez les flèches directionnelles pour vous déplacer dans le labyrinthe.\n\n";
    cout << "💡	Quittez avec F1.\n\n";
}

/** 
 * @brief	Menu principal du jeu.
 *
*/
void menu() {
	char choice;

	do {
		system("clear");	// Réinitialise l'affichage du terminal.

		cout << "=== MENU PRINCIPAL ===\n\n";
    	cout << "1. Jouer\n";
    	cout << "2. Lire les règles\n";
    	cout << "3. Quitter\n";
    	cout << "\nVotre choix : ";

    	cin >> choice;

	} while ((choice != '1') && (choice != '2') && (choice != '3'));

	system("clear");

	// Cas où le joueur souhaite jouer une partie.
	if(choice == '1') {
		int taille = chooseDifficulty();	// Choix de la difficulté de la / des partie(s)
		game(taille);	// Lancement d'une partie
	}

	// Cas où le joueur souhaite lire les règles
	if(choice == '2') {
		rules();	// Affichage des règles du jeu.
		waitForEnter();	// Attendre que l'utilisateur appuie sur ENTER pour lui laisser le temps de lire à son rythme.
	}

	// Cas où le joueur souhaite quitter le jeu.
	if(choice == '3') {
		cout << "À bientôt ! 👋" << endl;
		return;
	}

	menu();	// Retour au menu principal
}

int main() {
    srand(time(NULL));	// Permet d'avoir un labyrinthe différent à chaque génération.
    menu();	// Affichage du menu au lancement du programme.
    return 0;
}
