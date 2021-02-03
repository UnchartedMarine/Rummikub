#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

LISTE sparation(LISTE L)
{
  LISTE l1=cree_liste();
  LISTE l2=cree_liste();
  LISTE l3=cree_liste();
  LISTE l4=cree_liste();
  int n= nb_elements_liste(L);
  int i;
  TUILE t;
  for(0;n-1;i++)
  {
    t= renvoie_tuile_via_position(L,i);
    if(t.coul==0)
    {
      ajoute_liste(l1,t);
    }
        if(t.coul==1)
    {
      ajoute_liste(l2,t);
    }
        if(t.coul==2)
    {
      ajoute_liste(l3,t);
    }
        if(t.coul==3)
    {
      ajoute_liste(l4,t);
    }
  }
  return l0 l1 l2 l3;
}
