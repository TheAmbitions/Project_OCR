#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "grayscale.h"
#include "resolve.h"
#include "network.h"
#include "saveimage.h"
#include "recognition.h"

int main(int argc, char** argv)
{
	int a = grayscale(argc, argv);
	if (a == 1)
		return EXIT_FAILURE;
	printf("\nResolve:\n");
	resolve();
	printf("Network: XOR: \n");
	network();
	a = Image_Save_BMP("image.png", "testBMP.bmp");
	if (a == 1)
		return EXIT_FAILURE;
	printf("Recognition\n");
	recognition("image_01.jpeg");
	return 0;
}