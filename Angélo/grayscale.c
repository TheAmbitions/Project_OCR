#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

void init_sdl()
{
	// Init only the video part.
	// If it fails, die with an error message.
	
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
	SDL_Surface *img;

	// Load an image using SDL_image with format detection.
	// If it fails, die with an error message.
	img = IMG_Load(path);
	if (!img)
	    errx(3, "can't load %s: %s", path, IMG_GetError());

	return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
	SDL_Surface *screen;

	// Set the window to the same size as the image
	screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
	if (screen == NULL)
	{
	    // error management
	    errx(1, "Couldn't set %dx%d video mode: %s\n",
		    img->w, img->h, SDL_GetError());
	}

	// Blit onto the screen surface
	if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
	    warnx("BlitSurface error: %s\n", SDL_GetError());

	// Update the screen
	SDL_UpdateRect(screen, 0, 0, img->w, img->h);

	// return the screen for further uses
	return screen;
}

void wait_for_keypressed()
{
	SDL_Event event;

	// Wait for a key to be down.
	do
	{
	    SDL_PollEvent(&event);
	} while(event.type != SDL_KEYDOWN);

	// Wait for a key to be up.
	do
	{
	    SDL_PollEvent(&event);
	} while(event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);

void blur() //This manipulates with SDL_Surface and gives it box blur effect
{
   	SDL_Surface* imageSurface;

	// Initialize the SDL
	init_sdl();

	imageSurface = load_image("image_05.jpeg");
	int blur_extent = 1;	
	
	for (int y = 0; y < imageSurface->h; y++)
	{
		for (int x = 0; x < (imageSurface->pitch / 4); x++)
		{
		    Uint32 color = ((Uint32*)imageSurface->pixels)[(y * (imageSurface->pitch / 4)) + x];

		    //SDL_GetRGBA() is a method for getting color
		    //components from a 32 bit color
		    Uint8 r = 0, g = 0, b = 0, a = 0;
		    SDL_GetRGBA(color, imageSurface->format, &r, &g, &b, &a);

		    Uint32 rb = 0, gb = 0, bb = 0, ab = 0;

		    //Within the two for-loops below, colors of adjacent pixels are added up

		    for (int yo = -blur_extent; yo <= blur_extent; yo++)
		    {
			for (int xo = -blur_extent; xo <= blur_extent; xo++)                 {                   if (y + yo >= 0 && x + xo >= 0
				&& y + yo < imageSurface->h && x + xo < (imageSurface->pitch / 4)
				)
			    {
				Uint32 colOth = ((Uint32*)imageSurface->pixels)[((y + yo)
				                        * (imageSurface->pitch / 4)) + (x + xo)];

				Uint8 ro = 0, go = 0, bo = 0, ao = 0;
				SDL_GetRGBA(colOth, imageSurface->format, &ro, &go, &bo, &ao);

				rb += ro;
				gb += go;
				bb += bo;
				ab += ao;
			    }
			}
		    }

		    //The sum is then, divided by the total number of
		    //pixels present in a block of blur radius

		    //For blur_extent 1, it will be 9
		    //For blur_extent 2, it will be 25
		    //and so on...

		    //In this way, we are getting the average of
		    //all the pixels in a block of blur radius

		    //(((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)) calculates
		    //the total number of pixels present in a block of blur radius

		    r = (Uint8)(rb / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));
		    g = (Uint8)(gb / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));
		    b = (Uint8)(bb / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));
		    a = (Uint8)(ab / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));

		    //Bit shifting color bits to form a 32 bit proper colour
		    color = (r) | (g << 8) | (b << 16) | (a << 24);           ((Uint32*)imageSurface->pixels)[(y * (imageSurface->pitch / 4)) + x] = color;
		}
	}
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

	for(y=1; y<source->h; ++y)
	{
		for(x=1; x<source->w; ++x)
		{
			unsigned r = 0, g = 0, b = 0;
			Uint8 rt, gt, bt;

			unsigned a = get_pixel(source,x-1, y-1);
			SDL_GetRGB(a, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
		   
		   	unsigned o = get_pixel(source, x, y-1);
			SDL_GetRGB(o, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
			
			unsigned c = get_pixel(source, x+1, y-1);
			SDL_GetRGB(c, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
			
			unsigned d = get_pixel(source, x-1, y);
			SDL_GetRGB(d, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
			
			
			unsigned z = get_pixel(source, x, y);
			SDL_GetRGB(z, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
			
			
			
			unsigned e = get_pixel(source,x+1, y);
			SDL_GetRGB(e, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
			
			
			unsigned f = get_pixel(source,x-1, y+1);
			SDL_GetRGB(f, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
			
			
			unsigned k = get_pixel(source,x  , y+1);
			SDL_GetRGB(k, source->format, &rt, &gt, &bt);
			r += rt;
			g += gt;
			b += bt;
			
			
			unsigned h = get_pixel(source,x+1, y+1);
			SDL_GetRGB(h, source->format, &rt, &gt, &bt);
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

int main(int argc,char *argv[])
{
	char* filename;
	if (argc < 2)
		errx(1, "must provide filename");
	filename = argv[1];
	
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
 
    	image_surface = Filter(image_surface);
    	update_surface(screen_surface, image_surface);
    	wait_for_keypressed();
 
	for(int i = 0; i < w; i++)
	{
		for(int j = 0; j < h; j++)
		{
			pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			average = r * 0.3 + g * 0.59 + b * 0.11;
			/*if (average < 128 && average > 0 )
        		{
		            pixel = SDL_MapRGB(image_surface->format, average, average, average);
		        }
		        else if (average >= 128 && average< 255)
		        {
		            contraste = average + 2;
		            pixel = SDL_MapRGB(image_surface->format, contraste, contraste, contraste);
		        }*/
		        
			pixel = SDL_MapRGB(image_surface->format, average, average, average);
			put_pixel(image_surface,i,j,pixel);
		}
  	}

	/*update_surface(screen_surface, image_surface);
	
	wait_for_keypressed();*/
	
	for(int i = 0; i < w; i++)
	{
		for(int j = 0; j < h; j++)
		{
			pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			
			if(r < 127 && g < 127 && b < 127) 
     			{
		      		pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
		     	}
		     	else
		     	{
		     	 	pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
		     	}
		     	
			put_pixel(image_surface,i,j,pixel);
		}
  	}
	
	
	update_surface(screen_surface, image_surface);
	wait_for_keypressed();
	
	// Free the image surface.
	SDL_FreeSurface(image_surface);
	// Free the screen surface.
	SDL_FreeSurface(screen_surface);

	return 0;
}
