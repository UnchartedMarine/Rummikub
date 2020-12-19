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
void lit_pioche(TUILE* pioche);
void init_main(LISTE *liste,int *niveauPioche);

//Les fonctions de manipulation de liste
LISTE * cree_liste();
void ajoute_liste(LISTE *liste,TUILE tuile);
void lit_liste(LISTE *liste);
int nb_elements_liste(LISTE *liste);
int placement_tuile_liste(LISTE *liste, TUILE tuile, int position);
LISTE * separer_liste_en_deux(LISTE *liste, int position);
bool est_valide(LISTE *liste);












