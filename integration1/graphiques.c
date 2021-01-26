
#include<string.h>
#include<stdio.h>
#include "graphiques.h"

void init(SDL_DisplayMode* DM)
{

	if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS )) {
		printf("%s", SDL_GetError());
		quit(E_SDL_INIT);
	}
	if (0 != TTF_Init())
	{
		fprintf(stderr, "Erreur d'initialisation de SDL : %s\n", SDL_GetError());
		quit(E_TTF_INIT);
	}
	if (0 != SDL_GetCurrentDisplayMode(0, DM) ) {
		printf("%s", SDL_GetError());
		quit(E_GETDISPLAY);
	}
	init_fenetre_renderer(*DM);
}

void init_fenetre_renderer(SDL_DisplayMode DM)
{
	//printf("%d , %d\n",DM.w,DM.h);
	fenetre = SDL_CreateWindow("Rummikub", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DM.w,DM.h, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_INPUT_GRABBED);
	if (fenetre == NULL) {
		printf("%s", SDL_GetError());
		quit(E_SDL_FENETRE);
	}
	renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("%s", SDL_GetError());
		quit(E_SDL_RENDERER);
	}
}

void quit(erreur err)
{
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (fenetre != NULL) {
		SDL_DestroyWindow(fenetre);
		fenetre = NULL;
	}
	vide_tuiles();
	TTF_Quit();
	SDL_Quit();
	exit(err);
}

SDL_Texture* createTextureFromImage (SDL_Renderer** renderer,char* path) {
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(path);
	if (surface == NULL) {
		fprintf(stdout,"error load image: %s\n", IMG_GetError());
		return texture;
	}
	texture = SDL_CreateTextureFromSurface(*renderer, surface);
	SDL_FreeSurface(surface);
	return texture;

}
Texture createTextureFromText (TTF_Font* font, char* str, SDL_Color textColor) {

	Texture textTexture;
	SDL_Surface *surface = TTF_RenderText_Blended(font, str, textColor);
	if (surface == NULL) {
		printf("error load text: %s", TTF_GetError());
		textTexture.texture=NULL;
		return textTexture;
	}
	textTexture.texture = SDL_CreateTextureFromSurface(renderer, surface);
	textTexture.rect.h = surface->h;
	textTexture.rect.w = surface->w;
	SDL_FreeSurface(surface);
	return textTexture;

}

void affiche_texture(SDL_Texture* texture,SDL_Rect* destination)
{
	// peut etre pas utile car plus rapide de charger toutes les textures avant l'affichage
	SDL_RenderCopy(renderer, texture, NULL, destination);
	//SDL_RenderPresent(*renderer); //pose des probleme quand on veut afficher plusieurs textures.

}

Texture init_grille(SDL_DisplayMode DM,int *caseX, int *caseY)
{
	SDL_Rect infoGrille;
	int largeur = (int)((float)DM.w * G_RATIO_H);
	int hauteur = (int)((float)DM.h * G_RATIO_V_TAILLE)+1;
	SDL_Texture* grille = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET,largeur,hauteur);
	SDL_Color orange = {255,127,40,255};
	float x=DM.w * (1.0 - G_RATIO_H)/2;
	float y=DM.h * (1.0 - G_RATIO_V_POSITION)/2;
	infoGrille.x = x;
	infoGrille.y = y;
	infoGrille.w = largeur;
	infoGrille.h = hauteur;
	SDL_SetRenderTarget(renderer,grille);
	//fprintf(stdout,"%d %d %d %d \n",depart.x,depart.y,largeur,hauteur);
	SDL_SetRenderDrawColor(renderer,orange.r,orange.g,orange.b,orange.a);
	if ( init_grille_interieur(infoGrille,caseX,caseY) != 0)
	{
		printf("%s", SDL_GetError());
		quit(E_SDL_DESSIN);
	}
	SDL_RenderPresent(renderer);
	SDL_SetRenderTarget(renderer,NULL);
	Texture texture;
	texture.texture = grille;
	texture.rect = infoGrille;
	return texture;
}
int init_grille_interieur(SDL_Rect rect ,int *caseX, int *caseY)
{
	/*
		Le rectangle contient toutes les informations de la grille.
	*/
	int i,j;
	int espacementX = (int)((float)rect.w / (float)GRILLELARGEUR);
	int espacementY = (int)((float)rect.h / (float)GRILLEHAUTEUR);
	*caseX = espacementX;
	*caseY = espacementY;
	int x = 0;
	int y = 0;
	for(i = 0;i <= GRILLELARGEUR;i ++)
	{
		if( SDL_RenderDrawLine(renderer,x,0,x,GRILLEHAUTEUR * espacementY) != 0 )
			return -1;
		x+=espacementX;
	}
	for(j = 0;j <= GRILLEHAUTEUR;j++)
	{
		if( SDL_RenderDrawLine(renderer,0,y,GRILLELARGEUR * espacementX,y) != 0 )
			return -1;
		y+=espacementY;
	}
	return 0;
}

