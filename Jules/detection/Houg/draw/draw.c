#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <err.h>



void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
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





static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}


void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}


void PutPixel(SDL_Surface *surface, int x, int y, unsigned long pixel)
{
	int bpp = surface->format->BytesPerPixel;
	unsigned char *p = (unsigned char *)surface->pixels + y * surface->pitch + x * bpp;
	if (bpp==4)
		*(unsigned long*)p = pixel;
}

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


}

int main()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();
    image_surface = load_image("image_01.jpeg");
    for (int i = 50; i < 52; i++)
    {
        image_surface = drawLine(image_surface, i, i, 800, 800);
    }
    screen_surface = display_image (image_surface);
    wait_for_keypressed();
    SDL_FreeSurface(image_surface); 
    SDL_FreeSurface (screen_surface);
    return 0;
}
