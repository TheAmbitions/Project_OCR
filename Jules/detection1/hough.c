#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "detection.h"

#include "display.h"


SDL_Surface* drawLine(SDL_Surface* surf,int x1,int y1, int x2,int y2)  // Bresenham
{
        /*float x = (float)x1;
    float end = (float)(x2)+0.001;8*/
	long w = surf -> w;
        long h = surf -> h;
        Uint32 pixel = SDL_MapRGB(surf->format, 255, 0, 0);

        if (x1> x2)
        {
                int temp = x1;
                x1 = x2;
                x2 = temp;
                temp = y1;
                y1 = y2;
                y2 = temp;
        }

        else
        {
                if (x1 == x2)
                {
                        if (y1 > y2)
                        {
                                int temp = y1;
		                y1 = y2;
                		y2 = temp;
                		temp = x1;
                		x1 = x2;
                		x2 = temp;
                        }
			while ( y1 <= y2 && y1 < h)
                        {
                                if (y1 > -1)
                                	put_pixel(surf,x1,y1, pixel);
                                y1 += 1;
                        }

		}
	}

        float a = ((float)(y2-y1) / (float)(x2-x1));
        int y = y1;
        while (x1 <= x2 && x1 < w)
        {
                y = y1 + (int)((float)x1 * a);
                if (x1 > -1 && y > -1 && y < h)
                {
                    put_pixel(surf, x1,y,pixel);
                }
                x1 += 1;
        }
        return  surf;

}


