#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

/** 
*** Ce fichier est un test pour voir si c'est possible de jouer un son avec la SDL Mixer 
*** Le son est un domino
*** Il y a une fenetre qui s'affiche juste pour savoir si le programme fonctionne.
**/

int main(int argc, char const *argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	Mix_Chunk *son;
	SDL_Color orange = {255,127,40,255};
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	}
	if(SDL_Init( SDL_INIT_VIDEO ) == 0)
	{
		window = SDL_CreateWindow("TEST",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480,SDL_WINDOW_SHOWN);

		if(window != NULL)
		{
			renderer = SDL_CreateRenderer (window,-1,SDL_RENDERER_ACCELERATED);
			SDL_Point point = {50,75};
			SDL_Rect rect = {point.x,point.y,100,200};
			if( renderer == NULL )
			{
				renderer = SDL_CreateRenderer (window,-1,SDL_RENDERER_SOFTWARE);
				// ELSE ERROR
			}
			SDL_SetRenderDrawColor(renderer,orange.r,orange.g,orange.b,orange.a);
			SDL_RenderDrawRect(renderer,&rect);
			SDL_RenderPresent(renderer);
			Mix_AllocateChannels(10); //Creation de 10 Channels audio. Il faut un pour chaque son.
			Mix_Volume(0, MIX_MAX_VOLUME/2);
			son = Mix_LoadWAV("tuile.wav");// Charge le son
			SDL_Delay(2000); // Attend pendant 2 secondes
    		Mix_PlayChannel(1, son, 0);// Parametre : Channel, son a jouer, boucle infinie = -1 sinon nombre de boucles a faire
    		SDL_Delay(2000);
    		Mix_FreeChunk(son); // Enleve le son de la memoire
    		Mix_CloseAudio();
			SDL_DestroyRenderer(renderer);
			renderer = NULL;
			SDL_DestroyWindow(window);
			renderer = NULL;
			SDL_Quit();
		}
		else
		{
			printf("Echec Window %s \n",SDL_GetError() );
		}
		// SDL_CreateWindowAndRenderer(640,480,SDL_WINDOW_BORDERLESS,window,renderer);

	}
	else
	{
		SDL_Log("erreur : %s\n",SDL_GetError() );
	}

	return 0;
}