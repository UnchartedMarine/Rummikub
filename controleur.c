/******************************************
 * Controleur
 ******************************************/
#include "header.h" 


int main()
{   
	srand(time(NULL));

	int niveauPioche = 0;
	int tour;
	
	int nbJoueurs = 3;
	LISTE *mains[nbJoueurs];
	/**** note: je pense que ça serait bien de faire type enuméré pour associer les n° des joueurs à leur pseudo***/
	int points[nbJoueurs];
	int gagnant;

	int i;


	init_pioche();
	
	for(i=0;i<nbJoueurs;i++)
	{
		mains[i]=cree_liste();
		init_main(mains[i],&niveauPioche);
		lit_liste(mains[i]);
		printf("%d\n\n",niveauPioche);
		
	}

	tour=regarde_qui_commence(nbJoueurs);

	niveauPioche=100;    ////////// PROVISOIRE -> POUR TEST

	while(!est_partie_finie(mains,niveauPioche,nbJoueurs,tour))
	{
		tour=(tour+1)%nbJoueurs;
		niveauPioche++;
	}
	
	printf("nivPioche:%d ; tour:%d\n",niveauPioche,tour);


	int aVOIR; /*******    faut voir si on fait comme ça         ******/
	if(pioche_finie(niveauPioche))
		aVOIR=1;
	else
		aVOIR=2;
	
	gagnant=detecte_gagnant(mains,nbJoueurs,tour,aVOIR);

	for(i=0;i<nbJoueurs;i++)
	{
		if(i==gagnant)
			points[i]=calcule_points_gagnant(mains,nbJoueurs,gagnant,aVOIR);
		else
			points[i]=calcule_points_perdant(mains,gagnant,i,aVOIR);
	}

	printf("gagnant:%d ; 0:%d ; 1:%d ; 2:%d\n",gagnant,points[0],points[1],points[2]);

	return 0;
}


