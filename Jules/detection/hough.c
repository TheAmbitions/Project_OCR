#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "pixel_operations.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


unsigned long Color(int R, int G, int B)
{//Convertit RGB en long
	return 65536*R + 256*G + B;
}
 
void PutPixel(SDL_Surface *surface, int x, int y, unsigned long pixel)
{
	int bpp = surface->format->BytesPerPixel;
	unsigned char *p = (unsigned char *)surface->pixels + y * surface->pitch + x * bpp;
	if (bpp==4)
		*(unsigned long*)p = pixel;
}
 
void drawLine(SDL_Surface* surf,int x1,int y1, int x2,int y2, int R, int G, int B)  // Bresenham
{
	unsigned long couleur = Color(R, G, B);
	int x,y;
	int Dx,Dy;
	int xincr,yincr;
	int erreur;
	int i;
 
	Dx = abs(x2-x1);
	Dy = abs(y2-y1);
	if(x1<x2)
		xincr = 1;
	else
		xincr = -1;
	if(y1<y2)
		yincr = 1;
	else			
		yincr = -1;
 
	x = x1;
	y = y1;
	if(Dx>Dy)
	{
		erreur = Dx/2;
		for(i=0;i<Dx;i++)
		{
			x += xincr;
			erreur += Dy;
			if(erreur>Dx)
			{
				erreur -= Dx;
				y += yincr;
			}
			PutPixel(surf,x, y,couleur);
		}
	}
	else
	{
		erreur = Dy/2;
		for(i=0;i<Dy;i++)
		{
			y += yincr;
			erreur += Dx;
 
			if(erreur>Dy)
			{
				erreur -= Dy;
				x += xincr;
			}
			PutPixel(surf,x, y,couleur);
		}
	}
}


#define PI 3.1415927

void SDL_ExitWithError(const char *message);

void hough(SDL_Surface* img)
{

    Uint32 pixel;
    Uint8 r,g,b;

    double width = img -> w;
    double height = img -> h;

    double rho = 1;
    double theta = 1;
    double Ntheta = 180/theta;
    double Nrho = floor( sqrt(width * width + height * height))/rho;
    double dtheta = PI / Ntheta;
    double drho = floor(sqrt(width * width + height * height))/Nrho;
    double accum[(int) (Ntheta * Nrho)];
    double i_rho;
    int seuil = 130;

    for (double i = 0; i < Ntheta * Nrho; i++)
        accum[(int) i] = 0;

    for (double y = 0; y < height; y++)
    {
        for (double x = 0; x < width; x++)
        {
            pixel = get_pixel(img,x ,y);
            SDL_GetRGB (pixel, img -> format, &r,&g,&b);
            if ( r == 255)
            {
                for (double i_theta = 0; i_theta < Ntheta; i_theta++)
                {
                    theta = i_theta * dtheta;
                    rho = x * cos(theta) + (height - y) * sin(theta);
                    i_rho = (rho/drho);
                    if ((i_rho>0) && (i_rho < Nrho))
                        accum [(int)(i_theta * width +i_rho)] += 1; 
                }
            }
        }
    }
    int nb = 0;
    double accum_seuil[(int) (Ntheta * Nrho)];
    for (double i = 0; i < Ntheta * Nrho; i++)
        accum_seuil[(int)i] = accum[(int)i];
    
    for (double i_theta = 0; i_theta < Ntheta; i_theta++)
    {
        for (double i_rho = 0; i_rho < Nrho; i_rho++)
        {
            if (accum[(int)(i_theta* width +i_rho)] < seuil)
            {
                accum_seuil[(int)(i_theta * width + i_rho)] = 0;
            }
            else
                nb += 1;

        }
    }
    
    double lignes[nb*2];
    int i = 0;

    for (double i_theta = 0; i_theta < Ntheta; i_theta++)
    {
        for (double i_rho = 0; i_rho < Nrho; i_rho++)
        {
            if (accum_seuil[(int)(i_theta* width +i_rho)] !=0)
            {
                lignes[i * 2] = i_rho * drho;
                lignes[i *2 +1] = i_theta * dtheta;
                i = i + 1;
            }
        }
    }

    double a,m,x0,y0,x1,y1,x2,y2;
	
    //afficher les droites

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init (SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Initialisation SDL");

    if(SDL_CreateWindowAndRenderer(img -> w, img -> h, 0, &window, &renderer)
        !=0)
        SDL_ExitWithError("Impossible de creer la fenetre et le rendu");

    for (int i = 0; i < nb; i++)
    {
        rho = lignes[2 * i];
        theta = lignes[2 * i + 1];
        a = cos(theta);
        m = sin(theta);
        x0 = a * rho;
        y0 = m * rho;
        x1  = (x0 + 1000 * (-m));
        y1 = (y1 + 1000 * (a));
        x2 = (x0 - 1000 *(-m));
        y2 = (y0 - 1000 * (a));

       drawline(renderer,(int)x1, (int)y1, (int)x2, (int)y2,255,0,0);

    }

    SDL_RenderPresent(renderer);
    SDL_Delay(15000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
   
    SDL_Quit();
        
}




