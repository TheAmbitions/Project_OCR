#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include<math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

#include "display.h"

void SDL_FreeSurface(SDL_Surface *surface);


SDL_Surface* Filter(SDL_Surface* source)
{
	SDL_Surface *target;
	int x, y;

	if( source->flags & SDL_SRCCOLORKEY )
	{	
		target = SDL_CreateRGBSurface( SDL_SWSURFACE, source->w ,source->h, 
        source->format->BitsPerPixel, source->format->Rmask, 
        source->format->Gmask, source->format->Bmask, 0 );
        }
    	else
        {
        	target = SDL_CreateRGBSurface( SDL_SWSURFACE, source->w ,source->h, 
            source->format->BitsPerPixel, source->format->Rmask, 
            source->format->Gmask, source->format->Bmask, source->format->Amask );
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
			
			
			unsigned z = get_pixel(source, x, y);
			SDL_GetRGB(z, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
			
			
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
		

		   
		   	r/=9;
		   	g/=9;
		   	b/=9;
			
			unsigned avg = SDL_MapRGB(target->format, r, g, b);
		   	put_pixel(target,x, y, avg);
		}
	}
	return target;
}

float compute_first_order_cumulative_moment(float* hist,int k)
{
	float first_order_cumulative_moment = 0;

	for (int i = 0; i < k; i++)

	{

		first_order_cumulative_moment += i*hist[i];

	}


	return first_order_cumulative_moment;
}

float compute_zero_order_cumulative_moment(float* hist,int k)
{
	float zero_order_cumulative_moment = 0;

	for (int i = 0; i < k; i++)

	{

		zero_order_cumulative_moment += i*hist[i];

	}


	return zero_order_cumulative_moment;
}

float compute_variance_class_separability(float uT,float wk, float uk)

{

    return pow((uT*wk-uk),2)/(wk*(1-wk));

}

void otsu(SDL_Surface* img, float seuil)

{

	float hist[256];

	for (int i = 0; i < 256; i++)

	{

		hist[i] = 0;

	}

	int width = img -> w;
	int height = img -> h;
	Uint8 r,g,b;
	for (int i = 0; i < height ; i++)

	{

		for (int j = 0; j < width; j++)

		{
			Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r , &g, &b);
			hist[r] += 1;

		}

	}

	int N = width*height;

	for (int i = 0; i < 256; i++)

	{

		hist[i] = hist[i] / N;

	}

	float w[256],u[256],uT;

	for (int i = 0; i < 256; i++)

	{

		w[i] = compute_zero_order_cumulative_moment(hist, i);

		u[i] = compute_first_order_cumulative_moment(hist, i);

	}

	uT = compute_first_order_cumulative_moment(hist, 256);

	float variance_class_separability_max = -1;

	float best_threesold = 0;

	for (int i = 0; i < 256; i++)

	{

		int vk = compute_variance_class_separability(uT, w[i], u[i]);

		if (vk > variance_class_separability_max)

		{

			variance_class_separability_max = vk;

			best_threesold = i;

		}

	}

	for (int i = 0; i < height; i++)

	{

		for (int j = 0; j < width; j++)

		{

			Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);
			if (r < best_threesold * seuil)

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

void grayscale(char filename[])
{	
	float seuil;
	printf("enter threshold \n");
	
	if (scanf("%f", &seuil) < 0) {
	    printf("you want a positive seuil.\n");
	    return;
	} 
	
	SDL_Surface* image_surface;
	SDL_Surface* screen_surface;

	// Initialize the SDL
	init_sdl();

	image_surface = load_image(filename);
	// Display the image.
	screen_surface = display_image(image_surface);

	// Wait for a key to be pressed.
	wait_for_keypressed();
	
	
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
    
    update_surface(screen_surface, image_surface);
	
	wait_for_keypressed();
	

	image_surface = Filter(image_surface);
	image_surface = Filter(image_surface);
	
	update_surface(screen_surface, image_surface);
	//update_surface(screen_surface, image_surface);
	
	wait_for_keypressed();
	
	
	otsu(image_surface,seuil);
	
	
	update_surface(screen_surface, image_surface);
	wait_for_keypressed();
	
	// Free the image surface.
	SDL_FreeSurface(image_surface);
	// Free the screen surface.
	SDL_FreeSurface(screen_surface);
    SDL_Quit();
}
