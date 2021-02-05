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

//Permet d'afficher la couleur d'une tuile en caractères plutot qu'en nombre comme la structure est faite ainsi
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

	for(i=0;i<14;i++)
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

//Créer un plateau vide
PLATEAU * cree_plateau()
{
	PLATEAU *plateau = (PLATEAU*) malloc(sizeof(PLATEAU));
	plateau->premier =  NULL;
	return plateau;
}


PLATEAU * copie_plateau(PLATEAU* original)
{
	PLATEAU* copie= cree_plateau();
	if(original->premier == NULL)
		return copie;
	ELEMENT_PLATEAU* copie_element = malloc(sizeof(*copie_element));
	copie_element->suivant = NULL;



	copie->premier = copie_element;
	ELEMENT_PLATEAU* element =  original->premier;

	while (element != NULL)
	{
		copie_element->liste = copie_liste(element->liste);
		printf("BAB\n");
		lit_liste(element->liste);
		lit_liste(copie_element->liste);
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


//Retourne nb d'elements dans le plateau donné
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

//Ajout d'une liste à un plateau donné
void ajoute_plateau(LISTE *liste,PLATEAU *plato)
{
	//Création d'un nouvel element plateau
	ELEMENT_PLATEAU *nouveau = malloc(sizeof(*nouveau));

	//Connexion du nouvel element plateau au reste du plateau
	nouveau->suivant=plato->premier;
	//affectation de la liste a ce nouvel element
	nouveau->liste=liste;

	plato->premier=nouveau;
}

//Renvoie un boolen si chaque combinaison du plateau donné sont vraies; true si c'est valide sinon false
bool verif_plateau(PLATEAU * plato)
{
	ELEMENT_PLATEAU* element =  plato->premier;

	//parcourt de chaque element du plateau qui sont des liste
	while(element != NULL)
	{
		//Si la liste vérifiée n'est ni une suite ni une combinaison de memes nombres de couleurs différentes
		if(!est_suite(element->liste) && !est_liste_meme_nb(element->liste))
			return false;//retourne faux
		element=element->suivant;//on passe à l'element du plateau suivant
	}

	return true;//Si tout est ok retourne true
}


// les fonctions sur les listes:


//Créer une liste vide
LISTE * cree_liste()
{
	LISTE *liste = (LISTE*) malloc(sizeof(LISTE));
	liste->premier=NULL;
	return liste;
}


LISTE* copie_liste(LISTE* original)
{
	LISTE* copie= cree_liste();
	if(original->premier == NULL)
		return copie;
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

//Ajoute une tuile dans la liste donnée
void ajoute_liste(LISTE *liste,TUILE tuile)
{
	//création d'un nouvel élément pour récupérer la tuile donnée
	ELEMENT *nouveau = malloc(sizeof(*nouveau));
	//l'élément reprend les caractéristiques de la tuile donnée
	nouveau->tuile=tuile;
	//Connexion de ce nouvel element de la liste au reste de la liste
	nouveau->suivant=liste->premier;
	//cet element devient le premier de la liste
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






//Renvoie 0 si le placement n'a pas été réalisé; 1 si le placement est un succès.
//On considère que le premier élément d'une liste est l'élément 1
//position ici est la position à laquelle on souhaite placer la tuile dans la suite
int placement_element_liste(LISTE *liste, TUILE tuile, int position)
{
	//si la liste donnée est vide ou que la position donnée est 1 alors on ajoute simplement à la liste 
	if(position==1 || nb_elements_liste(liste)== 0){
        	ajoute_liste(liste,tuile);
	}
	//sinon si la position donnée est en dehors de la position d'un element de cette liste
	else if(position > nb_elements_liste(liste)+1){
		return 0;
	}
	else{//sinon
		//On créer un type ELEMENT pour la tuile fournie en paramètres afin de la placer dans la liste qui est composés d'éléments de type ELEMENT.
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
	ELEMENT *element=liste->premier;
	int i;

	//parcourt jusqu'à l'élément position voulue de la séparation -1
	for(i=1;i<position-1;i++){
		element=element->suivant;
	}

	//Creation nouvelle liste
	LISTE *liste2 = cree_liste();
	liste2->premier=element->suivant;

	ELEMENT *derniereTuile = NULL;
	element->suivant=derniereTuile;

	return liste2;
}


//Retourne 1 si la liste donnée est une suite de nombres de la meme couleur.sinon retourne 0
int est_suite(LISTE *liste){
	ELEMENT *elemVerif = liste->premier;
	int i;
	int nbElemSuite = nb_elements_liste(liste);
	int numElem = elemVerif->tuile.num;
	int coulElem = elemVerif->tuile.coul;
	int nbJoker=0;

	//si la suite est inférieure à trois éléments c'est faux
	if(nbElemSuite < 3){

		return 0;
	}
	elemVerif = elemVerif->suivant;
	//si la premiere tuile de la liste est un joker
	if(numElem==0){
		//si la tuile qui suit le premier joker en position 1 de la liste est suivie également par un autre joker alors c'est faux
		if(elemVerif->tuile.num == 0){
			return 0;
		}
		//compteur de joker incrémenté
		nbJoker+=1;
		//passage de la couleur et du numéro à la tuile 2 car le joker est bon en premiere place donc la comparaison du chiffre et de la couleur se fera avec cette tuile
		numElem = elemVerif->tuile.num;
		coulElem = elemVerif->tuile.coul;
		elemVerif = elemVerif->suivant;
	}

	//parcourt du reste des tuiles dans la liste
	for(i=1+nbJoker;i<nbElemSuite;i++){
		//si la tuile n'est pas un joker
		if(elemVerif->tuile.num != 0){
			//si la tuile courante n'a pas comme numero la valeur +1 par rapport à la tuile precedente ou n'a pas la meme couleur c'est faux
			if((elemVerif->tuile.num != numElem+1) || (elemVerif->tuile.coul != coulElem)){
				return 0;
			}
			//passage du numéro de comparaison au numéro de la tuile qui vient d'etre validé pour comparaison furture de la suivante
			numElem = elemVerif->tuile.num;
		}
		//sinon si c'est un joker 
		else{
			//on incrémente le nombre totale de joker vu depuis le debut de la liste
			nbJoker+=1;
			//s'il y a plus qu'un joker dans cette liste alors c'est faux
			if(nbJoker>1){
				return 0;
			}
			//passage du numéro de comparaison au numéro de la tuile qui vient d'etre validé pour comparaison furture de la suivante
			numElem = numElem+1;

		}
		//passage de l'element à verifier au suivant par rapport a celui qui vient d'etre validé
		elemVerif=elemVerif->suivant;
	}
	//si tout est bon retourne 1
	return 1;
}

//Retourne 1 si la liste donnée est une suite de meme nombre de couleurs différentes.sinon retourne 0
int est_liste_meme_nb(LISTE *liste){
	ELEMENT *elemVerif = liste->premier;
	int i,j;
	int nbElemSuite = nb_elements_liste(liste);
	int numElem = elemVerif->tuile.num;
	int coulElem[4]={5,5,5,5};
	int nbJoker=0;

	//si la suite est inférieure à trois éléments c'est faux
	if(nbElemSuite < 3){
		return 0;
	}
	//premiere case du tableau des couleurs rencontrées dans la liste complétée par la couleur de l'elementpremier de la liste
	coulElem[0]= elemVerif->tuile.coul;
	//passage à l'élément suivant dans la liste pour le verifier
	elemVerif = elemVerif->suivant;

	//si la premiere tuile de la liste est un joker
	if(numElem==0){
		//si la tuile qui suit le premier joker en position 1 de la liste est suivie également par un autre joker alors c'est faux
		if(elemVerif->tuile.num == 0){
			return 0;
		}
		//compteur de joker incrémenté
		nbJoker+=1;
		//passage du numéro à la tuile 2 car le joker est bon en premiere place donc la comparaison du chiffre et de la couleur se fera avec cette tuile.
		numElem = elemVerif->tuile.num;
		//tableau complété par la couleur rencontrée
		coulElem[1] = elemVerif->tuile.coul;
		//passage à la tuile suivante pour la valider en fonction de la précédente
		elemVerif = elemVerif->suivant;
	}

	//parcourt du reste des tuiles dans la liste
	for(i=1+nbJoker;i<nbElemSuite;i++){
		//si la tuile n'est pas un joker
		if(elemVerif->tuile.num != 0){
			//si la tuile courante n'a pas comme numero la meme valeur par rapport à la tuile precedente
			if((elemVerif->tuile.num != numElem)){
				return 0;
			}
			
			//parcours des couleurs déjà rencontrée dans la liste
			for(j=0;j<4;j++){
				//si la couleur de l'element est egal a la couleur stocké à la case j du tableau
				if(elemVerif->tuile.coul == coulElem[j]){
					return 0;//retourne 0 car la suite est fausse
				}
			}
			//la couleur n'ayant jamais été rencontrée intègre le tableau
			coulElem[i]=elemVerif->tuile.coul;
		}
		//sinon si c'est un joker 
		else{
			//on incrémente le nombre totale de joker vu depuis le debut de la liste
			nbJoker+=1;
			//s'il y a plus qu'un joker dans cette liste alors c'est faux
			if(nbJoker>1){
				return 0;
			}
			//la couleur n'ayant jamais été rencontrée intègre le tableau (car un seul joker par combinaison)
			coulElem[i]=elemVerif->tuile.coul;

		}
		//passage de l'element à verifier au suivant par rapport a celui qui vient d'etre validé
		elemVerif=elemVerif->suivant;
	}
	//si tout est bon retourne 1
	return 1;
}


//retourne le nombre de points d'une suite de nombres qui se suivent mais de meme couleur
int nb_points_suite(LISTE *liste){
	int nbElems = nb_elements_liste(liste);
	int i;
	int total=0;
	ELEMENT *element = liste->premier;
	int num = element->tuile.num;
	
	//ajout du num de la tuile au total
	for(i=0;i<nbElems;i++){
		total += num+i;
	}
	return total;
	
}


//retourne le nb de points d'une liste de meme nombre mais de couleurs différente
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

//retourne la position choisie par le joueur quand on lui demande qu'elle tuile il choisit en fonction de la liste donnée
int choisirTuile(LISTE *liste){
	int choix;
	int nbElems;
	do{	
		printf("\nSaisir le n° de la tuile à jouer\n(Si vous créez une combinaison, 0 pour valider sa combinaison)\nSaisie:");	
		scanf("%d",&choix);
		nbElems=nb_elements_liste(liste);
	}while(choix<0 || choix>nbElems);//tant qu'il ne donne pas une position qui correspond àune tuile dans la liste
	return choix;
}

//retourne le choix du joueur quand on lui demande s'il veut choisir une tuile de sa main ou de la liste des tuiles obligatoires à jouer durant ce tour 
int choisir_main_ou_recupTuile(){
	int choix;
	do{	
		printf("\nChoisir une tuile de la main ou de la liste des tuiles obligatoires à jouer ?\n");
		printf("1-main\n2-liste des tuiles obligatoires\nSaisie:");
		scanf("%d",&choix);
	}while(choix<0 || choix>2);
	return choix;
}

//retourne le choix du coup à jouer quand on demande à l'utilisateur
int choisit_tour(bool premierCoup,int tourMultiTemps)
{
	int choix;

	do
	{
		//si le joueur n'a jamais posé une combinaison de plus de 30 points
		if(!premierCoup)
			printf("1 - Piocher\n2 - Créer une combinaison d'au moins 30 points\nSaisie:");
		//Si le joueur en est à son deuxieme coup ou plus durant ce tour; il ne peut pas pioché
		else if(tourMultiTemps == 1){
			printf("2 - Créer une combinaison\n3 - Compléter une combinaison\n4 - Récupérer une tuile dans une combinaison de taille 4 ou supérieure\n5 - Séparer une suite en deux\n6 - Récupérer un joker dans une combinaison du plateau\nSaisie:");
		}
		//sinon
		else{
			printf("1 - Piocher\n2 - Créer une combinaison\n3 - Compléter une combinaison\n4 - Récupérer une tuile dans une combinaison de taille 4 ou supérieure\n5 - Séparer une suite en deux\n6 - Récupérer un joker dans une combinaison du plateau\nSaisie:");
		}
	
		scanf("%d",&choix);
	}while((!premierCoup && (choix<1 || choix>2)) || ((tourMultiTemps==1) && (choix<2 || choix>6)) || (premierCoup && (choix<1 || choix>6)));//tant que le choix entré ne correspondent pas aux propositions

	//i le joueur choisit de poser pour la premiere fois durant la partie une combinaison son choix est modifié à 7 pour le traitement dans joue_tour;
	if(!premierCoup && choix==2)
		choix=7;

	return choix;
}

//Retourne 1 si le coup joué par le joueur a été validé
//Retourne 2 si le joueur décide de piocher
//Retourne 3 si le joueur qui pose sa première combinaison et que son coup est validé
//Sinon, le coup non validé retourne 0
int joue_tour(JOUEUR * joueur,int *niveauPioche,int tourMultiTemps)
{
	int typeDeCoup;
	int choixSuite;

	//On récupère le choix du coup a jouer du joueur en appelant choisit_tour
	switch(choisit_tour(joueur->premierCoup,tourMultiTemps))
	{
	case 1://Si le joueur pioche
		printf("\nVOUS PIOCHEZ\n");
		pioche_tuile(joueur->main,niveauPioche);//le joueur dont c'est le tour pioche
		return 2;	//retourne 2 pour le traitement dans le controlleur pour finir obligatoirement le tour du joueur
		break;
	case 2://si le joueur veux créer une combinaison et qu'il en a deja posé une durant la partie
		do{
			printf("\nVOUS CREEZ UNE COMBINAISON\n");
			printf("1 - Suite de nombres de meme couleur\n2 - Liste d'un meme nombre de couleurs differentes\nSAISIE:");
			scanf("%d",&choixSuite);
		}while(choixSuite<1 || choixSuite>2);//tant que le joueur n'a pas choisit le type de combinaison qu'il veut créer

		//Si saisit_combinaison retourne 1 (le coup est valide)
		if(saisit_combinaison(choixSuite,joueur)==1){
			return 1;	//retourne 1 comme quoi le coup est valide
		}
		break;
	case 3://si le joueur souhaite compléter une combinaison
		do{
			printf("\nVOUS COMPLETEZ UNE COMBINAISON\n");
			printf("1 - Ajouter une tuile à une combinaison\n2 - Ajouter une tuile à un bout d'une combinaison et récupérer la tuile à l'autre bout\nSaisie:");
			scanf("%d",&typeDeCoup);
		}while(typeDeCoup<1 || typeDeCoup>2);//tant que le joueur n'a pas choisit quel type de complétion il veut faire

		//Si c'est une complétion simple
		if(typeDeCoup==1){
			complete_combinaison();//lancement du coup
			return 1;	//le coup est forcement valide (on vérifie dans le controleur si toutes les combinaison du plateau sont valide, ici on parle du coup en lui meme)
		}
		//Sinon si c'est une complétion dans le but d'obtenir une tuile de la position à l'extreme opposé de la complétion
		else if(typeDeCoup==2){
			if(complete_recup_combinaison()==1){//si la complétion et récupération est valide
				return 1;	//retourne 1 comme quoi le coup est valide
			}
		}
		break;

	case 4://si le joueur souhaite récupérer une tuile dans une combinaison de plus de 3 tuiles
		//Si la réucpération d'une tuile est valide
		if(recupere_tuile_combinaison()==1){
			return 1;	//retourne 1 comme quoi le coup est valide
		}
		break;
	case 5://si le joueur souhaite séparer une combinaison à un endroit spécifié
		//Si la sépration d'une liste en deux est valide
		if(separe_combinaison()==1){
			return 1;	//retourne 1 comme quoi le coup est valide
		}
		break;
	case 6://si le joueur souhaite remplacer un joker présent sur le plateau (plus précisemment copiePlateau)
		if(remplace_joker()==1){
			return 1;	//retourne 1 comme quoi le coup est valide
		}
		break;
	case 7://si le joueur souhaite poser une combinaison de plus de 30 points pour la première fois depuis le début de la partie.
		do{
			printf("\nVOUS DEVEZ CREER UNE COMBINAISON D'AU MOINS 30 PTS\n");
			printf("1 - Suite de nombres de meme couleur\n2 - Liste d'un meme nombre de couleurs differentes\nSAISIE:");
			scanf("%d",&choixSuite);
		}while(choixSuite<1 || choixSuite>2);//tant que le joueur n'a pas choisit quel type de combinaison il allait posé

		//Si saisit_combinaison retourne 1 (le coup est valide)
		if(saisit_combinaison(choixSuite,joueur)==1){
			joueur->premierCoup = true;	//le joueur a posé sa premiere liste donc il debloque d'autre possibilités de coup quand cet varaible =true
			return 3;	//retourne 3 pour dire que le coup est validé et que le joueur a posé pour la premiere fois (aide pour le traitement dans le controlleur)
		}
		break;
	
	}
	return 0;	//S'il n'y a eu aucun return avant celui-ci alors c'est que le coup que le joueur a joué n'est pas valide
}

//Le joueur va compléter une combinaison présente sur le plateau
void complete_combinaison()
{
	int positionCombinaison,positionTuile;
	int positionInsertion=-1;
	LISTE *quelleListe;
	LISTE * combinaison;
	TUILE tuile;

	do{
		printf("Saisir le numéro de la combinaison du plateau à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));//tant que le joueur n'a pas saisit la position d'une combianaison à compléter sur le plateau
	
	//récupération de la combinaison choisit dans le plateau
	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);
	printf("LA COMBINAISON:\n");
	lit_liste(combinaison);

	do{
		printf("1 - Ajouter à la fin de la combinaison\n2 - Ajouter au début de la combinaison (début = position 0 et fin = apres la dernière tuile de la combinaison)\nSaisie:");
		scanf("%d",&positionInsertion);
	}while(positionInsertion<1 || positionInsertion>2);//tant que le joueur n'a pas choisit om compléter dans la combinaison

	printf("MAIN");
	lit_liste(copieMain);
	printf("TUILES A JOUER OBLIGATOIREMENT CE TOUR\n");
	lit_liste(listeTuilesRecup);

	//si choix de la tuile dans la main du joueur car listeTuilesREcup est vide
	if(nb_elements_liste(listeTuilesRecup)<1){
		positionTuile=choisirTuile(copieMain);
		quelleListe=copieMain;
	}
	//sinon si choix de la tuile dans la main du joueur
	else if(choisir_main_ou_recupTuile()==1){
		positionTuile=choisirTuile(copieMain);
		quelleListe=copieMain;
	}
	//sinon choix de la tuile dans listeTuilesRecup
	else{
		positionTuile=choisirTuile(listeTuilesRecup);
		quelleListe=listeTuilesRecup;
	}
	
	//r"cupération de la tuile en fonction du choix du joueur dans la liste choisie
	tuile=renvoie_tuile_via_position(quelleListe,positionTuile);

	//s'il souhaite compléter à la fin
	if(positionInsertion==1){
		ajoute_liste(combinaison,tuile);
	}
	//sinon s'il souhaite compléter au début
	else if(positionInsertion==2){
		placement_element_liste(combinaison,tuile,nb_elements_liste(combinaison)+1);
	}
	enleve_element_liste(quelleListe,positionTuile);	//on enleve la tuile de sa main
}

//Le joueur va récupérer une tuile dans une combinaison du plateau de plus de 3 tuiles
int recupere_tuile_combinaison()
{	
	int positionCombinaison,positionTuile;
	int nbElemsCombi;
	LISTE * combinaison;
	TUILE tuile;

	do{
		printf("Saisir la combinaison dans laquelle récupérer une tuile:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));//tant que le joueur n'a pas choisit de combinaison présente sur le plateau
	
	//récupération de la combinaison en fonction du choix du joueur 
	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);

	//si la combinaison comporte un joker
	if(joker_est_dans_liste(combinaison)==true){
		printf("ON NE PEUT PAS FAIRE CE COUP S'IL Y A UN JOKER DANS LA COMBINAISON,");
		return 0;//retourne 0 car pas possible
	}
	nbElemsCombi=nb_elements_liste(combinaison);
	//si le nombres d'elements dans la combinaison est inférieur à 4
	if(nbElemsCombi<4){
		printf("LA COMBINAISON CONTIENT MOINS DE 4 TUILES, C'EST UN COUP IMPOSSIBLE");
		return 0;//retourne 0 car pas possible
	}


	printf("LA COMBINAISON CHOISIE:\n");
	lit_liste(combinaison);

	do{
		printf("1-Récupérer la tuile à la fin de la combinaison\n2-Récupérer la tuile au début de la combinaison\n");
		scanf("%d",&positionTuile);
	}while(positionTuile<1 || positionTuile>2);//tant que le joueur n'a pas choisit quelle tuile récupérer

	//si c'est la tuile de fin
	if(positionTuile==1){
		//recupere la tuile
		tuile=renvoie_tuile_via_position(combinaison,nbElemsCombi);
		//on enleve la tuile de la combinaison
		enleve_element_liste(combinaison,nbElemsCombi);
		//on la met dans listeTuilesRecup
		ajoute_liste(listeTuilesRecup,tuile);
	}
	//sinon si c'est la tuile du debut
	else if(positionTuile==2){
		//recupere la tuile
		tuile=renvoie_tuile_via_position(combinaison,1);
		//on enleve la tuile de la combinaison
		enleve_element_liste(combinaison,1);
		//on la met dans listeTuilesRecup
		ajoute_liste(listeTuilesRecup,tuile);
	}
	return 1;	//retourne 1 car coup valide
}

//Le joueur veut compléter et récupérer une tuile à l'extreme opposé de celle qu'il a récupéré
int complete_recup_combinaison()
{	
	int positionCombinaison,positionTuile;
	int positionInsertion=-1;
	int nbElemsCombi;

	LISTE * combinaison;
	LISTE *quelleListe;
	TUILE tuile;

	do{
		printf("Saisir la combinaison à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));	//tant que le joueur n'a pas choisit la combinaison dans laquelle opérer
	
	//récupérationde la combinaison choisie
	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);
	
	//si la combinaison comporte un joker
	if(joker_est_dans_liste(combinaison)==true){
		printf("ON NE PEUT PAS FAIRE CE COUP S'IL Y A UN JOKER DANS LA COMBINAISON, VOUS POUVEZ JUSTE COMPLETER");
		return 0;//coup non valide
	}
	nbElemsCombi = nb_elements_liste(combinaison);
	//si la combinaison contient moins de 3 elements
	if(nbElemsCombi<3){
		printf("IMPOSSIBLE DE REALISER CETTE ACTION SUR UNE COMBINAISON DE MOINS DE 3 ELEMENTS");
		return 0;//coup non valide
	}	
	
	printf("LA COMBINAISON:\n");
	lit_liste(combinaison);

	do{
		printf("1-Ajouter à la fin de la combinaison\n2-Ajouter au début de la combinaison\n");
		scanf("%d",&positionInsertion);
	}while(positionInsertion<1 || positionInsertion>2);	//tant que le joueur ne choisit pas où il va compléter
	
	printf("MAIN");
	lit_liste(copieMain);
	printf("TUILES A JOUER OBLIGATOIREMENT CE TOUR\n");
	lit_liste(listeTuilesRecup);

	//si le nombre de tuiles obligatoires ce tour est 0
	if(nb_elements_liste(listeTuilesRecup)<1){
		//le joueur choisit une tuile dans sa main
		positionTuile=choisirTuile(copieMain);
		//sa main c'est copieMain
		quelleListe=copieMain;
	}
	//Sinon si le joueur choisit de jouer une tuile de sa main
	else if(choisir_main_ou_recupTuile()==1){
		//le joueur choisit une tuile dans sa main
		positionTuile=choisirTuile(copieMain);
		//sa main c'est copieMain
		quelleListe=copieMain;
	}
	//sinon le joueur choisit une tuile dans la listes des tuiles obligatoires à jouer ce tour
	else{
		//le joueur choisit une tuile dans listeTuilesRecup
		positionTuile=choisirTuile(listeTuilesRecup);
		//sa main c'est listeTuilesRecup
		quelleListe=listeTuilesRecup;
	}
	
	//recupere la tuile
	tuile=renvoie_tuile_via_position(quelleListe,positionTuile);

	//si c'est la tuile de fin
	if(positionInsertion==1){
		//ajoute la tuile choisie à la combinaison
		ajoute_liste(combinaison,tuile);
		//enleve la tuile de l'endroit om on la prise
		enleve_element_liste(quelleListe,positionTuile);

		//recupere la tuile dans la combinaison
		tuile = renvoie_tuile_via_position(combinaison,1);
		//on enleve la tuile de la combinaison
		enleve_element_liste(combinaison,1);
		//ajoute la tuile récupérée dans listeTuilesRecup
		ajoute_liste(listeTuilesRecup,tuile);
	}
	//sinon si c'est la tuile du debut
	else if(positionInsertion==2){
		//ajoute la tuile choisie à la combinaison
		placement_element_liste(combinaison,tuile,nb_elements_liste(combinaison)+1);
		//enleve la tuile de l'endroit om on la prise
		enleve_element_liste(quelleListe,positionTuile);

		nbElemsCombi = nb_elements_liste(combinaison);
		//recupere la tuile dans la combinaison
		tuile = renvoie_tuile_via_position(combinaison,nbElemsCombi);
		//on enleve la tuile de la combinaison
		enleve_element_liste(combinaison,nbElemsCombi);
		//ajoute la tuile récupérée dans listeTuilesRecup
		ajoute_liste(listeTuilesRecup,tuile);
	}
	return 1;
	
}

//renvoie true si la liste donnée comporte un joker; false sinon
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

//Joueur sépare la combinaison en deux à une position choisie
int separe_combinaison()
{
	int positionCombinaison,positionSeparation;
	int posSepa;
	int nbElemsDansCombi;
	LISTE * combinaison;

	do{
		printf("Saisir la combinaison à séparer:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));	//tant que le joueur n'a pas choisit la combinaison à séparer
	
	//recupere combinaison
	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);

	//si un joker est dans la combinaison
	if(joker_est_dans_liste(combinaison)==true){
		return 0;//retourne 0 car pas valide
	}

	printf("LA COMBINAISON CHOISIE:");
	lit_liste(combinaison);

	do{
		printf("Saisir la position à laquelle séparer la combinaison (le numéro entré sera celui de la première tuile de la combinaison créée):");
		scanf("%d",&positionSeparation);
	}while(positionSeparation<1 || positionSeparation>nb_elements_liste(combinaison));	//tant que le joueur n'a pas choisit une position où séparer

	nbElemsDansCombi=nb_elements_liste(combinaison);
	//correction du choix car l'affichage de la liste est à l'envers par rapport à la position dans la liste réelle
	posSepa=nbElemsDansCombi+1-positionSeparation;

	//ajout à la copie du plateau la liste renvoyée
	ajoute_plateau(separer_liste_en_deux(combinaison,posSepa),copiePlateau);
	return 1;//retourne 1 car coup valide
}

//echange deux tuiles de deux listes différentes (l'une est la main)
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
	enleve_element_liste(liste2,positionListe2);
}

//echange deux tuiles de deux listes différentes (l'une est listeTuilesRecup)
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

//Le joueur souhaite remplacer un joker
int remplace_joker()
{	
	TUILE tuile;
	int positionCombinaison,positionJokerCombinaison,positionTuile;
	LISTE * combinaison;

	do{
		printf("Saisir la combinaison à compléter:");
		scanf("%d",&positionCombinaison);
	}while(positionCombinaison<1 || positionCombinaison>nb_elements_plateau(copiePlateau));//tant que le joueur n'a pas choisit de combinaison
	
	//recupere combinaison
	combinaison = renvoie_liste_via_position(copiePlateau,positionCombinaison);
	printf("LA COMBINAISON CHOISIE:\n");
	lit_liste(combinaison);

	do{
		printf("Saisir la position du joker:");
		scanf("%d",&positionJokerCombinaison);
		tuile=renvoie_tuile_via_position(combinaison,positionJokerCombinaison);
	}while((positionJokerCombinaison<1 || positionJokerCombinaison>nb_elements_liste(combinaison)) && (tuile.num!=0));//tant que le joueur n'a pas choisit la position du joker


	
	printf("MAIN");
	lit_liste(copieMain);
	printf("TUILES A JOUER OBLIGATOIREMENT CE TOUR\n");
	lit_liste(listeTuilesRecup);

	//choix de la tuile dans la main car liste des tuiles à jouer obligatoirement est vide
	if(nb_elements_liste(listeTuilesRecup)<1){
		//choix de la tuile qui remplace
		positionTuile=choisirTuile(copieMain);
		//echange
		echange_tuiles_listes_via_main(combinaison,copieMain,positionJokerCombinaison,positionTuile);
	}
	//choix dans la main du joueur
	else if(choisir_main_ou_recupTuile()==1){
		//choix de la tuile qui remplace
		positionTuile=choisirTuile(copieMain);
		//echange
		echange_tuiles_listes_via_main(combinaison,copieMain,positionJokerCombinaison,positionTuile);
	}
	//choix dans la listeTuilesRecup
	else{
		//choix de la tuile qui remplace
		positionTuile=choisirTuile(listeTuilesRecup);
		//echange
		echange_tuiles_listes_via_recup(combinaison,listeTuilesRecup,positionJokerCombinaison,positionTuile);
	}

	
	return 1;	//retourne 1 car coup valide
}


//Le joueur pose une combinaison sur le plateau
int saisit_combinaison(int typeSuite, JOUEUR * joueur)
{
	int choix=1;
	LISTE *combinaison=cree_liste();
	LISTE *quelleListe;
	int nbElemsCombi;

	do{
		printf("MAIN\n");
		lit_liste(copieMain);
		printf("TUILES A JOUER OBLIGATOIREMENT CE TOUR\n");
		lit_liste(listeTuilesRecup);
		printf("LA COMBINAISON EN TRAIN D'ETRE CREEE:\n");
		lit_liste(combinaison);

		//choix de la tuile dans la main car liste des tuiles à jouer obligatoirement est vide
		if(nb_elements_liste(listeTuilesRecup)<1){
			choix=choisirTuile(copieMain);
			quelleListe=copieMain;
		}
		//choix dans la main du joueur
		else if(choisir_main_ou_recupTuile()==1){
			choix=choisirTuile(copieMain);
			quelleListe=copieMain;
		}
		//choix dans la listeTuilesRecup
		else{
			choix=choisirTuile(listeTuilesRecup);
			quelleListe=listeTuilesRecup;
		}

		//si le joueur n'a pas finit de créer sa xombinaison
		if(choix!=0)
		{
			//ajout de la tuile à la combinaison créée
			ajoute_liste(combinaison,renvoie_tuile_via_position(quelleListe,choix));
			//enleve la tuile de l'endroit om on l'a prise
			enleve_element_liste(quelleListe,choix);
		}
	}while(choix!=0);//tant que le joueur n'a pas terminer sa combinaison

	nbElemsCombi=nb_elements_liste(combinaison);
	
	//si le nombre d'elements de la combinaison créée est supérieure à deux
	if(nbElemsCombi>2){
		//si c'est une suite de nombres qui se suivent de meme couleur
		if(typeSuite==1){
			//Si la suite est valide
			if(est_suite(combinaison)==1){
				//si  c'etait la premier pose du joueur dans la partie et que la combinaison ne fait pas 30 points
				if((joueur->premierCoup == false) && (nb_points_suite(combinaison)<30)){
					printf("CETTE SUITE FAIT MOINS DE 30 POINTS\n");
					return 0;//coup pas valide
				}
				ajoute_plateau(combinaison,copiePlateau);//ajout au plateau
				return 1;//coup valide
			}
			printf("CE N'EST PAS UNE SUITE DE NOMBRE DE MEME COULEUR\n");
		}
		//sinon si c'est une suite de meme nombres de couleurs différentes
		else if(typeSuite==2){
			//Si la suite est valide
			if(est_liste_meme_nb(combinaison)==1){
				//si  c'etait la premier pose du joueur dans la partie et que la combinaison ne fait pas 30 points
				if((joueur->premierCoup == false) && (nb_points_liste_mnombre(combinaison)<30)){
					printf("CETTE SUITE FAIT MOINS DE 30 POINTS\n");
					return 0;//coup pas valide
				}
				ajoute_plateau(combinaison,copiePlateau);//ajout au plateau
				return 1;//coup valide
			}
			printf("CE N'EST PAS UNE SUITE D'UN MEME NOMBRE DE COULEURS DIFFERENTES\n");
		}
	}
	
	
	return 0;//retourne 0 si coup pas valide

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
		element=element->suivant;
		indiceElement+=1;
	}
}

//eneleve un element d'une position donnée d'une liste en recontruisant la liste sans la tuile
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
	fichier=fopen("scores.csv", "a");
	
	if(fichier == NULL)
		exit(1); 

	fprintf(fichier,"%s %d\n",nom,score);
	fclose(fichier);
}


void affiche_tab(TUILE *t,int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		printf("%d;%d\n",t[i].num,t[i].coul);
	}
}

void initialise_tab(TUILE *t,int n)
{
	int i;
	TUILE u;
	u.num=-1;u.coul=-1;
	for(i=0;i<n;i++)
	{
		t[i]=u;
	}
}

void init_tab(TUILE *t,int n,LISTE *l)
{
	int i;
	for(i=0;i<n;i++)
	{
		t[i]=renvoie_tuile_via_position(l,i+1);
	}
}




void tri_par_couleur(TUILE *t, int n)
{
	int i,j;
	TUILE aux;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-i;j++)
		{
			if((t[j].coul)>(t[j+1].coul))
			{
				aux= t[j];
				t[j]=t[j+1];
				t[j+1]=aux;
			}
		}
	}
}

// tri croissant par couleur 
void tri_couleur_croissant(TUILE *t, int n)
{
	tri_par_couleur(t, n);
	int i,j;
	TUILE aux;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-i;j++)
		{
			if(t[j].coul==t[j+1].coul)
			{
				if(t[j+1].num < t[j].num)
				{
					aux= t[j];
					t[j]=t[j+1];
					t[j+1]=aux;
				}
			}
		}
	}
}

void suite_valide(TUILE *t, int n, PLATEAU *p)
{
	int i,j;
	LISTE *aux=cree_liste();
	TUILE u;
	TUILE pos[n];
	TUILE erreur[n];
	u.num=-1;u.coul=-1;
	initialise_tab(erreur,n);
	initialise_tab(pos,n);
	for(i=0;i<n;i++)
	{
		if(t[i].num>0)
		{
			if(i==n-1)
			{
				if(((t[i].num) == (t[i-1].num+1))&&(((t[i+1].num) == (t[i].num+1)))&&(t[i].coul==t[i-1].coul)&&(t[i].coul==t[i+1].coul))
				{
					ajoute_liste(aux,t[i]);
					pos[i]=t[i];
					ajoute_liste(aux,t[i+1]);
					pos[i+1]=t[i+1];
				}
				if(nb_elements_liste(aux)>= 3)
				{
					ajoute_plateau(aux,p);
					for(j=0;j<n;j++)
					{
						if(erreur[j].num ==-1)
						{
							if(pos[j].num !=-1)
							{
								erreur[j]=pos[j];
							}
						}
					}
					aux= cree_liste();
					initialise_tab(pos,n);
				}
				else
				{
					aux= cree_liste();	
					initialise_tab(pos,n);					
				}
			}
			else
			{
				if(nb_elements_liste(aux)==0)
				{
				  ajoute_liste(aux,t[i]);
				  pos[i]=t[i];
				  
				}
				else 
				{
					if((((t[i].num) != (t[i-1].num+1))&&((t[i].num) != (t[i-1].num)))|| ((t[i].coul) != (t[i-1].coul)))
					{	
						if(nb_elements_liste(aux)>=3)
						{
						  ajoute_plateau(aux,p);
						  for(j=0;j<n;j++)
							{
								if(erreur[j].num ==-1)
								{
									if(pos[j].num !=-1)
									{
										erreur[j]=pos[j];
									}
								}
							}
							aux= cree_liste();
							initialise_tab(pos,n);
						}
						else
						{
							aux= cree_liste();
							initialise_tab(pos,n);
						}
					}
					else
					{
						if((t[i].num) == (t[i-1].num))
						{
							if(nb_elements_liste(aux)==0)
							{
								aux=cree_liste();
								initialise_tab(pos,n);
								ajoute_liste(aux,t[i]);
								
							}
						}
						else
						{
							ajoute_liste(aux,t[i]);
							pos[i]=t[i];
						}
						
					}
					
				}
			}
		}
    }
    for(i=0;i<=n;i++)
	{
		if (((erreur[i]).num != -1)&&((erreur[i]).coul !=-1))
		{
			t[i]=u;
		}
	}
}


//tri par triplé par couleur


void tri_triple_croissant(TUILE *t, int n)
{
	tri_par_couleur(t,n);
	int i,j;
	TUILE aux;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-i;j++)
		{
			if(t[j+1].num < t[j].num)
			{
				aux= t[j];
				t[j]=t[j+1];
				t[j+1]=aux;
			}
		
		}
	}
}

