#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"



void array_swap(int array[], size_t i, size_t j)
{
  int element1 = array[i];
  int element2 = array[j];
  array[i] = element2;
  array[j] = element1;
}

void array_select_sort(int array[], size_t len)
{
  size_t i = 0;
  size_t j;
  int min_index;
  while(i<len)
  {
    /* Find Min Index */
    j= i;
    min_index = j;
    while(j<len)
    {
        if(array[j]<array[min_index])
        {
            min_index = j;
        }
        j+=1;
    }
    array_swap(array,i,min_index);
    i++;
  }
}

void noiseReduction(SDL_Surface *img)
{
  /* Variables */
  int w;
  int h;
  w = img -> w;
  h = img -> h;
  int pixelTable[5];

 for(int i = 0; i < w; i++)
 {
   for(int j = 0; j < h; j++)
   {
     for(int k = j; k <= j + 4; k++)
     {
       //Borders of picture
       if(i == 0)
       {
         if(k == 0)
         {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k);
            pixelTable[2] = get_pixel(img, i, k);
            pixelTable[3] = get_pixel(img, i, k + 1);
            pixelTable[4] = get_pixel(img, i + 1, k);
            break;
         }
         if(k == h)
         {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = get_pixel(img, i, k);
            pixelTable[3] = get_pixel(img, i, k);
            pixelTable[4] = get_pixel(img, i + 1, k);
            break;
         }
         else
         {
          pixelTable[0] = get_pixel(img, i, k);
          pixelTable[1] = get_pixel(img, i, k - 1);
          pixelTable[2] = get_pixel(img, i, k);
          pixelTable[3] = get_pixel(img, i, k + 1);
          pixelTable[4] = get_pixel(img, i + 1, k);
          break;
         }
       }
       if(i == w)
       {
          if(k == 0)
          {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k);
            pixelTable[2] = get_pixel(img, i - 1, k);
            pixelTable[3] = get_pixel(img, i, k + 1);
            pixelTable[4] = get_pixel(img, i, k);
            break;
          }
          if(k == h)
          {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = get_pixel(img, i - 1, k);
            pixelTable[3] = get_pixel(img, i, k);
            pixelTable[4] = get_pixel(img, i, k);
            break;
          }
          else
          {
            pixelTable[0] = get_pixel(img, i, k);
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = get_pixel(img, i - 1, k);
            pixelTable[3] = get_pixel(img, i, k + 1);
            pixelTable[4] = get_pixel(img, i, k);
            break;
          }
       }
       if(k == 0)
       {
          pixelTable[0] = get_pixel(img, i, k);
          pixelTable[1] = get_pixel(img, i, k);
          pixelTable[2] = get_pixel(img, i - 1, k);
          pixelTable[3] = get_pixel(img, i, k + 1);
          pixelTable[4] = get_pixel(img, i + 1, k);
          break;
       }
       if(k == h)
       {
        pixelTable[0] = get_pixel(img, i, k);
        pixelTable[1] = get_pixel(img, i, k - 1);
        pixelTable[2] = get_pixel(img, i - 1, k);
        pixelTable[3] = get_pixel(img, i, k);
        pixelTable[4] = get_pixel(img, i + 1, k);
        break;
       }
       else
       {
        pixelTable[0] = get_pixel(img, i, k);
        pixelTable[1] = get_pixel(img, i, k - 1);
        pixelTable[2] = get_pixel(img, i - 1, k);
        pixelTable[3] = get_pixel(img, i, k + 1);
        pixelTable[4] = get_pixel(img, i + 1, k);
        break;
       }
     }
      array_select_sort(pixelTable, 5);
      int med = pixelTable[2];
      put_pixel(img, i, j, med);
   }
 }
}

void GammaCorrection(SDL_Surface * img,float gamma)
{
    int h = img -> h;
    int w = img ->w ;
    Uint8 r,g,b;
    Uint8 newr,newg,newb;

    for (int i = 0;i < h;i++)
    {
        for (int j=0;j<w;j++)
        {
            float invgamma = 1/gamma;
            Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);

            newr = (int) (pow((float)(r)/255.0,invgamma)*255);
            newg = (int) (pow((float)(g)/255.0,invgamma)*255);
            newb = (int) (pow((float)(b)/255.0,invgamma)*255);

             Uint32 newpixel = SDL_MapRGB(img->format,newr,newg,newb);
            put_pixel(img,j,i,newpixel);
        }
    }
}
 
void otsu(SDL_Surface* img)
{
    int width = img -> w;
    int height = img -> h;
    Uint8 r,g,b;
    int total = width * height;

    int hist[256];
    for (int i =0;i<256;i++)
    {
        hist[i]=0;
    }

    for (int i = 0; i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);
            hist[r]+=1;

        }
    }

   float ut = 0;
    float E2= 0;
    float N = height*width;
   
    for (int i = 0;i<256;i++)
    {

        ut += i*(hist[i]/N);
        E2 += i*i*(hist[i]/N); 
    }
    
    
    float v= E2 - (ut*ut);
    float ecart = sqrt(v);

    if (ecart>100 && ut <100)
    {

        GammaCorrection(img,0.5);
    }
    if (ecart>55 && v < 3500)
    {

        GammaCorrection(img,4);
    }
    
    for (int i =0;i<256;i++)
    {
        hist[i]=0;
    }

    for (int i = 0; i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);
            hist[r]+=1;

        }
    }

   ut = 0;
   E2= 0;
   
    for (int i = 0;i<256;i++)
    {

        ut += i*(hist[i]/N);
        E2 += i*i*(hist[i]/N); 
    }
    v= E2 - (ut*ut);
    ecart = sqrt(v);
   
    
    float sum = 0;
    for (int t = 0; t<256;t++)
    {
        sum+=t * hist[t];
    }

    float sumB=0;
    int wB=0;
    int wF=0;

    float varMax=0;
    int threshold = 0;

    for (int t =0;t<256;t++)
    {
        wB += hist[t];
        if (wB==0)
        {
            continue;
        }

        wF = total - wB;
        if (wF==0)
        {
            break;
        }

        sumB+= (float) (t*hist[t]);

        float mB=sumB/wB;
        float mF = (sum -sumB)/wF;

        float varBetween = (float) wB * (float) wF * (mB -mF) * (mB-mF);

        if (varBetween > varMax)
        {
            varMax = varBetween;
            threshold = t;
        
        }
     }

    int alpha=0;
    if (ecart > 55)
    { 
        alpha = ut-threshold;
    }
    if (ut > 230)
    {    
        alpha = - ecart;
    }
  
        for (int i = 0; i < height; i++)
	    {

		    for (int j = 0; j < width; j++)
		    {

                Uint32 pixel = get_pixel(img, j, i);
			    SDL_GetRGB(pixel, img->format, &r ,&g,&b);

                if (r < threshold - alpha)
                {
                    pixel = SDL_MapRGB(img ->format, 0, 0, 0);
                    put_pixel(img,j,i,pixel);
                }
                else
                {
                    pixel = SDL_MapRGB(img->format, 255, 255, 255);
                    put_pixel(img,j,i,pixel);
                }
		    }
        }        
}






