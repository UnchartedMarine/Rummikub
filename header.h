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

typedef struct MAIN
{	
	TUILE tuile;
	struct MAIN * suivant;
} MAIN;

typedef struct LISTE
{	
	MAIN *premier;
} LISTE;


TUILE pioche[NOMBRE_TUILES];


//Les fonctions de la pioche
void init_pioche();
void init_tuiles();
void melange_pioche(TUILE* pioche);
void lit_tuile(TUILE tuile);
void lit_pioche(TUILE* pioche);
void init_main(LISTE *liste,int *niveauPioche);
int regarde_qui_commence(int nbJoueurs);

//Les fonctions de manipulation de liste
LISTE * cree_liste();
void ajoute_liste(LISTE *liste,TUILE tuile);
void lit_liste(LISTE *liste);
int nb_elements_liste(LISTE *liste);
int placement_tuile_liste(LISTE *liste, TUILE tuile, int position);
LISTE * separer_liste_en_deux(LISTE *liste, int position);
bool est_valide(LISTE *liste);

//les fonctions de détection de fin de partie et calcul de points
int trouve_joueur_precedent(int nbJoueurs, int tour);
bool main_finie(LISTE **mains, int nbJoueurs, int tour);
bool pioche_finie(int niveauPioche);
bool est_partie_finie(LISTE **mains, int niveauPioche, int nbJoueurs, int tour);
int additionne_points_main(LISTE *main);
int calcule_points_gagnant(LISTE **mains,int nbJoueurs,int gagnant,int aVOIR);
int calcule_points_perdant(LISTE **mains,int gagnant,int perdant,int aVOIR);
int plus_petite_main(LISTE **mains, int nbJoueurs);


void lit_tuile_liste(LISTE *liste, int position); /*****fonction de test pour éviter d'écrire à chaque fois*****/










