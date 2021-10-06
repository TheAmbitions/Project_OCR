#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "feedforward.h"
#include "backward.h"

#define INPUTSIZE 2
#define HIDDENSIZE 4

void generateRand(size_t n, size_t p, double w[n][p])
{
	size_t i,j;
	int rand_mid = RAND_MAX / 2;

	for (i = 0; i < n; i++)
		for (j = 0; j < p; j++)
		{
			double sign;
			if (rand() >= rand_mid)
				sign = 1.0;
			else
				sign = -1.0;
			w[i][j] = sign * ((double) rand() / RAND_MAX);
		}
}

void init_network(Network *net)
{
	net->inputsize = INPUTSIZE;
	net->outputsize = 1;
	net->hiddensize = HIDDENSIZE;

	generateRand(1, HIDDENSIZE, net->b1);
	generateRand(1, 1, net->b2);
	generateRand(INPUTSIZE, HIDDENSIZE, net->w1);
	generateRand(HIDDENSIZE, net->outputsize, net->w2);
}

void train_network(Network *net, double x[4][2], double y[4], unsigned long max_it)
{
	size_t i, j;
	for (i = 0; i < max_it; i++)
	{
		for (j = 0; j < 4; j++)
		{
			double o[1][1];
			double input[1][2] = {{x[j][0], x[j][1]}};
			double y_layer[1][1] = {{y[j]}};
			feedforward(net, input, o);
			backward(net, input, o, y_layer);
		}
	}
}

void network()
{
	double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	double expected[4] = {0, 1, 1, 0};

	Network network = {0, 0, 0, {}, {}, {}, {}, {} };
	init_network(&network);

	printf("Before training:\n");
	size_t i;
	double o[1][1];
	double inputlayer[1][2];

	for (i = 0; i < 4; i++)
	{
		inputlayer[0][0] = input[i][0];
		inputlayer[0][1] = input[i][1];
		feedforward(&network, inputlayer, o);
		printf("%f XOR %f -> expected value: %f | network value: %f\n", input[i][0], input[i][1], expected[i], o[0][0]);
	}

	unsigned long max_it = 1000;
	printf("\n");
	printf("-----------------------\n");
	printf("training...\n");
	train_network(&network, input, expected, max_it);
	printf("-----------------------\n");
	printf("\n");
	
	printf("After training (%lu iterations):\n", max_it);
	for (i = 0; i < 4; i++)
	{
		inputlayer[0][0] = input[i][0];
		inputlayer[0][1] = input[i][1];
		feedforward(&network, inputlayer, o);
		printf("%f XOR %f -> expected value: %f | network value: %f\n", input[i][0], input[i][1], expected[i], o[0][0]);
	}
	
	printf("\n");
	printf("-----------------------\n");
	printf("training...\n");
	train_network(&network, input, expected, max_it);
	printf("-----------------------\n");
	printf("\n");
	
	printf("After training (%lu iterations):\n", 2 * max_it);
	for (i = 0; i < 4; i++)
	{
		inputlayer[0][0] = input[i][0];
		inputlayer[0][1] = input[i][1];
		feedforward(&network, inputlayer, o);
		printf("%f XOR %f -> expected value: %f | network value: %f\n", input[i][0], input[i][1], expected[i], o[0][0]);
	}
}
