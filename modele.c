/******************************************
 * Modele
 ******************************************/
#include "modele.h" 



// les fonctions sur la mise ne place du jeu:
void init_pioche()
{
	init_tuiles();
	melange_pioche(pioche);
	//lit_pioche(pioche);
}


void init_tuiles()
{
	int i;
	TUILE tuile;

	tuile.num=0;
	tuile.coul=0;

	for(i=0;i<NOMBRE_TUILES;i++) 
	{
		pioche[i]=tuile;

		if(i%2==1)
			tuile.num+=1;
		if(i%26==1||i==1)
		{
			tuile.coul+=1;
			tuile.num=1;
		}
	}
}


void melange_pioche(TUILE* pioche) // 100 mélanges ça me paraît pas mal
{
	int i;
	TUILE tmp;
	
	for(i=0;i<100;i++) 
	{
		int position1=rand()%106;
		int position2=rand()%106;
		
		tmp=pioche[position1];
		pioche[position1]=pioche[position2];
		pioche[position2]=tmp;
	}

}


void init_joueurs(JOUEUR * joueurs,int nbJoueurs,int * niveauPioche)
{
	int i;

	for(i=0;i<nbJoueurs;i++)
	{
		demande_pseudo(&(joueurs[i]));
		joueurs[i].main=cree_liste();
		lit_liste(joueurs[i].main);
		init_main(joueurs[i].main,niveauPioche);
		lit_liste(joueurs[i].main);	
		joueurs[i].premierCoup = false;
	}
}


void lit_tuile(TUILE tuile)
{
	printf("Num:%d ; Coul:%d\n",tuile.num,tuile.coul);
}


void lit_pioche(TUILE *pioche)
{
	int i;

	for(i=0;i<NOMBRE_TUILES;i++) 
	{
		lit_tuile(pioche[i]);
	}	
}


void pioche_tuile(LISTE *liste,int *niveauPioche)
{
	ajoute_liste(liste,pioche[*niveauPioche]);
	*niveauPioche+=1;
}


void init_main(LISTE *liste,int *niveauPioche)
{
	int i;

	for(i=0;i<6;i++)
		pioche_tuile(liste,niveauPioche);
}

int regarde_qui_commence(int nbJoueurs,JOUEUR * joueurs)
{
	int borneInferieur = -1;
	return regarde_qui_commence_aux(nbJoueurs-1,borneInferieur,borneInferieur,joueurs);
}



int regarde_qui_commence_aux(int nbJoueurs,int valeurMax,int positionMax,JOUEUR * joueurs)
{
	int valeur;

	if(nbJoueurs<0)
		return positionMax;

	valeur=pioche[rand()%106].num;
	printf("%s pioche %d\n",joueurs[nbJoueurs].pseudo,valeur);

	if(valeur > valeurMax)
		return regarde_qui_commence_aux(nbJoueurs-1,valeur,nbJoueurs,joueurs);
	if(valeur == valeurMax)
		return regarde_qui_commence_aux(nbJoueurs,valeurMax,positionMax,joueurs);
	else
		return regarde_qui_commence_aux(nbJoueurs-1,valeurMax,positionMax,joueurs);
}

PLATEAU * cree_plateau()
{
	PLATEAU *plateau = (PLATEAU*) malloc(sizeof(PLATEAU));
	plateau->premier =  NULL;
	return plateau;
}

void ajoute_plateau(LISTE *liste)
{
	ELEMENT_PLATEAU *nouveau = malloc(sizeof(*nouveau));


	nouveau->suivant=plateau->premier;
	nouveau->liste=liste;

	plateau->premier=nouveau;
}


// les fonctions sur les listes:
LISTE * cree_liste()
{
	LISTE *liste = (LISTE*) malloc(sizeof(LISTE));
	liste->premier=NULL;
	return liste;
}


void ajoute_liste(LISTE *liste,TUILE tuile)
{
	ELEMENT *nouveau = malloc(sizeof(*nouveau));

	nouveau->tuile=tuile;
	nouveau->suivant=liste->premier;

	liste->premier=nouveau;
}