void triple_valide(TUILE *t,int n,PLATEAU *p)
{
	int i,j ;
	LISTE *aux=cree_liste(); 
	TUILE u;
	TUILE pos[n];
	TUILE erreur[n];
	u.num=-1;u.coul=-1;
	initialise_tab(erreur,n);
	initialise_tab(pos,n);
	
	for(i=0;i<n;i++)
	{
		if(t[i].num>0)
		{
			if(i==n-1)
			{
				if(((t[i].num) == (t[i-1].num))&&(((t[i+1].num) == (t[i].num)))&&(t[i].coul !=t[i-1].coul)&&(t[i].coul!=t[i+1].coul))
				{
					ajoute_liste(aux,t[i]);
					pos[i]=t[i];
					ajoute_liste(aux,t[i+1]);
					pos[i+1]=t[i+1];
				}
				if(nb_elements_liste(aux)>= 3)
				{
					ajoute_plateau(aux,p);
					for(j=0;j<n;j++)
					{
						if(erreur[j].num ==-1)
						{
							if(pos[j].num !=-1)
							{
								erreur[j]=pos[j];
							}
						}
					}
					aux= cree_liste();
					initialise_tab(pos,n);
				}
				else
				{
					aux= cree_liste();	
					initialise_tab(pos,n);					
				}
			}
			else
			{
				if(nb_elements_liste(aux)==0)
				{
					ajoute_liste(aux,t[i]);
					pos[i]=t[i];
				}
				else
				{
					if(t[i].num != t[i-1].num)
					{
						if(nb_elements_liste(aux)>= 3)
						{
							ajoute_plateau(aux,p);
							for(j=0;j<n;j++)
							{
								if(erreur[j].num ==-1)
								{
									if(pos[j].num !=-1)
									{
										erreur[j]=pos[j];
									}
								}
							}
							aux= cree_liste();
							initialise_tab(pos,n);
						}
						else
						{
							aux= cree_liste();
							initialise_tab(pos,n);
						}
					}
					else
					{
						if(t[i].coul != t[i-1].coul)
						{
							ajoute_liste(aux,t[i]);
							pos[i]=t[i];
						}
					}
				}

			}
		}		
	}
	for(i=0;i<=n;i++)
	{
		if (((erreur[i]).num != -1)&&((erreur[i]).coul !=-1))
		{
			t[i]=u;
		}
	}
}


