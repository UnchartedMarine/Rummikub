
#include<string.h>
#include<stdio.h>
#include "graphiques.h"

//initialise les module necessaire a la SDL.
void init_module(SDL_DisplayMode* DM)
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
	if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024) < 0 ) {
            printf("%s", Mix_GetError() );
			quit(E_SDL_SON);
    }
    gMusic = Mix_LoadMUS("Sons/blastrisa.mp3");
    onClick = Mix_LoadWAV("Sons/onClick.wav");
    unClick = Mix_LoadWAV("Sons/unClick.wav");
    Mix_PlayMusic(gMusic, -1);
	init_fenetre_renderer(*DM);
}
//initialise la fenetre et le renderer pour pouvoir dessiner sur la fenetre
void init_fenetre_renderer(SDL_DisplayMode DM)
{
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
//Charge toutes les tuiles dans le tableau tuileTextures
//On charge les tuiles a partir de d'image creer avec le generateur d'image (autre programme)
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
			textureTuiles[i][j] = charge_tuille(i,j+1);
		}
	}
	textureTuiles[NBRCOUL-1][0] = charge_tuille(i,0);
}
//Renvoie la texture d'une tuile a partir du nombre et de la couleur de la tuile
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
	return create_texture_from_image(image);
}
//Creer une SDL_Texture a partir d'une image
SDL_Texture* create_texture_from_image (char* path)
{

	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(path);
	if (surface == NULL) {
		printf("error load image: %s", IMG_GetError());
		return texture;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;

}
//Renvoie la texture et la hitbox de la grille
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
//Sous-fonction de init_grille qui permet de faire l'interieur de la grille
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
//initialise tout les elements necessaire au graphique
void init_graphique(SDL_DisplayMode* DM)
{
	init_module(DM);
	init_textureTuiles();
}
//Fonction qui prend un tableau des texture des tuiles de la main et l'affiche.
void affiche_main(SDL_Texture** main,int taille, SDL_DisplayMode DM, SDL_Rect infoGrille)
{
	SDL_Rect infoMain = {infoGrille.x,(DM.h *G_RATIO_V_POSITION),infoGrille.w,(DM.h * (1 - G_RATIO_V_POSITION))};
	SDL_Color jaune = {255,255,40,255};
	SDL_SetRenderDrawColor(renderer,jaune.r,jaune.g,jaune.b,jaune.a);
	int espacementX = infoMain.w/taille;
	int i;
	int x = infoMain.x;
	int y = infoMain.y;
	for (i = 0; i < taille; i++)
	{
		SDL_RenderDrawLine(renderer,x,y,x,y+infoMain.h);
		SDL_Rect tuile = {x+1,y,espacementX,infoMain.h};
		SDL_RenderCopy(renderer,main[i],NULL,&tuile);
		x+=espacementX;
	}
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
}
//Prend le tableau des textures des tuiles du plateau et les affiche
//Ce tableau est generer a partir du modele dans le main.c
void affiche_plateau(SDL_Texture* TexturePlateau[GRILLEHAUTEUR][GRILLELARGEUR],SDL_Rect infoCase)
{
	int i,j;
	int departX = infoCase.x;
	for(i = 0;i<GRILLEHAUTEUR;i++)
	{
		for(j = 0;j<GRILLELARGEUR;j++)
		{
			if(TexturePlateau[i][j] != NULL)
			{
				SDL_RenderCopy(renderer,TexturePlateau[i][j],NULL,&infoCase);
			}
			infoCase.x+=infoCase.w+2;
		}
		infoCase.x=departX;
		infoCase.y+=infoCase.h+2;
	}
}
//Vide le tableau textureTuiles de toutes les textures de tuiles pour eviter des memory leak
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
//fonction de nettoyage lorsqu'on quitte le programme
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
	destroy_menu();
	vide_tuiles();
	TTF_Quit();
	SDL_Quit();
	exit(err);
}





