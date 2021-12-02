#ifndef GRAYSCALE_H
#define GRAYSCALE_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

void noiseReduction(SDL_Surface *img);
void otsu (SDL_Surface* img);

#endif
