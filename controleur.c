/******************************************
 * Controleur
 ******************************************/
#include "modele.h" 


int main()
{   
	srand(time(NULL));

	int niveauPioche = 0;
	int tour;
	int i;

	int nbJoueurs = 3;
	JOUEUR joueurs[nbJoueurs];
	int gagnant;
	
	plateau=cree_plateau();
	init_pioche();
	init_joueurs(joueurs,nbJoueurs,&niveauPioche);


	tour=regarde_qui_commence(nbJoueurs,joueurs);
	printf("%s COMMENCE\n\n",joueurs[tour].pseudo);

	niveauPioche=100;    ////////// PROVISOIRE -> POUR FACILITER LES TESTS

	while(!est_partie_finie(joueurs,niveauPioche,nbJoueurs,tour))
	{
		printf("---------Tour du joueur %s---------\n",joueurs[tour].pseudo);

		printf("NIVEAU DE LA PIOCHE:%d/106\n\n",niveauPioche);

		printf("PLATEAU:\n");
		lit_plateau();

		printf("MAIN DU JOUEUR:\n");
		lit_liste(joueurs[tour].main);
	
		joue_tour(&(joueurs[tour]),&niveauPioche);
		tour=(tour+1)%nbJoueurs;

	}


	int aVOIR;  //////////////////////////faut voir si on fait comme ça 
	if(pioche_finie(niveauPioche))
		aVOIR=1;
	else
		aVOIR=2;


	gagnant=detecte_gagnant(joueurs,nbJoueurs,tour,aVOIR);

	for(i=0;i<nbJoueurs;i++)
	{
		if(i==gagnant)
			joueurs[i].points=calcule_points_gagnant(joueurs,nbJoueurs,gagnant,aVOIR);
		else
			joueurs[i].points=calcule_points_perdant(joueurs,gagnant,i,aVOIR);
	}

	printf("gagnant:%d ; 0:%d ; 1:%d ; 2:%d\n",gagnant,joueurs[0].points,joueurs[1].points,joueurs[2].points);
	

	return 0;
}