void lit_liste(LISTE *liste)
{
	ELEMENT * element=liste->premier;
	int indiceTuile=nb_elements_liste(liste);
	
	lit_liste_aux(element,indiceTuile);
	printf("\n");
}


void lit_liste_aux(ELEMENT * element,int indiceTuile)
{
	if(element != NULL)
	{
		lit_liste_aux(element->suivant,indiceTuile-1);
		printf("(%d) %d;%d\n",indiceTuile,element->tuile.num,element->tuile.coul);
	}
}


//Renvoie la taille d'une liste ou suite dans le jeu, en faisant abstraction de la tuile de fin dont la couleur et le numéro vallent NULL.
int nb_elements_liste(LISTE *liste)
{
	ELEMENT *tuileMain=liste->premier;
	int nombreElements=0;

	while(tuileMain != NULL)
	{
		nombreElements += 1;
		tuileMain=tuileMain->suivant;
	}
	return nombreElements;
}



//Renvoie 0 si le placement n'a pas été réalisé; 1 si le placement est un succès.
//On considère que le premier élément d'une liste est l'élément 1
//position ici est la position à laquelle on souhaite placer la tuile dans la suite
int placement_element_liste(LISTE *liste, TUILE tuile, int position)
{
	if(position > nb_elements_liste(liste)){
		return 0;
	}

	if(position==1){
		ajoute_liste(liste,tuile);
	}
	else{
		//On créer un type MAIN pour la tuile fournie en paramètres afin de la placer dans la liste qui est composés d'éléments de type MAIN.
		ELEMENT *tuileAvant=liste->premier;
		ELEMENT *nouveau = malloc(sizeof(*nouveau));
		int i;
		nouveau->tuile=tuile;

		//On parcourt la liste jusqu'à obtenir la tuile dont la position est celle qui précède la position où la tuile va être ajoutée.
		for(i=1;i<position-1;i++){
			tuileAvant=tuileAvant->suivant;
		}

		//La nouvelle tuile pointe la tuile suivante de la tuile qui la précède dorénavant.
		nouveau->suivant=tuileAvant->suivant;

		//la tuile qui la précède change son attribut suivant pour pointer la nouvelle tuile.
		tuileAvant->suivant=nouveau;
	}
	//Ici 1 indique que la fonction a réussie à placer la tuile dans la liste.
	return 1;
}

//Renvoie la liste créée; la première liste en paramètres a été modifiée
//On considère ici que la position c'est l'endroit de coupe soit l'element à cette position sera le premier élément de la seconde liste, que l'on va créer
LISTE * separer_liste_en_deux(LISTE *liste, int position){
	ELEMENT *tuileAvant=liste->premier;
	int i;

	for(i=1;i<position-1;i++){
		tuileAvant=tuileAvant->suivant;
	}
	//Creation nouvelle liste a partir de l element demande
	LISTE *liste2 = (LISTE*) malloc(sizeof(LISTE));
	liste2->premier=tuileAvant->suivant;

	//Remplacer la tuile suivante de la derniere tuile de la suite 1 par la tuile vide (dont le numero et la couleur sont NULL)
	ELEMENT *derniereTuile = (ELEMENT*) malloc(sizeof(ELEMENT));
	tuileAvant->suivant=derniereTuile;

	return liste2;
}



int est_suite(LISTE *liste){
	ELEMENT *elemVerif = liste->premier;
	int i;
	int nbElemSuite = nb_elements_liste(liste);
	int numElem = elemVerif->tuile.num;
	int coulElem = elemVerif->tuile.coul;
	int nbJoker=0;

	elemVerif = elemVerif->suivant;
	if(numElem==0){
		if(elemVerif->tuile.num == 0){
			return 1;
		}
		nbJoker+=1;
		numElem = elemVerif->tuile.num;
		coulElem = elemVerif->tuile.coul;
		elemVerif = elemVerif->suivant;
	}

	for(i=1+nbJoker;i<nbElemSuite;i++){
		if(elemVerif->tuile.num != 0){
			if((elemVerif->tuile.num != numElem+1) || (elemVerif->tuile.coul != coulElem)){
				return 1;
			}
			numElem = elemVerif->tuile.num;
		}
		else{
			nbJoker+=1;
			if(nbJoker>1){
				return 1;
			}
			numElem = numElem+1;

		}
		elemVerif=elemVerif->suivant;
	}
	return 0;
}

