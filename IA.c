#include "modele.c"

//quelque fonction necessaire
void tab_liste(TUILE *t,int n,LISTE * F)
{
	int i;
	for(i=0;i<n;i++)
	{
		if(t[i].num != -1)
		{
			ajoute_liste(F,t[i]);
		}
	}
}

void affiche_tab(TUILE *t,int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		printf("%d;%d\n",t[i].num,t[i].coul);
	}
}

void initialise_tab(TUILE *t,int n)
{
	int i;
	TUILE u;
	u.num=-1;u.coul=-1;
	for(i=0;i<n;i++)
	{
		t[i]=u;
	}
}

void init_tab(TUILE *t,int n,LISTE *l)
{
	int i;
	for(i=0;i<n;i++)
	{
		t[i]=renvoie_tuile_via_position(l,i+1);
	}
}




void tri_par_couleur(TUILE *t, int n)
{
	int i,j;
	TUILE aux;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-i;j++)
		{
			if((t[j].coul)>(t[j+1].coul))
			{
				aux= t[j];
				t[j]=t[j+1];
				t[j+1]=aux;
			}
		}
	}
}

// tri croissant par couleur 
void tri_couleur_croissant(TUILE *t, int n)
{
	tri_par_couleur(t, n);
	int i,j;
	TUILE aux;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-i;j++)
		{
			if(t[j].coul==t[j+1].coul)
			{
				if(t[j+1].num < t[j].num)
				{
					aux= t[j];
					t[j]=t[j+1];
					t[j+1]=aux;
				}
			}
		}
	}
}

void suite_valide(TUILE *t, int n, PLATEAU *p) //on stock les suites dans le plateeau  et on supprime les tuile utilisé
{
	int i,j;
	LISTE *aux=cree_liste();
	TUILE u;
	TUILE pos[n];
	TUILE erreur[n];
	u.num=-1;u.coul=-1;
	initialise_tab(erreur,n);
	initialise_tab(pos,n);
	for(i=0;i<n;i++)
	{
		if(t[i].num>0)
		{
			if(i==n-1)
			{
				if(((t[i].num) == (t[i-1].num+1))&&(((t[i+1].num) == (t[i].num+1)))&&(t[i].coul==t[i-1].coul)&&(t[i].coul==t[i+1].coul))
				{
					ajoute_liste(aux,t[i]);
					pos[i]=t[i];
					ajoute_liste(aux,t[i+1]);
					pos[i+1]=t[i+1];
				}
				if(nb_elements_liste(aux)>= 3)
				{
					ajoute_plateau(aux,p);
					for(j=0;j<n;j++)
					{
						if(erreur[j].num ==-1)
						{
							if(pos[j].num !=-1)
							{
								erreur[j]=pos[j];
							}
						}
					}
					aux= cree_liste();
					initialise_tab(pos,n);
				}
				else
				{
					aux= cree_liste();	
					initialise_tab(pos,n);					
				}
			}
			else
			{
				if(nb_elements_liste(aux)==0)
				{
				  ajoute_liste(aux,t[i]);
				  pos[i]=t[i];
				  
				}
				else 
				{
					if((((t[i].num) != (t[i-1].num+1))&&((t[i].num) != (t[i-1].num)))|| ((t[i].coul) != (t[i-1].coul)))
					{	
						if(nb_elements_liste(aux)>=3)
						{
						  ajoute_plateau(aux,p);
						  for(j=0;j<n;j++)
							{
								if(erreur[j].num ==-1)
								{
									if(pos[j].num !=-1)
									{
										erreur[j]=pos[j];
									}
								}
							}
							aux= cree_liste();
							initialise_tab(pos,n);
						}
						else
						{
							aux= cree_liste();
							initialise_tab(pos,n);
						}
					}
					else
					{
						if((t[i].num) == (t[i-1].num))
						{
							if(nb_elements_liste(aux)==0)
							{
								aux=cree_liste();
								initialise_tab(pos,n);
								ajoute_liste(aux,t[i]);
								
							}
						}
						else
						{
							ajoute_liste(aux,t[i]);
							pos[i]=t[i];
						}
						
					}
					
				}
			}
		}
    }
    for(i=0;i<=n;i++)
	{
		if (((erreur[i]).num != -1)&&((erreur[i]).coul !=-1))
		{
			t[i]=u;
		}
	}
}


