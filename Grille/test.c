#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string.h>
#include<stdio.h>
#include "test.h"

void init(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode* DM)
{

	if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS )) {
		printf("%s", SDL_GetError());
		quit(fenetre,renderer,E_SDL_INIT);
	}
	if (0 != SDL_GetCurrentDisplayMode(0, DM) ) {
		printf("%s", SDL_GetError());
		quit(fenetre,renderer,E_GETDISPLAY);
	}
	init_fenetre_renderer(fenetre,renderer,*DM);
}

void init_fenetre_renderer(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode DM)
{
	//printf("%d , %d\n",DM.w,DM.h);
	*fenetre = SDL_CreateWindow("Rummikub", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DM.w,DM.h, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_INPUT_GRABBED);
	if (fenetre == NULL) {
		printf("%s", SDL_GetError());
		quit(fenetre,renderer,E_SDL_FENETRE);
	}
	*renderer = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("%s", SDL_GetError());
		quit(fenetre,renderer,E_SDL_RENDERER);
	}
}

void quit(SDL_Window** fenetre,SDL_Renderer** renderer,erreur err)
{
	if (renderer != NULL) {
		SDL_DestroyRenderer(*renderer);
		renderer = NULL;
	}
	if (fenetre != NULL) {
		SDL_DestroyWindow(*fenetre);
		fenetre = NULL;
	}
	SDL_Quit();
	exit(err);
}

SDL_Texture* createTextureFromImage (SDL_Renderer** renderer,char* path) {

	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(path);
	if (surface == NULL) {
		fprintf(stdout,"error load image: %s", IMG_GetError());
		return texture;
	}
	texture = SDL_CreateTextureFromSurface(*renderer, surface);
	SDL_FreeSurface(surface);
	return texture;

}

void affiche_texture(SDL_Renderer** renderer,SDL_Texture* texture,SDL_Rect* destination) 
{
	// peut etre pas utile car plus rapide de charger toutes les textures avant l'affichage
	SDL_RenderCopy(*renderer, texture, NULL, destination);
	//SDL_RenderPresent(*renderer); //pose des probleme quand on veut afficher plusieurs textures.

}



