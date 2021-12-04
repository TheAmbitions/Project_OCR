#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>

#include "display.h"
#include "pixel_operations.h"
#include "matrix.h"
#include "network.h"

#include <dirent.h>

void print_arr(double arr[], size_t line, size_t cols)
{
	for (size_t i = 0; i < cols; i++)
	{
		for (size_t j = 0; j < line; j++)
			printf("%i ", (int)arr[i * cols + j]);
		printf("\n");
	}
}

/*------------------------------------------------------------------------------ Feedforward --------------------------------------------------------------------------------*/

double sigmoid(double x)
{
        return 1.0 / (1.0 + exp(-x));
}

double sigmoid_prime(double x)
{
        return sigmoid(x) * (1.0 - sigmoid(x));
}

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

	//update errors of the hidden layer
	double w2_t[net->outputsize][net->hiddensize];
	transpose(net->hiddensize, net->outputsize, net->w2, w2_t);

	double z2_delta[net->hiddensize];
	matrix_product(1, net->outputsize, net->hiddensize, net->output, w2_t, z2_delta);

	for (i = 0; i < net->hiddensize; i++)
		z2_delta[i] = z2_delta[i] * sigmoid_prime(net->values[i]) * eta;

	//update w1 of the network
	adjust_weights(net->inputsize, net->hiddensize, net->input, z2_delta, net->w1);
	//update w2 of the network
        adjust_weights(net->hiddensize, net->outputsize, net->values, net->output, net->w2);
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

void load(Network* net, char path[]);

void init_network(Network* net, int is_loaded)
{
	if (is_loaded == 0)
	{
	    generateBiases(HID, net->b1);
 	    generateBiases(1, net->b2);
	    generateWeights(SIZE, HID, net->w1);
	    generateWeights(HID, OUT, net->w2);
	}
	else 
	{
	    load(net, "../data/save.txt");
	}
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

	return max_i;
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

void training_mnist(Network* net, double y[], char *path)
{
    DIR* rep = NULL;
    rep = opendir(path);
    if (rep == NULL)
	    return;

    struct dirent* fileread = NULL;
    char dest[100] = "";
    char copy[100] = "";
    strcat(dest, path);
    strcat(copy, path);
    
    fileread = readdir(rep);
    fileread = readdir(rep);
    fileread = readdir(rep);
    while (fileread != NULL)
    {
	strcpy(dest, copy);
	strcat(dest, fileread->d_name);
	SDL_Surface* image_surface = load_image(dest);

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
                                net->input[i * height + j] = 0;
                        else
                                net->input[i * height + j] = 1;
                }
        }
	feedforward(net);
	backprop(net, y, 0.01);
	fileread = readdir(rep);
    }
    
    if (closedir(rep) == -1)
	    return;
}

void testing_mnist(Network* net, size_t y, size_t *nbtest, size_t *suc, char *path)
{
    DIR* rep = NULL;
    rep = opendir(path);
    if (rep == NULL)
            return;

    struct dirent* fileread = NULL;
    char dest[100] = "";
    strcat(dest, path);
    char copy[100] = "";
    strcat(copy, path);

    fileread = readdir(rep);
    fileread = readdir(rep);
    fileread = readdir(rep);
    while (fileread != NULL)
    {
	*nbtest += 1;
	strcpy(dest, copy);
	strcat(dest, fileread->d_name);
        SDL_Surface* image_surface = load_image(dest);

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
                                net->input[i * height + j] = 0;
                        else
                                net->input[i * height + j] = 1;
                }
        }
        feedforward(net);
        if (eval(net->output) == y)
		*suc += 1;
        fileread = readdir(rep);
    }

    if (closedir(rep) == -1)
            return;
}

void save(Network* net, char path[])
{
        FILE* file = NULL;

        file = fopen(path, "w");

        if (file != NULL)
        {
                size_t i, j;
                for (i = 0; i < SIZE; i++)
                {
                        for (j = 0; j < HID; j++)
                        {
                                fprintf(file, "%f\n", net->w1[i][j]);
                        }
                }

                for (i = 0; i < HID; i++)
                {
                        for (j = 0; j < OUT; j++)
                        {
                                fprintf(file, "%f\n", net->w2[i][j]);
                        }
                }

                for (i = 0; i < HID; i++)
                        fprintf(file, "%f\n", net->b1[i]);

                for (i = 0; i < OUT; i++)
                        fprintf(file, "%f\n", net->b2[i]);

                fclose(file);
        }
        else
                errx(1, "Error : cannot create the file");
}

void load(Network* net, char path[])
{
        FILE* file = NULL;

        file = fopen(path, "r");

        if (file != NULL)
        {
                const int MAX_SIZE = 20;
                //char str[MAX_SIZE];
                char* str = (char*)malloc(MAX_SIZE);
                size_t i, j;
                for (i = 0; i < SIZE; i++)
                {
                        for (j = 0; j < HID; j++)
                        {
                                str = fgets(str, MAX_SIZE, file);
                                net->w1[i][j] = atof(str);
                        }
                }

                for (i = 0; i < HID; i++)
                {
                        for (j = 0; j < OUT; j++)
                        {
                                str = fgets(str, MAX_SIZE, file);
                                net->w2[i][j] = atof(str);
                        }
                }

                for (i = 0; i < HID; i++)
                {
                        str = fgets(str, MAX_SIZE, file);
                        net->b1[i] = atof(str);
                }

                for (i = 0; i < OUT; i++)
                {
                        str = fgets(str, MAX_SIZE, file);
                        net->b2[i] = atof(str);
                }

                free(str);
                fclose(file);
        }
        else
		errx(1, "Error : the path does not exist");
}