//tri par triplé par couleur


void tri_triple_croissant(TUILE *t, int n)
{
	tri_par_couleur(t,n);
	int i,j;
	TUILE aux;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-i;j++)
		{
			if(t[j+1].num < t[j].num)
			{
				aux= t[j];
				t[j]=t[j+1];
				t[j+1]=aux;
			}
		
		}
	}
}

void triple_valide(TUILE *t,int n,PLATEAU *p)  //on stock les triplet ou quadruplet dans le plateau p et on supprime les tuile utilisé
{
	int i,j ;
	LISTE *aux=cree_liste(); 
	TUILE u;
	TUILE pos[n];
	TUILE erreur[n];
	u.num=-1;u.coul=-1;
	initialise_tab(erreur,n);
	initialise_tab(pos,n);
	
	for(i=0;i<n;i++)
	{
		if(t[i].num>0)
		{
			if(i==n-1)
			{
				if(((t[i].num) == (t[i-1].num))&&(((t[i+1].num) == (t[i].num)))&&(t[i].coul !=t[i-1].coul)&&(t[i].coul!=t[i+1].coul))
				{
					ajoute_liste(aux,t[i]);
					pos[i]=t[i];
					ajoute_liste(aux,t[i+1]);
					pos[i+1]=t[i+1];
				}
				if(nb_elements_liste(aux)>= 3)
				{
					ajoute_plateau(aux,p);
					for(j=0;j<n;j++)
					{
						if(erreur[j].num ==-1)
						{
							if(pos[j].num !=-1)
							{
								erreur[j]=pos[j];
							}
						}
					}
					aux= cree_liste();
					initialise_tab(pos,n);
				}
				else
				{
					aux= cree_liste();	
					initialise_tab(pos,n);					
				}
			}
			else
			{
				if(nb_elements_liste(aux)==0)
				{
					ajoute_liste(aux,t[i]);
					pos[i]=t[i];
				}
				else
				{
					if(t[i].num != t[i-1].num)
					{
						if(nb_elements_liste(aux)>= 3)
						{
							ajoute_plateau(aux,p);
							for(j=0;j<n;j++)
							{
								if(erreur[j].num ==-1)
								{
									if(pos[j].num !=-1)
									{
										erreur[j]=pos[j];
									}
								}
							}
							aux= cree_liste();
							initialise_tab(pos,n);
						}
						else
						{
							aux= cree_liste();
							initialise_tab(pos,n);
						}
					}
					else
					{
						if(t[i].coul != t[i-1].coul)
						{
							ajoute_liste(aux,t[i]);
							pos[i]=t[i];
						}
					}
				}

			}
		}		
	}
	for(i=0;i<=n;i++)
	{
		if (((erreur[i]).num != -1)&&((erreur[i]).coul !=-1))
		{
			t[i]=u;
		}
	}
}


//quelque algo pour le choix de la combinaison posé

LISTE * renvoie_element_plateau(PLATEAU *p, int pos)
{
	ELEMENT_PLATEAU *element=p->premier;
 	LISTE *erreur=cree_liste();

	while(element != NULL)
	{
		if(pos==1)
    		{
      			return element->liste;
    		}
    		element=element->suivant;
    		pos= pos-1;
	}
  	return erreur;
}

void remplissage_methode1(TUILE *t,int n, PLATEAU  *coup_valide)
{
	tri_couleur_croissant(t,n);
	suite_valide(t,n,coup_valide);
	tri_triple_croissant(t,n);
	triple_valide(t,n,coup_valide);
	
}

void remplissage_methode2(TUILE *t, int n, PLATEAU  *coup_valide)
{
	tri_triple_croissant(t,n);
	triple_valide(t,n,coup_valide);
	affiche_tab(t,n);
	printf("pute1\n");
	lit_plateau(coup_valide);
	printf("salope1\n");
	tri_couleur_croissant(t,n);
	suite_valide(t,n,coup_valide);
	affiche_tab(t,n);
	printf("pute2\n");
	lit_plateau(coup_valide);
	printf("salope2\n");
}

