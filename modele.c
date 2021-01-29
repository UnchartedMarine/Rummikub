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

int regarde_qui_commence(int nbJoueurs)
{
	int borneInferieur = -1;
	return regarde_qui_commence_aux(nbJoueurs-1,borneInferieur,borneInferieur);
}



int regarde_qui_commence_aux(int nbJoueurs,int valeurMax,int positionMax)
{
	int valeur;

	if(nbJoueurs<0)
		return positionMax;

	valeur=pioche[rand()%106].num;
	printf("Joueur n°%d pioche %d\n",nbJoueurs,valeur);

	if(valeur > valeurMax)
		return regarde_qui_commence_aux(nbJoueurs-1,valeur,nbJoueurs);
	if(valeur == valeurMax)
		return regarde_qui_commence_aux(nbJoueurs,valeurMax,positionMax);
	else
		return regarde_qui_commence_aux(nbJoueurs-1,valeurMax,positionMax);
}

PLATEAU * cree_plateau()
{
	PLATEAU *plateau = (PLATEAU*) malloc(sizeof(PLATEAU));
	plateau->premier =  NULL;
	return plateau;
}

void ajoute_plateau(PLATEAU *plateau,LISTE *liste)
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
	ELEMENT *tuileMain=liste->premier;
	int indiceTuile=1;
	
	while(tuileMain != NULL)
	{
		printf("(%d) %d;%d\n",indiceTuile,tuileMain->tuile.num,tuileMain->tuile.coul);	
		tuileMain=tuileMain->suivant;
		indiceTuile = indiceTuile + 1;
	}
	printf("\n");
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

//EN CHANTIER
//renvoi de est_valide le type de liste
bool pose_30_points(LISTE *liste){
	ELEMENT *tuileCourante=liste->premier;
	ELEMENT *tuileAvant;
	int tailleListe = nb_elements_liste(liste);
	int pointsListe=0;
	int i;
	//Si la premiere tuile de la suite est un joker, on se
	if(tuileCourante->tuile.num == 0){
	}
	for(i=1;i<tailleListe;i++){
		if(tuileCourante->tuile.num == 0){
		}
		pointsListe = pointsListe + tuileCourante->tuile.num;
		if(pointsListe >= 30){
			return true;
		}
		tuileAvant=tuileCourante;
		tuileCourante = tuileCourante->suivant;
	}
	return false;

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
	derniereTuile->tuile.num = NULL;
	derniereTuile->tuile.coul = NULL;
	derniereTuile->suivant = NULL;
	tuileAvant->suivant=derniereTuile;

	return liste2;
}


//soit la sdl demande au joueur quel type de suite il souhaite poser et ca me permet de verifier que ce type dans la fonction valide et 30 points
//soit si je rencontre un joker, je prend deux tuiles pour verification (jok=1st : 2et3; 2nd : 1 et 3; autre:les deux precedents)

