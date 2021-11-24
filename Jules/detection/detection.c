#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "display.h"
#include "hough.h"
#include "pixel_operations.h"


int is_inter(long x, long y, SDL_Surface* image, Uint32 pixel)
{
    Uint8 r1, g1, b1;
    Uint8 r2, g2, b2;
    Uint8 r3, g3, b3;
    Uint8 r4, g4, b4;
    Uint8 r5, g5, b5;
    Uint8 r6, g6, b6;
    Uint8 r7, g7, b7;
    Uint8 r8, g8, b8;
    Uint32 pixel1,pixel2,pixel3,pixel4,pixel5,pixel6,pixel7,pixel8;

	pixel1 = get_pixel(image_surface, x+1, y);
	pixel2 = get_pixel(image_surface, x+1, y+1);
	pixel3 = get_pixel(image_surface, x+1, y-1);
	pixel4 = get_pixel(image_surface, x, y-1);
	pixel5 = get_pixel(image_surface, x, y+1);
	pixel6 = get_pixel(image_surface, x-1, y-1);
	pixel7 = get_pixel(image_surface, x-1, y);
	pixel8 = get_pixel(image_surface, x-1, y+1);
		
	SDL_GetRGB(pixel1, image_surface -> format, &r1, &g1, &b1);
	SDL_GetRGB(pixel2, image_surface -> format, &r2, &g2, &b2);
	SDL_GetRGB(pixel3, image_surface -> format, &r3, &g3, &b3);
	SDL_GetRGB(pixel4, image_surface -> format, &r4, &g4, &b4);
	SDL_GetRGB(pixel5, image_surface -> format, &r5, &g5, &b5);
	SDL_GetRGB(pixel6, image_surface -> format, &r6, &g6, &b6);
	SDL_GetRGB(pixel7, image_surface -> format, &r7, &g7, &b7);
	SDL_GetRGB(pixel8, image_surface -> format, &r8, &g8, &b8);

    //test si une ligne horizontale ou presque passe par le pixel
    if(((r1==255 && g1==0 && b1==0)||(r2==255 && g2==0 && b2==0)||
        (r3==255 && g3==0 && b3==0))&&((r6==255 && g6==0 && b6==0)||
        (r7==255 && g7==0 && b7==0)||(r8==255 && g8==0 && b8==0)))
    {   
        //test si une ligne verticale ou presque passe par le pixel
        if(((r5==255 && g5==0 && b5==0)||(r2==255 && g2==0 && b2==0)||
            (r8==255 && g8==0 && b8==0))&&((r6==255 && g6==0 && b6==0)||
            (r4==255 && g4==0 && b4==0)||(r3==255 && g3==0 && b3==0)))
        {
            //est une intersection
            return 1;
        }

    }
    //n'est pas une intersection 
    return 0;
}

SDL_Surface* found_inter (SDL_Surface* origin)
{
    long w = origin -> w;
    long h = (origin -> h);
    long parcour_h = h/2;

    Uint32 pixel;
    Uint8 r,g,b;

    long mingauche =0;
    long maxdroite =0;
    long maxhauteur =0;

    long gauche = 0;
    long droite = 0;
    long hauteur = 1;

    for (long y = 1; y < parcour_h; y++)
    {
        for (long x = 1; x < w; x++)
        {
            pixel = get_pixel(image_surface,x,y);
            SDL_GetRGB(pixel, image_surface -> format,&r, &g, &b);

            if ((r==255 && g==0 && b == 0)&&(is_inter(x,y,SDL_Surface) == 1))
            {
                //on est sur une intersection
                if (gauche == 0)
                {
                    gauche = x;
                }
                droite = x;
            }
        }
        if (droite-gauche > maxdroite-maxgauche && droite - gauche + hauteur <
            h)
        {
            maxdroite = droite;
            maxgauche = gauche;
            maxhauteur = hauteur;

            gauche = 0;
            droite = 0;
        }
        hauteur +=1;
        
    }
    //renvoie l'image découpé 
}

int start_detection(int argc, char *argv[])
{
    if (argc != 2)
        errx(1,"must provide filename");

    #define filename argv[1]

    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(filename);

    launch_hough(image_surface);
}


