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


void lit_pioche(TUILE *pioche)
{
	int i;

	for(i=0;i<NOMBRE_TUILES;i++) 
	{
		printf("Num:%d ; Coul:%d\n",pioche[i].num,pioche[i].coul);
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

	while(tuileMain != NULL)
	{
		printf("%d;%d\n",tuileMain->tuile.num,tuileMain->tuile.coul);	
		tuileMain=tuileMain->suivant;			
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

//Renvoie un booléen pour dire si la suite est valide: false(0) si la suite n'est pas valide, true(1) si elle est valide.
bool est_valide(LISTE *liste){
	//tuileVerif est la tuile courante à vérifier et tuileVerifSuivante la tuile suivante de la tuile courante.
	MAIN *tuileVerif = liste->premier;
	MAIN *tuileVerifSuivante = tuileVerif->suivant;
	int tailleListe = nb_elements_liste(liste);
	int i,j;
	
	//typeListe permet de savoir à partir des deux premières tuiles de la liste fournie si l'on va traiter une suite de numéros ou une suite de tuile de même numéro et de couleurs différentes.
	int typeListe;
	
	//couleursVues est un tableau qui accueille les couleurs des tuiles rencontrées dans la suite de numéros similaires
	int couleursVues[4];
	
	//numRepete permet de savoir dans une suite de mêmes numéros et de couleurs différentes si le numéro de toutes les tuiles est le même.
	int numRepete;

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
	
	//Si typeListe est égal à 1 c'est une suite de tuiles de numéros qui se suivent et de même couleur.
	if (typeListe == 1){
		//Parcours du reste de la suite
		for(i=2;i<tailleListe;i++){
			//Si les deux tuiles courantes ont des numéros qui se suivent et la même couleur.
			if((tuileVerif->tuile.num == tuileVerifSuivante->tuile.num-1) && (tuileVerif->tuile.coul == tuileVerifSuivante->tuile.coul)){
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
	else{
		//Parcours du reste de la suite
		for(i=2;i<tailleListe;i++){
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




