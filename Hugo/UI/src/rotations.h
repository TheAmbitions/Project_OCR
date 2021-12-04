#ifndef ROTATION_H
#define ROTATION_H

#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <math.h>

void apply_rotation(SDL_Surface **img, float angle);
int RotationAuto(SDL_Surface* img,float varMax,int anglevarMax);
SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle);

#endif
