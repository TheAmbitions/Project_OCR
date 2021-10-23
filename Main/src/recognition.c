#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "display.h"

void recognition(char image[])
{
        SDL_Surface* image_surface;

        if(SDL_Init(SDL_INIT_VIDEO) == -1)
                errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());

        image_surface = IMG_Load(image);
        if (!image_surface)
                errx(3, "can't load %s: %s", image, IMG_GetError());

        // div: average of a width of one box
        int div = (image_surface->w / 9);

        SDL_Surface *img = NULL;
        SDL_Surface *surface = NULL;
        SDL_Rect spriteCoord, spriteSrc;

        spriteCoord.x = spriteCoord.y = 0;
        spriteSrc.w = div;
        spriteSrc.h = div;
        spriteSrc.x = 0;
        spriteSrc.y = 0;

        int count = 0;
        for (int i = 0; i < 9; i++)
        {
                for (int j = 0; j < 9; j++)
                {
                        surface = SDL_CreateRGBSurface (0, div, div, 32, 0, 0, 0, 0);
                        SDL_BlitSurface(image_surface, &spriteSrc, surface, &spriteCoord);
                        SDL_SaveBMP(surface, "../data/image.bmp");
                        if (count < 9)
                        {
                                display("../data/image.bmp");
                                count++;
                        }
                        spriteSrc.x += div;
                }

                spriteSrc.x = 0;
                spriteSrc.y += div;
       }

        SDL_FreeSurface(img);
        SDL_FreeSurface(image_surface);
        SDL_FreeSurface(surface);
        SDL_Quit();
}
