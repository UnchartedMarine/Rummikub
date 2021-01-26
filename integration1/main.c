#include "main.h"

void reset_liasion_graphique(ELEMENT* positionPlateau[GRILLELARGEUR][GRILLELARGEUR],SDL_Texture* TexturePlateau[GRILLELARGEUR][GRILLELARGEUR])
{
	int i,j;
	for ( i = 0; i < GRILLEHAUTEUR; i++)
	{
		for ( j = 0; j < GRILLELARGEUR; j++)
		{
			positionPlateau[i][j] = NULL;
			TexturePlateau[i][j] = NULL;
		}
	}
}

void lie_plateau_graphique(ELEMENT* positionPlateau[GRILLEHAUTEUR][GRILLELARGEUR],SDL_Texture* TexturePlateau[GRILLEHAUTEUR][GRILLELARGEUR])
{
	int i = 0;
	int j = 0;
	ELEMENT_PLATEAU *element=plateau->premier;
	reset_liasion_graphique(positionPlateau,TexturePlateau);
	while(element != NULL)
	{
		int taille = nb_elements_liste(element->liste);
		ELEMENT* premiere_liste_tuile = element->liste->premier;
		ELEMENT* liste_tuile= premiere_liste_tuile;
		if (j + taille <= GRILLELARGEUR)
		{
			while(liste_tuile != NULL)
			{
				/* remplit le tableau avec les pointer vers la liste */
				positionPlateau[i][j] = premiere_liste_tuile;
				/*remplit le tableau avec les texture SDL*/
				TUILE tuile = liste_tuile->tuile;
				if( tuile.num == 0)
				{
					TexturePlateau[i][j]=textureTuiles[NBRCOUL-1][0];
				}
				else
				{
					TexturePlateau[i][j]=textureTuiles[tuile.coul-1][tuile.num-1];
				}
				liste_tuile = liste_tuile->suivant;
				j++;
			}
			j++;
		}
		else if (j + taille > GRILLELARGEUR)
		{
			i++;
			j = 0;
		}
		else
		{
			j++;
		}
		element = element->suivant;
	}
}
SDL_Texture**  lie_main_graphique(LISTE* mainData,int* taille) //renvoie le nombre de tuille affiché
{
	//TODO detection main
	//utiliser infomain + nombre de tuiles.
	//exemple : infomaincase.w/nbtuile == case selectionné
	SDL_Texture** mainTexture;
	*taille = nb_elements_liste(mainData);
	int i = 0;
	mainTexture = malloc(*taille * sizeof(SDL_Texture*));
	ELEMENT* element = mainData->premier;
	while(element != NULL)
	{
		TUILE tuile = element->tuile;
		if( tuile.num == 0)
			{
				mainTexture[i]=textureTuiles[NBRCOUL-1][0];
			}
			else
			{
				mainTexture[i]=textureTuiles[tuile.coul-1][tuile.num-1];
			}
		if(mainTexture[i] != NULL)
		{
			printf("NULL\n");
		}
		i++;
		element = element->suivant;
	}
	return mainTexture;
}

int main (int argc, char *argv[])
{
	srand(time(NULL));
	fenetre = NULL;
	renderer = NULL;
	textureTuiles = NULL;
	SDL_DisplayMode DM;
	//SDL_Rect infoGrille; // point de départ + taille
	Texture grille;
	int caseX,caseY;
	init(&DM);
	TTF_Font* police = NULL;
	police = TTF_OpenFont("theboldfont.ttf",22);
	if(!police) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
	grille = init_grille(DM,&caseX,&caseY);
	SDL_Rect infoCase={grille.rect.x+1,grille.rect.y+1,caseX-2,caseY-2};
	init_textureTuiles(&renderer);
	ELEMENT* positionPlateau[GRILLEHAUTEUR][GRILLELARGEUR];
	SDL_Texture* TexturePlateau[GRILLEHAUTEUR][GRILLELARGEUR];
	int niveauPioche = 0;
	plateau=cree_plateau();
	init_pioche();
	int i;
	for( i = 0; i<10;i++)
	{
		printf("deux\n");
		LISTE* liste = cree_liste();
		init_main(liste,&niveauPioche);//juste pour generer des liste aléatoires
		ajoute_plateau(plateau,liste);
		lit_liste(liste);
		//fprintf(stdout,"OK%d\n",nb_elements_liste(liste));
	}
	lie_plateau_graphique(positionPlateau,TexturePlateau);
	//fprintf(stdout, "info : %d;%d\n",caseX,caseY);
	//SDL_Texture* test = textureTuiles[1][10];
	affiche_texture(grille.texture,&grille.rect);
	affiche_plateau(TexturePlateau,infoCase);
	SDL_RenderPresent(renderer);
	//affiche_texture(test,&infoCase);
	LISTE* liste = cree_liste();
	init_main(liste,&niveauPioche);
	init_main(liste,&niveauPioche);
	init_main(liste,&niveauPioche);
	int taille_main;
	SDL_Texture** mainTexture  = lie_main_graphique(liste,&taille_main);
	affiche_main(mainTexture,taille_main,DM,grille.rect);
	affiche_fin(&renderer,DM,grille.rect,police);
	affiche_nombre_tuille(&renderer,DM,grille.rect,police,35);
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
					int x = (e.button.x-grille.rect.x)/caseX;
	                int y = (e.button.y-grille.rect.y)/caseY;
	                fprintf(stdout, "\tcase selectionee : %d;%d\n",x ,y );
	                break;
			}
		}
	}
	SDL_DestroyTexture(grille.texture);
	TTF_CloseFont(police);
	quit(OK);



}