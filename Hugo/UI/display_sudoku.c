#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

#include "display.h"

int brd[9][9];
#define MAX_SIZE 12
#define SUDBOXMAX           3
#define SUDXMAX             (SUDBOXMAX * 3)
#define SUDYMAX             (SUDBOXMAX * 3)

void init_array(int grid[9][9])
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            grid[i][j] = 0;
}

void init_sudoku(char path[], int grid[9][9])
{
    FILE* file = NULL;

    file = fopen(path, "r");

    if (file != NULL)
    {
        char str[MAX_SIZE] = "";

        for (int i = 0; i < 9 && fgets(str, MAX_SIZE, file) != NULL; i++)
            for (int j = 0; j < 9; j++)
            {
                if (str[j] >= 49 && str[j] <= 57)
                    grid[i][j] = str[j] - 48;
            }
        

        fclose(file);
    }
    else 
        errx(1, "Error : the path does not exist");
}

/*
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
}*/

void
genboard(int grid[9][9], int grid_2[9][9])
{
    for (int y = 0; y < SUDYMAX; y++)
    {
	for (int x = 0; x < SUDYMAX; x++)
	{
            if (grid[y][x] != grid_2[y][x])
            {
                brd[y][x] = grid_2[y][x] + 10;
            }
            else
                brd[y][x] = grid[y][x];
	}
    }
}

/*SDL_Surface* display_image(SDL_Surface *img)
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

void SDL_FreeSurface(SDL_Surface *surface);
void SDL_ExitWithError(const char *message);
*/
SDL_Surface* drawLine(SDL_Surface* surf,int x1,int y1, int x2,int y2, Uint32 pixel)  // Bresenham
{
        /*float x = (float)x1;
    float end = (float)(x2)+0.001;8*/
	long w = surf -> w;
        long h = surf -> h;

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
/*void wait_for_keypressed()
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
}*/
void display (SDL_Surface *surface, SDL_Surface *chiffres, SDL_Surface *chiffres2)
{
    /*int width = surface -> w;
	int height = surface -> h;
    int width_chiffre = chiffre -> w;
	int height_chiffre = chiffre -> h;*/
    int ws = 0;
    int hs = 0;
    int acc = 0;
    Uint32 pixel;

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            acc = 0;
            int n = brd[y][x];
            if (n > 10)
            {
                n -= 10;
                for(int i = n * 100 - 100; i < n * 100 && i < 900; i++)
                {
                    for (int j = 0; j < 100; j++)
                    {
                        pixel = get_pixel(chiffres2, i, j);
                        ws = (x % 9) * 100 + acc;
                        hs = (y % 9) * 100 + j;
                        put_pixel(surface, ws, hs, pixel);
                    }
                    acc += 1;
                }
            }
            else
            {
                for(int i = n * 100 - 100; i < n * 100 && i < 900; i++)
                {
                    for (int j = 0; j < 100; j++)
                    {
                        pixel = get_pixel(chiffres, i, j);
                        ws = (x % 9) * 100 + acc;
                        hs = (y % 9) * 100 + j;
                        put_pixel(surface, ws, hs, pixel);
                    }
                    acc += 1;
                }
            }
            
        }
    }
    Uint32 pixeln = SDL_MapRGB(surface->format, 0, 0, 0);
    for (int l = 0; l < 9; l++)
    {
        drawLine( surface, l*100, 0, l*100, 900, pixeln);
        drawLine( surface, 0, l*100, 900, l *100, pixeln);
    }

    Uint32 pixelr = SDL_MapRGB(surface->format, 255, 0, 0);
    drawLine( surface, 300, 0, 300, 900, pixelr);
    drawLine(surface, 600, 0, 600, 900, pixelr);
    drawLine( surface, 0, 300, 900, 300, pixelr);
    drawLine( surface, 0, 600, 900, 600, pixelr);

    drawLine( surface, 301, 0, 301, 900, pixelr);
    drawLine(surface, 601, 0, 601, 900, pixelr);
    drawLine( surface, 0, 301, 900, 301, pixelr);
    drawLine( surface, 0, 601, 900, 601, pixelr);

    drawLine( surface, 299, 0, 299, 900, pixelr);
    drawLine(surface, 599, 0, 599, 900, pixelr);
    drawLine( surface, 0, 299, 900, 299, pixelr);
    drawLine( surface, 0, 599, 900, 599, pixelr);
}

void apply_display()
{
    /*
    int grid[9][9] = { { 3, 0, 6, 5, 0, 8, 4, 0, 0 },
                       { 5, 2, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 8, 7, 0, 0, 0, 0, 3, 1 },
                       { 0, 0, 3, 0, 1, 0, 0, 8, 0 },
                       { 9, 0, 0, 8, 6, 3, 0, 0, 5 },
                       { 0, 5, 0, 0, 9, 0, 6, 0, 0 },
                       { 1, 3, 0, 0, 0, 0, 2, 5, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 7, 4 },
                       { 0, 0, 5, 2, 0, 6, 3, 0, 0 } };

    int grid_res[9][9] = { { 3, 4, 6, 5, 0, 8, 4, 0, 0 },
                       { 5, 2, 5, 0, 0, 0, 0, 0, 0 },
                       { 0, 8, 7, 0, 0, 0, 0, 3, 1 },
                       { 0, 0, 3, 0, 1, 6, 0, 8, 0 },
                       { 9, 0, 0, 8, 6, 3, 0, 0, 5 },
                       { 0, 5, 0, 0, 9, 0, 6, 0, 0 },
                       { 1, 3, 0, 0, 0, 0, 2, 5, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 7, 4 },
                       { 0, 0, 5, 2, 0, 6, 3, 0, 0 } };
*/
    int grid[9][9];
    int grid_res[9][9];
    init_array(grid);
    init_array(grid_res);

    init_sudoku("sudoku.txt", grid);
    init_sudoku("result_grid.txt", grid_res);
    genboard(grid, grid_res);

    SDL_Surface* image_surface;
    SDL_Surface* image_surface2;
    SDL_Surface* screen_surface = SDL_CreateRGBSurface(0, 900, 900, 32, 0, 0, 0, 0);

    printf("ok\n");
    image_surface = load_image("UI_img/Number2.png");
    image_surface2 = load_image("UI_img/Number3.png");

    display(screen_surface, image_surface, image_surface2);
    //display_image(screen_surface);
    //wait_for_keypressed();

    SDL_SaveBMP(screen_surface,"tmp_img/result.bmp");

    //Free the image surface.
    SDL_FreeSurface(image_surface);
    //Free the image surface.
    SDL_FreeSurface(image_surface2);
    // Free the screen surface.
    SDL_FreeSurface(screen_surface);
}

