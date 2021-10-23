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
#include "kernel.h"
#include "recognition.h"

void wait_for_key(char *input)
{
	printf("\n");
	printf("Enter a key to continue:");
	*input = getchar();
	printf("\n");
}

int main()
{
	char input = 0;
    printf("\n");

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
    display(s);
	apply_rotation(s);
	input = getchar();
	
	wait_for_key(&input);
	printf("--------------KERNEL-----------------\n");
	wait_for_key(&input);
    display(s);
	apply_kernel(s);
    //apply_sobel(s);
	
	wait_for_key(&input);
	printf("--------------RECOGNITION-----------------\n");
	wait_for_key(&input);
    printf("Enter an image: ");
	input = scanf("%[^\n]", s);
	if (input == '\n')
		errx(1, "Error: input not valid");
	input = getchar();

    display(s);
	recognition(s);
   
	wait_for_key(&input);
	printf("--------------SAVE IMAGE-----------------\n");
	printf("Enter the name of the image you want to save (.bmp): ");
	char save[80];
	input = scanf("%[^\n]", save);
	if (input == '\n')
		errx(1, "Error: input not valid");
	Image_Save_BMP(s, save);
	input = getchar();

	wait_for_key(&input);
	printf("--------------NETWORK XOR-----------------\n");
	wait_for_key(&input);
	network();

    input = getchar();
	wait_for_key(&input);
	printf("--------------RESOLVE-----------------\n");
	wait_for_key(&input);
	resolve();

	wait_for_key(&input);
	printf("\n");
	printf("THANK YOU FOR LISTENING\n");
	return 0;
}
