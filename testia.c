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
