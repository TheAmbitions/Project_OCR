#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "network.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "detection.h"
#include "otsu.h"
#include "rotations.h"
#include "display.h"
#include "SDL/SDL_rotozoom.h"

//SDL_Surface* drawLine(SDL_Surface* surf,int x1,int y1, int x2,int y2)  // Bresenham
//{
        /*float x = (float)x1;
    float end = (float)(x2)+0.001;8*/
	/*long w = surf -> w;
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

}*/


/*SDL_Surface* Filter(SDL_Surface* source)
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
}*/

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

                if (color > 50)
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

SDL_Surface* carre(SDL_Surface* surface);
void print_grid2(int grid[]);
SDL_Surface* resize2(SDL_Surface* img,int w,int h,int nw,int nh);

SDL_Surface* fill_grid (SDL_Surface* s, int grid[], int pos, Network* net)
{
    int bo = 1;
    int i =0;
    int w = s->w;
    int h = (s->h)/2;
    Uint32 p;
    Uint8 r,g,b;
    while (i<w && bo)
    {
        p =get_pixel(s,i,h);
        SDL_GetRGB(p,s->format,&r,&g,&b);
        bo = r==255 && g == 255 && b==255;
        i++;
    } 
    s = resize2 (s,s->w,s->h,28,28);
    if (bo)
    {
        grid[pos] = 0;
    }
    else
    {
        grid[pos] = apply_network(net,s);
    }
    return s;
}

void draw_detection(SDL_Surface* s,SDL_Surface* dest,int x, int y)
{
    printf("size s: %i %i\n",s->w,s->h);
    Uint32 p;
    Uint8 r,g,b;
    int i,j;
    for(i = 0; i<30;i++)
    {
        for (j = 0; j<30;j++)
        {
            if (i == 0||i==29||j==0||j==29)
            {
                //printf("if 1\n");
                p = SDL_MapRGB(dest->format,255,0,0);
                put_pixel(dest,j+x,i+y,p); 
            }
            else
            {
                p = get_pixel(s,j-1,i-1);
                SDL_GetRGB(p,s->format,&r,&g,&b);
                if (r<255 || g<255 || b<255)
                {
                    put_pixel(dest,j+x,i+y,p); 
                }
            }            
            //printf("w = %i h = %i\nx = %i y =
            //%i\n\n",dest->w,dest->h,j+x,i+y);
            
        }
    }
    printf ("sortie %i %i\n",x+j,y+i);

}

SDL_Surface* resize2 (SDL_Surface *img, int w, int h, int nw,int nh)
{
    double zoomx = (double)nw/ (double)w;
    double zoomy = (double)nh/ (double)h;

    img = zoomSurface(img, zoomx,zoomy, 0);
    return img;
}

