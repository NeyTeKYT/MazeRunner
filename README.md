# 🚀 Maze Runner - Générateur et Explorateur de Labyrinthe en C++ 🧩

Bienvenue sur le dépôt de mon projet **Maze Runner**, un programme interactif en C++ permettant la **création, l'exploration et la résolution de labyrinthes parfaits**.

Ce projet a été réalisé dans le cadre de l'**Unité d'Enseignement (UE) Bases de la Programmation** du premier semestre de ma **première année de licence en informatique** à l'université d’*Avignon*.

🔍 Ce travail m’a permis de consolider ma compréhension des structures de données (notamment la **pile**), de la manipulation dybamique de tableaux en C++, ainsi que de la bibliothèque **ncurses** pour la gestion graphique en mode terminal.

---

## ✅ Lecture & Affichage de Labyrinthe

🧾 Lecture d'un fichier décrivant un labyrinthe (`laby * lire_Laby(laby & L)`)

🖼️ Affichage du labyrinthe avec `ShowLab()` via **ncurses**

📌 Marquage / démarquage des cases (`Mark_Case`, `UMark_CASE`)

---

## 🎮 Déplacement du Joueur

👣 Implémentation d'un mode de **jeu interactif** : utilisation des flèches directionnelles (via `LireCommande()`), le joueur part de la case (0, 0) jusqu'à (p-1, q-1), gestion des erreurs de déplacement (fonce dans un mur) et de la victoire.

---

## 🏗️ Génération Automatique de Labyrinthe

✨ Création de **labyrinthes parfaits** (chemin unique entre chaque paire de cases) : parcours récursif avec une **pile** grâce à la fonction `laby * My_Creation(int p, int q)`

---

## 🔍 Recherche de Chemin (IA)

🤖 Algorithme de **parcours en profondeur** (DFS) : recherche d'un chemin entre la case de départ et la case d'arrivée, utilisation d'une pile pour stocker le chemin, via la fonction `Pile * explorer(laby & L)`

---

## 🧭 Affichage du Chemin Trouvé

🗺️ Affichage progressif avec marquage visuel du chemin résolu via la fonction `void affiche_chemin(Pile & P)`

---

## 🎯 Mode Jeu Complet

💡 Fonction centrale `menu()` qui **génère le labyrinthe**, **permet le déplacement manuel du joueur**, **affiche la solution en cas d'abandon (F1)**.

---

## 📦 Compilation & Lancement

Ce projet utilise la bibliothèque **ncurses**. 

Pour compiler :  
`g++ *.cpp -o mazeRunner -lncurses`  

Puis lancer le jeu :  
`./mazeRunner`  

