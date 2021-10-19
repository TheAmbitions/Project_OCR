#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "feedforward.h"
#include "backward.h"
#include "train.h"

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
	net->inputsize = INPUTSIZE;
	net->outputsize = OUTPUTSIZE;
	net->hiddensize = HIDDENSIZE;

	generateBiases(HIDDENSIZE, net->b1);
	generateBiases(1, net->b2);
	generateWeights(INPUTSIZE, HIDDENSIZE, net->w1);
	generateWeights(HIDDENSIZE, OUTPUTSIZE, net->w2);
}

size_t eval(double o[OUTPUTSIZE])
{	
	double max = o[0];
	size_t max_i = 0;
	size_t i;
	for (i = 1; i < OUTPUTSIZE; i++)
	{
		if (max < o[i])
		{
			max = o[i];
			max_i = i;
		}
	}

	return max_i;
}

void network()
{
	Network network = { 0, 0, 0, {}, {}, {}, {}, {} };
	init_network(&network);

	printf("Before training:\n");
	size_t i;
	double o[OUTPUTSIZE];
	double expected[OUTPUTSIZE];
	double inputlayer[INPUTSIZE];

	//initialization_layer(&network, "Test_number/0.txt", "Test_number/0e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/1.txt", "Test_number/1e.txt", inputlayer, o, expected, 0);
	/*initialization_layer(&network, "Test_number/2.txt", "Test_number/2e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/3.txt", "Test_number/3e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/4.txt", "Test_number/4e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/5.txt", "Test_number/5e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/6.txt", "Test_number/6e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/7.txt", "Test_number/7e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/8.txt", "Test_number/8e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/9.txt", "Test_number/9e.txt", inputlayer, o, expected, 0);

	unsigned long max_it = 1000;
	printf("\n");
	printf("-----------------------\n");
	printf("training...\n");
	train_network(&network, max_it, inputlayer, expected, o);
	printf("-----------------------\n");
	printf("\n");
	
	printf("After training (%lu iterations):\n", max_it);
	initialization_layer(&network, "Test_number/0.txt", "Test_number/0e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/1.txt", "Test_number/1e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/2.txt", "Test_number/2e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/3.txt", "Test_number/3e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/4.txt", "Test_number/4e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/5.txt", "Test_number/5e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/6.txt", "Test_number/6e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/7.txt", "Test_number/7e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/8.txt", "Test_number/8e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/9.txt", "Test_number/9e.txt", inputlayer, o, expected, 0);
	
	printf("\n");
	printf("-----------------------\n");
	printf("training...\n");
	train_network(&network, max_it, inputlayer, expected, o);
	printf("-----------------------\n");
	printf("\n");
	
	printf("After training (%lu iterations):\n", 2 * max_it);
	initialization_layer(&network, "Test_number/0.txt", "Test_number/0e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/1.txt", "Test_number/1e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/2.txt", "Test_number/2e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/3.txt", "Test_number/3e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/4.txt", "Test_number/4e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/5.txt", "Test_number/5e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/6.txt", "Test_number/6e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/7.txt", "Test_number/7e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/8.txt", "Test_number/8e.txt", inputlayer, o, expected, 0);
	initialization_layer(&network, "Test_number/9.txt", "Test_number/9e.txt", inputlayer, o, expected, 0);*/

}
