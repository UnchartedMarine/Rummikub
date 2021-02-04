
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
	int tourPasValide=1;//ajout

	int modeJeu = demande_mode_jeu();
	int nbJoueurs = demande_nb_joueurs();
	JOUEUR joueurs[nbJoueurs];
	int gagnant;
	int typeTour;
	int tourMultiTemps=0;
	int estPremierTour;

	plateau=cree_plateau();
	init_pioche();
	init_joueurs(joueurs,modeJeu,nbJoueurs,&niveauPioche);


	tour=regarde_qui_commence(nbJoueurs,joueurs);
	printf("%s COMMENCE\n\n",joueurs[tour].pseudo);


	while(!est_partie_finie(joueurs,niveauPioche,nbJoueurs,tour))
	{

		printf("---------Tour du joueur %s---------\n",joueurs[tour].pseudo);

		printf("IL RESTE %d TUILE(S) DANS LA PIOCHE\n\n",NOMBRE_TUILES-niveauPioche);

		printf("PLATEAU:\n");
		lit_plateau(plateau);

		printf("MAIN DU JOUEUR:\n");
		lit_liste(joueurs[tour].main);

		copiePlateau=copie_plateau(plateau);
		copieMain = copie_liste(joueurs[tour].main);
	
		estPremierTour=0;

		while(tourPasValide){
	
			if(nb_elements_plateau(copiePlateau)-1 != 0){
				printf("---------Tour du joueur %s---------\n",joueurs[tour].pseudo);

				printf("PLATEAU:\n");
				lit_plateau(copiePlateau);

				printf("MAIN DU JOUEUR:\n");
				lit_liste(copieMain);
			}

			typeTour=joue_tour(&(joueurs[tour]),&niveauPioche,tourMultiTemps);
			if(typeTour!=0){
				if(typeTour == 2){
					tourPasValide=0;
				}
				else{
					printf("VOULEZ-VOUS EFFECTUER UNE AUTRE ACTION ?\n");
					printf("1 - OUI\n0 - NON, J'AI TERMINÉ\nSaisie:");
					scanf("%d",&tourPasValide);
					if(tourPasValide==1){
						tourMultiTemps=1;
						if(typeTour==3){
							estPremierTour=1;
						}
					}
					
				}
			}
			else{
				printf("L'ACTION EFFECTUEE EST IMPOSSIBLE\n");
				printf("VOUS DEVEZ RECOMMENCER L'ENTIERETE DE VOTRE TOUR\n");
				copiePlateau=copie_plateau(plateau);
				copieMain = copie_liste(joueurs[tour].main);
				tourPasValide=1;
				tourMultiTemps=0;
				if(estPremierTour==1){
					joueurs[i].premierCoup=false;
				}
			}


			//if((verif_plateau_bon(plateau_copie)!=1) || (tourPasValide==1)){tourPasValide=1}else{plateau=copie_plateau(copiePlateau);joueurs[tour].main=copie_liste(copieMain);}


			//if((tourPasValide == 0)){
				//if(plateau_valide(plateauCopie)==1) {plateau=copie_plateau(copiePlateau);joueurs[tour].main=copie_liste(copieMain);}
				//else{copiePlateau=copie_plateau(plateau);copieMain = copie_liste(joueurs[tour].main);tourMultiTemps=0;if(estPremierTour==1){joueur->premierCoup=false;estPremierTour=0;}}



		}




		tour=(tour+1)%nbJoueurs;
		tourPasValide=1;
		tourMultiTemps=0;

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



	//poser combi
		//poser suite
		//poser mm nb


	//completer une combi
		//juste ajouter
		//recuperer tuile à l'autre bout si pas de joker dans combi



	//recup tuile pour la reposer si combi > 4 elems
		//reposer sur suite existante
		//fairte nouvelle suite avec main
	//diviser suite et ajouter tuile
		//si joker annuler
	//prendre un joker d'une liste et le remplacer
	//changer le num des case

//a la fin d'une action réussie demander si veux jouer autre chose, du coup le while va reboucler 

	//multi prise: verifier chaque coup s'il peut le faire sans verifier les suites posées qui seront verifiees a la toute fin

	//verifier suites qu'apres le while dans une boucle qui parcourt le plateau

	//remplacer les ajoute plateau en donnant en parametre la copie du plateau

	//regler probleme du choix du num inversé par rapport a l'affichage (notamment dans remplacement joker ou alors dans ajout combinaison ...)

	
	//spécifier a l'utilisateur de créer sa suite de nombre dans l'ordre decroissant
	
	//à chaque fois qu'il choisit dans sa main je dois regler la position par rapport a l'affichage

	//enlever possibilité de créer une suite de moins de 2 tuiles