SDL_Surface* Filter(SDL_Surface* source)
{
	SDL_Surface *target;
	int x, y;

	if( source->flags & SDL_SRCCOLORKEY )
	{	
		target = SDL_CreateRGBSurface( SDL_SWSURFACE, source->w ,source->h, source->format->BitsPerPixel, source->format->Rmask, source->format->Gmask, source->format->Bmask, 0 );
        }
    	else
        {
        	target = SDL_CreateRGBSurface( SDL_SWSURFACE, source->w ,source->h, source->format->BitsPerPixel, source->format->Rmask, source->format->Gmask, source->format->Bmask, source->format->Amask );
	}

	int w = source->w;
	int h = source->h;
	for(y=0; y<h; y++)
	{
		for(x=0; x<w; x++)
		{
			unsigned r = 0, g = 0, b = 0;
			Uint8 rt, gt, bt;
			if (y != 0) 
			{
			   
			   	unsigned o = get_pixel(source, x, y-1);
				SDL_GetRGB(o, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
				
			}
			

			if (x != 0)
			{
				unsigned d = get_pixel(source, x-1, y);
				SDL_GetRGB(d, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
			if ((x != 0) && (y != 0))
			{
				unsigned a = get_pixel(source,x-1, y-1);
				SDL_GetRGB(a, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
			if ((x < w-1) && (y !=0))
			{
				
				unsigned c = get_pixel(source, x+1, y-1);
				SDL_GetRGB(c, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
			if (x < w-1) 
			{
				unsigned e = get_pixel(source,x+1, y);
				SDL_GetRGB(e, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
			if ((x !=0 ) && (y < h+1))
			{
					
				unsigned f = get_pixel(source,x-1, y+1);
				SDL_GetRGB(f, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			
			}
			
			
			if (y < h-1) 
			{
				unsigned k = get_pixel(source,x  , y+1);
				SDL_GetRGB(k, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
			}
			
						
			if ((x < w+1 ) && (y < h+1))
			{
					
				unsigned h = get_pixel(source,x+1, y+1);
				SDL_GetRGB(h, source->format, &rt, &gt, &bt);
				r += rt;
				g += gt;
				b += bt;
							
			}
			
			unsigned z = get_pixel(source, x, y);
			SDL_GetRGB(z, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
		
		   	
		   	r/=9;
		   	g/=9;
		   	b/=9;
			
			
			unsigned avg = SDL_MapRGB(target->format, r, g, b);
		   	put_pixel(target,x, y, avg);
		}
	}
	return target;
}

SDL_Surface* kernel (SDL_Surface* image_surface)
{


    Uint8 r, g, b;
    Uint8 r1, g1, b1;
    Uint8 r2, g2, b2;
    Uint8 r3, g3, b3;
    Uint8 r4, g4, b4;
    Uint8 r5, g5, b5;
    Uint8 r6, g6, b6;
    Uint8 r7, g7, b7;
    Uint8 r8, g8, b8;
    Uint8 color;
    Uint32 pixel,pixel1,pixel2,pixel3,pixel4,pixel5,pixel6,pixel7,pixel8;

    Uint32 Blackpixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
    Uint32 Whitepixel = SDL_MapRGB(image_surface->format, 255, 255, 255);

    long width = image_surface -> w;
    long hight = image_surface -> h;

    SDL_Surface* destination = SDL_CreateRGBSurface(SDL_HWSURFACE, width, hight, image_surface->format->BitsPerPixel,
                 image_surface->format->Rmask, image_surface->format->Gmask, image_surface->format->Bmask, image_surface->format->Amask);

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

                color = r -(r1/8 + r2/8 + r3/8 + r4/8 + r5/8 + r6/8 + r7/8 + r8/8);

                if (color > 246)
                {
                        put_pixel(destination, x, y, Whitepixel);
                }
                else
                {
                        put_pixel(destination, x, y, Blackpixel);
                }
         }          
    }
    
    return destination;
}



#define PI 3.1415927

void SDL_ExitWithError(const char *message);

SDL_Surface* hough(SDL_Surface* img, SDL_Surface* dest)
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
	


    for (int i = 0; i < nb; i++)
    {
        rho = lignes[2 * i];
        theta = lignes[2 * i + 1];
        a = cos(theta);
        m = sin(theta);
        x0 = a * rho;
        y0 = m * rho;
        x1  = (x0 + 1000 * (-m));
        y1 = (y0 + 1000 * (a));
        x2 = (x0 - 1000 *(-m));
        y2 = (y0 - 1000 * (a));
       	dest = drawLine(dest,(int)x1, (int)y1, (int)x2,(int)y2);
        
    }
    search (lignes, 2*nb);
    return dest;


        
}

int main(int argc,char *argv[])
{
	
	if (argc != 2)
		errx(1, "must provide filename");
	
	#define filename argv[1]
	
	SDL_Surface* image_surface;
	SDL_Surface* screen_surface;

	// Initialize the SDL
	init_sdl();

	image_surface = load_image(filename);
    SDL_Surface*  image_traite = image_surface;
	
	Uint32 pixel;
	int w;
	int h;
	w = image_surface -> w;
	h = image_surface -> h;
	
	Uint8 r, g, b, average;
 
    	

    
	for(int i = 0; i < w; i++)
	{
		for(int j = 0; j < h; j++)
		{
			pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			average = r * 0.3 + g * 0.59 + b * 0.11;
		        
			pixel = SDL_MapRGB(image_surface->format, average, average, average);
			put_pixel(image_surface,i,j,pixel);
		}
  	}
 	
	image_traite = Filter(image_surface);
	
	//image_surface = sobel(image_surface);
	image_traite = kernel(image_traite);
	screen_surface =  display_image(image_traite);
	wait_for_keypressed();

	//update_surface(screen_surface, image_surface);
	//wait_for_keypressed();
    printf("avant hough\n\n\n");
	image_surface = hough(image_traite, image_surface);
    printf("apres hough\n\n");
	update_surface(screen_surface, image_surface);
	
	//update_surface(screen_surface, image_surface);
	wait_for_keypressed();
	
	//Free the image surface.
	SDL_FreeSurface(image_surface);
	// Free the screen surface.
	SDL_FreeSurface(screen_surface);

	return 0;
}
