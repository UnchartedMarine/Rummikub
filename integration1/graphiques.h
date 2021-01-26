#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#define G_RATIO_V_TAILLE 0.6
#define G_RATIO_V_POSITION 0.8
#define G_RATIO_H 0.7
#define LARGEURIMAGE 25
#define HAUTEURIMAGE 50
#define GRILLELARGEUR 20
#define GRILLEHAUTEUR 6
#define UI_BAS 0.85
#define	NBRCOUL 5
#define NBRTUILLE 13
typedef enum erreur {OK,E_SDL_INIT,E_TTF_INIT,E_GETDISPLAY,E_SDL_FENETRE,E_SDL_RENDERER,E_SDL_DESSIN}erreur;
typedef enum couleurs {NOIR,BLEU,ROUGE,VERT}couleurs;
typedef struct Texture{
		SDL_Texture* texture;
		SDL_Rect rect;
	}Texture;

typedef struct {
		int largeur;
		int hauteur;
	}Resolution;
SDL_Window* fenetre;
SDL_Renderer* renderer;
SDL_Texture*** textureTuiles;
void init(SDL_DisplayMode* DM);
void init_fenetre_renderer(SDL_DisplayMode DM);
void init_textureTuiles();
Texture init_grille(SDL_DisplayMode DM,int *caseX, int *caseY);
void affiche_texture(SDL_Texture* texture,SDL_Rect* destination);
int init_grille_interieur(SDL_Rect rect,int *caseX, int *caseY);
SDL_Texture* createTextureFromImage (SDL_Renderer** renderer,char* path);
SDL_Texture* get_tuile(char chemin[14],SDL_Texture** tuiles);
void affiche_main(SDL_Texture** main,int taille, SDL_DisplayMode DM, SDL_Rect infoGrille);
void affiche_fin(SDL_Renderer** renderer,SDL_DisplayMode DM, SDL_Rect infoGrille,TTF_Font* police);
void affiche_nombre_tuille(SDL_Renderer** renderer,SDL_DisplayMode DM, SDL_Rect infoGrille,TTF_Font* police,int nombre);
SDL_Texture* charge_tuille(int couleur,int nombre);
void affiche_plateau(SDL_Texture* TexturePlateau[GRILLEHAUTEUR][GRILLELARGEUR],SDL_Rect infoCase);

void vide_tuiles();
//int compare_resolution(Resolution res1, Resolution res2);
//float multiplicateur_taille_image(Resolution res);
void quit(erreur err);