/***************************************
TODO
Fonction pour l'interaction avec les hitbox et boutons.
A voir si on met dans un autre fichier
Faire un fichier pour include la SDL

*****************************************/

/*
* Creates a text texture and adds it to the vTextures array.
*/
void create_text(int compt, int x, int y, SDL_Color color, TTF_Font* font, char* text) {
	Texture textTexture;
	textTexture = create_texture_from_text(font, text, color);
	textTexture.rect.x = x - textTexture.rect.w/2;
	textTexture.rect.y = y - textTexture.rect.h/2;
	vTextures[compt] = textTexture;
	nbTextures++;
}
/*
* Same as above except the size (height and width) of the texture is given at the same time, so it comes
* out as a Texture instead of a SDL_Texture*. Only thing to change is the position.
*/
Texture create_texture_from_text (TTF_Font* font, char* str, SDL_Color textColor)
{

	Texture textTexture;
	textTexture.texture = NULL;
	SDL_Surface *surface = TTF_RenderText_Blended(font, str, textColor);
	if (surface == NULL) {
		printf("error load text: %s", TTF_GetError());
		return textTexture;
	}
	textTexture.texture = SDL_CreateTextureFromSurface(renderer, surface);
	textTexture.rect.h = surface->h;
	textTexture.rect.w = surface->w;
	SDL_FreeSurface(surface);
	return textTexture;

}
/*
* Creates a Hitbox and a ButtonTexture and adds them to bHitboxes and bTextures resp.
*/
void create_button(int compt, int x, int y,int w, int h,SDL_Texture* idle, SDL_Color color, TTF_Font* font, char* text) {
	SDL_Rect box;
	box.x = x-w/2;
	box.y = y-h/2;
	box.w = w;
	box.h = h;
	bHitboxes[compt].box = box;
	ButtonTexture button;Texture texture;Texture textTexture;
	texture.texture = idle;
	texture.rect = box;
	textTexture = create_texture_from_text(font, text, color);
	textTexture.rect.x = x - textTexture.rect.w/2;
	textTexture.rect.y = y - textTexture.rect.h/2;
	button.buttonTexture = texture;
	button.textTexture = textTexture;
	bTextures[compt] = button;
	nbButtons++;
}

