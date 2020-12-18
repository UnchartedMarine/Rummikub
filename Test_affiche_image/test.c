//gcc -Wall test.c -o bol -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer $(sdl2-config --cflags --libs)

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<string.h>
#include<stdio.h>

#define V

#ifndef M
#define M extern
#endif
#ifndef V
#define V extern
#endif
#ifndef C
#define C extern
#endif

typedef struct {
		SDL_Texture* texture;
		SDL_Rect rect;
	}Texture;

V SDL_Window* gWindow;
V SDL_Renderer* gRenderer;

SDL_Texture* createTextureFromImage (char* path) {

	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(path);
	if (surface == NULL) {
		printf("error load image: %s", IMG_GetError());
		return texture;
	}
	texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);
	return texture;

}

Texture createTextureFromText (TTF_Font* font, char* str, SDL_Color textColor) {

	Texture textTexture;
	SDL_Surface *surface = TTF_RenderText_Blended(font, str, textColor);
	if (surface == NULL) {
		printf("error load text: %s", TTF_GetError());
		return textTexture;
	}
	textTexture.texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	textTexture.rect.h = surface->h;
	textTexture.rect.w = surface->w;
	SDL_FreeSurface(surface);
	return textTexture;

}

int main (int argc, char *argv[]) {

	gWindow = NULL;
	gRenderer = NULL;
	if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER)) {
		printf("%s", SDL_GetError());
		goto Quit;
	}
	gWindow = SDL_CreateWindow("DA TITRE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("%s", SDL_GetError());
		goto Quit;
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	IMG_Init(IMG_INIT_JPG);
	TTF_Init();

	Texture texture;
	texture.texture = createTextureFromImage("BiggieCheese.jpg");
	texture.rect.x = 0;
	texture.rect.y = 0;
	texture.rect.w = 640;
	texture.rect.h = 480;

	SDL_Color color = {0, 0, 0};
	TTF_Font* font = TTF_OpenFont("theboldfont.ttf", 14);
	Texture textureText = createTextureFromText(font, "da text", color);
	textureText.rect.x = 0;
	textureText.rect.y = 0;

	SDL_Event e;int quit = 0;int x,y;
	while (quit != 1) {
		while(SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_MOUSEMOTION:
					SDL_GetMouseState(&x, &y);
					textureText.rect.x = x - textureText.rect.w/2;
					textureText.rect.y = y - textureText.rect.h/2;
					break;
			}
		}
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, texture.texture, NULL, &texture.rect);
		SDL_RenderCopy(gRenderer, textureText.texture, NULL, &textureText.rect);
		SDL_RenderPresent(gRenderer);
	}

	SDL_DestroyTexture(texture.texture);
	SDL_DestroyTexture(textureText.texture);
	texture.texture = NULL;
	TTF_CloseFont(font);
	font = NULL;

Quit:
	if (gRenderer != NULL) {
		SDL_DestroyRenderer(gRenderer);
		gRenderer = NULL;
	}
	if (gWindow != NULL) {
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
	}
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;

}