//quelque algo pour le choix de la combinaison posé

LISTE * renvoie_element_plateau(PLATEAU *p, int pos)
{
	ELEMENT_PLATEAU *element=p->premier;
 	LISTE *erreur=cree_liste();

	while(element != NULL)
	{
		if(pos==1)
    		{
      			return element->liste;
    		}
    		element=element->suivant;
    		pos= pos-1;
	}
  	return erreur;
}

void remplissage_methode1(TUILE *t,int n, PLATEAU  *coup_valide)
{
	tri_couleur_croissant(t,n);
	suite_valide(t,n,coup_valide);
	tri_triple_croissant(t,n);
	triple_valide(t,n,coup_valide);
	
}

void remplissage_methode2(TUILE *t, int n, PLATEAU  *coup_valide)
{
	tri_triple_croissant(t,n);
	triple_valide(t,n,coup_valide);
	lit_plateau(coup_valide);
	tri_couleur_croissant(t,n);
	suite_valide(t,n,coup_valide);
	lit_plateau(coup_valide);

}

int decompte_point(PLATEAU *p)
{
	int n=nb_elements_plateau(p);
	int s=0;
	int i,j;
	LISTE *l;
	TUILE y;

	for(i=1;i<=n;i++)
	{
		l=renvoie_element_plateau(p, i);
		int m=nb_elements_liste(l);
		for(j=1;j<=m;j++)
		{
			y=renvoie_tuile_via_position(l,j);
			s=s+y.num;
		}
	}
	return s;
}

