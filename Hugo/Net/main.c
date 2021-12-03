#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>

#include "display.h"
#include "pixel_operations.h"
#include "sigmoid.h"
#include "matrix.h"

#define SIZE 784
#define LINE 28
#define HID 30
#define OUT 9

void print_arr(double arr[], size_t line, size_t cols)
{
	for (size_t i = 0; i < cols; i++)
	{
		for (size_t j = 0; j < line; j++)
			printf("%i ", (int)arr[i * cols + j]);
		printf("\n");
	}
}

typedef struct Network
{
	size_t inputsize;
	size_t hiddensize;
	size_t outputsize;
	double input[SIZE];
	double values[HID];
	double output[OUT];
	double b1[HID];
	double b2[OUT];
	double w1[SIZE][HID];
	double w2[HID][OUT];
}Network;

/*------------------------------------------------------------------------------ Feedforward --------------------------------------------------------------------------------*/

void softmax(double input[], size_t size)
{
	size_t i;
	double m;
	/* Find maximum value from input array */
	m = input[0];
	for (i = 1; i < size; i++)
	{
		if (input[i] > m) 
		{
			m = input[i];
		}
	}

	double sum = 0;
	for (i = 0; i < size; i++)
	{
		sum += expf(input[i] - m);
	}

	for (i = 0; i < size; i++) 
	{
		input[i] = expf(input[i] - m - log(sum));
	}
}

void create_values(size_t line, size_t col, double m1[], double w[line][col], double b[], double val[])
{
	size_t j, k;

	for (j = 0; j < col; j++)
	{
		val[j] = 0;
		for (k = 0; k < line; k++)
		{
			//printf("OK, line = %li --> k = %li, j = %li\n", line2, k, j);
			val[j] += m1[k] * w[k][j];
		}
		val[j] += b[j];
	}
}

void feedforward(Network* net)
{
	//input layer to hidden layer
	create_values(net->inputsize, net->hiddensize, net->input, net->w1, net->b1, net->values);
	for (size_t i = 0; i < net->hiddensize; i++)
		net->values[i] = sigmoid(net->values[i]);

	// hidden layer to output layer
	create_values(net->hiddensize, net->outputsize, net->values, net->w2, net->b2, net->output);
	softmax(net->output, net->outputsize);
}

/*------------------------------------------------------------------------------ Backprop --------------------------------------------------------------------------------*/

void adjust_weights(size_t line, size_t col, double m1[], double m2[], double w[line][col])
{
	size_t i, j;

	for (i = 0; i < line; i++)
	{
		for (j = 0; j < col; j++)
			w[i][j] += m1[i] * m2[j];
	}
}

//To respect the condition of 80 words/ligne:
//y -> expected array
void backprop(Network *net, double y[], float eta)
{
	//update errors of the output layer
	size_t i;
	for (i = 0; i < net->outputsize; i++)
	{
		net->output[i] = (y[i] - net->output[i]) * sigmoid_prime(net->output[i]) * eta;
	}

	//update w2 of the network
	adjust_weights(net->hiddensize, net->outputsize, net->values, net->output, net->w2);

	//update errors of the hidden layer
	double w2_t[net->outputsize][net->hiddensize];
	transpose(net->hiddensize, net->outputsize, net->w2, w2_t);

	double z2_delta[net->hiddensize];
	matrix_product(1, net->outputsize, net->hiddensize, net->output, w2_t, z2_delta);

	for (i = 0; i < net->hiddensize; i++)
		z2_delta[i] = z2_delta[i] * sigmoid_prime(net->values[i]) * eta;

	//update w1 of the network
	adjust_weights(net->inputsize, net->hiddensize, net->input, z2_delta, net->w1);
}

/*------------------------------------------------------------------------------ Init network --------------------------------------------------------------------------------*/

void generateBiases(size_t n, double b[n])
{
	size_t i;
	int rand_mid = RAND_MAX / 2;

	for (i = 0; i < n; i++)
	{
		double sign;
		if (rand() >= rand_mid)
			sign = 1.0;
		else
			sign = -1.0;
		b[i] = sign * ((double)rand() / RAND_MAX);
	}
}

void generateWeights(size_t n, size_t p, double w[n][p])
{
	size_t i, j;
	int rand_mid = RAND_MAX / 2;

	for (i = 0; i < n; i++)
		for (j = 0; j < p; j++)
		{
			double sign;
			if (rand() >= rand_mid)
				sign = 1.0;
			else
				sign = -1.0;
			w[i][j] = sign * ((double)rand() / RAND_MAX);
		}
}

