#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "display.h"
#include "pixel_operations.h"

#define SIZE 784
#define LINE 28

void print_arr(int* arr, size_t line, size_t cols)
{
	for (size_t i = 0; i < cols; i++)
	{
		for (size_t j = 0; j < line; j++)
			printf("%i ", *(arr + (i * cols + j)));
		printf("\n");
	}
}

typedef struct Network
{
	int* input;
}Network;

void training(Network *net, char *path, int expected)
{
	SDL_Surface *image_surface = load_image(path);

	int width = image_surface->w;
	int height = image_surface->h;
	Uint32 p;
	Uint8 r, g, b;
	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			p = get_pixel(image_surface, j, i);
			SDL_GetRGB(p, image_surface->format, &r, &g, &b);
			if ((r + g + b) / 3 > 127)
				*(net->input + (i * height + j)) = 0;
			else
				*(net->input + (i * height + j)) = 1;
		}
	}

	printf("Expected = %i\n", expected);
}

int main()
{
	Network network =
	{
		.input = calloc(SIZE, sizeof(int)),
	};

	training(&network, "1.png", 1);
	print_arr(network.input, LINE, LINE);
}
