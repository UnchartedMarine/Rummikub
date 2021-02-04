#include "graphiques.h"
#include "modele.h"

LISTE* (*positionPlateau)[GRILLELARGEUR];
SDL_Texture* (*TexturePlateau)[GRILLELARGEUR];

LISTE** liste_manipulation;
int nb_liste_manipulation;
int liste_actuelle;
LISTE* reserve;
LISTE* main_actuelle;

// Fonction Liant le plateau a la grille graphique
void reset_liasion_graphique(LISTE* [GRILLELARGEUR][GRILLELARGEUR],SDL_Texture* [GRILLELARGEUR][GRILLELARGEUR]);
void lie_plateau_graphique(LISTE* [GRILLELARGEUR][GRILLELARGEUR],SDL_Texture* [GRILLELARGEUR][GRILLELARGEUR]);

//Fonction permettant d'afficher les combinaisons de tuiles sur l'ecran
SDL_Texture**  lie_liste_graphique(LISTE* ,int* );
void lie_select_graphique(SDL_Texture** ,int ,int);
void lie_reserve_graphique(SDL_Texture** ,int ,int);
void lie_main_graphique(SDL_Texture** ,int ,int );
void dessine_liste(SDL_Rect ,int ,SDL_Texture** );
void affiche_liste_zoom(LISTE* ,int ,int );
//Fonction gerant l'interaction avec le joueur
void update_menu_jeu(int* , int* , int* ,SDL_Event ,  int* );
void update_menu_zoom(int* , int* , int* ,SDL_Event ,  int* );
void redirect_menu_jeu(int , int*);
void redirect_menu_zoom(int , int* );


//fonction de manipulation graphique
int check_liste_select(int , int ,int );
int ajouter(int ,int);
int retirer(int );
int scinder();
int changer_de_liste(int );


