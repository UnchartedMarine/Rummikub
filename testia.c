#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "modele.c"

LISTE * separation1(LISTE *L)
{
  LISTE * l=cree_liste();
  int n= nb_elements_liste(L);
  int i;
  TUILE t;
  for(1;n;i++)
  {
    t= renvoie_tuile_via_position(L,i);
    if(t.coul==1)
    {
      ajoute_liste(l1,t);
    }
  }
  return l ;
}

LISTE * separation2(LISTE *L)
{
  LISTE * l=cree_liste();
  int n= nb_elements_liste(L);
  int i;
  TUILE t;
  for(1;n;i++)
  {
    t= renvoie_tuile_via_position(L,i);
    if(t.coul==2)
    {
      ajoute_liste(l,t);
    }
  }
  return l ;
}
LISTE * separation3(LISTE *L)
{
  LISTE * l=cree_liste();
  int n= nb_elements_liste(L);
  int i;
  TUILE t;
  for(1;n;i++)
  {
    t= renvoie_tuile_via_position(L,i);
    if(t.coul==3)
    {
      ajoute_liste(l,t);
    }
  }
  return l ;
}

LISTE * separation4(LISTE *L)
{
  LISTE * l=cree_liste();
  int n= nb_elements_liste(L);
  int i;
  TUILE t;
  for(1;n;i++)
  {
    t= renvoie_tuile_via_position(L,i);
    if(t.coul==4)
    {
      ajoute_liste(l,t);
    }
  }
  return l ;
}


LISTE * fusion(LISTE *l, LISTE *k)
{
  int i;
  int n= nb_elements_liste(l);
  int m= nb_elements_liste(k);
  LISTE *L=cree_liste();
  int j=0;
  int u=0;
  for(1;n+m;i++)
  {
    if((j<=n) & (k<=m))
    {
      TUILE a=renvoie_tuile_via_position(l,j);
      TUILE b=renvoie_tuile_via_position(k,u);
      if(a.num < b.num)
      {
        ajoute_liste(L,a);
        j=j+1;
      }
      else
      {
        ajoute_liste(L,b);
        u=u+1;
      }
    }
    else
    {
      if(j<=n)
      {
        ajoute_liste(L,a);
        j=j+1;
      }
      else
      {
        ajoute_liste(L,b);
        u=u+1;
      }
    }
  }
  return L;
}

LISTE * scinder_inf(LISTE *l)
{
  LISTE *L=cree_liste();
  int n= nb_elements_liste(l);
  for(1;n/2;i++)
  {
    TUILE x=renvoie_tuile_via_position(l,i);
    ajoute_liste(L,x);
  }
  return L;
}


LISTE * scinder_sup(LISTE *l)
{
  LISTE *L=cree_liste();
  int n= nb_elements_liste(l);
  for((n/2)+1;n;i++)
  {
    TUILE x=renvoie_tuile_via_position(l,i);
    ajoute_liste(L,x);
  }
  return L;
}

LISTE * tri(LISTE *l)
{
  int n= nb_elements_liste(l);
  if(n>1)
  {
    LISTE *a= scinder_inf(l);
    LISTE *b=scinder_sup(l);
    tri(a);
    tri(b);
    l=fusion(a,b);
  }
  renvoyer l;
}

LISTE * tri_couleur(LISTE *L)
{
  int i;
  LISTE *l=cree_liste();
  LISTE *l1=separation1(L);
  LISTE *l2=separation2(L);
  LISTE *l3=separation3(L);
  LISTE *l4=separation4(L);
  LISTE *L1=tri(l1);
  LISTE *L2=tri(l2);
  LISTE *L3=tri(l3);
  LISTE *L4=tri(l4);
  int n=nb_elements_liste(L);
  int n1=nb_elements_liste(L1);
  int n2=nb_elements_liste(L2);
  int n3=nb_elements_liste(L3);
  int n4=nb_elements_liste(L4);
  for(1;n;i++)
  {
    if(i<=n1)
    {
      TUILE x=renvoie_tuile_via_position(l1,i);
      ajoute_liste(l,x);
    }
    else
    {
      if((i>n1)&((i-n1)<n2))
      {
        TUILE x=renvoie_tuile_via_position(l2,i-n1);
        ajoute_liste(l,x);
      }
      else
      {
        if(((i-n1)>n2)&((i-n1-n2)<n3))
        {
          TUILE x=renvoie_tuile_via_position(l3,i-n1-n2);
          ajoute_liste(l,x);
        }
        else
        {
          TUILE x=renvoie_tuile_via_position(l4,i-n1-n2-n3);
          ajoute_liste(l,x);
        }
      }
    }
  }
  return l;
}


