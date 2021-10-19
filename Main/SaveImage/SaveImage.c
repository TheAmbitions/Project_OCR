#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "SaveImage.h"

int Image_Save_BMP(const char* image_name, const char* path)
{
    SDL_Surface * image = IMG_Load(image_name);
    SDL_SaveBMP(image, path);
    SDL_FreeSurface(image);
 
    return 0;
}

/*int Image_Save_PNG(const char* image_name, const char* path)
{
	SDL_Surface * image = SDL_LoadBMP(image_name);
	IMG_SavePNG(image, path);
	SDL_FreeSurface(image);

	return 0;
}*/
int main()
{
    return Image_Save_BMP("image.png", "testBMP.bmp");
}
