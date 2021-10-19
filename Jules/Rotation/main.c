#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <math.h>
#include <err.h>
#include "rotations.h"

int main (int argc,char *argv[])
{
        char *filename;
        if (argc < 2)
                errx(1, "must provide filename");
        filename = argv[1];

        float seuil;
        printf("enter threshold \n");

        if (scanf("%f", &seuil) < 0) {
            printf("you want a positive seuil.\n");
            return 1;
        } 
	SDL_Surface* image_surface;
        SDL_Surface* screen_surface;

        // Initialize the SDL
        init_sdl();

        image_surface = load_image(filename);

	//make the rotation
	image_surface = SDL_RotationCentralN (image_surface, angle );

    	//Display the image
    	screen_surface = display_image (image_surface);

    	//Wait for a key to be pressed
    	wait_for_keypressed();

   	/* //Change the picture
    	image_surface = SDL_RotationCentralN (image_surface, 90);

    	//Display the image
    	update_surface (screen_surface, image_surface);

    	//Wait for a key to be pressed
*/
    	//Free the image surface
    	SDL_FreeSurface(image_surface);

    	//Free the screen surface
    	SDL_FreeSurface (screen_surface);

    	return 0;
}

