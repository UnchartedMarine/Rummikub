//TODO
//Trouver un moyen de include la SDL a chaque fois
#if defined __has_include_
#   if __has_include (<SDL2/SDL.h>)
#       include<SDL2/SDL.h>
#   else
#       include<SDL.h>
#   endif
#   if __has_include (<SDL2/SDL_image.h>)
#       include<SDL2/SDL_image.h>
#   else
#       include<SDL_image.h>
#   endif
#   if __has_include (<SDL2/SDL_ttf.h>)
#       include<SDL2/SDL_ttf.h>
#   else
#       include<SDL_ttf.h>
#   endif
#endif