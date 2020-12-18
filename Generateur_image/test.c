#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <string.h>
#define WIDTH 25
#define HEIGHT 50
#define	NBRCOUL 3
#define NBRTUILLE 14

// Les fonctions getpixel et putpixel sont de la documentation
//https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html#AEN90
/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
//Genere les couleurs des differentes équipes et le stocke dans un tableau
void generateur_couleur(SDL_Color couleurs[NBRCOUL])
{
	int i;
	SDL_Color coleur;
	for (i = 0; i < NBRCOUL; i++)
	{
		coleur.r=0;
		coleur.g=0;
		coleur.b=0;
		coleur.a=255;
		if(i == 0)
			coleur.r = 255;
		if(i == 1)
			coleur.g=255;
		if(i == 2)
			coleur.b=255;
		couleurs[i] = coleur;

	}
}

int main (int argc, char *argv[]) {
	int z,k,i,j;
	SDL_Window* gWindow;
	Uint32 pixel,pixeltmp;
	SDL_PixelFormat *format;
	SDL_Surface *surface;
	char * chemin = "Source/";
	char * dest = "Destination/";
	char * extension = ".png";
	char source[16];
	char destination[20];
	char *tuille[NBRTUILLE]={"01","02","03","04","05","06","07","08","09","10","11","12","13","00"};
	char *equipe[NBRCOUL+1] = {"R","V","L","B"};
	SDL_Color couleurs[NBRCOUL];
	generateur_couleur(couleurs);
	if (0 != SDL_Init(SDL_INIT_VIDEO)) {
		printf("%s", SDL_GetError());
		goto Quit;
	}
	gWindow = SDL_CreateWindow("Generateur_Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("%s", SDL_GetError());
		goto Quit;
	}
	IMG_Init(IMG_INIT_PNG);

	for(z = 0;z < NBRCOUL;z++)
	{
		for(k = 0;k < NBRTUILLE;k++)
		{
			source[0]='\0';
			strcat(source, chemin);
			strcat(source, tuille[k]);
			strcat(source, equipe[3]);
			strcat(source, extension);
			printf("%s\n",source);
			surface = IMG_Load(source);
			if (surface != NULL)
			{
				format = surface->format;
				//const char* surfacePixelFormatName = SDL_GetPixelFormatName(format->format);
				//SDL_Log("The surface's pixelformat is %s", surfacePixelFormatName);
				if( SDL_LockSurface(surface) != 0) //Bloque la surface, empeche la lecture.
				{
					printf("%s", SDL_GetError());
					goto Quit;
				}
				Uint8 r,g,b,a;
				for(i = 0; i < HEIGHT;i++)
				{
					for ( j = 0;j < WIDTH;j++)
					{
						pixel = getpixel(surface,j,i);
						SDL_GetRGBA(pixel,format,&r,&g,&b,&a);
						//SDL_Log("X: %d Y: %d R: %d G : %d B: %d A: %d",j,i,r,g,b,a);
						if ( (r == 0) && (g == 0) && (b == 0) )
						{
							pixeltmp = SDL_MapRGBA(format,couleurs[z].r,couleurs[z].g,couleurs[z].b,couleurs[z].a);
							putpixel(surface, j, i,pixeltmp);
						}
						
					}
				}
				destination[0]='\0';
				strcat(destination, dest);
				strcat(destination, tuille[k]);
				strcat(destination, equipe[z]);
				strcat(destination, extension);
				IMG_SavePNG(surface,destination);
				SDL_UnlockSurface(surface);
			}
		}
	}

	
Quit:
	if (gWindow != NULL) {
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
	}
	IMG_Quit();
	SDL_Quit();
	return 0;

}

