/******************************************
 * Modele
 ******************************************/
#include "header.h" 



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


void init_main(LISTE *liste,int *niveauPioche)
{
	int i=*niveauPioche+6;	

	for(*niveauPioche;(*niveauPioche)<i;(*niveauPioche)++)
	{
		ajoute_liste(liste,pioche[*niveauPioche]);
	}
}


int regarde_qui_commence(int nbJoueurs)  
/*****si 2 piochent le max le premier commence ? ou il faut retirer ? + problème c'est que ça aide car on peut deviner comment est la pioche et on peut connaître des tuiles de la main des autres*****/
{
	int max=-1;	
	int i,position;

	for(i=0;i<nbJoueurs;i++)
	{
		printf("Joueur %d pioche %d\n",i,pioche[i]);
		
		if(pioche[i].num>max)
		{
			max=pioche[i].num;
			position=i;		
		}			
	}
	return position;
}



// les fonctions sur les listes:
LISTE * cree_liste()
{
	MAIN *main = (MAIN*) malloc(sizeof(MAIN));
	LISTE *liste = (LISTE*) malloc(sizeof(LISTE));

	main->tuile.num = NULL;
	main->tuile.coul = NULL;
	main->suivant = NULL;
	
	liste->premier=main;

	return liste;
}


void ajoute_liste(LISTE *liste,TUILE tuile)
{
	MAIN *nouveau = malloc(sizeof(*nouveau));

	nouveau->tuile=tuile;
	nouveau->suivant=liste->premier;

	liste->premier=nouveau;
}


void lit_liste(LISTE *liste)
{
	MAIN *tuileMain=liste->premier;
	int indiceTuile=1;
	
	while(tuileMain != NULL)
	{
		printf("(%d) %d;%d\n",indiceTuile,tuileMain->tuile.num,tuileMain->tuile.coul);	
		tuileMain=tuileMain->suivant;
		indiceTuile = indiceTuile + 1;
	}
}

//Renvoie la taille d'une liste ou suite dans le jeu, en faisant abstraction de la tuile de fin dont la couleur et le numéro vallent NULL.
int nb_elements_liste(LISTE *liste)
{
	MAIN *tuileMain=liste->premier;
	int nombreElements=0;

	while(tuileMain != NULL)
	{
		nombreElements = nombreElements +1;
		tuileMain=tuileMain->suivant;			
	}
	return nombreElements -1;
}


//EN CHANTIER
//renvoi de est_valide le type de liste 
bool pose_30_points(LISTE *liste){
	MAIN *tuileCourante=liste->premier;
	MAIN *tuileAvant;
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
int placement_tuile_liste(LISTE *liste, TUILE tuile, int position)
{
	if(position > nb_elements_liste(liste)){
		return 0;
	}

	if(position==1){
		ajoute_liste(liste,tuile);
	}
	else{
		//On créer un type MAIN pour la tuile fournie en paramètres afin de la placer dans la liste qui est composés d'éléments de type MAIN.
		MAIN *tuileAvant=liste->premier;
		MAIN *nouveau = malloc(sizeof(*nouveau));
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
	MAIN *tuileAvant=liste->premier;
	int i;

	for(i=1;i<position-1;i++){
		tuileAvant=tuileAvant->suivant;
	}
	
	//Creation nouvelle liste a partir de l element demande
	LISTE *liste2 = (LISTE*) malloc(sizeof(LISTE));
	liste2->premier=tuileAvant->suivant;

	//Remplacer la tuile suivante de la derniere tuile de la suite 1 par la tuile vide (dont le numero et la couleur sont NULL)
	MAIN *derniereTuile = (MAIN*) malloc(sizeof(MAIN));
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
	MAIN *tuileVerif = liste->premier;
	MAIN *tuileVerifSuivante = tuileVerif->suivant;
	MAIN *tuileAvantJoker;
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
		MAIN *premiereTuile = liste->premier;
		for(position;position>0;position--) //boucle qui met à position à 0 afin de savoir quand la position est bonne
		{
			premiereTuile = premiereTuile->suivant;
		}
		lit_tuile(premiereTuile->tuile);
	}
}

}

int additionne_points(LISTE *main, int fin) //Jonathan j'ai enlevé main à la fin de l'intitulé de la fonction car pour eviter une redondance de code j'utilise cette fonction pour compter les points d'une suite
{
	MAIN *tuileMain=main->premier;
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
					MAIN *tuileSuivante = tuileMain->suivant;
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


int calcule_points_gagnant(LISTE **mains,int nbJoueurs,int gagnant,int aVOIR)
{
	int i;
	int points=0;

	if(aVOIR==1)
	{
		for(i=0;i<nbJoueurs;i++)
		{
			if(i!=gagnant)
				points+=additionne_points_main(mains[i])-additionne_points_main(mains[gagnant]);
		}
	}
	else
	{
		for(i=0;i<nbJoueurs;i++)
			points+=additionne_points_main(mains[i]);
	}

	return points;
}


int calcule_points_perdant(LISTE **mains,int gagnant,int perdant,int aVOIR)
{
	int points=0;
	
	if(aVOIR==1)
		points=additionne_points_main(mains[gagnant])-additionne_points_main(mains[perdant]);
	else
		points-=additionne_points_main(mains[perdant]);

	return points;
}


int plus_petite_main(LISTE **mains, int nbJoueurs)
{
	int i;
	int tmp;
	int min=1000;  //valeur qui ne sera pas dépassée
	int gagnant;

	for(i=0;i<nbJoueurs;i++) 
	{
		tmp=additionne_points_main(mains[i]);
		printf("tmp:%d\n",tmp);
		if(tmp<min)   /*que faire si 2 personnes ont un même score en main ???? */
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


int detecte_gagnant(LISTE **mains,int nbJoueurs,int tour,int aVOIR)
{
	int gagnant;

	if(aVOIR==1)
		gagnant=plus_petite_main(mains,nbJoueurs);
	else
		gagnant=trouve_joueur_precedent(nbJoueurs,tour);

	return gagnant;
}


bool main_finie(LISTE **mains, int nbJoueurs, int tour)
{
	int joueurPrecedent=trouve_joueur_precedent(nbJoueurs,tour);

	if(nb_elements_liste(mains[joueurPrecedent])==0) //si le joueur a une main de 0 éléments
		return true;		
	return false;
}


bool pioche_finie(int niveauPioche)
{
	if(niveauPioche==NOMBRE_TUILES-1)
		return true;
	return false;
}



bool est_partie_finie(LISTE **mains, int niveauPioche, int nbJoueurs, int tour)
{
	if(pioche_finie(niveauPioche)|| main_finie(mains,nbJoueurs,tour)) //si la pioche est finie ou si la main du joueur a été finie
		return true;
	return false;
}