//Renvoie un booléen pour dire si la suite est valide: false(0) si la suite n'est pas valide, true(1) si elle est valide.
bool est_valide(LISTE *liste){ //Penser à intégrer le joker à cette vérification
	//tuileVerif est la tuile courante à vérifier et tuileVerifSuivante la tuile suivante de la tuile courante.
	ELEMENT *tuileVerif = liste->premier;
	ELEMENT *tuileVerifSuivante = tuileVerif->suivant;
	ELEMENT *tuileAvantJoker;
	int tailleListe = nb_elements_liste(liste);
	int i,j;
	
	//typeListe permet de savoir à partir des deux premières tuiles de la liste fournie si l'on va traiter une suite de numéros ou une suite de tuile de même numéro et de couleurs différentes.
	int typeListe;
	
	//couleursVues est un tableau qui accueille les couleurs des tuiles rencontrées dans la suite de numéros similaires
	int couleursVues[4];
	
	//numRepete permet de savoir dans une suite de mêmes numéros et de couleurs différentes si le numéro de toutes les tuiles est le même.
	int numRepete;
	
	//
	int tuileJoker = 0;

	//Si un joker est la première tuile, la suite du programme regardera si la suite de la suite est valide
	if(tuileVerif->tuile.num == 0){
		tuileVerif = tuileVerif->suivant;
		tuileVerifSuivante = tuileVerifSuivante->suivant;
		tuileJoker = 1;
	}
	//Si un joker est la seconde tuile, vérifie le type de liste entre la première tuile et la troisième tuile
	else if(tuileVerifSuivante->tuile.num == 0){
		tuileVerifSuivante=tuileVerifSuivante->suivant;
		tuileJoker = 1;
		
		if((tuileVerif->tuile.num == tuileVerifSuivante->tuile.num-2) && (tuileVerif->tuile.coul == tuileVerifSuivante->tuile.coul)){
			typeListe=1;
			tuileVerif = tuileVerifSuivante;
			tuileVerifSuivante = tuileVerifSuivante->suivant;
		}
		else if((tuileVerif->tuile.coul != tuileVerifSuivante->tuile.coul) && (tuileVerif->tuile.num == tuileVerifSuivante->tuile.num)){
			typeListe=2;	
			couleursVues[0] = tuileVerif->tuile.coul;
			couleursVues[1] = 0; 
			couleursVues[2] = tuileVerifSuivante->tuile.coul;
			tuileVerif = tuileVerifSuivante->suivant;
			numRepete = tuileVerif->tuile.num;
		}
		else{
			return false;
		}
	}
	//SI pas de joker dans les deux premières tuiles, on regarde le type de liste
	else{
		//Si les deux premières tuiles ont des numéros qui se suivent et la même couleur.
		if((tuileVerif->tuile.num == tuileVerifSuivante->tuile.num-1) && (tuileVerif->tuile.coul == tuileVerifSuivante->tuile.coul)){
			typeListe=1;
			tuileVerif = tuileVerif->suivant;
			tuileVerifSuivante = tuileVerifSuivante->suivant;
		}
		//Si les deux premières tuiles ont des couleurs différentes mais le même numéro
		else if((tuileVerif->tuile.coul != tuileVerifSuivante->tuile.coul) && (tuileVerif->tuile.num == tuileVerifSuivante->tuile.num)){
			typeListe=2;	
			couleursVues[0] = tuileVerif->tuile.coul;
			couleursVues[1] = tuileVerifSuivante->tuile.coul;
			tuileVerif = tuileVerifSuivante->suivant;
			numRepete = tuileVerif->tuile.num;
		}
		//Sinon la suite n'est déja pas valide 
		else{
			return false;
		}
	}
	
	
	//Si typeListe est égal à 1 c'est une suite de tuiles de numéros qui se suivent et de même couleur.
	if (typeListe == 1){
		//Parcours du reste de la suite
		for(i=2+tuileJoker;i<tailleListe;i++){
			//Si la tuile courante est un joker (cela signifie qu'il y a une tuile apres le jokerà
			if(tuileVerif->tuile.num == 0){
				//On vérifie la suite entre la tuile avant le joker (car il n'a pas de valeur) et la tuile suivante du joker
				if((tuileAvantJoker->tuile.num == tuileVerifSuivante->tuile.num-2) && (tuileAvantJoker->tuile.coul == tuileVerifSuivante->tuile.coul)){
					typeListe=1;
					tuileVerif = tuileVerifSuivante;
					tuileVerifSuivante = tuileVerifSuivante->suivant;
				}
			}
			//Si la tuile suiavnte de la tuile courante est un joker
			if(tuileVerifSuivante->tuile.num==0){
				tuileJoker=tuileJoker+1;
				if(tuileJoker>1){
					return false;
				}
				tuileAvantJoker = tuileVerif;
				tuileVerif = tuileVerif->suivant;
				tuileVerifSuivante = tuileVerifSuivante->suivant;
			}
			//Si les deux tuiles courantes ont des numéros qui se suivent et la même couleur.
			else if((tuileVerif->tuile.num == tuileVerifSuivante->tuile.num-1) && (tuileVerif->tuile.coul == tuileVerifSuivante->tuile.coul)){
				tuileVerif = tuileVerif->suivant;
				tuileVerifSuivante = tuileVerifSuivante->suivant;
			}
			//Sinon on s'arrete et on renvoie que la suite n'est pas valide
			else{
				return false;
			}
		}
	}
	//Sinon c'est une suite de tuiles de même numéro et de couleurs différentes.
	else if(typeListe == 2){
		//Parcours du reste de la suite
		for(i=2+tuileJoker;i<tailleListe;i++){
			if(tuileVerif->tuile.num == 0){ //si joker
				tuileJoker=tuileJoker+1;
				if(tuileJoker>1){
					return false;
				}
				tuileVerif->tuile.num = numRepete;
			}
			//Si le numéro de la tuile courante est le même que celui du reste de la suite
			if(tuileVerif->tuile.num == numRepete){
				//Parcours des couleurs déjà rencontrées
				for(j=0;j<4;j++){
					//Si la couleur de la tuile courante est retrouvée dans le tableau des couleurs déjà vues dans la suite alors la suite n'est pas valide
					if(couleursVues[j] == tuileVerif->tuile.coul){
						return false;
					}			
				}
				//Ajout de la couleur de la tuile courante qui est valide par rapport au début de la suite
				couleursVues[i] = tuileVerif->tuile.coul;
				tuileVerif->tuile.num = 0;
				tuileVerif = tuileVerif->suivant;
			}
			//Sinon la liste n'est pas valide
			else{
				return false;
			}
		}
	}
	
	//La suite est valide
	return true;
}



