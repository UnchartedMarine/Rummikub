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

void affiche_grille(SDL_Window** fenetre,SDL_Renderer** renderer,SDL_DisplayMode DM)
{
	int largeur = (int)((float)DM.w * G_RATIO);
	int hauteur = (int)((float)DM.h * G_RATIO);
	SDL_Color orange = {255,127,40,255};
	float x=DM.w * (1.0 - G_RATIO)/2;
	float y=DM.h * (1.0 - G_RATIO)/2;
	SDL_Point depart ={x,y};
	SDL_Rect rect = {depart.x,depart.y,largeur,hauteur};
	SDL_SetRenderDrawColor(*renderer,orange.r,orange.g,orange.b,orange.a);
	/*if( SDL_RenderDrawRect(*renderer,&rect) != 0 )
	{
		printf("%s", SDL_GetError());
		quit(fenetre,renderer,E_SDL_DESSIN);
	}*/
	if ( affiche_grille_interieur(rect ,renderer) != 0)
	{
		printf("%s", SDL_GetError());
		quit(fenetre,renderer,E_SDL_DESSIN);
	}
	SDL_RenderPresent(*renderer);
}

int affiche_grille_interieur(SDL_Rect rect ,SDL_Renderer** renderer)
{
	/*
		Le rectangle contient toutes les informations de la grille.
	*/
	int i,j;
	Resolution res = {rect.w,rect.h};	
	int multiplicateur = multiplicateur_taille_image(res);
	int nbrLigneVertical = (int)((float)rect.w /((float)LARGEURIMAGE * multiplicateur));
	int nbrLigneHorizontale = (int)((float)rect.h /((float)HAUTEURIMAGE * multiplicateur));
	int espacementX = (int)((float)rect.w / (float)nbrLigneVertical);
	int espacementY = (int)((float)rect.h / (float)nbrLigneHorizontale);
	/*
	for(i = rect.x;i <= rect.x+rect.w;i += espacementY)
	{
		
		if( SDL_RenderDrawLine(*renderer,i,rect.y,i,rect.y+rect.h) != 0 )
			return -1;
	}
	for(j=rect.y;j<= rect.y+rect.h;j+=espacementX)
	{
		if( SDL_RenderDrawLine(*renderer,rect.x,j,rect.x+rect.w,j) != 0 )
			return -1;
	}
	*/
	int x = rect.x;
	int y = rect.y;
	for(i = 0;i <= nbrLigneVertical;i ++)
	{
		
		if( SDL_RenderDrawLine(*renderer,x,rect.y,x,rect.y+rect.h) != 0 )
			return -1;
		x+=espacementX;
	}
	for(j = 0;j <= nbrLigneHorizontale;j++)
	{
		if( SDL_RenderDrawLine(*renderer,rect.x,y,rect.x+rect.w,y) != 0 )
			return -1;
		y+=espacementY;
	}
	return 0;
}

float multiplicateur_taille_image(Resolution res)
{
	// TODO renvoyer le multiplicateur en fonction de la resolution.
	return 1;
}

int main (int argc, char *argv[]) {


	SDL_Window* fenetre;
	SDL_Renderer* renderer;
	fenetre = NULL;
	renderer = NULL;
	SDL_DisplayMode DM;
	init(&fenetre,&renderer,&DM);
	affiche_grille(&fenetre,&renderer,DM);
	SDL_Delay(10000);
	quit(&fenetre,&renderer,OK);



}


