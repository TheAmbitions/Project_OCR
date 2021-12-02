#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

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
 angle_radian = -angle * M_PI / 180.0;

 tcos = cos(angle_radian);
 tsin = sin(angle_radian);

 largeurdest=   ceil(origine->w * fabs(tcos) + origine->h * fabs(tsin)),
 hauteurdest=   ceil( origine->w * fabs(tsin) + origine->h * fabs(tcos)),


 destination = SDL_CreateRGBSurface(SDL_HWSURFACE, largeurdest, hauteurdest, origine->format->BitsPerPixel,
			origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);


 if(destination==NULL)
  return NULL;

 mxdest = destination->w/2.;
 mydest = destination->h/2.;
 mx = origine->w/2.;
 my = origine->h/2.;

 for(j=0;j<destination->h;j++)
  for(i=0;i<destination->w;i++)
  {
   bx = (ceil (tcos * (i-mxdest) + tsin * (j-mydest) + mx));
   by = (ceil (-tsin * (i-mxdest) + tcos * (j-mydest) + my));

   if (bx>=0 && bx< origine->w && by>=0 && by< origine->h)
   {
     couleur = get_pixel(origine, bx, by);
     put_pixel(destination, i, j, couleur);
   }
 }

return destination;
}

 int RotationAuto(SDL_Surface* img,float varMax,int anglevarMax)
{
   	int w = img -> w;
	int h = img -> h;
	Uint8 r,g,b;

    int hist[h];
    for (int i =0;i<h;i++)
    {
        hist[i]=0;
    }

    float ut = 0;
    float E2= 0;
    //float N = h*w;
    float varBetween =0;
 

    for (int i = 0; i<h;i++)
    {
        for (int j=0;j<w;j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
		    SDL_GetRGB(pixel, img->format, &r ,&g,&b);
            
            if (r<127)
            {
                hist[i]+=1;   
            }
        }
    }

    for (int i = 0;i<h;i++)
    {
        ut += hist[i];
    }
   
    ut=ut/h;

    for (int i=0;i<h;i++)
    {
        E2+= (hist[i]-ut) * (hist[i]-ut);
    }
    
    E2=E2/h;

    varBetween=sqrt(E2);
    
    

    if(anglevarMax<=45 && anglevarMax>=0)
    {
        if (varBetween > varMax-1)
        {
            varMax = varBetween;
            anglevarMax+=1; 
            img=SDL_RotationCentralN (img, 1);
            return RotationAuto(img,varMax,anglevarMax);
        }

        if (anglevarMax>37)
        {
            return 0; 
        }

    }
    else
    {
        anglevarMax = -anglevarMax;
        img=SDL_RotationCentralN (img, anglevarMax);
    }
    return anglevarMax;
    
}