int choix_de_depot(TUILE *t1, TUILE *t2,TUILE *finall,PLATEAU *coup_valide1, PLATEAU *coup_valide2, PLATEAU *coup_final)
{
	int x=decompte_point(coup_valide1);
	int y=decompte_point(coup_valide2);
	if(x>y)
	{
		coup_final=coup_valide1;
		finall=t1;
		return x;
	}
	else
	{
		coup_final=coup_valide2;
		finall=t2;
		return y;
	}
}

//pour poser les joker sur le plateau 
void poser_joker(TUILE *finall,int n,PLATEAU *plateau)
{
	int i,j,k;
	LISTE *l=cree_liste();
	int m=nb_elements_plateau(plateau);
	for(i=1;i<=m;i++)
	{
		l=renvoie_element_plateau(plateau,i);
		k=nb_elements_liste(l);
		for(j=0;j<n;j++)
		{
			if(finall[j].num==0)
			{
				if(renvoie_tuile_via_position(l,1).num < renvoie_tuile_via_position(l,k).num)
				{
					if ((renvoie_tuile_via_position(l,1).num >1)&& (renvoie_tuile_via_position(l,1).num !=0))
					{
						ajoute_liste(l,finall[j]);
						finall[j].num=-1;finall[j].coul=-1;
					}
					else
					{
						if ((renvoie_tuile_via_position(l,k).num <13)&& (renvoie_tuile_via_position(l,1).num !=0))
						{
							placement_element_liste(l, finall[j], k+1);
							finall[j].num=-1;finall[j].coul=-1;
						}
					}
				}
				else 
				{
					if(renvoie_tuile_via_position(l,1).num < renvoie_tuile_via_position(l,k).num)
					{
						if ((renvoie_tuile_via_position(l,1).num <13)&& (renvoie_tuile_via_position(l,1).num !=0))
						{
							ajoute_liste(l,finall[j]);
							finall[j].num=-1;finall[j].coul=-1;
						}
						else
						{
							if ((renvoie_tuile_via_position(l,k).num >1)&& (renvoie_tuile_via_position(l,1).num !=0))
							{
								placement_element_liste(l, finall[j], k+1);
								finall[j].num=-1;finall[j].coul=-1;
							}
						}
					}
					else 
					{
						if(k<4)
						{
							ajoute_liste(l,finall[j]);
							finall[j].num=-1;finall[j].coul=-1;
						}
					}
				}
			}
		}
	}
}

