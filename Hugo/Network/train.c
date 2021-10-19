#include "train.h"
#include "feedforward.h"
#include "backward.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void print_arr(double y[]) 
{
	size_t k = 0;
	for (size_t i = 0; i < 28; i++)
	{
		for (size_t j = 0; j < 28; i++)
		{
			printf("%i", (int)y[k]);
			k++;
		}
		printf("\n");
	}
	printf("\n");
}

void initialization_layer(Network *net, char path_in[], char path_e[], double x[], double o[], double e[], int back)
{
	FILE* file = NULL;

	file = fopen(path_in, "r");
	size_t i = 0;
	char c = 0;

	if (file != NULL)
	{
		do
		{
			c = fgetc(file);
			x[i] = (double)(c - 48);
			i++;
		} while (i < INPUTSIZE && c != EOF);

		fclose(file);
	}
	else
		errx(1, "Error : the path_in does not exist");

	file = fopen(path_e, "r");

	if (file != NULL)
	{
		i = 0;

		do
		{
			c = fgetc(file);
			e[i] = (double)(c - 48);
			i++;
		} while (i < OUTPUTSIZE && c != EOF);

		fclose(file);
	}
	else
		errx(1, "Error : the path_e does not exist");

	net->inputsize = INPUTSIZE;
	print_arr(x);
	/*
	//printf("size: %li | %li | %li\n", net->inputsize, net->hiddensize, net->outputsize);
	feedforward(net, x, o);
	if (back == 1)
		backward(net, x, o, e);
	else
	{
		for (i = 0; i < OUTPUTSIZE; i++)
		{
			printf("Expected value: %f | Predicted value: %f\n", e[i], o[i]);
		}

		printf("\n");
	}*/
}

void train_network(Network *net, size_t nb, double input_layer[INPUTSIZE], double expect[OUTPUTSIZE], double o[OUTPUTSIZE])
{
	size_t i, j;
	for (i = 0; i < nb; i++)
	{
		for (j = 0; j < 10; j++)
		{
			initialization_layer(net, "Test_number/0.txt", "Test_number/0e.txt", input_layer, o, expect, 1);
			initialization_layer(net, "Test_number/1.txt", "Test_number/1e.txt", input_layer, o, expect, 1);
			initialization_layer(net, "Test_number/2.txt", "Test_number/2e.txt", input_layer, o, expect, 1);
			initialization_layer(net, "Test_number/3.txt", "Test_number/3e.txt", input_layer, o, expect, 1);
			initialization_layer(net, "Test_number/4.txt", "Test_number/4e.txt", input_layer, o, expect, 1);
			initialization_layer(net, "Test_number/5.txt", "Test_number/5e.txt", input_layer, o, expect, 1);
			initialization_layer(net, "Test_number/6.txt", "Test_number/6e.txt", input_layer, o, expect, 1);
			initialization_layer(net, "Test_number/7.txt", "Test_number/7e.txt", input_layer, o, expect, 1);
			initialization_layer(net, "Test_number/8.txt", "Test_number/8e.txt", input_layer, o, expect, 1);
			initialization_layer(net, "Test_number/9.txt", "Test_number/9e.txt", input_layer, o, expect, 1);
		}
	}
}
