#include "main.h"
//vide les tableaux de liaison entre la liste des series et la grille
void reset_liasion_graphique(LISTE* positionPlateau[GRILLELARGEUR][GRILLELARGEUR],SDL_Texture* TexturePlateau[GRILLELARGEUR][GRILLELARGEUR])
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
//Lit le plateau du modèle et remplie deux tableaux
//positionPlateau : pour chaque case avec une tuile, on remplit avec le pointer vers la tete d'une combinaison
//TexturePlateau contient les textures des tuiles a afficher sur la grille. ce tableau est utiliser par affiche_plateau
void lie_plateau_graphique(LISTE* positionPlateau[GRILLEHAUTEUR][GRILLELARGEUR],SDL_Texture* TexturePlateau[GRILLEHAUTEUR][GRILLELARGEUR])
{
	int i = 0;
	int j = 0;
	ELEMENT_PLATEAU *element=plateau->premier;
	reset_liasion_graphique(positionPlateau,TexturePlateau);
	while(element != NULL)//on parcourt le plateau et regarde chaque liste
	{
		int taille = nb_elements_liste(element->liste);
		ELEMENT* liste_tuile= element->liste->premier;
		if (j + taille <= GRILLELARGEUR)
		{
			while(liste_tuile != NULL)
			{
				/* remplit le tableau avec les pointer vers la liste */
				positionPlateau[i][j] = element->liste;
				/*remplit le tableau avec les texture SDL*/
				TUILE tuile = liste_tuile->tuile;
				if( tuile.num == 0)// Si la tuile est un joker
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
			j++;// on met un espace pour pouvoir distinguer les listes
		}
		else if (j + taille > GRILLELARGEUR)
		{
			i++;
			j = 0;
		}
		element = element->suivant;
	}
}
//Transforme une liste de tuile en un tableau de Textures correspondantes
//Renvoie le tableau des Textures
//Parametre: prend la liste a afficher et un entier qui permet de savoir la taille du tableau en sortie
SDL_Texture**  lie_liste_graphique(LISTE* mainData,int* taille) //renvoie le nombre de tuille affiché
{
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
		i++;
		element = element->suivant;
	}
	return mainTexture;
}
//Creer la texture a afficher de la combinaison en train d'etre manipuler
//La texture est stocker dans la variable vTextures et sa hitbox dans vHitboxes
//Parametre : tableau contenant un pointer vers les textures des tuiles, la taille du tableau
//et une variable qui permet de savoir si la fonction est utiliser pour la premierefois
void lie_select_graphique(SDL_Texture** listeData,int taille,int premierefois)
{
	SDL_Rect infoListePosition = {grille.rect.x,grille.rect.y+grille.rect.h*0.3,grille.rect.w,(grille.rect.h*0.5)};
	SDL_Color bleu = {255,255,40,255};
	SDL_SetRenderDrawColor(renderer,bleu.r,bleu.g,bleu.b,bleu.a);
	SDL_Texture* textureSelect = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET,infoListePosition.w,infoListePosition.h);
	//
	SDL_SetRenderTarget(renderer,textureSelect);
	if(taille == 0)
	{
		infoListePosition.x = 0;
		infoListePosition.y = 0;
		SDL_RenderDrawRect(renderer,&infoListePosition);
		infoListePosition.x = grille.rect.x;
		infoListePosition.y = grille.rect.y+grille.rect.h*0.3;
	}
	else
	{
		dessine_liste(infoListePosition,taille,listeData);
	}
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_SetRenderTarget(renderer,NULL);
	Hitbox box = {infoListePosition};
	vHitboxes[0]=box;
	Texture texture = {textureSelect,infoListePosition};
	if(premierefois == 0 )
	{
		SDL_DestroyTexture(vTextures[2].texture);
		vTextures[2]=texture;

	}
	else
	{
		nbHitboxes++;
		vTextures[2] = texture;//2
		nbTextures++;
	}
}
//Creer la texture a afficher de la reserve.
//La reserve contient l'ensemble des tuiles qui on était retirer de combinaison sur le plateau qui reste a jouer.
//La texture est stocker dans la variable vTextures et sa hitbox dans vHitboxes
//Parametre : tableau contenant un pointer vers les textures des tuiles, la taille du tableau
//et une variable qui permet de savoir si la fonction est utiliser pour la premierefois
void lie_reserve_graphique(SDL_Texture** listeData,int taille,int premierefois)
{
	SDL_Rect infoListePosition = {0,0,grille.rect.w,(grille.rect.h*0.4)};
	SDL_Color jaune = {255,255,40,255};
	SDL_SetRenderDrawColor(renderer,jaune.r,jaune.g,jaune.b,jaune.a);
	SDL_Texture* textureSelect = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET,infoListePosition.w,infoListePosition.h);
	SDL_SetRenderTarget(renderer,textureSelect);
	if(taille == 0)
	{
		SDL_RenderDrawRect(renderer,&infoListePosition);
		infoListePosition.x = grille.rect.x;
	}
	else
	{
		infoListePosition.x = grille.rect.x;
		dessine_liste(infoListePosition,taille,listeData);
	}
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_SetRenderTarget(renderer,NULL);
	Hitbox box = {infoListePosition};
	vHitboxes[1]=box;
	Texture texture = {textureSelect,infoListePosition};
	if(premierefois == 0 )
	{
		SDL_DestroyTexture(vTextures[3].texture);
		vTextures[3]=texture;
	}
	else
	{
		nbHitboxes++;
		vTextures[3] = texture;//2
		nbTextures++;
	}
}
//Creer la texture a afficher de la main
//La reserve contient l'ensemble des tuiles qui on était retirer de combinaison sur le plateau qui reste a jouer.
//La texture est stocker dans la variable vTextures et sa hitbox dans vHitboxes
//Parametre : tableau contenant un pointer vers les textures des tuiles, la taille du tableau
//et une variable qui permet de savoir si la fonction est utiliser pour la premierefois
void lie_main_graphique(SDL_Texture** listeData,int taille,int premierefois)
{
	SDL_Rect infoListePosition = {grille.rect.x,(DM.h *G_RATIO_V_POSITION),grille.rect.w,(DM.h * (1 - G_RATIO_V_POSITION))};
	SDL_Color jaune = {255,255,40,255};
	SDL_SetRenderDrawColor(renderer,jaune.r,jaune.g,jaune.b,jaune.a);
	SDL_Texture* textureSelect = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET,infoListePosition.w,infoListePosition.h);
	SDL_SetRenderTarget(renderer,textureSelect);
	if(taille == 0)
	{
		SDL_RenderDrawRect(renderer,&infoListePosition);
		infoListePosition.x = grille.rect.x;
	}
	else
	{
		infoListePosition.x = grille.rect.x;
		dessine_liste(infoListePosition,taille,listeData);
	}
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_SetRenderTarget(renderer,NULL);
	Hitbox box = {infoListePosition};
	vHitboxes[2]=box;
	Texture texture = {textureSelect,infoListePosition};
	if(premierefois == 0 )
	{
		SDL_DestroyTexture(vTextures[4].texture);
		vTextures[4]=texture;
	}
	else
	{
		nbHitboxes++;
		vTextures[4] = texture;//2
		nbTextures++;
	}
}
//Fonction commune a tout les lie_XXX_graphique.
//Cette fonction permet de dessiner a l'ecran les textures.
//Parametre : le rectangle où on doit dessiner, la taille du tableau et le tableau des textures.
void dessine_liste(SDL_Rect infoListePosition,int taille,SDL_Texture** listeData)
{
	int espacementX = infoListePosition.w/taille;
	int i;
	int x =0;
	int y =0;
	for (i = 0; i < taille; i++)
	{
		SDL_RenderDrawLine(renderer,x,y,x,y+infoListePosition.h);
		SDL_Rect tuile = {x+1,y,espacementX,infoListePosition.h};
		SDL_RenderCopy(renderer,listeData[i],NULL,&tuile);
		x+=espacementX;
	}
}
//Cette fonction permet de selectionner ou afficher les listes.
//Parametre : la liste a afficher,
//mode correspond a l'endroit ou l'afficher :
//0 = la suite en train d'etre manipuler, 1 = affichage de la reserve, 2 = affichage de la main.
//si c'est la premiere fois que la fonction est utilisee.
void affiche_liste_zoom(LISTE* liste,int mode,int premierefois)
{
	int taille;
	SDL_Texture** data = lie_liste_graphique(liste,&taille);
	if(mode == 0)//select
	{
		lie_select_graphique(data,taille,premierefois);
	}
	else if(mode == 1)//reserve
	{
		lie_reserve_graphique(data,taille,premierefois);
	}
	else if(mode == 2)//main
	{
		lie_main_graphique(data,taille,premierefois);
	}
	free(data);
}
//Appelle handle_mickey_mouse avec le bon menu rederect
//ici le menu qui correspond à la grille
void update_menu_jeu(int* quit, int* clickHandlerState, int* indexButton,SDL_Event e,  int* oldIndex) {
	while(SDL_PollEvent(&e) != 0) {
		handle_mickey_mouse(quit,clickHandlerState,indexButton,e,oldIndex,redirect_menu_jeu);
	}
}
//Appelle handle_mickey_mouse avec le bon menu rederect
//ici le menu utiliser lors de la modification d'une combinaison
void update_menu_zoom(int* quit, int* clickHandlerState, int* indexButton,SDL_Event e,  int* oldIndex) {
	while(SDL_PollEvent(&e) != 0) {
		handle_mickey_mouse(quit,clickHandlerState,indexButton,e,oldIndex,redirect_menu_zoom);
	}
}
//Les fonction redirect indique comment chaque bouton doit reagir en fonction du menu.
void redirect_menu_jeu(int index, int* quit) {
	switch(index) {
		case -1:
			inGame = 2;
			destroy_menu();
			init_menu_zoom();
			update=update_menu_zoom;
			liste_manipulation = malloc(5 * sizeof(liste_manipulation));
			nb_liste_manipulation = 1;
			liste_manipulation[0] = copie_liste(positionPlateau[tuileSelect.y][tuileSelect.x]);
			affiche_liste_zoom(liste_manipulation[0],0,1);
			reserve =cree_liste();
			affiche_liste_zoom(reserve,1,1);
			affiche_liste_zoom(main_actuelle,2,1);
			break;
		case 0:
			printf("Fin de tour\n");
			break;
		case 1:
			inGame = 2;
			destroy_menu();
			init_menu_zoom();
			update=update_menu_zoom;
			reserve =cree_liste();
			liste_manipulation = malloc(5 * sizeof(liste_manipulation));
			liste_manipulation[0] =cree_liste();
			nb_liste_manipulation = 1;
			affiche_liste_zoom(liste_manipulation[0],0,1);
			reserve =cree_liste();
			affiche_liste_zoom(reserve,1,1);
			affiche_liste_zoom(main_actuelle,2,1);
			break;
		case 2:
			printf("Quitter\n");
			break;
	}
}
void redirect_menu_zoom(int index, int* quit) {
	printf("index :%d\n",index);
	switch(index) {
		case 0: // annuler
			inGame =1;
			destroy_menu();
			init_menu_jeu();
			update= update_menu_jeu;
			break;
			case 1:
				break;
			case 2:; // ajout début
				ajouter(1,0);
				break;
			case 3 :;
				printf("taille : %d\n",nb_elements_liste(liste_manipulation[liste_actuelle]));
				ajouter(nb_elements_liste(liste_manipulation[liste_actuelle])+1,1);
				break;
			case 4:// retirer début
				retirer(1);
				break;
			case 5 : ; //retirer fin
				int listeTaille = nb_elements_liste(liste_manipulation[liste_actuelle]);
				retirer(listeTaille);
				break;
			case 6 : ;//Scinder en deux
				scinder();
				break;
			case 8: ;//precedent
				changer_de_liste(0);
				break;
			case 9: ;//suivant
				changer_de_liste(1);
				break;
		default:
			printf("TEST\n");
			break;
	}
}
//Fonction qui permet de scinder une combinaison en deux
//La nouvelle liste est ajouter au tableau liste_manipulation
//Elle fait la liaison entre le graphique et le modele
int scinder()
{
	SDL_Event e; int x,y,quit;
	quit =0;
	while(quit == 0)
	{
		SDL_PollEvent(&e);
		if(e.type == SDL_MOUSEBUTTONUP)
		{
			SDL_GetMouseState(&x, &y);
			if(check_liste_select(x,y,1))
			{
				nb_liste_manipulation++;
				liste_manipulation[nb_liste_manipulation-1] = separer_liste_en_deux(liste_manipulation[liste_actuelle],tuileSelect.x);
				int taille;
				SDL_Texture** data = lie_liste_graphique(liste_manipulation[liste_actuelle],&taille);
				lie_select_graphique(data,taille,0);
				quit =1;
			}
			quit =1;
		}
	}
	return 0;
}
//Fonction qui permet d'ajouter une tuile a une combinaison
//Elle fait la liaison entre le graphique et le modele
//Parametre : la position dans la combinaison ou on veut ajouter et savoir si en rajoute a la fin ou au début.
int ajouter(int position,int ajoutFin)
{
	SDL_Event e; int x,y,quit;
	quit =0;
	while(quit == 0)
	{
		SDL_PollEvent(&e);
		if(e.type == SDL_MOUSEBUTTONUP)
		{
			SDL_GetMouseState(&x, &y);
			if(check_liste_select(x,y,0))
			{
				//printf("OK : %d %d\n",tuileSelect.x,tuileSelect.y);
				if(tuileSelect.y == 1)//reserve
				{
					TUILE tuile = renvoie_tuile_via_position(reserve,tuileSelect.x+1);
					enleve_element_liste(reserve,tuileSelect.x+1);
					lit_liste(liste_manipulation[liste_actuelle]);
					if(position == 1 && ajoutFin == 0)
					{
						placement_element_liste(liste_manipulation[liste_actuelle],tuile,position);
					}
					else if (position == 1 && ajoutFin == 1)
					{
						placement_element_liste(liste_manipulation[liste_actuelle],tuile,position+1);
					}
					else
						placement_element_liste(liste_manipulation[liste_actuelle],tuile,position);
					affiche_liste_zoom(reserve,1,0);
					affiche_liste_zoom(liste_manipulation[liste_actuelle],0,0);
				}
				else // main
				{
					TUILE tuile = renvoie_tuile_via_position(main_actuelle,tuileSelect.x+1);
					enleve_element_liste(main_actuelle,tuileSelect.x+1);
					if(position == 1 && ajoutFin == 0)
					{
						placement_element_liste(liste_manipulation[liste_actuelle],tuile,position);
					}
					else if (position == 1 && ajoutFin == 1)
					{
						placement_element_liste(liste_manipulation[liste_actuelle],tuile,position+1);
					}
					else
						placement_element_liste(liste_manipulation[liste_actuelle],tuile,position);
					affiche_liste_zoom(main_actuelle,2,0);
					affiche_liste_zoom(liste_manipulation[liste_actuelle],0,0);
				}
				return 1;
			}
			quit =1;
		}
	}
	return 0;
}
//Fonction qui permet de retirer une tuile d'une combinaison
//Elle met la tuile dans la reserve
//Elle fait la liaison entre le graphique et le modele
int retirer(int position)
{
	if(nb_elements_liste(liste_manipulation[liste_actuelle]) > 0)
	{
		TUILE tuile = renvoie_tuile_via_position(liste_manipulation[liste_actuelle],position);
		enleve_element_liste(liste_manipulation[liste_actuelle],position);
		ajoute_liste(reserve,tuile);
		affiche_liste_zoom(reserve,1,0);
		affiche_liste_zoom(liste_manipulation[liste_actuelle],0,0);
	}
	return 1;
}
//Fonction qui permet de parcourir les differentes combinaison une fois que la liste de base a était scinder en deux.
//Elle fait la liaison entre le graphique et le modele
int changer_de_liste(int direction)
{
	if(nb_liste_manipulation < 2)
	{
		return 0;
	}
	else if (nb_liste_manipulation >= 2)
	{
		if(direction == 0)// gauche
		{
			if(liste_actuelle -1 <0)
			{
				liste_actuelle = nb_liste_manipulation -1;
			}
			else
			{
				liste_actuelle--;
			}
		}
		else if(direction == 1)
		{
			if(liste_actuelle+1 > nb_liste_manipulation-1)
			{
				liste_actuelle = 0;
			}
			else
			{
				liste_actuelle++;
			}
		}
		affiche_liste_zoom(liste_manipulation[liste_actuelle],0,0);
		return 1;
	}
	return 0;
}
//Fonction qui permet de savoir si l'utilisateur clique sur les listes (soit la reserve,combinaison manipule ou la main)
//Si l'utilisateur clique bien dans les zones alors la fonction donne la position de la tuile selectionnee.
//Parametre : La position de la souris et le mode 0 = l'utilisateur veut ajouter une tuile, 1 = l'utilisateur veut scinder la suite en deux
int check_liste_select(int x, int y,int mode)
{
	switch (mode)
	{
	case 0: //ajout
		if((x > vHitboxes[1].box.x && x < (vHitboxes[1].box.x+vHitboxes[1].box.w)) //reserve
		&& (y > vHitboxes[1].box.y && y < (vHitboxes[1].box.y+vHitboxes[1].box.h)))
		{
			int taille = nb_elements_liste(reserve);
			if(taille == 0)
			{
				return 0;
			}
			else
			{
				int caseX = vHitboxes[1].box.w/taille;
				tuileSelect.x = (x-vHitboxes[1].box.x)/caseX;
				tuileSelect.y = 1;
				return 1;
			}

		}
		if((x > vHitboxes[2].box.x && x < (vHitboxes[2].box.x+vHitboxes[2].box.w)) // Main
		&& (y > vHitboxes[2].box.y && y < (vHitboxes[2].box.y+vHitboxes[2].box.h)))
		{
			int caseX = vHitboxes[2].box.w/nb_elements_liste(main_actuelle);
			tuileSelect.x = (x-vHitboxes[2].box.x)/caseX;
			tuileSelect.y = 0;
			return 1;
		}
		return 0;
		break;
	case 1: //scinder
		if((x > vHitboxes[0].box.x && x < (vHitboxes[0].box.x+vHitboxes[0].box.w)) // select
		&& (y > vHitboxes[0].box.y && y < (vHitboxes[0].box.y+vHitboxes[0].box.h)))
		{
			int taille = nb_elements_liste(liste_manipulation[liste_actuelle]);
			if(taille == 0)
			{
				return 0;
			}
			else
			{
				int caseX = vHitboxes[0].box.w/taille;
				tuileSelect.x = (x-vHitboxes[0].box.x)/caseX;
				tuileSelect.y = 0;
				return 1;
			}
		}
		return 0;
		break;
	}
	return 0;
}

