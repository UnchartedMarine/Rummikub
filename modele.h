/******************************************
 * Header principal
 ******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#define NOMBRE_TUILES 106


typedef struct TUILE 
{	
	int num,coul;
} TUILE;

typedef struct ELEMENT
{	
	TUILE tuile;
	struct ELEMENT * suivant;
} ELEMENT;

typedef struct LISTE
{	
	ELEMENT * premier;
} LISTE;


typedef struct ELEMENT_PLATEAU
{	
	LISTE * liste;
	struct ELEMENT_PLATEAU * suivant;
} ELEMENT_PLATEAU;

typedef struct PLATEAU
{	
	ELEMENT_PLATEAU * premier;
} PLATEAU;


typedef struct JOUEUR 
{	
	LISTE * main;
	int points; //mettre NULL de base ? c'est inutile en vrai, mais pour la clarté ?
	bool premierCoup;
} JOUEUR;


TUILE pioche[NOMBRE_TUILES];
PLATEAU *plateau;

//Les fonctions de la pioche
void init_pioche();
void init_tuiles();
void melange_pioche(TUILE* pioche);
void lit_tuile(TUILE tuile);
void lit_pioche(TUILE* pioche);
void piocher_tuile(LISTE *liste,int *niveauPioche);
void init_main(LISTE *liste,int *niveauPioche);
int regarde_qui_commence(int nbJoueurs);
int regarde_qui_commence_aux(int nbJoueurs,int valeurMax,int positionMax);

//Les fonctions de manipulation de liste
LISTE * cree_liste();
void ajoute_liste(LISTE *liste,TUILE tuile);
void lit_liste(LISTE *liste);
int nb_elements_liste(LISTE *liste);
int placement_element_liste(LISTE *liste, TUILE tuile, int position);
LISTE * separer_liste_en_deux(LISTE *liste, int position);
bool est_valide(LISTE *liste);

//les fonctions liées au plateau
PLATEAU * cree_plateau();
void ajoute_plateau(PLATEAU *plateau,LISTE *liste);
void lit_plateau();

//Les fonctions pour que le joueur joue son tour
int choisit_tour(bool premierCoup);
void joue_tour(JOUEUR * joueur, int *niveauPioche);
void saisit_combinaison(LISTE *main);

//les fonctions de détection de fin de partie et calcul de points
bool pioche_finie(int niveauPioche);
bool main_finie(JOUEUR *joueurs, int nbJoueurs, int tour);
int detecte_gagnant(JOUEUR *joueurs,int nbJoueurs,int tour,int aVOIR);
int trouve_joueur_precedent(int nbJoueurs, int tour);
int plus_petite_main(JOUEUR *joueur, int nbJoueurs);
bool est_partie_finie(JOUEUR *joueurs, int niveauPioche, int nbJoueurs, int tour);
int additionne_points(LISTE *main,int fin);
int calcule_points_gagnant(JOUEUR *joueurs,int nbJoueurs,int gagnant,int aVOIR);
int calcule_points_perdant(JOUEUR *joueurs,int gagnant,int perdant,int aVOIR);


void lit_tuile_liste(LISTE *liste, int position);
TUILE renvoie_tuile_via_position(LISTE *liste,int position);
int enleve_element_liste(LISTE * liste,int position);
LISTE * renvoie_liste_via_position(int position);
int nb_elements_plateau();


bool pose_30_points(LISTE *liste);



