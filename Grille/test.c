#include<SDL2/SDL.h>
#include<string.h>
#include<stdio.h>
#include "test.h"

void init(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode* DM)
{

	if (0 != SDL_Init(SDL_INIT_VIDEO )) {
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

void affiche_grille(SDL_DisplayMode DM,SDL_Renderer** renderer)
{
	int largeur = (int)((float)DM.w * G_RATIO);
	int hauteur = (int)((float)DM.h * G_RATIO);
	SDL_Color orange = {255,127,40,255};
	float x=DM.w * (1.0 - G_RATIO)/2;
	float y=DM.h * (1.0 - G_RATIO)/2;
	SDL_Point depart ={x,y};
	int i,j;
	SDL_Rect rect = {depart.x,depart.y,largeur,hauteur};
	SDL_SetRenderDrawColor(*renderer,orange.r,orange.g,orange.b,orange.a);
	SDL_RenderDrawRect(*renderer,&rect);
	for(i=0;i<hauteur;i++)
	{
		for(j=0;j<largeur;j+=150)
		{
			//Dessiner ligne verticale a mettre dans une fonction
			// Faire une autre fonction pour les lignes horizontale
		}
	}
	SDL_RenderPresent(*renderer);
}



int main (int argc, char *argv[]) {


	SDL_Window* fenetre;
	SDL_Renderer* renderer;
	fenetre = NULL;
	renderer = NULL;
	SDL_DisplayMode DM;
	init(&fenetre,&renderer,&DM);
	affiche_grille(DM,&renderer);
	SDL_Delay(5000);
	quit(&fenetre,&renderer,OK);



}


