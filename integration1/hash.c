#include <stdlib.h>

// association de la case et du pointer de la suite
//cle = position en y
//Taille map = 4
//incomplet car il faut l'integration avec le modele
typedef struct noeud{
    int cle;
    int val;//liste suite;
    int x;
    noeud *suivant;
}noeud;
typedef struct table_hash{
    int taille;
    struct noeud **liste;
}table_hash;

table_hash *init_table_hash(int taille){
    table_hash *t = (table_hash*)malloc(sizeof(table_hash));
    t->taille = taille;
    t->liste = (noeud**)malloc(sizeof(noeud*)*taille);
    int i;
    for(i=0;i<taille;i++)
        t->liste[i] = NULL;
    return t;
}

void insert(table_hash *t,int cle,int val/*,Pointer vers suite*/){
    noeud *liste = t->liste[cle];
    noeud *newNode = (noeud*)malloc(sizeof(noeud));
    noeud *temp = liste;
    while(temp){
        if(temp->cle==cle){
            temp->val = val;
            return;
        }
        temp = temp->suivant;
    }
    newNode->cle = cle;
    newNode->val = val;
    newNode->suivant = liste;
    t->liste[cle] = newNode;
}
int lookup(table_hash *t,int cle){
    int pos = hashCode(t,cle);
    noeud *liste = t->liste[pos];
    noeud *temp = liste;
    while(temp){
        if(temp->cle==cle){
            return temp->val;
        }
        temp = temp->suivant;
    }
    return -1;
}