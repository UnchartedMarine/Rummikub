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