void save_sudoku(int grid[], char* path)
{
    FILE* file = NULL;

    file = fopen(path, "w");

    if (file != NULL)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if(grid[i*9+j] == 0)
                {
                    fprintf(file, ".");
                }
                else
                    fprintf(file, "%d", grid[i*9 +j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
    }
    else
        errx(1, "Error : cannot create the file");
}



void recognition(char image[], double *ecar, Network* net)
{
        SDL_Surface* image_surface;

        //if(SDL_Init(SDL_INIT_VIDEO) == -1)
                //errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());

        image_surface = IMG_Load(image);
        if (!image_surface)
                errx(3, "can't load %s: %s", image, IMG_GetError());

        // div: average of a width of one box
        //int div = (image_surface->w / 9);
        //*ecar = 0;
        int div = *ecar;

        SDL_Surface *img = NULL;
        SDL_Surface *surface = NULL;
        SDL_Rect spriteCoord, spriteSrc;

        spriteCoord.x = spriteCoord.y = 0;
        spriteSrc.w = div;
        spriteSrc.h = div;
        spriteSrc.x = 0;
        spriteSrc.y = 0;
        
        SDL_Surface *dest = load_image("../Image/UI_img/blanc1.png");
        int grid[81];
        for (int i = 0; i < 9; i++)
        {
                for (int j = 0; j < 9; j++)
                {
                        surface = SDL_CreateRGBSurface (0, div, div, 32, 0, 0, 0, 0);
                        SDL_BlitSurface(image_surface, &spriteSrc, surface, &spriteCoord);
                        surface = carre (surface);
                        surface = fill_grid(surface,grid,i*9+j, net);
                        //surface = resize(surface,surface->w,surface->h,28,28);
                        /*SDL_SaveBMP(surface, "image.bmp");
                        surface = load_image("image.bmp");*/
                        draw_detection(surface,dest,j*30,i*30);                      
                        //SDL_SaveBMP(dest,"grille.bmp");

                        /*if (count < 9)
                        {
                                display("image.bmp");
                                count++;
                        }*/
                        spriteSrc.x += div;
                }

                spriteSrc.x = 0;
                spriteSrc.y += div;
       }
        SDL_SaveBMP(dest,"../Image/tmp_img/grille.bmp");
        //display("grille.bmp");
        print_grid2(grid);
        save_sudoku(grid,"../data/sudoku.txt");
        SDL_FreeSurface(dest);
        SDL_FreeSurface(img);
        SDL_FreeSurface(image_surface);
        //SDL_FreeSurface(surface);
        //SDL_Quit();
}


#define PI 3.1415927


SDL_Surface* hough(SDL_Surface* img, SDL_Surface* dest, int seuil, double
accum[], double accum_seuil[], Network* net)
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

    double i_rho;
    //printf ("seuil = %i\n",seuil);

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
    //double accum_seuil[(int) (Ntheta * Nrho)];
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
    printf("Fin boucle 3\n");


    double a,m,x0,y0,x1,y1,x2,y2;
	
    printf("nb ligne = %i\n", i);
    if (i>=20&&i<100)
    {
     
        double *ecar = malloc(sizeof(double));
        *ecar = 0;
        search(lignes, 2*nb, ecar);
        printf("ecar = %lf\n",*ecar);
        double l[16];
        int j = 0;
        for (int i = 0; i < 8; i += 2)
        //for (int i = 0; i < nb*2; i += 2)      
        {
            rho = lignes[i];
            theta = lignes[i + 1];
            a = cos(theta);
            m = sin(theta);
            x0 = a * rho;
            y0 = m * rho;
            x1  = (x0 + 1000 * (-m));
            l[j] = x1;
            y1 = (y0 + 1000 * (a));
            l[j+1] = y1;
            x2 = (x0 - 1000 *(-m));
            l[j+2] = x2;
            y2 = (y0 - 1000 * (a));
            l[j+3] = y2;
            j += 4;
            //dest = drawLine(dest,(int)x1, (int)y1, (int)x2,(int)y2);    
        }
        SDL_Rect rect,cord;
        cord.x = 0;
        cord.y = 0;
        rect.w = l[4] - l[0];
        printf("rect.w = %lf\n",l[4] - l[0]);
        rect.h = l[13] -l[9]; 
        printf("rect.h = %lf\n",l[13] -l[9]);
        rect.x = l[0];
        printf("rect.x = %lf\n", l[0]);
        rect.y = l[9];
        printf("rect.y = %lf\n",l[9]);
        SDL_Surface* surface = SDL_CreateRGBSurface (0, rect.w,rect.h, 32, 0,
        0, 0, 0);
        SDL_BlitSurface(dest, &rect, surface, &cord);
        /*Uint32 pixel = SDL_MapRGB(dest->format,0,255,0);
        put_pixel(dest, l[0],l[9],pixel);*/
        if (surface == NULL)
        {
            printf("error surface\n");
        }
        SDL_SaveBMP(surface, "../Image/tmp_img/test.bmp");
        recognition("../Image/tmp_img/test.bmp", ecar, net);
        free(ecar);
        return dest;
    }

    else
    {
        if (i<20)
            return hough(img,dest,seuil -20,accum, accum_seuil,net);
        return hough(img,dest,seuil +20,accum, accum_seuil,net);
    }


        
}

void print_grid2(int grid[])
{
    for (int i =0; i< 9;i++)
    {
        for (int j =0; j< 9; j++)
        {
            printf("%i ", grid[i*9+j]);
        }
        printf("\n");
    }
}



void boucle_fin (SDL_Surface* surface,int w,int h)
{
    printf("dans la boucle\n\n");
    int endx = w/9;
    int endy = h/9;
    int x=0;
    int y=0;
    int i;
    Uint32 p;
    do
    {
        //horizon haut
        for (i = 0 ; i< w-x;i++)
        {
            p = SDL_MapRGB(surface->format,255,255,255);
            put_pixel(surface,x+i,y,p);
        }
        //vertical droit
        for (i = 0 ; i< h-y;i++)
        {
            p = SDL_MapRGB(surface->format,255,255,255);
            put_pixel(surface,w-x-1,y+i,p);
        }
        //horizon bas
        for (i = 0 ; i< w-x;i++)
        {
            p = SDL_MapRGB(surface->format,255,255,255);
            put_pixel(surface,x+i,h-y-1,p);
        }
        //vertica gauche
        for (i = 0 ; i< h-y;i++)
        {
            p = SDL_MapRGB(surface->format,255,255,255);
            put_pixel(surface,x,y+i,p);
        }
        x++;
        y++;
        if(x>=endx || y>=endy)
        {
            printf(" boucle sous\n");
            int k = 5;
            while (k>0)
            {
                for (i = 0 ; i< w-x;i++)
                {
                    p = SDL_MapRGB(surface->format,255,255,255);
                    put_pixel(surface,x+i,y,p);
                }

                for (i = 0 ; i< h-y;i++)
                {
                    p = SDL_MapRGB(surface->format,255,255,255);
                    put_pixel(surface,x,y+i,p);
                }
                k-=1;
                x++;
                y++;

            }
            break;
        }
    }while(x<endx && y <endy);
}


