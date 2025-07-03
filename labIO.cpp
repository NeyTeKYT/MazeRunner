#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lab.h"

WINDOW * boite;
WINDOW * win;

// Affiche les bords de la case 
void Show_Case(int i , int j, Case c) {
	boite = subwin(stdscr, 3, 5, i*2, j*4);
	wborder(boite,(not c.W)?'|':' ',(not c.E)?'|':' ',(not c.N)?'-':' ',(not c.S)?'-':' ', '+', '+', '+', '+');
}

// Affiche le labyrinthe
void Show_Lab(laby & L) {
	for(int i = 0; i < L.p; i++) {
		for(int j = 0; j < L.q; j++) {
			Show_Case(i, j , L.tab[i][j]);
		}
	}
}
// Affiche les bords de la case 
void Verif_Case(int i , int j, Case c) {
	boite = subwin(stdscr, 3, 5, i*3, j*5);
	wborder(boite,not c.E?'|':' ',not c.W?'|':' ',not c.N?'-':' ',not c.S?'-':' ', '+', '+', '+', '+');
}

// Vérifie le labyrinthe
void Verif_Lab(laby & L) {
	for(int i = 0; i < L.p; i++) {
		for(int j = 0; j < L.q; j++) {
			Verif_Case(i, j , L.tab[i][j]);
		}
	}
}

// Affiche une etoile sur la case
void Mark_Case(int li, int col) {
	attron(A_REVERSE);
	mvprintw(li*2+1, (col+1)*4-3, " * ");
	mvprintw(li*2+1, (col+1)*4-3, " ");
	attroff(A_REVERSE);
}

void Mark_Case_Pile(pile & p) {
	for(int i=0;i<p.nbe;i++) Mark_Case(p.T[i].i, p.T[i].j);
}

// Efface la case
void UMark_Case(int li, int col) {
	//attron(A_STANDOUT);
	mvprintw(li*2+1, (col+1)*4-3, "   ");
	mvprintw(li*2+1, (col+1)*4-3, " ");
	//attroff(A_STANDOUT);
}

int LireCommande() {
	MEVENT event;
	int ch, CC, L;
	ch = wgetch(stdscr);

	switch(ch) {
		case KEY_F(1):	// Pour quitter le jeu
			return -1;
		case KEY_F(2):	// Pour recommencer une partie
			return -2;
		case KEY_DOWN:	// Aller en bas
			return 6;
		case KEY_UP:	// Aller en haut
			return 12;
		case KEY_RIGHT:	// Aller à droite
			return 3;
		case KEY_LEFT:	// Aller à gauche
			return 9;
	}

	return 0;
}

void InitCurses() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	//mousemask(ALL_MOUSE_EVENTS, NULL);
	wrefresh(win);
}

void EndCurses() {
	endwin();
}