int est_liste_meme_nb(LISTE *liste){
	ELEMENT *elemVerif = liste->premier;
	int i,j;
	int nbElemSuite = nb_elements_liste(liste);
	int numElem = elemVerif->tuile.num;
	int coulElem[4]={5,5,5,5};
	int nbJoker=0;

	coulElem[0]= elemVerif->tuile.coul;
	elemVerif = elemVerif->suivant;
	if(numElem==0){
		if(elemVerif->tuile.num == 0){
			return 1;
		}
		nbJoker+=1;
		numElem = elemVerif->tuile.num;
		coulElem[1] = elemVerif->tuile.coul;
		elemVerif = elemVerif->suivant;
	}

	for(i=1+nbJoker;i<nbElemSuite;i++){
		if(elemVerif->tuile.num != 0){
			if((elemVerif->tuile.num != numElem)){
				return 1;
			}
			for(j=0;j<4;j++){//pas sur de cette boucle
				if(elemVerif->tuile.coul == coulElem[j]){
					return 1;
				}
			}
			coulElem[i]=elemVerif->tuile.coul;
		}
		else{
			nbJoker+=1;
			if(nbJoker>1){
				return 1;
			}
			coulElem[i]=elemVerif->tuile.coul;

		}
		elemVerif=elemVerif->suivant;
	}
	return 0;
}



int nb_points_suite(LISTE *liste){
	int nbElems = nb_elements_liste(liste);
	int i;
	int total=0;
	ELEMENT *element = liste->premier;
	int num = element->tuile.num;
	
	
	for(i=0;i<nbElems;i++){
		total += num+i;
	}
	return total;
	
}

int nb_points_liste_mnombre(LISTE *liste){
	int nbElems = nb_elements_liste(liste);
	ELEMENT *element = liste->premier;
	int num = element->tuile.num;

	return nbElems * num;
}



void lit_tuile_liste(LISTE *liste, int position)  //fonction pour lire la tuile de la liste à la position donnée 
{
	if(position>=0 && position<=nb_elements_liste(liste))  //vérifie que position soit bien une position de la liste
	{
		ELEMENT *premiereTuile = liste->premier;
		for(;position>0;position--) //boucle qui met à position à 0 afin de savoir quand la position est bonne
		{
			premiereTuile = premiereTuile->suivant;
		}
		lit_tuile(premiereTuile->tuile);
	}
}



int additionne_points(LISTE *main, int fin)
{
	ELEMENT *tuileMain=main->premier;
	int points=0;
	int valTuilePrecedente = -1;
	int valTuileSuivante;

	while(tuileMain != NULL)
	{
		if(tuileMain->tuile.num == 0) 
			if(fin == 1){
				points += 30;//à la fin de la partie, si c'est un joker: 30 points
			}
			else{ //si c'est pour avoir la valeur du joker dans la suite
				if(valTuilePrecedente == -1){ //Si joker est le premier de la suite à calculer
					ELEMENT *tuileSuivante = tuileMain->suivant;
					valTuileSuivante = tuileSuivante->tuile.num;
					points += valTuileSuivante;
				}
				else{ //Si joker n'est pas la première tuile de la suite à calculer
					points += valTuilePrecedente + 1;
				}
			}
		else  //sinon les points correspondent au numéro de chaque tuile
			points += tuileMain->tuile.num;
		valTuilePrecedente = tuileMain->tuile.num;
		tuileMain=tuileMain->suivant;	
	}		
	return points;
}