SDL_Texture* init_grille(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode DM,int *caseX, int *caseY,SDL_Rect *infoGrille)
{
	int largeur = (int)((float)DM.w * G_RATIO);
	int hauteur = (int)((float)DM.h * G_RATIO);
	SDL_Color orange = {255,127,40,255};
	SDL_Color black = {0,0,0,255};
	float x=DM.w * (1.0 - G_RATIO)/2;
	float y=DM.h * (1.0 - G_RATIO)/2;
	SDL_Point depart ={x,y};
	infoGrille->x = depart.x;
	infoGrille->y = depart.y;
	infoGrille->w = largeur;
	infoGrille->h = hauteur;
	//fprintf(stdout,"%d %d %d %d \n",depart.x,depart.y,largeur,hauteur);
	SDL_SetRenderDrawColor(*renderer,orange.r,orange.g,orange.b,orange.a);
	/*if( SDL_RenderDrawRect(*renderer,&rect) != 0 )
	{
		printf("%s", SDL_GetError());
		quit(fenetre,renderer,E_SDL_DESSIN);
	}*/
	if ( init_grille_interieur(*infoGrille ,renderer,caseX,caseY) != 0)
	{
		printf("%s", SDL_GetError());
		quit(fenetre,renderer,E_SDL_DESSIN);
	}
	SDL_RenderPresent(*renderer);
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface *surface = SDL_CreateRGBSurface(0, DM.w, DM.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(*renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
	IMG_SavePNG(surface,"grille.png");
	IMG_Quit();
	SDL_SetRenderDrawColor(*renderer,black.r,black.g,black.b,black.a);
	SDL_RenderClear(*renderer);
	SDL_RenderPresent(*renderer);
	return createTextureFromImage (renderer,"grille.png");
}
int init_grille_interieur(SDL_Rect rect ,SDL_Renderer** renderer,int *caseX, int *caseY)
{
	/*
		Le rectangle contient toutes les informations de la grille.
	*/
	int i,j;
	int espacementX = (int)((float)rect.w / (float)GRILLELARGEUR);
	int espacementY = (int)((float)rect.h / (float)GRILLEHAUTEUR);
	*caseX = espacementX;
	*caseY = espacementY;
	int x = rect.x;
	int y = rect.y;
	for(i = 0;i <= GRILLELARGEUR;i ++)
	{
		
		if( SDL_RenderDrawLine(*renderer,x,rect.y,x,rect.y+GRILLEHAUTEUR * espacementY) != 0 )
			return -1;
		x+=espacementX;
		//printf("%d\n",x );
	}
	for(j = 0;j <= GRILLEHAUTEUR;j++)
	{
		if( SDL_RenderDrawLine(*renderer,rect.x,y,rect.x+GRILLELARGEUR * espacementX,y) != 0 )
			return -1;
		y+=espacementY;
	}
	return 0;
}

SDL_Texture*** init_tuilles(SDL_Renderer** renderer)
{
	int i;
	SDL_Texture*** tuilles;
	tuilles = malloc(NBRCOUL * sizeof(SDL_Texture**));
	
	for (i = 0; i < NBRCOUL; i++)
	{
		if(i < NBRCOUL)
			tuilles[i] = malloc(NBRTUILLE * sizeof(SDL_Texture*));
		else 
			tuilles[i] = malloc(sizeof(SDL_Texture*));
	}
	return tuilles;

}

SDL_Texture* get_tuille(char chemin[14],SDL_Texture** tuilles) // inutile
{
	char buffer[2];
	buffer[0]=chemin[0];
	buffer[1]=chemin[1];
	int nombre = atoi(buffer);
	int couleur = get_couleur(chemin[3]);
	return *((tuilles+nombre)+couleur);
}

int get_couleur(char c) // A remplacer par une enumeration 
{
	if(c == 'B')
		return 0;
	else if (c == 'L')
		return 1;
	else if (c == 'R')
		return 2;
	else if (c == 'J')
		return 3;
	return 4;
}

void affiche_tuille(SDL_Renderer** renderer,SDL_Rect* destination,SDL_Texture*** tuilles, int couleur,int nombre)
{
	if( nombre == 0 )
	{
		affiche_texture(renderer,tuilles[NBRCOUL][0],destination);
	}
	else
	{
		affiche_texture(renderer,tuilles[couleur][nombre],destination);
	}
}

void affiche_tuilles(SDL_Renderer** renderer,SDL_Rect* destination,SDL_Texture*** tuilles/*,Liste plateau*/)
{
	
	//TODO
	//regarder chaque case de la table de hasage
	//recuperer les tuilles de la suites aet les afficher.
	//Chaque case du tableau est un pointer vers une suite de tuilles donc, il faut juste parcourir la suite
	//pour savoir les tuilles à afficher
	//la clé est la position de la case en Y et il y a une variable X. 
}



int main (int argc, char *argv[]) 
{
	SDL_Window* fenetre;
	SDL_Renderer* renderer;
	fenetre = NULL;
	renderer = NULL;
	SDL_DisplayMode DM;
	SDL_Rect infoGrille; // point de départ + taille
	SDL_Texture* grille;
	SDL_Texture*** tuilles=NULL;
	int caseX,caseY;
	init(&fenetre,&renderer,&DM);
	grille = init_grille(&fenetre,&renderer,DM,&caseX,&caseY,&infoGrille);
	SDL_Rect infoCase={infoGrille.x,infoGrille.y,caseX,caseY};
	tuilles=init_tuilles(&renderer);
	tuilles[0][0] = createTextureFromImage (renderer,"Image/01B.png");
	//fprintf(stdout, "info : %d;%d\n",caseX,caseY);
	SDL_Texture* test = *(*(tuilles));
	affiche_texture(&renderer, grille,NULL);
	//SDL_RenderPresent(renderer);
	affiche_texture(&renderer, test,&infoCase);
	SDL_RenderPresent(renderer);
	SDL_Event e;int exit = 0;
	/*
		Le code qui suit permet de savoir sur quel case l'utilisateur clique.
	*/
	while (exit != 1) {
		while(SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				 case SDL_KEYDOWN:
                    
                    if ( e.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
                    {
                    	fprintf(stdout, "Un appuie sur Echap \n");
                        exit = 1;
                    }
					break;
				case SDL_MOUSEBUTTONUP:
	                fprintf(stdout, "Un relachement d'un bouton de la souris :\n");
	                fprintf(stdout, "\tfenêtre : %d\n",e.button.windowID);
	                fprintf(stdout, "\tsouris : %d\n",e.button.which);
	                fprintf(stdout, "\tbouton : %d\n",e.button.button);
	                fprintf(stdout, "\tclics : %d\n",e.button.clicks);
	                fprintf(stdout, "\tposition : %d;%d\n",e.button.x,e.button.y);
	                
					int x = (e.button.x-infoGrille.x)/caseX;
	                int y = (e.button.y-infoGrille.y)/caseY;
	                fprintf(stdout, "\tcase selectionee : %d;%d\n",x ,y );
	                break;
			}
		}
	}
	quit(&fenetre,&renderer,OK);



}