void init_textureTuiles()
{
	int i,j;
	textureTuiles = malloc(NBRCOUL * sizeof(SDL_Texture**));
	for (i = 0; i < NBRCOUL; i++)
	{
		if(i < NBRCOUL)
			textureTuiles[i] = malloc(NBRTUILLE * sizeof(SDL_Texture*));
		else
			textureTuiles[i] = malloc(sizeof(SDL_Texture*));
	}
	for ( i = 0; i < NBRCOUL-1; i++)
	{
		for (j = 0; j < NBRTUILLE; j++)
		{
			//TODO enumeration couleurs
			//chercher les tuiles avec le nom
			textureTuiles[i][j] = charge_tuille(i,j+1);
		}
	}
	textureTuiles[NBRCOUL-1][0] = charge_tuille(i,0);
}

SDL_Texture* charge_tuille(int couleur,int nombre)
{
	char couleurs[5][2] = {"B","L","R","V","J"};
	char* chemin = "Image/";
	char* extension = ".png";
	char numero[4];
	sprintf(numero,"%02d",nombre);
	char image[14];
	image[0]='\0';
	strcat(image,chemin);
	strcat(image,numero);
	strcat(image,couleurs[couleur]);
	strcat(image,extension);
	return createTextureFromImage(&renderer,image);
}
void vide_tuiles()
{
	int i,j;
	for ( i = 0; i < NBRCOUL-1; i++)
	{
		for (j = 0; j < NBRTUILLE; j++)
		{
			SDL_DestroyTexture(textureTuiles[i][j]);
		}
		free(textureTuiles[i]);
	}
	SDL_DestroyTexture(textureTuiles[NBRCOUL-1][0]);
	free(textureTuiles[NBRCOUL-1]);
	free(textureTuiles);
}
void affiche_tuille(SDL_Renderer** renderer,SDL_Rect* destination,SDL_Texture*** textureTuiles, int couleur,int nombre)
{
	if( nombre == 0 )
	{
		affiche_texture(textureTuiles[NBRCOUL][0],destination);
	}
	else
	{
		affiche_texture(textureTuiles[couleur][nombre],destination);
	}
}

void affiche_plateau(SDL_Texture* TexturePlateau[GRILLEHAUTEUR][GRILLELARGEUR],SDL_Rect infoCase)
{
	int i,j;
	int departX = infoCase.x;
	//SDL_Rect infoCase={grille.rect.x+1,grille.rect.y+1,caseX-2,caseY-2};
	for(i = 0;i<GRILLEHAUTEUR;i++)
	{
		for(j = 0;j<GRILLELARGEUR;j++)
		{
			if(TexturePlateau[i][j] != NULL)
			{
				printf("PASS\n");
				SDL_RenderCopy(renderer,TexturePlateau[i][j],NULL,&infoCase);
			}
			//printf("EMEKY%d\n",infoCase.x);
			infoCase.x+=infoCase.w+2;
		}
		infoCase.x=departX;
		infoCase.y+=infoCase.h+2;
	}
}

void affiche_main(SDL_Texture** main,int taille, SDL_DisplayMode DM, SDL_Rect infoGrille)
{
	SDL_Rect infoMain = {infoGrille.x,(DM.h *G_RATIO_V_POSITION),infoGrille.w,(DM.h * (1 - G_RATIO_V_POSITION))};
	SDL_Color jaune = {255,255,40,255};
	SDL_SetRenderDrawColor(renderer,jaune.r,jaune.g,jaune.b,jaune.a);
	//SDL_RenderDrawRect(renderer,&infoMain);
	int espacementX = infoMain.w/taille;
	int i;
	int x = infoMain.x;
	int y = infoMain.y;
	printf("HOHO%d\n",taille);
	for (i = 0; i < taille; i++)
	{
		printf("KOKOKO %d\n",i);
		SDL_RenderDrawLine(renderer,x,y,x,y+infoMain.h);
		SDL_Rect tuile = {x+1,y,espacementX,infoMain.h};
		SDL_RenderCopy(renderer,main[i],NULL,&tuile);
		x+=espacementX;
	}
}

