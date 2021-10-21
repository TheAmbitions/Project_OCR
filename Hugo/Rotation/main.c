#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <math.h>
#include <err.h>
#include "rotations.h"

int main ()
{
    apply_rotation("my_image.jpg");
    return 0;
}

