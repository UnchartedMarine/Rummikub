
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
	int modeJeu = demande_mode_jeu();
	int nbJoueurs = demande_nb_joueurs();
	JOUEUR joueurs[nbJoueurs];
	int gagnant;

	//Entier qui permet de savoir si le tour est fini ou si le joueur doit recommencer ou continuerà jouer
	int tourPasValide;

	//Entier qui permet de savoir quel coup le joueur à jouer à tel instant
	int typeTour;

	//Si tourMultiTemps=1 alors le joueur se trouve actuellement au minimum au deuxieme coup dans son tour sinon(0) c'est son premier coup dans le tour
	int tourMultiTemps;

	//Si estPremierTour=1 alors le joueur est dans un tour dans lequel son premier coup durant ce tour, il devait poser une combinaison de plus de 30 points sinon (0)
	int estPremierTour;

	//Si refaitTour=1, alors le joueur à décidé de finir son tour mais ce qu'il y a posé sur le plateau est faux alors il doit recommencer son tour entièrement.
	int refaitTour;

	//création d'une liste vide dans la variable qui contiendra les tuiles que le joueur est obligé de jouer dans son tour
	listeTuilesRecup=cree_liste();

	//création d'une liste vide dans la variable qui contiendra la copie de la main du joueur
	copieMain=cree_liste();

	//création d'un plateau vide dans la variable qui contiendra la copie du plateau de jeu
	copiePlateau=cree_plateau();

	//création d'un plateau vide dans la variable qui contiendra le plateau de jeu
	plateau=cree_plateau();
	init_pioche();
	init_joueurs(joueurs,modeJeu,nbJoueurs,&niveauPioche);


	tour=regarde_qui_commence(nbJoueurs,joueurs);
	printf("%s COMMENCE\n\n",joueurs[tour].pseudo);

	//Tant que la partie n'est pas terminée 
	while(!est_partie_finie(joueurs,niveauPioche,nbJoueurs,tour))
	{
	
		//affichage des informations et instructions du tour d'un joueur dans la console	
		printf("\n\n---------Tour du joueur %s---------\n",joueurs[tour].pseudo);


		if(modeJeu==2 && tour>0)
			pose_ia(&(joueurs[tour]),plateau,&niveauPioche);

		else
		{
			printf("IL RESTE %d TUILE(S) DANS LA PIOCHE\n\n",NOMBRE_TUILES-niveauPioche);
			printf("PLATEAU:\n");
			lit_plateau(plateau);
			printf("MAIN DU JOUEUR:\n");
			lit_liste(joueurs[tour].main);

			//copiePlateau copie le plateau tel qu'il est avant toutes modifications dans le tour du joueur
			copiePlateau=copie_plateau(plateau);

			//copieMain copie la main telle qu'elle est avant toutes modifications dans le tour du joueur
			copieMain = copie_liste(joueurs[tour].main);
	
			//initialisation des cas spéciaux dans un tour à 0
			estPremierTour=0;
			tourMultiTemps=0;
			tourPasValide=1;
			refaitTour=0;

			//tant que le tour du joueur lorsqu'il a décidé de terminer n'est pas valide, il recommence tout son tour
			while(tourPasValide){
	
				//Si c'est un tour avec plusieur coup joués alors on réaffiche l'état actuel du jeu
				if((tourMultiTemps!= 0) || (refaitTour==1)){
					printf("---------Continuez votre tour %s---------\n",joueurs[tour].pseudo);
					printf("PLATEAU:\n");
					lit_plateau(copiePlateau);
					printf("MAIN DU JOUEUR:\n");
					lit_liste(copieMain);
					printf("TUILES A JOUER OBLIGATOIREMENT CE TOUR:\n");
					lit_liste(listeTuilesRecup);
				}

				//typeTour récupère un entier qui permet de savoir comment traiter le coup joué par le joueur
				typeTour=joue_tour(&(joueurs[tour]),&niveauPioche,tourMultiTemps);

				//Si typeTour !=0, alors le coup joué par le joueur est valide (cela ne veut pas dire que les combinaison jouées sur la copie du plateau sont bonnes)
				if(typeTour!=0){
					//Si le joueur a pioché
					if(typeTour == 2){
						tourPasValide=0;//le joueur ne peut pas faire d'autres actions dans le tour
					}
					else{
						printf("\n\nVOULEZ-VOUS EFFECTUER UNE AUTRE ACTION ?\n");
						printf("1 - OUI\n0 - NON, J'AI TERMINÉ\nSaisie:");
						scanf("%d",&tourPasValide);
						//Si le joueur décide de faire un autre coup dans son tour
						if(tourPasValide==1){
							tourMultiTemps=1;//tour multi Temps
							//Si le coup joué par le joueur était sa première pose d'une combinaison de plus de 30 points 
							if(typeTour==3){
								estPremierTour=1;
							}
						}
					
					}
				}
				else{//Sinon le coup jouer par le joueur n'est pas valide
					printf("\n\nL'ACTION EFFECTUEE EST IMPOSSIBLE\n");
					printf("VOUS DEVEZ RECOMMENCER L'ENTIERETE DE VOTRE TOUR\n");

					//Le joueur doit recommencer tout son tour donc on réaffecte à la copie du plateau et de la main les éléments présent au début du tour du joueur
					copiePlateau=copie_plateau(plateau);
					copieMain = copie_liste(joueurs[tour].main);
					//pas de verification du plateau car le coup est pas bon
					tourPasValide=1;
					tourMultiTemps=0;
					//s'il y a eu une erreur dans un coup suivant sa premiere pose de plus de 30 points qui lui debloque le reste des autres coups jouables
					if(estPremierTour==1){
						joueurs[tour].premierCoup=false;//il doit recommencer tout son tour et reposer sa combinaison de plus de 30 points
						estPremierTour=0;
					}
				}

				//Si le joueur a décidé de terminer son tour
				if(tourPasValide == 0){
					//si le joueur a joué toutes les tuiles qui étaient à jouer obligatoirement durant ce tour
					if(nb_elements_liste(listeTuilesRecup)<1){
						//Si la vérification de chaque combinaison présente sur la copie du plateau de jeu retourne true (donc chaque combinaison est bonne)
						if(verif_plateau(copiePlateau)==true){
							printf("\nFIN DE VOTRE TOUR");
							//alors le plateau de jeu recoit la copie de la copie du plateau de jeu du début du tour sur laquelle le joueur à effectué des changements par ses coups
							plateau=copie_plateau(copiePlateau);

							//Si durant le tour le joueur n'a pas pioché
							if(typeTour!=2){
								joueurs[tour].main=copie_liste(copieMain); //on affecte à la main du joueur la copie de la copie de la main du joueur sur laquelle les coups joués l'ont impactés
							}
						}
						//Sinon les combinaisons présentes sur la copie du plateau quand le joueur décide de trerminer sdon tour sont fausses
						else{
							printf("\nUNE OU PLUSIEURS COMBINAISONS PRESENTES SUR LE PLATEAU NE SONT PAS VALIDES");
							printf("VOUS DEVEZ RECOMMENCER L'ENTIERETE DE VOTRE TOUR");
							//On lui redonne sa main et le plateau,tels qu'ils étaient au début de son tour dans une copie sur laquelle il va refaire ses coups
							copiePlateau=copie_plateau(plateau);
							copieMain = copie_liste(joueurs[tour].main);
							tourMultiTemps=0;
							tourPasValide=1;
							refaitTour=1;
							//s'il y a eu une erreur sur le plateau suite à un coup suivant sa premiere pose de plus de 30 points qui lui debloque le reste des autres coups jouables
							if(estPremierTour==1){
								joueurs[tour].premierCoup=false;//il doit recommencer tout son tour et reposer sa combinaison de plus de 30 points
								estPremierTour=0;
							}
						}
					}
					//Sinon il n'a pas joué toutes les tuiles obligatoires à jouer durant ce tour et il va devoir recommencer l'entièreté de ce tour
					else{
						printf("\nUNE OU PLUSIEURS TUILES QUE VOUS DEVIEZ JOUER OBLIGATOIREMENT CE TOUR N'ONT PAS ETE JOUEES");
						printf("VOUS DEVEZ RECOMMENCER L'ENTIERETE DE VOTRE TOUR");
						//On lui redonne sa main et le plateau,tels qu'ils étaient au début de son tour dans une copie sur laquelle il va refaire ses coups
						copiePlateau=copie_plateau(plateau);
						copieMain = copie_liste(joueurs[tour].main);
						tourMultiTemps=0;
						tourPasValide=1;
						refaitTour=1;
						//s'il y a eu une erreur sur le plateau suite à un coup suivant sa premiere pose de plus de 30 points qui lui debloque le reste des autres coups jouables
						if(estPremierTour==1){
							joueurs[tour].premierCoup=false;//il doit recommencer tout son tour et reposer sa combinaison de plus de 30 points
							estPremierTour=0;
						}
					}
				}



			}

			//on remet les variables de traitements telle qu'un joueur commence son tour
			tourPasValide=1;
			tourMultiTemps=0;
			estPremierTour=0;
			refaitTour=0;
		}
	
	tour=(tour+1)%nbJoueurs; //on passe au tour du joueur suivant
	
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


	printf("\nGAGNANT:%s\n",joueurs[gagnant].pseudo);
 	
	for(i=0;i<nbJoueurs;i++)
		printf("%s:%d points\n",joueurs[i].pseudo,joueurs[i].points);
	

	return 0;
}
	