void affiche_fin(SDL_Renderer** renderer,SDL_DisplayMode DM, SDL_Rect infoGrille,TTF_Font* police)
{
	//TODO Fin
	//enregistrer hitbox et textures
	//remplacer les surfaces par des textures
	SDL_Color bleu = {0,0,255,255};
	SDL_Color gris = {150, 150, 150,255};
	SDL_Surface* surface,*texte;
	SDL_Texture* bouton;
	int hauteur,largeur,x,y;
	hauteur = DM.h * 0.1;
	largeur = DM.w * 0.1;
	x = (infoGrille.x+infoGrille.w)+ (DM.w * 0.025);
	y = DM.h * UI_BAS;
	SDL_Rect infoBouton ={x,y,largeur,hauteur};
	SDL_Rect infoTexte = {largeur/6,hauteur/(2.5),largeur/4,(hauteur/2)};
	printf("%d\n",infoTexte.y);
	surface = SDL_CreateRGBSurface(0, largeur, hauteur, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, gris.r, gris.g, gris.b));
	texte = TTF_RenderUTF8_Shaded(police,"Fin du tour",bleu,gris);
	SDL_BlitSurface(texte,NULL,surface,&infoTexte);
	bouton = SDL_CreateTextureFromSurface(*renderer,surface);
	SDL_RenderCopy(*renderer,bouton,NULL,&infoBouton);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(texte);
	SDL_DestroyTexture(bouton);
	//doit donner info du rectangle
}
/*
Texture init_bouton_fin(SDL_DisplayMode DM, SDL_Rect infoGrille,TTF_Font* police)
{
	//TODO Fin
	//enregistrer hitbox et textures
	//remplacer les surfaces par des textures
	//createTextureFromText (TTF_Font* font, char* str, SDL_Color textColor)
	SDL_Color bleu = {0,0,255,255};
	SDL_Color gris = {150, 150, 150,255};
	//SDL_Surface* surface,*texte;
	SDL_Texture* bouton;
	Texture texte;
	int hauteur,largeur,x,y;
	hauteur = DM.h * 0.1;
	largeur = DM.w * 0.1;
	x = (infoGrille.x+infoGrille.w)+ (DM.w * 0.025);
	y = DM.h * UI_BAS;
	SDL_Rect infoBouton ={0,0,largeur,hauteur};
	SDL_Rect infoTexte = {largeur/6,hauteur/(2.5),largeur/4,(hauteur/2)};
	bouton = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET,largeur,hauteur);
	texte = createTextureFromText(police,"Fin du tour",bleu);
	SDL_SetRenderTarget(renderer,bouton);
	SDL_RenderFillRect(renderer,&infoBouton);
	infoBouton.x = x;
	infoBouton.y = y;
	surface = SDL_CreateRGBSurface(0, largeur, hauteur, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, gris.r, gris.g, gris.b));
	texte = TTF_RenderUTF8_Shaded(police,"Fin du tour",bleu,gris);
	SDL_BlitSurface(texte,NULL,surface,&infoTexte);
	bouton = SDL_CreateTextureFromSurface(*renderer,surface);
	SDL_RenderCopy(renderer,bouton,NULL,&infoBouton);
	SDL_DestroyTexture(bouton);
	//doit donner info du rectangle
}
*/
void affiche_nombre_tuille(SDL_Renderer** renderer,SDL_DisplayMode DM, SDL_Rect infoGrille,TTF_Font* police,int nombre)
{
	SDL_Color vert = {0,255,0,255};
	SDL_Color gris = {150, 150, 150,255};
	SDL_Surface* surface,*texte;
	SDL_Texture* bouton;
	char buffer[23];
	int hauteur,largeur,x,y;
	hauteur = DM.h * 0.1;
	largeur = DM.w * 0.15;
	x = (infoGrille.x+infoGrille.w)+ (DM.w * 0.025);
	y = 0;
	SDL_Rect infoBouton ={x,y,largeur,hauteur};
	SDL_Rect infoTexte = {0,hauteur/(2.5),largeur/4,(hauteur/2)};
	printf("%d\n",infoTexte.y);
	surface = SDL_CreateRGBSurface(0, largeur, hauteur, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, gris.r, gris.g, gris.b));
	sprintf(buffer,"Tuille restante : %03d",nombre);
	texte = TTF_RenderUTF8_Shaded(police,buffer,vert,gris);
	SDL_BlitSurface(texte,NULL,surface,&infoTexte);
	bouton = SDL_CreateTextureFromSurface(*renderer,surface);
	SDL_RenderCopy(*renderer,bouton,NULL,&infoBouton);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(texte);
	SDL_DestroyTexture(bouton);
}




