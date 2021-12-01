#ifndef ROTATION_H
#define ROTATION_H
    
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle);
int RotationAuto(SDL_Surface* img,float varMax,int anglevarMax);


#endif