int calcule_points_gagnant(JOUEUR *joueurs,int nbJoueurs,int gagnant,int typeFinDePartie)
{
	int i;
	int points=0;

	if(typeFinDePartie==1)
	{
		for(i=0;i<nbJoueurs;i++)
		{
			if(i!=gagnant)
				points+=additionne_points(joueurs[i].main,1)-additionne_points(joueurs[gagnant].main,1);
		}
	}
	else
	{
		for(i=0;i<nbJoueurs;i++)
			points+=additionne_points(joueurs[i].main,1);
	}
	printf("ptsssss:%d\n",points);
	return points;
}


int calcule_points_perdant(JOUEUR *joueurs,int gagnant,int perdant,int typeFinDePartie)
{
	int points=0;
	
	if(typeFinDePartie==1)
		points=additionne_points(joueurs[gagnant].main,1)-additionne_points(joueurs[perdant].main,1);
	else
		points-=additionne_points(joueurs[perdant].main,1);

	return points;
}


int plus_petite_main(JOUEUR *joueurs, int nbJoueurs)
{
	int i;
	int tmp;
	int min=10000;  //valeur qui ne sera pas dépassée
	int gagnant;
	for(i=0;i<nbJoueurs;i++) 
	{
		tmp=additionne_points(joueurs[i].main,1);

		if(tmp<min)   /*REMARQUE IMPORTANTE: que faire si 2 personnes ont un même score en main ???? */
		{
			min=tmp;
			gagnant=i;
		}
	}
	return gagnant;
}



int trouve_joueur_precedent(int nbJoueurs, int tour) /***Est-ce que je crée une variable exprès pour mieux comprendre ce qui est renvoyé ?****/
{
	if(tour==0) //si on arrive au joueur zero, on veut faire une rotation
		tour=nbJoueurs;
	tour-=1;

	return tour;
}


int detecte_gagnant(JOUEUR *joueurs,int nbJoueurs,int tour,int typeFinDePartie)
{
	int gagnant;

	if(typeFinDePartie==1)
		gagnant=plus_petite_main(joueurs,nbJoueurs);
	else
		gagnant=trouve_joueur_precedent(nbJoueurs,tour);

	return gagnant;
}


bool main_finie(JOUEUR *joueurs, int nbJoueurs, int tour)
{
	int joueurPrecedent=trouve_joueur_precedent(nbJoueurs,tour);

	if(nb_elements_liste(joueurs[joueurPrecedent].main)==0) //si le joueur a une main de 0 éléments
		return true;		
	return false;
}


bool pioche_finie(int niveauPioche)
{
	if(niveauPioche==NOMBRE_TUILES-1)
		return true;
	return false;
}



bool est_partie_finie(JOUEUR *joueurs, int niveauPioche, int nbJoueurs, int tour)
{
	if(pioche_finie(niveauPioche)|| main_finie(joueurs,nbJoueurs,tour)) //si la pioche est finie ou si la main du joueur a été finie
		return true;
	return false;
}


int choisit_tour(bool premierCoup)
{
	int choix;

	do
	{
		if(!premierCoup)
			printf("1-Piocher\n2-Créer une combinaison d'au moins 30 points\nSaisie:");
		else
			printf("1-Piocher\n2-Créer une combinaison (jsais plus si y'a cette option)\n3-Compléter une combinaison\nSaisie:");
	
		scanf("%d",&choix);
	}while((!premierCoup && (choix<1 || choix>2)) || (premierCoup && (choix<1 || choix>3)));

	if(!premierCoup && choix==2)
		choix=4;

	return choix;
}


