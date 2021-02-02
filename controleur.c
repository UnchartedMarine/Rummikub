/******************************************
 * Controleur
 ******************************************/
#include "modele.h" 


int main()
{   
	srand(time(NULL));

	int niveauPioche = 0;
	int tour;
	int typeFinDePartie;
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

		printf("NIVEAU DE LA PIOCHE:%d/106\n\n",niveauPioche+1);

		printf("PLATEAU:\n");
		lit_plateau();

		printf("MAIN DU JOUEUR:\n");
		lit_liste(joueurs[tour].main);
	
		joue_tour(&(joueurs[tour]),&niveauPioche);
		tour=(tour+1)%nbJoueurs;

	}


	if(pioche_finie(niveauPioche))
		typeFinDePartie=1;
	else
		typeFinDePartie=2;

	gagnant=detecte_gagnant(joueurs,nbJoueurs,tour,typeFinDePartie);

	for(i=0;i<nbJoueurs;i++)
	{
		if(i==gagnant)
			joueurs[i].points=calcule_points_gagnant(joueurs,nbJoueurs,gagnant,typeFinDePartie);
		else
			joueurs[i].points=calcule_points_perdant(joueurs,gagnant,i,typeFinDePartie);
		rentrer_nom_score(joueurs[i].points,joueurs[i].pseudo);
	}

	printf("gagnant:%d ; 0:%d ; 1:%d ; 2:%d\n",gagnant,joueurs[0].points,joueurs[1].points,joueurs[2].points);
	


	return 0;
}


