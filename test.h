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
typedef enum couleurs {NOIR,BLEU,ROUGE,JAUNE}couleurs;
typedef struct Texture{
		SDL_Texture* texture;
		SDL_Rect rect;
	}Texture;

typedef struct {
		int largeur;
		int hauteur;
	}Resolution;
void init(SDL_DisplayMode* DM);
void init_fenetre_renderer(SDL_DisplayMode DM);
void affiche_texture(SDL_Texture* texture,SDL_Rect* destination);
Texture init_grille(SDL_DisplayMode DM,int *caseX, int *caseY);
int init_grille_interieur(SDL_Rect rect,int *caseX, int *caseY);
SDL_Texture* createTextureFromImage (SDL_Renderer** renderer,char* path);
SDL_Texture* get_tuille(char chemin[14],SDL_Texture** tuilles);
int get_couleur(char c);
void init_tuilles();
void vide_tuilles();
//int compare_resolution(Resolution res1, Resolution res2);
//float multiplicateur_taille_image(Resolution res);
void quit(erreur err);