int apply_network(Network *net, SDL_Surface *image_surface)
{
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
	return (int)eval(net->output);
}

void apply_training(Network* net, char* path)
{
	double y[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	double y1[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
	double y2[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
	double y3[] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	double y4[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
	double y5[] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
	double y6[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
	double y7[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
	double y8[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
	double y9[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	for (int i = 0; i < 100; i++)
	{
		training_mnist(net, y, "../Image/test_images/0/");
		training_mnist(net, y1, "../Image/test_images/1/");
		training_mnist(net, y2, "../Image/test_images/2/");
		training_mnist(net, y3, "../Image/test_images/3/");
		training_mnist(net, y4, "../Image/test_images/4/");
		training_mnist(net, y5, "../Image/test_images/5/");
		training_mnist(net, y6, "../Image/test_images/6/");
		training_mnist(net, y7, "../Image/test_images/7/");
		training_mnist(net, y8, "../Image/test_images/8/");
		training_mnist(net, y9, "../Image/test_images/9/");
	}

	save(net, path);
}

/*
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
	
	for (i = 0; i < 18; i++)
		training(&network, train_arr[i], expected[i], 0);

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
	printf("\n\n");*/
	
	/*double grid[784];
	SDL_Surface* image_surface = load_image("../../../mnist_png/training/1/3.png");

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
                                grid[i * height + j] = 0;
                        else
                                grid[i * height + j] = 1;
                }
        }

	print_arr(grid, 28, 28);

	Network net =
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

        init_network(&net, 0);

	size_t nbtest = 0;
	size_t suc = 0;
	double y[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	//training_mnist(&net, y, "../../../mnist_png/training/1/");
	training_mnist(&net, y, "../../../mnist_png/training/0/");
	y[0] = 0;
	y[1] = 1;
	printf("Folder 0 pass\n");
	training_mnist(&net, y, "../../../mnist_png/training/1/");
	y[1] = 0;
        y[2] = 1;
	printf("Folder 1 pass\n");
	training_mnist(&net, y, "../../../mnist_png/training/2/");
	y[2] = 0;
        y[3] = 1;
	printf("Folder 2 pass\n");
	training_mnist(&net, y, "../../../mnist_png/training/3/");
	y[3] = 0;
        y[4] = 1;
	printf("Folder 3 pass\n");
	training_mnist(&net, y, "../../../mnist_png/training/4/");
	y[4] = 0;
        y[5] = 1;
	printf("Folder 4 pass\n");
	training_mnist(&net, y, "../../../mnist_png/training/5/");
	y[5] = 0;
        y[6] = 1;
	printf("Folder 5 pass\n");
	training_mnist(&net, y, "../../../mnist_png/training/6/");
	y[6] = 0;
        y[7] = 1;
	printf("Folder 6 pass\n");
	training_mnist(&net, y, "../../../mnist_png/training/7/");
	y[7] = 0;
        y[8] = 1;
	printf("Folder 7 pass\n");
	training_mnist(&net, y, "../../../mnist_png/training/8/");
	y[8] = 0;
        y[9] = 1;
	printf("Folder 8 pass\n");
	training_mnist(&net, y, "../../../mnist_png/training/9/");
	printf("Folder 9 pass\n");

	save(&net, "save.txt");

	testing_mnist(&net, 0, &nbtest, &suc, "../../../mnist_png/testing/0/");
	printf("Test 0 pass.\n");
	testing_mnist(&net, 1, &nbtest, &suc, "../../../mnist_png/testing/1/");
	printf("Test 1 pass.\n");
	testing_mnist(&net, 2, &nbtest, &suc, "../../../mnist_png/testing/2/");
	printf("Test 2 pass.\n");
	testing_mnist(&net, 3, &nbtest, &suc, "../../../mnist_png/testing/3/");
	printf("Test 3 pass.\n");
	testing_mnist(&net, 4, &nbtest, &suc, "../../../mnist_png/testing/4/");
	printf("Test 4 pass.\n");
	testing_mnist(&net, 5, &nbtest, &suc, "../../../mnist_png/testing/5/");
	printf("Test 5 pass.\n");
	testing_mnist(&net, 6, &nbtest, &suc, "../../../mnist_png/testing/6/");
	printf("Test 6 pass.\n");
	testing_mnist(&net, 7, &nbtest, &suc, "../../../mnist_png/testing/7/");
	printf("Test 7 pass.\n");
	testing_mnist(&net, 8, &nbtest, &suc, "../../../mnist_png/testing/8/");
	printf("Test 8 pass.\n");
	testing_mnist(&net, 9, &nbtest, &suc, "../../../mnist_png/testing/9/");
	printf("Test 9 pass.\n");

	printf("Pourcentage de réussite : %f\n",((double)suc / nbtest) * 100);

	return 0;
}*/