int main (int argc, char *argv[])
{
	srand(time(NULL));
	fenetre = NULL;
	renderer = NULL;
	textureTuiles = NULL;
	init_graphique(&DM);
	grille = init_grille(DM,&caseX,&caseY);
	positionPlateau= malloc(sizeof(LISTE*[GRILLEHAUTEUR][GRILLELARGEUR]));
	TexturePlateau= malloc(sizeof(SDL_Texture*[GRILLEHAUTEUR][GRILLELARGEUR]));
	SDL_Rect infoCase={grille.rect.x+1,grille.rect.y+1,caseX-2,caseY-2};
	int niveauPioche = 0;
	plateau=cree_plateau();
	init_pioche();
	int i;
	for( i = 0; i<10;i++)
	{
		LISTE* liste = cree_liste();
		init_main(liste,&niveauPioche);//juste pour generer des liste aléatoires
		ajoute_plateau(liste);
	}
	lie_plateau_graphique(positionPlateau,TexturePlateau);
	SDL_RenderCopy(renderer, grille.texture, NULL, &grille.rect);
	affiche_plateau(TexturePlateau,infoCase);
	//on créer une main et on la remplit a chaque fois.
	main_actuelle = cree_liste();
	init_main(main_actuelle,&niveauPioche);
	init_main(main_actuelle,&niveauPioche);
	init_main(main_actuelle,&niveauPioche);
	int taille_main;
	SDL_Texture** mainTexture  = lie_liste_graphique(main_actuelle,&taille_main);
	affiche_main(mainTexture,taille_main,DM,grille.rect);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	//on va directement sur le menu avec la grille
	//les menu precedent ne sont pas implimentee dans cette version
	update = update_menu_jeu;
	inGame=1;
	SDL_Event e;int exit = 0;int clickHandlerState = 0;int indexButton = -1;int oldIndex = -1;//int x,y;
	init_menu_jeu();
	while (exit != 1)
	{
		update(&exit, &clickHandlerState, &indexButton, e, &oldIndex);
		SDL_RenderClear(renderer);
		if(inGame == 1)
		{
			SDL_RenderCopy(renderer, grille.texture, NULL, &grille.rect);
			affiche_main(mainTexture,taille_main,DM,grille.rect);
			affiche_plateau(TexturePlateau,infoCase);
		}
		for (int i = 0; i < nbTextures; i++) {
			SDL_RenderCopy(renderer, vTextures[i].texture, NULL, &vTextures[i].rect);
		}
		for (int i = 0; i < nbButtons; i++) {
			SDL_RenderCopy(renderer, bTextures[i].buttonTexture.texture, NULL, &bTextures[i].buttonTexture.rect);
			SDL_RenderCopy(renderer, bTextures[i].textTexture.texture, NULL, &bTextures[i].textTexture.rect);
		}
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(grille.texture);
	Mix_FreeMusic(gMusic);
    Mix_FreeChunk(onClick);
    Mix_FreeChunk(unClick);
    Mix_Quit();
	quit(OK);



}