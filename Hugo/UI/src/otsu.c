#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

#include "display.h"

/*void init_sdl()
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

void SDL_FreeSurface(SDL_Surface *surface);*/

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

void test(SDL_Surface *img,int ecart)
{
    int h = img->h;
    int w = img->w ;
    int r = 0;
    int T = h*w;
	Uint8 rt, gt, bt;

    for (int i =0; i< h ;i++)
    {
        for (int j= 0;j<h ; j++)
        {

            unsigned pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &rt, &gt, &bt);
			r += rt;
        }
    }

   int avg = r /T;

    for (int i =0; i <h;i++)
    {
        for (int j=0; j<w;j++)
        {
            unsigned pixel = get_pixel(img,j,i);
            SDL_GetRGB(pixel,img->format,&rt,&gt,&bt);
            if (rt < avg + ecart)
            {
                pixel = SDL_MapRGB(img->format, 0,0,0);
                put_pixel(img,j,i,pixel);
            }
        }
    }
}


void ots(SDL_Surface* img)
{
    int width = img -> w;
	int height = img -> h;
	Uint8 r,g,b;

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

    if (ut > 197 && ut<205)
    {
        test(img,ecart);
    }
    if (ecart<60 && ecart>50)
    {
        ut-=20;
    }

    for (int i = 0; i < height; i++)

	{

		for (int j = 0; j < width; j++)

		{

            Uint32 pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img->format, &r ,&g,&b);

			if (r + ecart < ut)

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

void ot(SDL_Surface* img)
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

        printf("%i\n",threshold);
        printf("%f\n",ecart);
        printf("%f\n",ut);

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




void SDL_ExitWithError(const char *message);


SDL_Surface* apply_otsu(SDL_Surface* image_surface)
{
	
	/*if (argc != 2)
		errx(1, "must provide filename");*/
	
	#define filename argv[1]
	
	/*float seuil;
	printf("enter threshold \n");
	
	if (scanf("%f", &seuil) < 0) {
	    printf("you want a positive seuil.\n");
	    return 1;
	} */
	
	//SDL_Surface* image_surface;
	//SDL_Surface* screen_surface;

	// Initialize the SDL
	//init_sdl();

	//image_surface = load_image(filename);
	// Display the image.
	//screen_surface = display_image(image_surface);

	// Wait for a key to be pressed.
	//wait_for_keypressed();
	
	
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
 
 	//update_surface(screen_surface, image_surface);
 	//wait_for_keypressed();
 	
    
    Filter(image_surface);

    noiseReduction(image_surface);


	//update_surface(screen_surface, image_surface);

	
	//wait_for_keypressed();
	
	//otsu(image_surface,seuil);
    //ots(image_surface);
    //printf("OK2\n");
    ot(image_surface);
	
    //image_surface = Filter(image_surface);

	
	//update_surface(screen_surface, image_surface);
	//wait_for_keypressed();
	
    SDL_SaveBMP(image_surface,"../Image/tmp_img/otsu.bmp");	
	
	//Free the image surface.
	//SDL_FreeSurface(image_surface);
	// Free the screen surface.
	//SDL_FreeSurface(screen_surface);

	return image_surface;
}