void suite_possible(LISTE *L, PLATEAU coup_valide1)
{
  int i;
  int n = nb_elements_liste(L);
  LISTE *aux=cree_liste();
  for(1;n;i++)
  {
    if(nb-elements_liste(aux)==0)
    {
      TUILE x=renvoie_tuile_via_position(L,i);
      ajoute_liste(aux,x);
    }
    else 
    {
      if(((renvoie_tuile_via_position(L,i).num) != (renvoie_tuile_via_position(L,i-1).num+1))|| (i=n) || ((renvoie_tuile_via_position(L,i).coul) != (renvoie_tuile_via_position(L,i-1).coul)))
      {
        if(nb_elements_liste(aux)>= 3)
        {
          ajoute_plateau(aux, coup_valide1);
          aux= cree_liste();
        }
        else
        {
          aux= cree_liste();
        }
      }
      
      else
      {
        TUILE x=renvoie_tuile_via_position(L,i);
        ajoute_liste(aux,x);
      }
    }
  }
}

LISTE renvoie_element_plateau(PLATEAU p, int pos)
{
	ELEMENT_PLATEAU *element=p->premier;
 	LISTE *erreur=cree_liste;

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

int nb_elements_plateau(PLATEAU p)
{
	ELEMENT_PLATEAU *element=p->premier;
	int nombreElements=0;

	while(element != NULL)
	{
		nombreElements += 1;
		element=element->suivant;
	}
	return nombreElements;
}

LISTE * supprime_tuile_a_jouer(LISTE *L, PLATEAU p)
{
  	int n=nb_elements_liste(L);
	int m=nb_elements_plateau(p);
	int i;
	int j;
	int k;
	for(1;n;i++)
	{
		for(1;m;j++)
		{
			LISTE *l=renvoie_element_plateau(p, j);
			int a= int n=nb_elements_liste(l);
			for(0;a;k++)
			{
				TUILE x=renvoie_tuile_via_position(L,i);
				TUILE y=renvoie_tuile_via_position(l,k);
				if(x==y)
				{
					enleve_element_liste(LISTE * liste,int position);
				}
			}
		}
	}
	return L;
}
  
LISTE * tri_triplet_1(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==1)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_2(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==2)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_3(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==3)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_4(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==4)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_5(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==5)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_6(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==6)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_7(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==7)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_8(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==8)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_9(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==9)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_10(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==10)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_11(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==11)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_12(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==12)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

LISTE * tri_triplet_13(LISTE *L)
{
	LISTE *l=cree_liste();
	int n=nb_elements_liste(L);
	int i;
	for(1;n;i++)
	{
		TUILE t=renvoie_elements_liste(L,i);
		if(t.num==13)
		{
			ajoute_liste(l,x);
		}
	}
	return l;
}

void regroupement(LISTE *l, PLATEAU triplet1)
{
	LISTE *l1=tri_triplet_1(L);
	LISTE *l2=tri_triplet_2(L);
	LISTE *l3=tri_triplet_3(L);
	LISTE *l4=tri_triplet_4(L);
	LISTE *l5=tri_triplet_5(L);
	LISTE *l6=tri_triplet_6(L);
	LISTE *l7=tri_triplet_7(L);
	LISTE *l8=tri_triplet_8(L);
	LISTE *l9=tri_triplet_9(L);
	LISTE *l10=tri_triplet_10(L);
	LISTE *l11=tri_triplet_11(L);
	LISTE *l12=tri_triplet_12(L);
	LISTE *l13=tri_triplet_13(L);
	int i;
	ajoute_plateau(l1,triplet1);
	ajoute_plateau(l2,triplet1);
	ajoute_plateau(l3,triplet1);
	ajoute_plateau(l4,triplet1);
	ajoute_plateau(l5,triplet1);
	ajoute_plateau(l6,triplet1);
	ajoute_plateau(l7,triplet1);
	ajoute_plateau(l8,triplet1);
	ajoute_plateau(l9,triplet1);
	ajoute_plateau(l10,triplet1);
	ajoute_plateau(l11,triplet1);
	ajoute_plateau(l12,triplet1);
	ajoute_plateau(l13,triplet1);
}


void triplet_possible(PLATEAU triplet1, PLATEAU coup_valide1)
{
	int n=nb_element_plateau(triplet1);
	int i,j;
	int aux =0;
	int aux1=-1;
	int aux2=-1;
	int aux3=-1;
	int aux4=-1;
	for(1;n;i++)
	{
		LISTE *l=renvoie_element_plateau(triplet1, i);
		int m=nb_element_liste(l);
		if(m>=3)
		{
			for(1;m;j++)
			{
				TUILE y=renvoie_elements_liste(L,i);
				if((y.coul==1) & (aux1==-1))
				{
					aux1=0;
					aux= aux+1;
				}
				else
				{
					if((y.coul==2) & (aux2==-1))
					{
						aux2=0;
						aux= aux+1;
					}
					else
					{
						if((y.coul==3) & (aux3==-1))
						{
							aux3=0;
							aux= aux+1;
						}
						else
						{
							if((y.coul==4) & (aux4==-1))
							{
								aux4=0;
								aux= aux+1;
							}
						}
					}
				}
			}
		}
		if(aux>=3)
		{
			ajout_plateau(l,coup_valide1);
		}
	}
}