void init_buttonState()
{
	buttonStates = (SDL_Texture**)malloc (BOUTTONSTATES*sizeof(SDL_Texture*));
	buttonStates[0] = create_texture_from_image("buttons/idle.png");
	buttonStates[1] = create_texture_from_image("buttons/mouseOver.png");
	buttonStates[2] = create_texture_from_image("buttons/mouseClick.png");
}
void init_menu_zoom()
{
	init_buttonState();
	nbButtons = 0;
	nbTextures = 0;
	nbHitboxes = 0;
	bHitboxes = (Hitbox *) malloc(10 * sizeof(Hitbox));
	vHitboxes = (Hitbox *) malloc(3*sizeof(Hitbox));
	bTextures = (ButtonTexture *) malloc(10 * sizeof(ButtonTexture));
	vTextures = (Texture *) malloc(5 * sizeof(Texture));
	SDL_Color color = {255, 0, 0};
	TTF_Font* font = TTF_OpenFont("theboldfont.ttf", 14);
	//initialize "textbox"
	create_text(nbTextures,54,10,color,font,"Nombre de tuiles dans la pioche : 15 ");
	create_text(nbTextures,54,10,color,font,"nom Joueur");
	//initialize buttons
	float Buttonsize = 0.08;
	create_button(nbButtons,DM.w*0.92,DM.h*0.7,DM.w*0.1,DM.h*0.1,buttonStates[0],color,font,"Annuler");
	create_button(nbButtons,DM.w*0.92,DM.h*UI_BAS,DM.w*0.1,DM.h*0.1,buttonStates[0],color,font,"Valider");
	create_button(nbButtons,DM.w*0.20,DM.h*0.70,DM.w*Buttonsize,DM.h*Buttonsize,buttonStates[0],color,font,"Ajouter au debut");
	create_button(nbButtons,DM.w*0.3,DM.h*0.70,DM.w*Buttonsize,DM.h*Buttonsize,buttonStates[0],color,font,"Ajouter a la fin");
	create_button(nbButtons,DM.w*0.4,DM.h*0.70,DM.w*Buttonsize,DM.h*Buttonsize,buttonStates[0],color,font,"Retirer au debut");
	create_button(nbButtons,DM.w*0.5,DM.h*0.70,DM.w*Buttonsize,DM.h*Buttonsize,buttonStates[0],color,font,"Retirer a la fin");
	create_button(nbButtons,DM.w*0.6,DM.h*0.70,DM.w*Buttonsize,DM.h*Buttonsize,buttonStates[0],color,font,"Scinder en deux");
	create_button(nbButtons,DM.w*0.7,DM.h*0.70,DM.w*Buttonsize,DM.h*Buttonsize,buttonStates[0],color,font,"Remplacer joker");
	create_button(nbButtons,DM.w*0.8,DM.h*0.65,DM.w*Buttonsize,DM.h*Buttonsize,buttonStates[0],color,font,"Precedant");
	create_button(nbButtons,DM.w*0.8,DM.h*0.75,DM.w*Buttonsize,DM.h*Buttonsize,buttonStates[0],color,font,"Suivant");
	TTF_CloseFont(font);
}
void init_menu_jeu()
{
	init_buttonState();
	//TODO init dans une autre fonction ?
	nbButtons = 0;
	nbTextures = 0;
	bHitboxes = (Hitbox *) malloc(2 * sizeof(Hitbox));
	vHitboxes = (Hitbox *) malloc(1 * sizeof(Hitbox));
	bTextures = (ButtonTexture *) malloc(2 * sizeof(ButtonTexture));
	vTextures = (Texture *) malloc(2 * sizeof(Texture));
	//
	SDL_Color color = {255, 0, 0};
	TTF_Font* font = TTF_OpenFont("theboldfont.ttf", 14);
	//initialize "textbox"
	create_text(nbTextures,54,10,color,font,"Nombre de tuiles dans la pioche : 15 ");
	create_text(nbTextures,54,10,color,font,"nom Joueur");
	//initialize buttons
	create_button(nbButtons,DM.w*0.92,DM.h*UI_BAS,DM.w*0.1,DM.h*0.1,buttonStates[0],color,font,"Fin de tour");
	create_button(nbButtons,DM.w*0.92,DM.h*0.6,DM.w*0.1,DM.h*0.1,buttonStates[0],color,font,"Creer une combinaison");
	//initialize hitbox tuile
	Hitbox box = {grille.rect};
	vHitboxes[0] = box;
	TTF_CloseFont(font);
}

