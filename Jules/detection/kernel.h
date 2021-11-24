#ifndef KERNEL_H
#define KERNEL_H

#include <stdio.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"


void apply_kernel(char path[]);

#endif