void joue_tour(JOUEUR * joueur,int *niveauPioche)
{
	int typeDeCoup = 0;

	switch(choisit_tour(joueur->premierCoup))
	{
	case 1:
		printf("VOUS PIOCHEZ\n");
		pioche_tuile(joueur->main,niveauPioche);
		break;
	case 2:
		printf("VOUS CREEZ UNE COMBINAISON\n");
		saisit_combinaison(joueur->main);
		break;
	case 3:
		printf("VOUS COMPLETEZ UNE COMBINAISON\n");


		do
		{
			printf("1 - Ajouter à la combinaison\n2 - Récupérer une tuile\n3 - Séparer une combinaison\n4 - Remplacer un joker\n5 - Valider le tour\nSaisie:");
			scanf("%d",&typeDeCoup);	
			
			lit_plateau();
			printf("MAIN:\n");
			lit_liste(joueur->main);

			switch(typeDeCoup)
			{
			case 1:
				complete_combinaison(joueur->main);
				break;
			case 2:
				recupere_tuile_combinaison(joueur->main);
				break;
			case 3:
				separe_combinaison();
				break;
			case 4:
				remplace_joker(joueur->main);
				break;
			}

		}while(typeDeCoup!=5);

		break;
	case 4:
		printf("VOUS DEVEZ CREER UNE COMBINAISON D'AU MOINS 30 PTS\n");
		saisit_combinaison(joueur->main);
		joueur->premierCoup = true;
		break;
	}

}


