/******************************************
 * Modele
 ******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NOMBRE_TUILES 106

typedef struct TUILE 
{	
	int num,coul;
} TUILE;

TUILE pioche[NOMBRE_TUILES];

void init_pioche();
void init_tuiles();
void melange_pioche(TUILE* pioche);
void lit_pioche(TUILE* pioche);


int main()
{   
	srand(time(NULL));

	init_pioche();
	return 0;
}


void init_pioche()
{
	init_tuiles();
	melange_pioche(pioche);
	lit_pioche(pioche);
}


void init_tuiles()
{
	int i;
	TUILE tuile;

	tuile.num=0;
	tuile.coul=0;

	for(i=0;i<NOMBRE_TUILES;i++) 
	{
		pioche[i]=tuile;

		if(i%2==1)
			tuile.num+=1;
		if(i%26==1||i==1)
		{
			tuile.coul+=1;
			tuile.num=1;
		}
	}
}


void melange_pioche(TUILE* pioche) // 100 mélanges ça me paraît pas mal
{
	int i;
	TUILE tmp;
	
	for(i=0;i<100;i++) 
	{
		int position1=rand()%106;
		int position2=rand()%106;
		
		tmp=pioche[position1];
		pioche[position1]=pioche[position2];
		pioche[position2]=tmp;
	}

}



void lit_pioche(TUILE* pioche)
{
	int i;

	for(i=0;i<NOMBRE_TUILES;i++) 
	{
		printf("Num:%d ; Coul:%d\n",pioche[i].num,pioche[i].coul);
	}	
}



















