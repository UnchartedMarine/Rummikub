#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "modele.c"

LISTE * separation0(LISTE L)
{
  LISTE * l=cree_liste();
  int n= nb_elements_liste(L);
  int i;
  TUILE t;
  for(1;n;i++)
  {
    t= renvoie_tuile_via_position(L,i);
    if(t.coul==0)
    {
      ajoute_liste(l1,t);
    }
  }
  return l ;
}

LISTE * separation1(LISTE L)
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
      ajoute_liste(l,t);
    }
  }
  return l ;
}
LISTE * separation2(LISTE L)
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

LISTE * separation3(LISTE L)
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
}


LISTE * scinder_sup(LISTE *l)
{
  LISTE *L=cree_liste();
  int n= nb_elements_liste(l);
  for(n/2;n;i++)
  {
    TUILE x=renvoie_tuile_via_position(l,i);
    ajoute_liste(L,x);
  }
}
