#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "grayscale.h"
#include "display.h"
#include "resolve.h"
#include "network.h"
#include "saveimage.h"
#include "rotations.h"
#include "sobel.h"
#include "recognition.h"

void wait_for_key(char *input)
{
	printf("Enter a key to continue:");
	*input = getchar();
	printf("\n");
}

int main()
{
	char input = 0;

	printf("WELCOME TO THE AMBITIONS APPLICATION!\n");
	printf("--------------------------------------\n");
	printf("\n");
	
	wait_for_key(&input);
	printf("--------------GRAYSCALE-----------------\n");
	char s[80];
	printf("Enter an image: ");
	input = scanf("%[^\n]", s);
	if (input == '\n')
		errx(1, "Error: input not valid");
	input = getchar();
	grayscale(s);
	input = getchar();
	
	wait_for_key(&input);
	printf("--------------ROTATION-----------------\n");
	wait_for_key(&input);
	apply_rotation(s);
	input = getchar();
	
	wait_for_key(&input);
	printf("--------------SOBEL-----------------\n");
	wait_for_key(&input);
	apply_sobel(s);
	
	wait_for_key(&input);
	printf("--------------RECOGNITION-----------------\n");
	wait_for_key(&input);
	recognition(s);

	wait_for_key(&input);
	printf("--------------SAVE IMAGE-----------------\n");
	printf("Enter the name of the image you want to save (.bmp):");
	char save[80];
	input = scanf("%[^\n]", save);
	if (input == '\n')
		errx(1, "Error: input not valid");
	Image_Save_BMP(s, save);
	input = getchar();

	Image_Save_BMP(s, save);
	printf("--------------NETWORK XOR-----------------\n");
	Image_Save_BMP(s, save);
	network();

	Image_Save_BMP(s, save);
	printf("--------------RESOLVE-----------------\n");
	Image_Save_BMP(s, save);
	resolve();

	Image_Save_BMP(s, save);
	printf("\n");
	printf("THANK YOU FOR LISTENING\n");
	return 0;
}