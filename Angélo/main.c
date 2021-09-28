#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>


/*gcc main.c $(sdl2-config --cflags --libs) -o prog */

int main(int argc, char **argv)
{
	SDL_Window *window = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("ERREUR : initialisation SDL > %s\n",SDL_GetError());
		exit(EXIT_FAILURE);
	}

	//Execution du programme...

	window = SDL_CreateWindow("Première fenêtre SDL 2",	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	
	if (window == NULL)
	{
		SDL_Log("ERREUR : Creation fenetre echouee > %s\n",SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_Delay(5000);

	SDL_Quit();

	return EXIT_SUCCESS; //Return 0;
}