void init_network(Network* net)
{
	generateBiases(HID, net->b1);
	generateBiases(1, net->b2);
	generateWeights(SIZE, HID, net->w1);
	generateWeights(HID, OUT, net->w2);
}

void init_tests(double arr[], double expected[], int number, char *path)
{
	SDL_Surface* image_surface = load_image(path);

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
			if ((r + g + b) / 3 <= 127)
				arr[i * height + j] = 1;
			else
				arr[i * height + j] = 0;
		}
	}

	for (i = 0; i < 9; i++)
	{
		if (i == number - 1)
			expected[i] = 1;
		else
			expected[i] = 0;
	}
}

/*------------------------------------------------------------------------------ Main --------------------------------------------------------------------------------*/

void copy_arr(double arr1[], double arr2[], size_t size)
{
	for (size_t i = 0; i < size; i++)
		arr1[i] = arr2[i];
}

double eval_test(double o)
{
	if (o < 0.5f)
		return 0;
	else
		return 1;
}

size_t eval(double o[])
{
	double max = o[0];
	size_t max_i = 0;
	
	size_t i;
	for (i = 1; i < OUT; i++)
	{
		if (max < o[i])
		{
			max = o[i];
			max_i = i;
		}
	}

	return max_i + 1;
}

void print_train(Network* net, double e[])
{
	for (size_t i = 0; i < net->outputsize; i++)
	{
		printf("Expected value: %f | Predicted value: %f                          --> %f\n", e[i], net->output[i], eval_test(net->output[i]));
	}
	printf("\n");
}

void training(Network* net, double test[], double expected[], int is_train)
{
	copy_arr(net->input, test, net->inputsize);
	feedforward(net);
	if (is_train == 1)
	{
		backprop(net, expected, 0.01);
	}
	else
	{
		print_train(net, expected);
	}
}

void test_img(Network* net, char *path, int expected)
{
	SDL_Surface* image_surface = load_image(path);

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
			if ((r + g + b) / 3 <= 127)
				net->input[i * height + j] = 1;
			else
				net->input[i * height + j] = 0;
		}
	}
	
	feedforward(net);
	printf("Expected : %i | return value : %li\n", expected, eval(net->output));
}

int main()
{
	double train_arr[18][SIZE];
	double expected[18][9];
	char img[18][20] = { "test_images/1.png",
						"test_images/2.png",
						"test_images/3.png",
						"test_images/4.png",
						"test_images/5.png",
						"test_images/6.png",
						"test_images/7.png",
						"test_images/8.png",
						"test_images/9.png",
						"test_images/1,1.png",
						"test_images/2,1.png",
						"test_images/3,1.png",
						"test_images/4,1.png",
						"test_images/5,1.png",
						"test_images/6,1.png",
						"test_images/7,1.png",
						"test_images/8,1.png",
						"test_images/9,1.png" };

	Network network =
	{
		.inputsize = SIZE,
		.hiddensize = HID,
		.outputsize = OUT,
		.input = {},
		.values = {},
		.output = {},
		.b1 = {},
		.b2 = {},
		.w1 = {},
		.w2 = {},
	};

	init_network(&network);
	for (int i = 0; i < 18; i++)
		init_tests(train_arr[i], expected[i], i % 9 + 1, img[i]);

	printf("\n\n");
	printf("Before training:\n\n");
	test_img(&network, "test_images/1.png", 1);
	test_img(&network, "test_images/2.png", 2);
	test_img(&network, "test_images/3.png", 3);
	test_img(&network, "test_images/4.png", 4);
	test_img(&network, "test_images/5.png", 5);
	test_img(&network, "test_images/6.png", 6);
	test_img(&network, "test_images/7.png", 7);
	test_img(&network, "test_images/8.png", 8);
	test_img(&network, "test_images/9.png", 9);

	size_t i;
	size_t n = 100000;
	for (i = 0; i < n; i++)
		training(&network, train_arr[i % 18], expected[i % 18], 1);
	
	/*for (i = 0; i < 18; i++)
		training(&network, train_arr[i], expected[i], 0);
        */

	printf("\n\n\n");
	printf("After training :\n\n");
	test_img(&network, "test_images/1.png", 1);
	test_img(&network, "test_images/2.png", 2);
	test_img(&network, "test_images/3.png", 3);
	test_img(&network, "test_images/4.png", 4);
	test_img(&network, "test_images/5.png", 5);
	test_img(&network, "test_images/6.png", 6);
	test_img(&network, "test_images/7.png", 7);
	test_img(&network, "test_images/8.png", 8);
	test_img(&network, "test_images/9.png", 9);
	printf("\n\n");
}