void destroy_menu(void) {
	for (int i = 0; i < nbTextures; i++) {
		SDL_DestroyTexture(vTextures[i].texture);
	}
	for (int i = 0; i < nbButtons; i++) {
		SDL_DestroyTexture(bTextures[i].buttonTexture.texture);
		SDL_DestroyTexture(bTextures[i].textTexture.texture);
	}
	for (int i =0;i<3;i++) {
		SDL_DestroyTexture(buttonStates[i]);
	}
	free(buttonStates);
	free(bHitboxes);
	free(bTextures);
	free(vTextures);

}
/*
* Mouse handler. See documentation for explanation.
*/
void handle_mickey_mouse(int* exit, int* clickHandlerState, int* indexButton,SDL_Event e, int* oldIndex, void (*redirect)(int,int*)) {
		int x,y;
		SDL_GetMouseState(&x, &y);
		switch (e.type) {
			case SDL_QUIT:
				//menu specific
				destroy_menu();
				*exit=1;
				break;
			case SDL_KEYDOWN:
                    if ( e.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
                    {
                    	fprintf(stdout, "Un appuie sur Echap \n");
                        *exit = 1;
						quit(OK);
                    }
					break;
			case SDL_MOUSEMOTION:
				if (*clickHandlerState != 1) {
					*oldIndex = *indexButton;
					*indexButton = get_index_hitbox_clicked(x,y);
					if (*oldIndex != -1) {
						bTextures[*oldIndex].buttonTexture.texture = buttonStates[0];
						*oldIndex = -1;
					}
					if (*indexButton != -1) {
						bTextures[*indexButton].buttonTexture.texture = buttonStates[1];
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				Mix_PlayChannel(-1, onClick, 0);
				switch (e.button.button) {
					case SDL_BUTTON_LEFT:
						if (*indexButton != -1 && check_hitbox(x,y,*indexButton)) {
							bTextures[*indexButton].buttonTexture.texture = buttonStates[2];
							*clickHandlerState = 1;
						}
						break;
					case SDL_BUTTON_RIGHT:
						if (*indexButton != -1) {
							bTextures[*indexButton].buttonTexture.texture = buttonStates[0];
							*oldIndex = *indexButton;
							*indexButton = -1;
							*clickHandlerState = 0;
						}
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				Mix_PlayChannel(-1, unClick, 0);
				switch (e.button.button) {
					case SDL_BUTTON_LEFT:
						if(inGame == 1)
						{
							if(check_grille(x,y))
							{
								redirect(-1,exit);
							}
						}
						if (*indexButton != -1) {
							if (check_hitbox(x,y,*indexButton)) {
								*clickHandlerState = 2;
								//menu specific
								redirect(*indexButton, exit);
							}
							else {
								bTextures[*indexButton].buttonTexture.texture = buttonStates[0];
								*oldIndex = *indexButton;
								*indexButton = -1;
								*clickHandlerState = 0;
							}
						}
						break;
				}
				break;
		}
}
/*
* Returns the button Hitbox hit on x and y.
*/
int get_index_hitbox_clicked(int x, int y) {
	for (int i=0; i<nbButtons; i++) {
		if (x > bHitboxes[i].box.x && y > bHitboxes[i].box.y &&
			x < bHitboxes[i].box.x+bHitboxes[i].box.w && y < bHitboxes[i].box.y+bHitboxes[i].box.h) {
			return i;
		}
	}
	return -1;
}
/*
* Only called when a Hitbox is already hit and check if it does it again. Used for click confirmation.
*/
int check_hitbox(int x, int y, int index) {
	if (x > bHitboxes[index].box.x && y > bHitboxes[index].box.y &&
		x < bHitboxes[index].box.x+bHitboxes[index].box.w && y < bHitboxes[index].box.y+bHitboxes[index].box.h) {
		return 1;
	}
	return 0;
}
int check_grille(int x,int y)
{
	if((x > vHitboxes[0].box.x && x < (vHitboxes[0].box.x+vHitboxes[0].box.w))
	&& (y > vHitboxes[0].box.y && y < (vHitboxes[0].box.y+vHitboxes[0].box.h)))
	{
		tuileSelect.x = (x-vHitboxes[0].box.x)/caseX;
		tuileSelect.y = (y-vHitboxes[0].box.y)/caseY;
		return 1;
	}
	else
	{
		tuileSelect.x = -1;
		tuileSelect.y = -1;
		return 0;
	}
	return 0;
}

/***************************************
Fonctions non utilisee car implimente dans
une autre version de l'interface graphique
*****************************************/
void init_menu(void) {

	nbButtons = 0;
	bHitboxes = (Hitbox *) malloc(8 * sizeof(Hitbox));
	bTextures = (ButtonTexture *) malloc(8 * sizeof(ButtonTexture));
	vTextures = (Texture *) malloc(4 * sizeof(Texture));
	SDL_Color color = {255, 0, 0};
	TTF_Font* font = TTF_OpenFont("theboldfont.ttf", 14);
	//initialize "textbox"
	create_text(0,54,10,color,font,"LACUCARACHA : ");
	create_text(1,50,22,color,font,"The name");
	//initialize buttons
	create_button(0,200,200,100,40,buttonStates[0],color,font,"da text");
	create_button(1,200,250,100,40,buttonStates[0],color,font,"da text");
	create_button(2,200,300,100,40,buttonStates[0],color,font,"da text");
	create_button(3,200,350,100,40,buttonStates[0],color,font,"da text");
	create_button(4,350,200,100,40,buttonStates[0],color,font,"da text");
	create_button(5,350,250,100,40,buttonStates[0],color,font,"da text");
	create_button(6,350,300,100,40,buttonStates[0],color,font,"da text");
	create_button(7,350,350,100,40,buttonStates[0],color,font,"da text");
	TTF_CloseFont(font);
}
void init_textbox_menu(void) {
	nbButtons = 0;
	nbTextures = 0;
	bHitboxes = (Hitbox *) malloc(4 * sizeof(Hitbox));
	bTextures = (ButtonTexture *) malloc(4 * sizeof(ButtonTexture));
	vTextures = (Texture *) malloc(4 * sizeof(Texture));
	SDL_Color color = {255, 0, 0};
	TTF_Font* font = TTF_OpenFont("theboldfont.ttf", 14);
	//initialize "textbox"
	create_text(0,54,10,color,font,"LACUCARACHA : ");
	create_text(1,50,22,color,font,"The name");
	//initialize buttons
	create_button(0,200,200,100,40,buttonStates[0],color,font,"confirm");
	create_button(1,350,200,100,40,buttonStates[0],color,font,"cancel");
	TTF_CloseFont(font);

}


void update_menu(int* quit, int* clickHandlerState, int* indexButton,SDL_Event e,  int* oldIndex) {
	while(SDL_PollEvent(&e) != 0) {
		handle_mickey_mouse(quit,clickHandlerState,indexButton,e,oldIndex,redirect_menu);
	}
}

void update_textbox_menu(int* quit, int* clickHandlerState, int* indexButton,SDL_Event e, int* oldIndex) {
	while(SDL_PollEvent(&e) != 0) {
		handle_mickey_mouse(quit,clickHandlerState,indexButton,e,oldIndex,redirect_textbox_menu);
		handle_textbox(e,font,1);
	}
}

void redirect_menu(int index, int* quit) {
	//destroy
	switch(index) {
		case 0:
			//init
			break;
		case 1:
			//init
			destroy_menu();
			init_textbox_menu();
			update = update_textbox_menu;
			break;
	}
}

void redirect_textbox_menu(int index, int* quit) {
	//destroy
	switch(index) {
		case 0:
			//init
			*quit = 1;
			break;
		case 1:
			//init
			destroy_menu();
			init_menu();
			update = update_menu;
			break;
	}
}
/*
* Changes the text of a text texture in vTextures. Writes in memory twice so it's costly.
*/
void set_text(int index, TTF_Font* font, char* name, SDL_Color color) {
	int x = vTextures[index].rect.x + vTextures[index].rect.w/2;
	int y = vTextures[index].rect.y + vTextures[index].rect.h/2;
	SDL_DestroyTexture(vTextures[index].texture);
	Texture textTexture;
	textTexture = create_texture_from_text(font, name, color);
	textTexture.rect.x = x - textTexture.rect.w/2;
	textTexture.rect.y = y - textTexture.rect.h/2;
	vTextures[index] = textTexture;
}
/*
* Treats keyboard inputs and modifies the texture in vTexture at index.
*/
void handle_textbox (SDL_Event e, TTF_Font* font, int index) {
	SDL_Color color = {255, 0, 0};int l = strlen(name);
	switch(e.type) {
		case SDL_KEYDOWN:
			if( e.key.keysym.sym == SDLK_BACKSPACE && l >0 ) {
				name[l-1]='\0';
				//refresh text texture set textbox index
				set_text(index,font,name,color);
			}
			break;
		case SDL_TEXTINPUT:
			if(l < 26) {
				strcat(name,e.text.text);
				//refresh text texture set textbox index
				set_text(index,font,name,color);
			}
			break;
	}
}