void complete_combinaison(LISTE * main)
{
	int positionCombinaison,positionTuile;
	int positionInsertion=-1;
	LISTE * combinaison;
	TUILE tuile;

	do{
		printf("Saisir la combinaison à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau());
	
	combinaison = renvoie_liste_via_position(positionCombinaison);
	printf("LA COMBINAISON:\n");
	lit_liste(combinaison);

	do{
		printf("1-Ajouter à la fin de la combinaison\n2-Ajouter au début de la combinaison\n");
		scanf("%d",&positionInsertion);
	}while(positionInsertion<1 || positionInsertion>2);

	do{
		printf("Position de la tuile à insérer:\n");
		scanf("%d",&positionTuile);
	}while(positionTuile<1 || positionTuile>nb_elements_liste(main));
	
	tuile=renvoie_tuile_via_position(combinaison,positionTuile);

	if(positionInsertion==1)
		ajoute_liste(combinaison,tuile);
	else if(positionInsertion==2)
		placement_element_liste(combinaison,tuile,0);

	enleve_element_liste(main,positionTuile);
}


void recupere_tuile_combinaison(LISTE * main)
{	
	int positionCombinaison,positionTuile;
	LISTE * combinaison;
	TUILE tuile;

	do{
		printf("Saisir la combinaison à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau());
	
	combinaison = renvoie_liste_via_position(positionCombinaison);
	printf("LA COMBINAISON CHOISIE:\n");
	lit_liste(combinaison);

	do{
		printf("Saisir la position de la tuile à récupérer:");
		scanf("%d",&positionTuile);
	}while(positionTuile<1 || positionTuile>nb_elements_liste(combinaison));
	
	tuile = renvoie_tuile_via_position(combinaison,positionTuile);
	enleve_element_liste(combinaison,positionTuile);
	ajoute_liste(main,tuile);
}


void separe_combinaison()
{
	int positionCombinaison,positionSeparation;
	LISTE * combinaison;

	do{
		printf("Saisir la combinaison à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau());
	
	combinaison = renvoie_liste_via_position(positionCombinaison);
	printf("LA COMBINAISON CHOISIE:");
	lit_liste(combinaison);

	do{
		printf("Saisir la position à laquelle séparer la combinaison:");
		scanf("%d",&positionSeparation);
	}while(positionSeparation<1 || positionSeparation>nb_elements_liste(combinaison));

	ajoute_plateau(separer_liste_en_deux(combinaison,positionSeparation));
}


void echange_tuiles_listes(LISTE * liste1,LISTE * liste2,int positionListe1,int positionListe2)
{
	ELEMENT * element1=liste1->premier;
	ELEMENT * element2=liste2->premier;
	TUILE tuileTmp;
	int i,j;

	for(i=1;i<positionListe1;i++)
		element1=element1->suivant;

	for(j=1;j<positionListe2;j++)
		element2=element2->suivant;

	tuileTmp=element1->tuile;
	element1->tuile=element2->tuile;
	element2->tuile=tuileTmp;
}


void remplace_joker(LISTE * main)
{	
	int positionCombinaison,positionJokerCombinaison,positionTuileMain;
	LISTE * combinaison;

	do{
		printf("Saisir la combinaison à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau());
	
	combinaison = renvoie_liste_via_position(positionCombinaison);
	printf("LA COMBINAISON CHOISIE:\n");
	lit_liste(combinaison);

	do{
		printf("Saisir la position de la tuile à remplacer:");
		scanf("%d",&positionJokerCombinaison);
	}while(positionJokerCombinaison<1 || positionJokerCombinaison>nb_elements_liste(combinaison));

	do{
		lit_liste(main);
		printf("Saisir par quelle tuile remplacer ce joker:");
		scanf("%d",&positionTuileMain);
	}while(positionTuileMain<1 || positionTuileMain>nb_elements_liste(main));

	echange_tuiles_listes(combinaison,main,positionJokerCombinaison,positionTuileMain);
}


void saisit_combinaison(LISTE *main)
{
	int choix=1;
	LISTE *combinaison=cree_liste();

	do{
		lit_liste(main);
		
		do{	
			printf("Saisir le n° de la tuile à jouer\n0 pour valider sa combinaison:");	
			scanf("%d",&choix);
		}while(choix<0 || choix>nb_elements_liste(main));

		if(choix!=0)
		{
			ajoute_liste(combinaison,renvoie_tuile_via_position(main,choix));
			enleve_element_liste(main,choix);
		}
	}while(choix!=0);
	ajoute_plateau(combinaison);
}



LISTE * renvoie_liste_via_position(int position)
{
	ELEMENT_PLATEAU *element=plateau->premier;

	while(element != NULL)
	{
		if(position==1)
			return element->liste;
		element=element->suivant;
		position-=1;
	}
	
	return cree_liste();
}

int nb_elements_plateau()
{
	ELEMENT_PLATEAU *element=plateau->premier;
	int nombreElements=0;

	while(element != NULL)
	{
		nombreElements += 1;
		element=element->suivant;
	}
	return nombreElements;
}



TUILE renvoie_tuile_via_position(LISTE *liste,int position)
{
	ELEMENT *element=liste->premier;
	TUILE tuileErreur;

	while(element != NULL)
	{
		if(position==1) //position est à 1 quand on se trouve sur la tuile cherchée
			return element->tuile;
		element=element->suivant;
		position-=1;
	}
	tuileErreur.num=-1;
	tuileErreur.coul=-1;
	return tuileErreur;
}


void lit_plateau()
{
	ELEMENT_PLATEAU *element=plateau->premier;
	int indiceElement=1;

	while(element != NULL)
	{
		printf("%d:\n",indiceElement);	
		lit_liste(element->liste);
		element=element->suivant;
		indiceElement+=1;
	}
}


int enleve_element_liste(LISTE * liste,int position){
	ELEMENT * courant=liste->premier;
	ELEMENT * elemASuppr;
	int i;
	
	if((position > nb_elements_liste(liste)) || (position <= 0)){
		return 0;
	}

	if(position==1){
		liste->premier=courant->suivant;
		free(courant);
	}
	else{		
		//On parcourt la liste jusqu'à obtenir la tuile dont la position est celle qui précède la position où la tuile va être supprimée.
		for(i=1;i<position-1;i++){
			courant=courant->suivant;
		}
		elemASuppr=courant->suivant;
		courant->suivant=elemASuppr->suivant;
		free(elemASuppr);
	}
	//Ici 1 indique que la fonction a réussie à placer la tuile dans la liste.
	return 1;
}


void demande_pseudo(JOUEUR * joueur)
{
	printf("Choisissez votre pseudo:");
	scanf("%s",joueur->pseudo);
}


void rentrer_nom_score(int score, char * nom)
{
	FILE * fichier = NULL;
	fichier=fopen("scores.csv", "ayyi");
	
	if(fichier == NULL)
		exit(1); 

	fprintf(fichier,"%s %d\n",nom,score);
	fclose(fichier);
}