void lit_tuile_liste(LISTE *liste, int position)  //fonction pour lire la tuile de la liste à la position donnée 
{
	if(position>=0 && position<=nb_elements_liste(liste))  //vérifie que position soit bien une position de la liste
	{
		ELEMENT *premiereTuile = liste->premier;
		for(position;position>0;position--) //boucle qui met à position à 0 afin de savoir quand la position est bonne
		{
			premiereTuile = premiereTuile->suivant;
		}
		lit_tuile(premiereTuile->tuile);
	}
}



int additionne_points(LISTE *main, int fin) //Jonathan j'ai enlevé main à la fin de l'intitulé de la fonction car pour eviter une redondance de code j'utilise cette fonction pour compter les points d'une suite
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


int calcule_points_gagnant(JOUEUR *joueurs,int nbJoueurs,int gagnant,int aVOIR)
{
	int i;
	int points=0;

	if(aVOIR==1)
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


int calcule_points_perdant(JOUEUR *joueurs,int gagnant,int perdant,int aVOIR)
{
	int points=0;
	
	if(aVOIR==1)
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


int detecte_gagnant(JOUEUR *joueurs,int nbJoueurs,int tour,int aVOIR)
{
	int gagnant;

	if(aVOIR==1)
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

	if(!premierCoup)
		printf("1-Piocher\n2-Créer une combinaison d'au moins 30 points\nSaisie:");
	else
		printf("1-Piocher\n2-Créer une combinaison (jsais plus si y'a cette option)\n3-Compléter une combinaison\nSaisie:");
	
	scanf("%d",&choix);

	if(!premierCoup && choix==2) ////////////////////// PROVISOIRE MAIS BERKKKKKKKKKKKKKKK
		choix=4;


	return choix;
}


void joue_tour(JOUEUR * joueur,int *niveauPioche)
{
	int a = 0;

	switch(choisit_tour(joueur->premierCoup))
	{
	case 1:
		printf("1 (pioche)\n");
		pioche_tuile(joueur->main,niveauPioche);
		break;
	case 2:
		printf("2 (crée combi)\n");
		saisit_combinaison(joueur->main);
		break;
	case 3:
		printf("3 (complète combi)\n");
/*
		while(a!=5)
		{
			printf("1 - Ajouter à la combinaison\n2 - Récupérer une tuile\n3 - Remplacer une tuile\n4 - Diviser une combinaison\n5 - Finir tour\n");
			scanf("%d",&a);	


			if(a==1)
				complete_combinaison();
			else if(a==2)
				recupere_tuile_combinaison(combinaison);
			else if(a==3)
			else if(a==4)
				separer_liste_en_deux(combinaison,?????);
			else if(a!=5)
				printf("inutile\n");
		}*/

		//lit_liste(combinaison);
		break;
	case 4:
		printf("4 (30pts à mettre)\n");
		saisit_combinaison(joueur->main);
		joueur->premierCoup = true;
		break;
	}

}


/*
void complete_combinaison(LISTE combinaison)
{
	int position;
	LISTE * combinaison;
	
	lit_plateau();
	printf("Saisir la combinaison à compléter:\n");
	scanf("%d",&position);
	combinaison = renvoie_liste_via_position(position);

	lit_liste(combinaison);
	printf("1 - Compléter au début\n2 - Compléter à la fin\n");
	scanf("%d",&a);
	if(a==1)
	else

}
*/



void saisit_combinaison(LISTE *main)
{
	int choix=1;
	LISTE *combinaison=cree_liste();

	while(choix!=0)
	{
		lit_liste(main);
		printf("Saisir le n° de la tuile à jouer\n0 pour valider sa combinaison:\n");
		scanf("%d",&choix);
		if(choix!=0)
		{
			ajoute_liste(combinaison,renvoie_tuile_via_position(main,choix));
			enleve_element_liste(main,choix);
		}
	}
	ajoute_plateau(plateau,combinaison);
}



LISTE * renvoie_liste_via_position(int position)
{
	ELEMENT_PLATEAU *element=plateau->premier;

	if((position>0) && (position<=nb_elements_plateau())) ////////////////////////////////PROBLEME AVEC CAAAAAAA
	{
		while(element != NULL)
		{
			if(position==1)
				return element->liste;
			element=element->suivant;
			position-=1;
		}
	}
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

	if((position>0) && (position<=nb_elements_liste(liste))) ////////////////////////////////PROBLEME AVEC CAAAAAAA
	{
		while(element != NULL)
		{
			if(position==1) //position est à 1 quand on se trouve sur la tuile cherchée
				return element->tuile;
			element=element->suivant;
			position-=1;
		}
	}
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







