#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <math.h>

#include "display.h"

/*effectue une rotation centrale, alloue automatiquement la mémoire*/
SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle)
{
 SDL_Surface* destination;
 int i;
 int j;
 Uint32 couleur;
 int mx, my, mxdest, mydest;
 int bx, by;
 float angle_radian;
 float tcos;
 float tsin;
 double largeurdest;
 double hauteurdest;
 
/* détermine la valeur en radian de l'angle*/
 angle_radian = -angle * M_PI / 180.0;

/*pour éviter pleins d'appel, on stocke les valeurs*/
 tcos = cos(angle_radian);
 tsin = sin(angle_radian);
 
/*calcul de la taille de l'image de destination*/
 largeurdest=   ceil(origine->w * fabs(tcos) + origine->h * fabs(tsin)),
 hauteurdest=   ceil( origine->w * fabs(tsin) + origine->h * fabs(tcos)),


/* 
 * alloue la mémoire à l'espace de destination, attention, 
 * la surface est de même taille
 */
 destination = SDL_CreateRGBSurface(SDL_HWSURFACE, largeurdest, hauteurdest, origine->format->BitsPerPixel,
			origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);

 /*on vérifie que la mémoire a été allouée*/
 if(destination==NULL)
  return NULL;
 
 /*calcul du centre des images*/
 mxdest = destination->w/2.;
 mydest = destination->h/2.;
 mx = origine->w/2.;
 my = origine->h/2.;
 
 for(j=0;j<destination->h;j++)
  for(i=0;i<destination->w;i++)
  {
/* on détermine la valeur de pixel qui correspond le mieux pour la position
 * i,j de la surface de destination */

/* on détermine la meilleure position sur la surface d'origine en appliquant
 * une matrice de rotation inverse
 */

   bx = (ceil (tcos * (i-mxdest) + tsin * (j-mydest) + mx));
   by = (ceil (-tsin * (i-mxdest) + tcos * (j-mydest) + my));
   /* on vérifie que l'on ne sort pas des bords*/
   if (bx>=0 && bx< origine->w && by>=0 && by< origine->h)
   {
     couleur = get_pixel(origine, bx, by);
     put_pixel(destination, i, j, couleur);
   }
 }

return destination;
}


void apply_rotation(char filename[])
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    float angle;
    printf("enter an angle \n");

    if (scanf("%f", &angle) < 0) 
    {
         printf("you want a positive seuil.\n");
         return;
    } 


    //Init SDL
    init_sdl();

    image_surface = load_image(filename);
    
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
    wait_for_keypressed();
*/
    //Free the image surface
    SDL_FreeSurface(image_surface);

    //Free the screen surface
    SDL_FreeSurface (screen_surface);
    SDL_Quit();
}
