#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

SDL_Surface* canny (SDL_Surface* image_surface)
{


    Uint8 r, g, b;
    Uint8 r1, g1, b1;
    Uint8 r2, g2, b2;
    Uint8 r3, g3, b3;
    Uint8 r4, g4, b4;
    Uint8 r5, g5, b5;
    Uint8 r6, g6, b6;
    Uint8 r7, g7, b7;
    Uint8 r8, g8, b8, color;
    Uint32 pixel,pixel1,pixel2,pixel3,pixel4 ,pixel5,pixel6,pixel7,pixel8;

    Uint32 Blackpixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
    Uint32 Whitepixel = SDL_MapRGB(image_surface->format, 255, 255, 255);

    long width = image_surface -> w;
    long hight = image_surface -> h;

    SDL_Surface* destination = SDL_CreateRGBSurface(SDL_HWSURFACE, width, hight, image_surface->format->BitsPerPixel,
		 image_surface->format->Rmask, image_surface->format->Gmask, image_surface->format->Bmask, image_surface->format->Amask);

    float G[width*hight];
    float T[width*hight]; //theta

    for (long y = 1 ; y < hight - 1; y++)
    {
        for (long x = 1; x < width - 1; x++)
        {
          //    pixel = get_pixel(image_surface, x, y);
                pixel1 = get_pixel(image_surface, x-1, y);
                pixel2 = get_pixel(image_surface, x+1, y);
                pixel3 = get_pixel(image_surface, x, y-1);
                pixel4 = get_pixel(image_surface, x, y+1);
                /*      pixel5 = get_pixel(image_surface, x, y+1);
                pixel6 = get_pixel(image_surface, x-1, y-1);
                pixel7 = get_pixel(image_surface, x-1, y);
                pixel8 = get_pixel(image_surface, x-1, y+1);*/

                //SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
                SDL_GetRGB(pixel1, image_surface -> format, &r1, &g1, &b1);
                SDL_GetRGB(pixel2, image_surface -> format, &r2, &g2, &b2);
                SDL_GetRGB(pixel3, image_surface -> format, &r3, &g3, &b3);
                SDL_GetRGB(pixel4, image_surface -> format, &r4, &g4, &b4);/*
                SDL_GetRGB(pixel5, image_surface -> format, &r5, &g5, &b5);
                SDL_GetRGB(pixel6, image_surface -> format, &r6, &g6, &b6);
                SDL_GetRGB(pixel7, image_surface -> format, &r7, &g7, &b7);
                SDL_GetRGB(pixel8, image_surface -> format, &r8, &g8, &b8);*/

		G[x + y * width] = sqrtf(powf(((float)r2-(float)r1,2)) + powf(((float)r3-(float)r4,2)));
		T[x + y * width] = atanf(((float)r3-(float)r4)/.((float)r2-(float)r1));
         }
    }

    for (long y = 1 ; y < hight - 1; y++)
    {
        for (long x = 1; x < width - 1; x++)
        {
		pixel = get_pixel(image_surface, x, y);
		pixel1 = get_pixel(image_surface, x+1, y);
		pixel2 = get_pixel(image_surface, x+1, y+1);
		pixel3 = get_pixel(image_surface, x+1, y-1);
		pixel4 = get_pixel(image_surface, x, y-1);
		pixel5 = get_pixel(image_surface, x, y+1);
		pixel6 = get_pixel(image_surface, x-1, y-1);
		pixel7 = get_pixel(image_surface, x-1, y);
		pixel8 = get_pixel(image_surface, x-1, y+1);
		
		SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);
		SDL_GetRGB(pixel1, image_surface -> format, &r1, &g1, &b1);
		SDL_GetRGB(pixel2, image_surface -> format, &r2, &g2, &b2);
		SDL_GetRGB(pixel3, image_surface -> format, &r3, &g3, &b3);
		SDL_GetRGB(pixel4, image_surface -> format, &r4, &g4, &b4);
		SDL_GetRGB(pixel5, image_surface -> format, &r5, &g5, &b5);
		SDL_GetRGB(pixel6, image_surface -> format, &r6, &g6, &b6);
		SDL_GetRGB(pixel7, image_surface -> format, &r7, &g7, &b7);
		SDL_GetRGB(pixel8, image_surface -> format, &r8, &g8, &b8);

		color = r * 8 -(r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8);

		if (color < 165 || G[x + y * width] < 130)
		{
			put_pixel(destination, x, y, Blackpixel);
		}
		else
		{
			put_pixel(destination, x, y, Whitepixel);
		}
         } 
    
    return destination;
}



