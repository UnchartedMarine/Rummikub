/******************************************
 * Controleur
 ******************************************/
#include "header.h" 


int main()
{   
	srand(time(NULL));

	int niveauPioche = 0;
	int nbJoueurs = 3;
	int i;

	init_pioche();
	
	for(i=0;i<nbJoueurs;i++)
	{
		LISTE *liste=cree_liste();
		init_main(liste,&niveauPioche);
		lit_liste(liste);
		printf("%d\n\n",niveauPioche);
	}

	return 0;
}


