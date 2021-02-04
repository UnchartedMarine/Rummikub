/******************************************
 * Modele
 ******************************************/
#include "modele.h" 




int demande_mode_jeu()
{
	int mode;
	do{
		printf("1 - Joueurs contre joueurs\n2 - Joueurs contre IA\nSaisie:");
		scanf("%d",&mode);
	}while(mode<1 || mode>2);
	
	return mode;
}



int demande_nb_joueurs()
{
	int nbJoueurs;
	do{
		printf("\nChoisir le nombre de joueurs (2 à 4)\nSaisie:");
		scanf("%d",&nbJoueurs);
	}while(nbJoueurs<2 || nbJoueurs>4);
	
	return nbJoueurs;
}


void init_pioche()
{
	init_tuiles();
	melange_pioche(pioche);
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


void init_joueurs(JOUEUR * joueurs,int modeJeu, int nbJoueurs,int * niveauPioche)
{
	int i;

	for(i=0;i<nbJoueurs;i++)
	{
		if(modeJeu==2 && i>0)  //si le joueur est l'IA, lui donne un pseudo défini
			sprintf(joueurs[i].pseudo,"ORDI n°%d",i);
		else
		{
			printf("Choisissez votre pseudo:");
			scanf("%s",joueurs[i].pseudo);
		}
		joueurs[i].main=cree_liste();  //crée la liste de la main du joueur
		init_main(joueurs[i].main,niveauPioche);  //distribue au joueur les tuiles de début de partie
		joueurs[i].premierCoup = false;  //pour dire que le joueur n'a pas encore créé de combinaison de 30pts
	}
}


void lit_tuile(TUILE tuile)
{
	char couleur[10];

	switch(tuile.coul)
	{
	case 0:
		sprintf(couleur,"JOKER");
		break;
	case 1:
		sprintf(couleur,"ROUGE");
		break;
	case 2:
		sprintf(couleur,"BLEU");
		break;
	case 3:
		sprintf(couleur,"JAUNE");
		break;
	case 4:
		sprintf(couleur,"NOIR");
		break;
	}

	printf("%3d ; %1s\n",tuile.num,couleur);
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

	for(i=0;i<35;i++)
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

PLATEAU * copie_plateau(PLATEAU* original)
{
    PLATEAU* copie= cree_plateau();
    ELEMENT_PLATEAU* copie_element = malloc(sizeof(*copie_element));
    copie_element->suivant = NULL;
    copie->premier = copie_element;
    ELEMENT_PLATEAU* element =  original->premier;
    while (element != NULL)
    {
        copie_element->liste = copie_liste(element->liste);
        copie_element->suivant = NULL;
        if (element->suivant != NULL)
        {
            ELEMENT_PLATEAU *nouveau = malloc(sizeof(*nouveau));
            copie_element->suivant=nouveau;
            copie_element = copie_element->suivant;
        }
        element = element->suivant;
    }
    return copie;
}

int nb_elements_plateau(PLATEAU *plato){
	ELEMENT_PLATEAU *nouveau=plato->premier;
	int nombreElements=0;

	while(nouveau != NULL)
	{
		nombreElements += 1;
		nouveau=nouveau->suivant;
	}
	return nombreElements;
	
}

void ajoute_plateau(LISTE *liste,PLATEAU *plato)
{
	ELEMENT_PLATEAU *nouveau = malloc(sizeof(*nouveau));


	nouveau->suivant=plato->premier;
	nouveau->liste=liste;

	plato->premier=nouveau;
}

bool verif_plateau(PLATEAU * plato)
{
	ELEMENT_PLATEAU* element =  plato->premier;

	while(element != NULL)
	{
		if(!est_suite(element->liste) && !est_liste_meme_nb(element->liste))
			return false;
		element=element->suivant;
	}

	return true;
}


// les fonctions sur les listes:
LISTE * cree_liste()
{
	LISTE *liste = (LISTE*) malloc(sizeof(LISTE));
	liste->premier=NULL;
	return liste;
}

LISTE * copie_liste(LISTE* original)
{
    LISTE* copie= cree_liste();
    ELEMENT* copie_element = malloc(sizeof(*copie_element));
    copie_element->suivant = NULL;
    copie->premier = copie_element;
    ELEMENT* element =  original->premier;
    while (element != NULL)
    {
        copie_element->tuile = element->tuile;
        copie_element->suivant = NULL;
        if (element->suivant != NULL)
        {
            ELEMENT *nouveau = malloc(sizeof(*nouveau));
            copie_element->suivant=nouveau;
            copie_element = copie_element->suivant;
        }
        element = element->suivant;
    }
    return copie;
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
		printf(" (%2d)",indiceTuile);		
		lit_tuile(element->tuile);
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
	if(position==1 || nb_elements_liste(liste)== 0){
        	ajoute_liste(liste,tuile);
	}
	else if(position > nb_elements_liste(liste)+1){
		return 0;
	}
	else{
		//On créer un type MAIN pour la tuile fournie en paramètres afin de la placer dans la liste qui est composés d'éléments de type MAIN.
		ELEMENT *tuileAvant=liste->premier;
		ELEMENT *nouveau = malloc(sizeof(*nouveau));
		int i;
		nouveau->tuile=tuile;

		//On parcourt la liste jusqu'à obtenir la tuile dont la position est celle qui précède la position où la tuile va être ajoutée.
		if(nb_elements_liste(liste) != 0)
		{
			for(i=1;i<position-1;i++){
				tuileAvant=tuileAvant->suivant;
			}
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


	if(nbElemSuite < 3){
		return 0;
	}
	elemVerif = elemVerif->suivant;
	if(numElem==0){
		if(elemVerif->tuile.num == 0){
			return 0;
		}
		nbJoker+=1;
		numElem = elemVerif->tuile.num;
		coulElem = elemVerif->tuile.coul;
		elemVerif = elemVerif->suivant;
	}

	for(i=1+nbJoker;i<nbElemSuite;i++){
		if(elemVerif->tuile.num != 0){
			if((elemVerif->tuile.num != numElem+1) || (elemVerif->tuile.coul != coulElem)){
				return 0;
			}
			numElem = elemVerif->tuile.num;
		}
		else{
			nbJoker+=1;
			if(nbJoker>1){
				return 0;
			}
			numElem = numElem+1;

		}
		elemVerif=elemVerif->suivant;
	}
	return 1;
}

int est_liste_meme_nb(LISTE *liste){
	ELEMENT *elemVerif = liste->premier;
	int i,j;
	int nbElemSuite = nb_elements_liste(liste);
	int numElem = elemVerif->tuile.num;
	int coulElem[4]={5,5,5,5};
	int nbJoker=0;

	if(nbElemSuite < 3){
		return 0;
	}
	coulElem[0]= elemVerif->tuile.coul;
	elemVerif = elemVerif->suivant;
	if(numElem==0){
		if(elemVerif->tuile.num == 0){
			return 0;
		}
		nbJoker+=1;
		numElem = elemVerif->tuile.num;
		coulElem[1] = elemVerif->tuile.coul;
		elemVerif = elemVerif->suivant;
	}

	for(i=1+nbJoker;i<nbElemSuite;i++){
		if(elemVerif->tuile.num != 0){
			if((elemVerif->tuile.num != numElem)){
				return 0;
			}
			for(j=0;j<4;j++){//pas sur de cette boucle
				if(elemVerif->tuile.coul == coulElem[j]){
					return 0;
				}
			}
			coulElem[i]=elemVerif->tuile.coul;
		}
		else{
			nbJoker+=1;
			if(nbJoker>1){
				return 0;
			}
			coulElem[i]=elemVerif->tuile.coul;

		}
		elemVerif=elemVerif->suivant;
	}
	return 1;
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
	int gagnant=-1;
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
	int gagnant=-1;

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
	if(niveauPioche==NOMBRE_TUILES)
		return true;
	return false;
}



bool est_partie_finie(JOUEUR *joueurs, int niveauPioche, int nbJoueurs, int tour)
{
	if(pioche_finie(niveauPioche)|| main_finie(joueurs,nbJoueurs,tour)) //si la pioche est finie ou si la main du joueur a été finie
		return true;
	return false;
}


int choisit_tour(bool premierCoup,int tourMultiTemps)
{
	int choix;

	do
	{
		if(!premierCoup)
			printf("1 - Piocher\n2 - Créer une combinaison d'au moins 30 points\nSaisie:");
		else if(tourMultiTemps == 1){
			printf("2 - Créer une combinaison\n3 - Compléter une combinaison\n4 - Récupérer une tuile dans une combinaison de taille 4 ou supérieure\n5-Séparer une suite en deux\n6-Récupérer un joker dans une combinaison du plateau\nSaisie:");
		}
		else{
			printf("1 - Piocher\n2 - Créer une combinaison\n3 - Compléter une combinaison\n4 - Récupérer une tuile dans une combinaison de taille 4 ou supérieure\n5-Séparer une suite en deux\n6-Récupérer un joker dans une combinaison du plateau\nSaisie:");
		}
	
		scanf("%d",&choix);
	}while((!premierCoup && (choix<1 || choix>2)) || (premierCoup && (choix<1 || choix>3)) || ((tourMultiTemps==1) && (choix<2 || choix>3)));

	if(!premierCoup && choix==2)
		choix=7;

	return choix;
}


int joue_tour(JOUEUR * joueur,int *niveauPioche,int tourMultiTemps)
{
	int typeDeCoup;
	int choixSuite;

	switch(choisit_tour(joueur->premierCoup,tourMultiTemps))
	{
	case 1:
		printf("VOUS PIOCHEZ\n");
		pioche_tuile(joueur->main,niveauPioche);
		return 2;
		break;
	case 2:
		do{
			printf("VOUS CREEZ UNE COMBINAISON\n");
			printf("1 - Suite de nombres de meme couleur\n2 - Liste d'un meme nombre de couleurs differentes\nSAISIE:");
			scanf("%d",&choixSuite);
		}while(choixSuite<1 || choixSuite>2);

		if(saisit_combinaison(copieMain, choixSuite,joueur)==1){
			return 4;
		}
		break;
	case 3:
		do{
			printf("VOUS COMPLETEZ UNE COMBINAISON\n");
			printf("1 - Ajouter une tuile à une combinaison\n2 - Ajouter une tuile à un bout d'une combinaison et récupérer la tuile à l'autre bout\nSaisie:");
			scanf("%d",&typeDeCoup);
		}while(typeDeCoup<1 || typeDeCoup>2);

		if(typeDeCoup==1){
			complete_combinaison(copieMain);
			return 1;
		}
		else if(typeDeCoup==2){
			if(complete_recup_combinaison(copieMain)==1){
				return 1;
			}
		}
		break;

	case 4:
		if(recupere_tuile_combinaison(copieMain)==1){
			return 1;
		}
		break;
	case 5:
		if(separe_combinaison()==1){
			return 1;	
		}
		break;
	case 6:
		if(remplace_joker(copieMain)==1){
			return 1;	
		}
		break;
	case 7:
		do{
			printf("VOUS DEVEZ CREER UNE COMBINAISON D'AU MOINS 30 PTS\n");
			printf("1 - Suite de nombres de meme couleur\n2 - Liste d'un meme nombre de couleurs differentes\nSAISIE:");
			scanf("%d",&choixSuite);
		}while(choixSuite<1 || choixSuite>2);

		if(saisit_combinaison(copieMain, choixSuite,joueur)==1){
			joueur->premierCoup = true;
			return 3;
		}
		break;
	
	return 0;
	}
}


void complete_combinaison(LISTE * main)
{
	int positionCombinaison,positionTuile;
	int positionInsertion=-1;
	int nbElemsPlateau = nb_elements_plateau(copiePlateau);
	LISTE *quelleListe;
	LISTE * combinaison;
	TUILE tuile;

	do{
		printf("Saisir la combinaison à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));
	

	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);
	printf("LA COMBINAISON:\n");
	lit_liste(combinaison);

	do{
		printf("1 - Ajouter à la fin de la combinaison\n2 - Ajouter au début de la combinaison\n");
		scanf("%d",&positionInsertion);
	}while(positionInsertion<1 || positionInsertion>2);

	if(nb_elements_liste(listeTuilesRecup)<1){
		positionTuile=choisirTuile(copieMain);
		quelleListe=main;
	}
	else if(choisir_main_ou_recupTuile()==1){
		positionTuile=choisirTuile(copieMain);
		quelleListe=main;
	}
	else{
		positionTuile=choisirTuile(listeTuilesRecup);
		quelleListe=listeTuilesRecup;
	}
	
	tuile=renvoie_tuile_via_position(quelleListe,positionTuile);

	if(positionInsertion==1){
		ajoute_liste(combinaison,tuile);
	}
	else if(positionInsertion==2){
		placement_element_liste(combinaison,tuile,nb_elements_liste(combinaison)+1);
	}
	enleve_element_liste(quelleListe,positionTuile);
}


int recupere_tuile_combinaison(LISTE * main)
{	
	int positionCombinaison,positionTuile;
	int nbElemsCombi;
	int nbElemsPlateau = nb_elements_plateau(copiePlateau);
	int posDansCombi;
	int choixCombi;
	LISTE * combinaison;
	TUILE tuile;

	do{
		printf("Saisir la combinaison dans laquelle récupérer une tuile:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));
	
	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);
	if(joker_est_dans_liste(combinaison)==true){
		printf("ON NE PEUT PAS FAIRE CE COUP S'IL Y A UN JOKER DANS LA COMBINAISON,");
		return 0;
	}
	nbElemsCombi=nb_elements_liste(combinaison);
	if(nbElemsCombi<4){
		printf("LA COMBINAISON CONTIENT MOINS DE 4 TUILES, C'EST UN COUP IMPOSSIBLE");
		return 0;
	}


	printf("LA COMBINAISON CHOISIE:\n");
	lit_liste(combinaison);

	do{
		printf("1-Récupérer la tuile à la fin de la combinaison\n2-Récupérer la tuile au début de la combinaison\n");
		scanf("%d",&positionTuile);
	}while(positionTuile<1 || positionTuile>2);

	if(positionTuile==1){
		tuile=renvoie_tuile_via_position(combinaison,nbElemsCombi);
		enleve_element_liste(combinaison,nbElemsCombi);
		ajoute_liste(listeTuilesRecup,tuile);
	}
	else if(positionTuile==2){
		tuile=renvoie_tuile_via_position(combinaison,1);
		enleve_element_liste(combinaison,1);
		ajoute_liste(listeTuilesRecup,tuile);
	}
	return 1;
}

int complete_recup_combinaison(LISTE * main)
{	
	int positionCombinaison,positionTuile;
	int positionInsertion=-1;
	int nbElemsCombi;
	int nbElemsPlateau = nb_elements_plateau(copiePlateau);

	LISTE * combinaison;
	LISTE *quelleListe;
	TUILE tuile;

	do{
		printf("Saisir la combinaison à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));
	
	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);
	
	if(joker_est_dans_liste(combinaison)==true){
		printf("ON NE PEUT PAS FAIRE CE COUP S'IL Y A UN JOKER DANS LA COMBINAISON, VOUS POUVEZ JUSTE COMPLETER");
		return 0;
	}
	nbElemsCombi = nb_elements_liste(combinaison);
	if(nbElemsCombi<3){
		printf("IMPOSSIBLE DE REALISER CETTE ACTION SUR UNE COMBINAISON DE MOINS DE 3 ELEMENTS");
		return 0;
	}	
	
	printf("LA COMBINAISON:\n");
	lit_liste(combinaison);

	do{
		printf("1-Ajouter à la fin de la combinaison\n2-Ajouter au début de la combinaison\n");
		scanf("%d",&positionInsertion);
	}while(positionInsertion<1 || positionInsertion>2);
	
	if(nb_elements_liste(listeTuilesRecup)<1){
		positionTuile=choisirTuile(copieMain);
		quelleListe=main;
	}
	else if(choisir_main_ou_recupTuile()==1){
		positionTuile=choisirTuile(copieMain);
		quelleListe=main;
	}
	else{
		positionTuile=choisirTuile(listeTuilesRecup);
		quelleListe=listeTuilesRecup;
	}
	
	tuile=renvoie_tuile_via_position(quelleListe,positionTuile);

	if(positionInsertion==1){
		ajoute_liste(combinaison,tuile);
		enleve_element_liste(quelleListe,positionTuile);

		nbElemsCombi = nb_elements_liste(combinaison);
		tuile = renvoie_tuile_via_position(combinaison,nbElemsCombi);
		enleve_element_liste(combinaison,nbElemsCombi);
		ajoute_liste(listeTuilesRecup,tuile);
	}
	else if(positionInsertion==2){
		placement_element_liste(combinaison,tuile,nb_elements_liste(combinaison)+1);
		enleve_element_liste(quelleListe,positionTuile);

		tuile = renvoie_tuile_via_position(combinaison,1);
		enleve_element_liste(combinaison,1);
		ajoute_liste(listeTuilesRecup,tuile);
	}
	return 1;
	
}

bool joker_est_dans_liste(LISTE * liste)
{
	ELEMENT * element=liste->premier;
	
	while(element != NULL)
	{
		if(element->tuile.num == 0)
			return true;
		element=element->suivant;
	}
	return false;
}


int separe_combinaison()
{
	int positionCombinaison,positionSeparation;

	int posSepa;
	int nbElemsDansCombi;
	LISTE * combinaison;

	do{
		printf("Saisir la combinaison à séparer:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));
	
	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);

	if(joker_est_dans_liste(combinaison)==true){
		return 0;
	}

	printf("LA COMBINAISON CHOISIE:");
	lit_liste(combinaison);

	do{
		printf("Saisir la position à laquelle séparer la combinaison (le numéro entré sera celui de la première tuile de la combinaison créée):");
		scanf("%d",&positionSeparation);
	}while(positionSeparation<1 || positionSeparation>nb_elements_liste(combinaison));

	nbElemsDansCombi=nb_elements_liste(combinaison);
	posSepa=nbElemsDansCombi+1-positionSeparation;

	ajoute_plateau(separer_liste_en_deux(combinaison,posSepa),copiePlateau);
	return 1;
}


void echange_tuiles_listes_via_main(LISTE * liste1,LISTE * liste2,int positionListe1,int positionListe2)
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
	
	//ajoute la tuile a listeTuilesRecup pour que le joueur soit obligé de la rejouer
	ajoute_liste(listeTuilesRecup,element2->tuile);
	//enleve cet element de la main car il est dans listeTuilesRecup
	enleve_elements_liste(liste2,positionListe2);
}


void echange_tuiles_listes_via_recup(LISTE * liste1,LISTE * liste2,int positionListe1,int positionListe2)
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


int remplace_joker(LISTE * main)
{	
	TUILR tuile;
	int positionCombinaison,positionJokerCombinaison,positionTuile;
	LISTE * combinaison;

	do{
		printf("Saisir la combinaison à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));
	
	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);
	printf("LA COMBINAISON CHOISIE:\n");
	lit_liste(combinaison);

	do{
		printf("Saisir la position du joker:");
		scanf("%d",&positionJokerCombinaison);
	}while(positionJokerCombinaison<1 || positionJokerCombinaison>nb_elements_liste(combinaison));

	tuile=renvoie_tuile_via_position(combinaison,positionJoker);
	if(tuile.num!=0){
		return 0;
	}
	
	//choix de la tuile dans la main ou la liste des tuiles à jouer obligatoirement
	if(nb_elements_liste(listeTuilesRecup)<1){
		positionTuile=choisirTuile(copieMain);
		echange_tuiles_listes(combinaison,main,positionJokerCombinaison,positionTuile);
	}
	else if(choisir_main_ou_recupTuile()==1){
		positionTuile=choisirTuile(copieMain);
		echange_tuiles_listes(combinaison,main,positionJokerCombinaison,positionTuile);
	}
	else{
		positionTuile=choisirTuile(listeTuilesRecup);
		echange_tuiles_listes(combinaison,listeTuilesRecup,positionJokerCombinaison,positionTuile);
	}

	
	return 1;
}

int choisirTuile(LISTE *liste){
	do{	
		printf("Saisir le n° de la tuile à jouer\n(Si vous créez une combinaison, 0 pour valider sa combinaison):");	
		scanf("%d",&choix);
		nbElems=nb_elements_liste(liste);
	}while(choix<0 || choix>nbElems);
	return choix;
}

int choisir_main_ou_recupTuile(){
	do{	
		printf("Choisir une tuile de la main ou de la liste des tuiles obligatoires à jouer ?");
		printf("1-main\n2-liste des tuiles obligatoires");
		scanf("%d",&choix);
	}while(choix<0 || choix>2);
	return choix;
}

int saisit_combinaison(LISTE *main, int typeSuite, JOUEUR * joueur)
{
	int choix=1;
	LISTE *combinaison=cree_liste();
	int nbElems;
	LISTE *quelleListe;

	do{
		printf("MAIN\n");
		lit_liste(main);
			printf("LA COMBINAISON EN TRAIN D'ETRE CREEE:\n");
		lit_liste(listeTuilesRecup);
		lit_liste(combinaison);
		
		if(nb_elements_liste(listeTuilesRecup)<1){
			choix=choisirTuile(copieMain);
			quelleListe=main;
		}
		else if(choisir_main_ou_recupTuile()==1){
			choix=choisirTuile(main);
			quelleListe=main;
		}
		else{
			choix=choisirTuile(listeTuilesRecup);
			quelleListe=listeTuilesRecup;
		}	

		if(choix!=0)
		{
			ajoute_liste(combinaison,renvoie_tuile_via_position(quelleListe,choix));
			enleve_element_liste(quelleListe,choix);
		}
	}while(choix!=0);

	if(typeSuite==1){
		if(est_suite(combinaison)==1){
			
			if((joueur->premierCoup == false) && (nb_points_suite(combinaison)<30)){
				printf("CETTE SUITE FAIT MOINS DE 30 POINTS\n");
				return 0;
			}
			ajoute_plateau(combinaison,copiePlateau);
			return 1;
		}
		printf("CE N'EST PAS UNE SUITE DE NOMBRE DE MEME COULEUR\n");
	}
	else if(typeSuite==2){
		if(est_liste_meme_nb(combinaison)==1){
			if((joueur->premierCoup == false) && (nb_points_liste_mnombre(combinaison)<30)){
				printf("CETTE SUITE FAIT MOINS DE 30 POINTS\n");
				return 0;
			}
			ajoute_plateau(combinaison,copiePlateau);
			return 1;
		}
		printf("CE N'EST PAS UNE SUITE D'UN MEME NOMBRE DE COULEURS DIFFERENTES\n");
	}
	
	
	return 0;

}



LISTE * renvoie_liste_via_position(PLATEAU *plato,int position)
{
	ELEMENT_PLATEAU *element=plato->premier;

	while(element != NULL)
	{
		if(position==1)
			return element->liste;
		element=element->suivant;
		position-=1;
	}
	
	return cree_liste();
}

TUILE renvoie_tuile_via_position(LISTE *liste,int position)
{
	ELEMENT *element=liste->premier;
	TUILE tuileErreur;
	position = nb_elements_liste(liste)+1 - position;    //équilibrage vu qu'on a pris nos fonctions à l'envers

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


void lit_plateau(PLATEAU *plato)
{
	ELEMENT_PLATEAU *element=plato->premier;
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
	int nbElems=nb_elements_liste(liste);
	int pos=nbElems+1-position;
	
	if((pos > nb_elements_liste(liste)) || (pos <= 0) || nb_elements_liste(liste) == 0){
		return 0;
	}

	if(pos==1){
		liste->premier=courant->suivant;
		free(courant);
	}
	else{		
		//On parcourt la liste jusqu'à obtenir la tuile dont la position est celle qui précède la position où la tuile va être supprimée.
		for(i=1;i<pos-1;i++){
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
