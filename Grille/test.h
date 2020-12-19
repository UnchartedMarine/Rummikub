
#define G_RATIO 0.8

typedef enum erreur {OK,E_SDL_INIT,E_GETDISPLAY,E_SDL_FENETRE,E_SDL_RENDERER}erreur;

typedef struct {
		SDL_Texture* texture;
		SDL_Rect rect;
	}Texture;

void init(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode* DM);
void init_fenetre_renderer(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode DM);
void affiche_grille(SDL_DisplayMode DM,SDL_Renderer** renderer);
void quit(SDL_Window** fenetre,SDL_Renderer** renderer,erreur err);