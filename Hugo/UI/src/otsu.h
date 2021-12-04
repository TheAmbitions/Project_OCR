#ifndef OTSU_H
#define OTSU_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


void noiseReduction(SDL_Surface *img);
void ot(SDL_Surface* img);
SDL_Surface* apply_otsu(SDL_Surface* img);

#endif