SDL_Surface* carre (SDL_Surface* surface)
{
    //grid[0] = 0;
    int w =surface->w;
    int h = surface->h;
    printf("h = %i w = %i\n",h,w);
    int x = 0;
    int y =0;
    int f,i;
    Uint8 r,g,b;
    Uint32 p;
    //SDL_Surface *s = SDL_CreateRGBSurface (0, w, h, 32, 0, 0, 0, 0);
    //SDL_BlitSurface(surface, NULL, s, NULL);
    SDL_SaveBMP(surface,"../Image/tmp_img/backup.bmp");
    do
    {
        f = 0;
        //horizon haut
        for (i = 0 ; i< w - x;i++)
        {
            p = get_pixel(surface,x+i,y);
            SDL_GetRGB(p,surface->format,&r,&g,&b);
            if (r != 255 && g!= 255 && b != 255)
            {
                f = 1;
            }
            p = SDL_MapRGB(surface->format,255,255,255);
            put_pixel(surface,x+i,y,p);
        }
        //vertical droit
        for (i = 0 ; i< h - y;i++)
        {
            p = get_pixel(surface,w-x-1,y+i);
            SDL_GetRGB(p,surface->format,&r,&g,&b);
            if (r != 255 && g!= 255 && b != 255)
            {
                f = 1;
            }
            p = SDL_MapRGB(surface->format,255,255,255);
            put_pixel(surface,w-x-1,y+i,p);
        }
        //horizon bas
        for (i = 0 ; i< w - x;i++)
        {
            p = get_pixel(surface,x+i,h-y-1);
            SDL_GetRGB(p,surface->format,&r,&g,&b);
            if (r != 255 && g!= 255 && b != 255)
            {
                f = 1;
            }
            p = SDL_MapRGB(surface->format,255,255,255);
            put_pixel(surface,x+i,h-y-1,p);
        }
        //vertica gauche
        for (i = 0 ; i< h - y;i++)
        {
            p = get_pixel(surface,x,y+i);
            SDL_GetRGB(p,surface->format,&r,&g,&b);
            if (r != 255 && g!= 255 && b != 255)
            {
                f = 1;
            }
            p = SDL_MapRGB(surface->format,255,255,255);
            put_pixel(surface,x,y+i,p);

        }
        if(f == 1)
        {
            x++;
            y++;
        }
        if (x >= w/4 || y >= h/4)
        {
            f=-1;
            surface = load_image("../Image/tmp_img/backup.bmp");
            boucle_fin (surface,w,h);
            return surface;
        }

    }while(f==1);
    return surface;
    /*Uint32 p;
    p = SDL_MapRGB(surface->format,0,255,0);
    put_pixel (surface,0,0,p);*/

}


void  apply_hough(Network* net, char* filename, int is_auto)
{
	
	SDL_Surface* image_surface;
	//SDL_Surface* screen_surface;

	// Initialize the SDL
	//init_sdl();

	image_surface = load_image(filename);
	init_network(net, 1);
	
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
 	
	//image_traite = Filter(image_surface);
	//otsu(image_surface);
    	if (is_auto == 1)
    	{
    		ot(image_surface);
		//screen_surface =  display_image(image_surface);
		//wait_for_keypressed();
		//image_surface = sobel(image_surface);
		int angle= RotationAuto(image_surface,0,0);
		printf("angle = %i\n",angle);
		if (angle>35)
		{  
			image_surface=SDL_RotationCentralN(image_surface,angle-1);
		}
		if (angle<0)
	    	{
		    	image_surface=SDL_RotationCentralN(image_surface,angle+1);
	    	}
	 }	
        noiseReduction(image_surface);
		//update_surface(screen_surface, image_surface);
	//screen_surface =  display_image(image_surface);
	//wait_for_keypressed();
	SDL_SaveBMP(image_surface, "../Image/tmp_img/hihi.bmp");
    SDL_Surface* image_traite = load_image("../Image/tmp_img/hihi.bmp");
    image_traite = kernel(image_traite);


	//update_surface(screen_surface, image_surface);
	//wait_for_keypressed();

 
    double accum[(int)floor(sqrt(image_traite->w*image_traite->w +
    image_traite->h*image_traite->h)) * 180];
    double accum_seuil[(int)floor(sqrt(image_traite->w*image_traite->w +
    image_traite->h*image_traite->h)) * 180];
    printf("avant hough\n\n\n");
    image_surface = hough(image_traite, image_surface,240, accum, accum_seuil, net);
    printf("apres hough\n\n");
    SDL_SaveBMP(image_surface, "../Image/tmp_img/hihi.bmp");


	//update_surface(screen_surface, image_surface);
	
	//update_surface(screen_surface, image_surface);
	//wait_for_keypressed();
	
	//Free the image surface.
	SDL_FreeSurface(image_surface);
	// Free the screen surface.
	//SDL_FreeSurface(screen_surface);

}



/*int main(int argc,char** argv)
{
    if (argc != 2)
    {
        errx(1,"need a filename");
    }
    Network net = {
    .inputsize = 784,
    .hiddensize = 30,
    .outputsize = 10,
    .input = {},
    .values = {},
    .output = {},
    .b1 = {},
    .b2 = {},
    .w1 = {},
    .w2 = {}
        };
    init_network(&net,0);
    apply_hough(&net, argv[1], 1);
    return 0;
}*/
