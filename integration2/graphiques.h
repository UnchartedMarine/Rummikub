#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#define G_RATIO_V_TAILLE 0.6
#define G_RATIO_V_POSITION 0.8
#define G_RATIO_H 0.7
#define LARGEURIMAGE 25
#define HAUTEURIMAGE 50
#define GRILLELARGEUR 20
#define GRILLEHAUTEUR 6
#define UI_BAS 0.9
#define	NBRCOUL 5
#define NBRTUILLE 13
#define BOUTTONSTATES 3
typedef enum erreur {OK,E_SDL_INIT,E_TTF_INIT,E_GETDISPLAY,E_SDL_SON,E_SDL_FENETRE,E_SDL_RENDERER,E_SDL_DESSIN}erreur;
typedef enum couleurs {NOIR,BLEU,ROUGE,VERT}couleurs;
typedef struct Texture{
		SDL_Texture* texture;
		SDL_Rect rect;
	}Texture;

typedef struct {
		int largeur;
		int hauteur;
	}Resolution;

typedef struct {
		Texture buttonTexture;
		Texture textTexture;
	}ButtonTexture;

typedef struct {
		SDL_Rect box;
	}Hitbox;

typedef struct {
	int x;
	int y;
}pos;

//variable de base pour l'affichage
SDL_Window* fenetre;
SDL_Renderer* renderer;
SDL_DisplayMode DM;
//variable contenant les textures a afficher et les hitboxes
SDL_Texture*** textureTuiles; // tableau contenant toutes les textures des tuiles
SDL_Texture** buttonStates;// tableau contenant les trois etat d'un bouton
Hitbox* bHitboxes;//hitbox des boutons
Hitbox* vHitboxes;//hitbox des textures des combinaisons
ButtonTexture* bTextures;//Textures des boutons
Texture* vTextures;//Textures de tout element qui change lentement d'etat
Texture grille;//texture de la grille
int nbButtons;
int nbTextures;
int nbHitboxes;
//Variable pour les menus (pas implimenté dans cette version)
char* name;
TTF_Font* font;
//Variable pour la manipulation des listes
int inGame;
int caseX,caseY;//la taille d'une tuille dans la grille
pos tuileSelect;//position de la tuille selectionnee par l'utilisateur
//variable pour les sons et musique
Mix_Music* gMusic;
Mix_Chunk* onClick;
Mix_Chunk* unClick;
//variable pointant vers une fonction
void (*update)(int*, int* , int* ,SDL_Event , int* );

//initialisation de la SDL et composant necessaire au fonction de l'interface
void init_module(SDL_DisplayMode* );
void init_fenetre_renderer(SDL_DisplayMode );
void init_textureTuiles();
SDL_Texture* charge_tuille(int ,int );
SDL_Texture* create_texture_from_image (char* );
Texture init_grille(SDL_DisplayMode ,int *, int *);
int init_grille_interieur(SDL_Rect ,int *, int *);
void init_graphique(SDL_DisplayMode* );
//Affichage de la grille et de la main
void affiche_main(SDL_Texture** ,int , SDL_DisplayMode , SDL_Rect );
void affiche_plateau(SDL_Texture* [GRILLEHAUTEUR][GRILLELARGEUR],SDL_Rect );
//Fonction pour quitter et nettoyer
void vide_tuiles();
void quit(erreur err);
//fonction pour la gestion des interactions avec le joueur
void create_text(int , int , int , SDL_Color , TTF_Font* , char* );
Texture create_texture_from_text (TTF_Font* , char* , SDL_Color );
void create_button(int , int , int ,int , int , SDL_Texture* , SDL_Color , TTF_Font* , char* );
void init_buttonState(void);
void init_menu_zoom(void);
void init_menu_jeu(void);
void destroy_menu(void);
void handle_mickey_mouse(int* , int* , int* ,SDL_Event , int* , void (*redirect)(int,int*)) ;
int get_index_hitbox_clicked(int x, int y);
int check_hitbox(int x, int y, int index);
int check_grille(int ,int );

//fonction pour les menu (pas implimenté dans cette version)
void init_menu(void);
void init_textbox_menu(void);
void update_menu(int* , int* , int* ,SDL_Event , int* );
void update_textbox_menu(int*, int* , int* ,SDL_Event, int* );
void redirect_menu(int inex, int* );
void redirect_textbox_menu(int, int*);
void set_text(int index, TTF_Font* font, char* name, SDL_Color color);
void handle_textbox (SDL_Event e, TTF_Font* font, int index);





