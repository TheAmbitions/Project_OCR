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
int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3)
    {
	    printf ("Enter the image to save and/or the path with .bmp at the end\n");
	    return -1;
    }

    #define imagename argv[1]
    if (argc != 2)
    {
        #define path argv[2]
	return Image_Save_BMP(imagename, path);    
    }
    
    return Image_Save_BMP(imagename, "result.bmp");
}
