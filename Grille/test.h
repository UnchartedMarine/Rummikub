#define G_RATIO 0.8
#define LARGEURIMAGE 25
#define HAUTEURIMAGE 50
#define GRILLELARGEUR 25
#define GRILLEHAUTEUR 5
#define	NBRCOUL 5
#define NBRTUILLE 13
typedef enum erreur {OK,E_SDL_INIT,E_GETDISPLAY,E_SDL_FENETRE,E_SDL_RENDERER,E_SDL_DESSIN}erreur;

typedef struct {
		SDL_Texture* texture;
		SDL_Rect rect;
	}Texture;

typedef struct {
		int largeur;
		int hauteur;
	}Resolution;
void init(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode* DM);
void init_fenetre_renderer(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode DM);
void affiche_texture(SDL_Renderer** renderer,SDL_Texture* texture,SDL_Rect* destination);
SDL_Texture* init_grille(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode DM,int *caseX, int *caseY,SDL_Rect *infoGrille);
int init_grille_interieur(SDL_Rect rect ,SDL_Renderer** renderer,int *caseX, int *caseY);
SDL_Texture* createTextureFromImage (SDL_Renderer** renderer,char* path);
SDL_Texture* get_tuille(char chemin[14],SDL_Texture** tuilles);
int get_couleur(char c);
SDL_Texture*** init_tuilles(SDL_Renderer** renderer);
//int compare_resolution(Resolution res1, Resolution res2);
//float multiplicateur_taille_image(Resolution res);
void quit(SDL_Window** fenetre,SDL_Renderer** renderer,erreur err);