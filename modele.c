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


int nb_elements_liste(LISTE *liste)
{
	MAIN *tuileMain=liste->premier;
	int nombreElements=0;

	while(tuileMain != NULL)
	{
		nombreElements = nombreElements +1;
		tuileMain=tuileMain->suivant;			
	}
	return nombreElements
}

//On considère que le premier élément d'une liste est l'élément 1
//position ici est la position à laquelle on souhaite placer la tuile dans la suite
void placement_tuile_plateau(LISTE *liste, TUILE tuile, int position)
{
	MAIN *tuileAvant=liste->premier;
	MAIN *nouveau = malloc(sizeof(*nouveau));
	int i;

	nouveau->tuile=tuile;
	
	if(position==1){
		ajoute_liste(liste,tuile);
	}
	else{
		for(i=1;i<position-1;i++){
			tuileAvant=tuileAvant->suivant;
		}

		nouveau->suivant=tuileAvant->suivant;
		tuileAvant->suivant=nouveau;
	}
}

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

	//Remplacer la tuile suivante de la derniere tuile de la suite 1 par la tuile vide (null)
	MAIN *derniereTuile = (MAIN*) malloc(sizeof(MAIN));
	derniereTuile->tuile.num = NULL;
	derniereTuile->tuile.coul = NULL;
	derniereTuile->suivant = NULL;
	tuileAvant->suivant=derniereTuile;

	return liste2;
}




