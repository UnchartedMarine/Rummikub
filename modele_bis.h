/******************************************
 * Header principal
 ******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#define NOMBRE_TUILES 106

enum coul{JOKER,BLEU,ROUGE,JAUNE,VERT};

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
	char pseudo[21];
	LISTE * main;
	int points;
	bool premierCoup;
} JOUEUR;


TUILE pioche[NOMBRE_TUILES];
PLATEAU *plateau;
PLATEAU *copiePlateau;
LISTE *copieMain;
LISTE *listeTuilesRecup;

//Les fonctions de la pioche
void init_pioche();
void init_tuiles();
void melange_pioche(TUILE* pioche);
void lit_pioche(TUILE* pioche);
void piocher_tuile(LISTE *liste,int *niveauPioche);
void init_main(LISTE *liste,int *niveauPioche);
int regarde_qui_commence(int nbJoueurs,JOUEUR * joueurs);
int regarde_qui_commence_aux(int nbJoueurs,int valeurMax,int positionMax,JOUEUR * joueurs);

//Les fonctions de manipulation de liste
LISTE * cree_liste();
LISTE * copie_liste(LISTE* original);
void ajoute_liste(LISTE *liste,TUILE tuile);
void ajoute_liste_position_donnee(LISTE *liste,TUILE tuile, int position);
void lit_liste(LISTE *liste);
void lit_liste_aux(ELEMENT * element,int indiceTuile);
int nb_elements_liste(LISTE *liste);
int placement_element_liste(LISTE *liste, TUILE tuile, int position);
LISTE * separer_liste_en_deux(LISTE *liste, int position);
int enleve_element_liste(LISTE * liste,int position);
TUILE renvoie_tuile_via_position(LISTE *liste,int position);
void echange_tuiles_listes_via_main(LISTE * liste1,LISTE * liste2,int positionListe1,int positionListe2);
void echange_tuiles_listes_via_recup(LISTE * liste1,LISTE * liste2,int positionListe1,int positionListe2);
int est_suite(LISTE *liste);
int est_liste_meme_nb(LISTE *liste);
int nb_points_suite(LISTE *liste);
int nb_points_liste_mnombre(LISTE *liste);
bool joker_est_dans_liste(LISTE * liste);


//les fonctions liées au plateau
PLATEAU * cree_plateau();
PLATEAU * copie_plateau(PLATEAU* original);
int nb_elements_plateau(PLATEAU *plato);
void ajoute_plateau(LISTE *liste, PLATEAU *plato);
void lit_plateau();
bool verif_plateau(PLATEAU * plato);

//Les fonctions pour que le joueur joue son tour
int choisit_tour(bool premierCoup,int tourMultiTemps);
int joue_tour(JOUEUR * joueur, int *niveauPioche,int tourMultiTemps);
int complete_combinaison(LISTE * main);
int saisit_combinaison(LISTE * main, int typeSuite, JOUEUR * joueur);
int recupere_tuile_combinaison(LISTE * main);
int separe_combinaison();
int remplace_joker(LISTE * main);

//les fonctions de détection de fin de partie et calcul de points
bool pioche_finie(int niveauPioche);
bool main_finie(JOUEUR * joueurs, int nbJoueurs, int tour);
int detecte_gagnant(JOUEUR * joueurs,int nbJoueurs,int tour,int typeFinDePartie);
int trouve_joueur_precedent(int nbJoueurs, int tour);
int plus_petite_main(JOUEUR * joueur, int nbJoueurs);
bool est_partie_finie(JOUEUR * joueurs, int niveauPioche, int nbJoueurs, int tour);
int additionne_points(LISTE * main,int fin);
int calcule_points_gagnant(JOUEUR *joueurs,int nbJoueurs,int gagnant,int typeFinDePartie);
int calcule_points_perdant(JOUEUR *joueurs,int gagnant,int perdant,int typeFinDePartie);


void lit_tuile_liste(LISTE *liste, int position);
LISTE * renvoie_liste_via_position(PLATEAU *plato,int position);
void init_joueurs(JOUEUR * joueurs,int modeJeu, int nbJoueurs,int * niveauPioche);
bool pose_30_points(LISTE *liste);
void demande_pseudo(JOUEUR * joueur);
void rentrer_nom_score(int score, char * nom);
int complete_recup_combinaison(LISTE * main);
int demande_mode_jeu();
int demande_nb_joueurs();