int decompte_point(PLATEAU *p)
{
	int n=nb_elements_plateau(p);
	int s=0;
	int i,j;
	LISTE *l;
	TUILE y;

	for(i=1;i<=n;i++)
	{
		l=renvoie_element_plateau(p, i);
		int m=nb_elements_liste(l);
		for(j=1;j<=m;j++)
		{
			y=renvoie_tuile_via_position(l,j);
			s=s+y.num;
		}
	}
	return s;
}

int choix_de_depot(TUILE *t1, TUILE *t2,TUILE *final,PLATEAU *coup_valide1, PLATEAU *coup_valide2, PLATEAU *coup_final)
{
	int x=decompte_point(coup_valide1);
	int y=decompte_point(coup_valide2);
	if(x>y)
	{
		coup_final=coup_valide1;
		final=t1;
		return x;
	}
	else
	{
		coup_final=coup_valide2;
		final=t2;
		return y;
	}
}

//pour poser les joker sur le plateau 
void poser_joker(TUILE *final,int n,PLATEAU *plateau)
{
	int i,j,k;
	LISTE *l=cree_liste();
	int m=nb_elements_plateau(plateau);
	for(i=1;i<=m;i++)
	{
		l=renvoie_element_plateau(plateau,i);
		k=nb_elements_liste(l);
		for(j=0;j<n;j++)
		{
			if(final[j].num==0)
			{
				if(renvoie_tuile_via_position(l,1).num < renvoie_tuile_via_position(l,k).num)
				{
					if ((renvoie_tuile_via_position(l,1).num >1)&& (renvoie_tuile_via_position(l,1).num !=0))
					{
						ajoute_liste(l,final[j]);
						final[j].num=-1;final[j].coul=-1;
					}
					else
					{
						if ((renvoie_tuile_via_position(l,k).num <13)&& (renvoie_tuile_via_position(l,1).num !=0))
						{
							placement_element_liste(l, final[j], k+1);
							final[j].num=-1;final[j].coul=-1;
						}
					}
				}
				else 
				{
					if(renvoie_tuile_via_position(l,1).num < renvoie_tuile_via_position(l,k).num)
					{
						if ((renvoie_tuile_via_position(l,1).num <13)&& (renvoie_tuile_via_position(l,1).num !=0))
						{
							ajoute_liste(l,final[j]);
							final[j].num=-1;final[j].coul=-1;
						}
						else
						{
							if ((renvoie_tuile_via_position(l,k).num >1)&& (renvoie_tuile_via_position(l,1).num !=0))
							{
								placement_element_liste(l, final[j], k+1);
								final[j].num=-1;final[j].coul=-1;
							}
						}
					}
					else 
					{
						if(k<4)
						{
							ajoute_liste(l,final[j]);
							final[j].num=-1;final[j].coul=-1;
						}
					}
				}
			}
		}
	}
}

// L'ia le bloc
void pose_ia(JOUEUR *j, PLATEAU *plateau,int *niveauPioche)
{
	int score;
	int i=1;
	LISTE *L=j->main;
	LISTE *F=cree_liste();
	int n=nb_elements_liste(L);
	PLATEAU *coup_final=cree_plateau();
	PLATEAU *coup_valide1=cree_plateau();
	PLATEAU *coup_valide2=cree_plateau();
	TUILE t1[n];
	TUILE t2[n];
	TUILE final[n];
	
	
	init_tab(t1, n, L);
	init_tab(t2, n, L);
	initialise_tab(final,n);
	remplissage_methode1(t1, n,coup_valide1);
	remplissage_methode2(t2, n,coup_valide2);
	score= choix_de_depot(t1, t2, final, coup_valide1, coup_valide2, coup_final);
	if((j->premierCoup) || (score>=30))
	{
		int n=nb_elements_plateau(coup_final);
		for(i=1;i<=n;i++)
		{
			LISTE *l=renvoie_element_plateau(coup_final,i);
			ajoute_plateau(l,plateau);
			if(j->premierCoup==false)
			{
				j->premierCoup=true;
			}
		}
		poser_joker(final,n,plateau);
		tab_liste(final,n,F);
		j->main=F;
	}
	else 
	{
		 pioche_tuile(L,niveauPioche);
	}
	
	

}