// L'ia le bloc
void pose_ia(JOUEUR *j, PLATEAU *plateau,int *niveauPioche)
{
	int score;
	int i=1;
	LISTE *L=j->main;
	LISTE *F=cree_liste();
	int n=nb_elements_liste(L);
	PLATEAU *coup_final=cree_plateau();
	PLATEAU *coup_valide1=cree_plateau();
	PLATEAU *coup_valide2=cree_plateau();
	TUILE t1[n];
	TUILE t2[n];
	TUILE finall[n];
	
	
	init_tab(t1, n, L);
	init_tab(t2, n, L);
	initialise_tab(finall,n);
	remplissage_methode1(t1, n,coup_valide1);
	remplissage_methode2(t2, n,coup_valide2);
	score= choix_de_depot(t1, t2, finall, coup_valide1, coup_valide2, coup_final);
	if((j->premierCoup) || (score>=30))
	{
		int n=nb_elements_plateau(coup_final);
		for(i=1;i<=n;i++)
		{
			LISTE *l=renvoie_element_plateau(coup_final,i);
			ajoute_plateau(l,plateau);
			if(j->premierCoup==false)
			{
				j->premierCoup=true;
			}
		}
		poser_joker(finall,n,plateau);
		j->main=F;
	}
	else 
	{
		 pioche_tuile(L,niveauPioche);
	}
}



