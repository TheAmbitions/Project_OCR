#ifndef DISPLAY_H
#define DISPLAY_H

void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();
void display(char path[]);

#endif
