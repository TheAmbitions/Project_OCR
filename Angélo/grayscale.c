#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
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

SDL_Surface* FilterCanny(SDL_Surface* source)
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
			//r += rt;
			g += gt;
			b += bt;
		
			r=rt*8 - r ;
		   	
		   	//r/=9;
		   	g/=9;
		   	b/=9;
			
			unsigned avg = SDL_MapRGB(target->format, r, r, r);
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

SDL_Surface* sobel (SDL_Surface* image_surface)
{

    Uint8 r1, g1, b1;
    Uint8 r2, g2, b2;
    Uint8 r3, g3, b3;
    Uint8 r4, g4, b4;
    Uint8 r5, g5, b5;
    Uint8 r6, g6, b6;
    Uint8 r7, g7, b7;
    Uint8 r8, g8, b8;
    int colorx, colory;
    Uint32 pixel1,pixel2,pixel3,pixel4,pixel5,pixel6,pixel7,pixel8;

    Uint32 Blackpixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
    Uint32 Whitepixel = SDL_MapRGB(image_surface->format, 255, 255, 255);

    long width = image_surface -> w;
    long hight = image_surface -> h;
    
    SDL_Surface* destination = SDL_CreateRGBSurface(SDL_HWSURFACE, width, hight,
    image_surface->format->BitsPerPixel,image_surface->format->Rmask, 
    image_surface->format->Gmask, image_surface->format->Bmask, image_surface->format->Amask);

    for (long y = 1 ; y < hight - 1; y++)
    {
        for (long x = 1; x < width - 1; x++)
        {
		    pixel1 = get_pixel(image_surface, x-1, y+1);
		    pixel2 = get_pixel(image_surface, x-1, y);
		    pixel3 = get_pixel(image_surface, x-1, y-1);
		    pixel4 = get_pixel(image_surface, x+1, y-1);
		    pixel5 = get_pixel(image_surface, x+1, y);
		    pixel6 = get_pixel(image_surface, x+1, y+1);
		    pixel7 = get_pixel(image_surface, x, y-1);
		    pixel8 = get_pixel(image_surface, x, y+1);
		
		    SDL_GetRGB(pixel1, image_surface -> format, &r1, &g1, &b1);
		    SDL_GetRGB(pixel2, image_surface -> format, &r2, &g2, &b2);
		    SDL_GetRGB(pixel3, image_surface -> format, &r3, &g3, &b3);
		    SDL_GetRGB(pixel4, image_surface -> format, &r4, &g4, &b4);
		    SDL_GetRGB(pixel5, image_surface -> format, &r5, &g5, &b5);
		    SDL_GetRGB(pixel6, image_surface -> format, &r6, &g6, &b6);
		    SDL_GetRGB(pixel7, image_surface -> format, &r7, &g7, &b7);
		    SDL_GetRGB(pixel8, image_surface -> format, &r8, &g8, &b8);

		    colorx = (int)(r4 + 2 * r5 + r6 - r1 - r2 * 2 - r3); //div peut faire de la merde
		    colory = (int)(r1 + 2 * r8 + r6 - r3 - r4 - 2 * r7);
            if (colorx >275 || colory > 275)
                put_pixel(destination, x, y, Whitepixel);
            else
                put_pixel(destination, x, y, Blackpixel);
        }	    


    }
    
    return destination;
}

#define PI 3.1415927

void SDL_ExitWithError(const char *message);

/*void hough(SDL_Surface* img)
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

        if(SDL_RendererDawLine(renderer,(int)x1, (int)y1, (int)x2, (int)y2) !=0)
            SDL_ExitWithError("Impossible de dessiner");

    }

    SDL_RenderPresent(renderer);
    SDL_Delay(15000);
    SDL_DestroyRender(renderer);
    SDL_DestroyWindow(window);
    
   
    SDL_Quit();
        
}*/

int main(int argc,char *argv[])
{
	
	if (argc != 2)
		errx(1, "must provide filename");
	
	#define filename argv[1]
	
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
		
	
	update_surface(screen_surface, image_surface);
	//update_surface(screen_surface, image_surface);
	
	wait_for_keypressed();
	
	/*otsu(image_surface,seuil);
	
	
	
	
	
	update_surface(screen_surface, image_surface);
	//update_surface(screen_surface, image_surface);
	
	wait_for_keypressed();*/
	
	
	image_surface = sobel(image_surface);
	
	
	update_surface(screen_surface, image_surface);
	wait_for_keypressed();
	
	//hough(image_surface);
	
	
	/*update_surface(screen_surface, image_surface);
	wait_for_keypressed();*/
	
	//Free the image surface.
	SDL_FreeSurface(image_surface);
	// Free the screen surface.
	SDL_FreeSurface(screen_surface);

	return 0;
}
