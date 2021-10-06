#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "sigmoid.h"
#include "feedforward.h"
#include "matrix.h"

#define INPUTSIZE 784
#define HIDDENSIZE 15

void generateRand(int n, int p, double w[n][p])
{
	int i,j;
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
	net->outputsize = 10;
	net->hiddensize = HIDDENSIZE;

	generateRand(1, HIDDENSIZE, net->b1);
	generateRand(1, 10, net->b2);
	generateRand(INPUTSIZE, HIDDENSIZE, net->w1);
	generateRand(HIDDENSIZE, net->outputsize, net->w2);
}

void network()
{
	Network network = { 0, 0, 0, {}, {}, {}, {}, {} };
	init_network(&network);

	for (int i = 0; i < INPUTSIZE; i++)
		for (int j = 0; j < HIDDENSIZE; j++)
			printf("%f\n", network.w1[i][j]);

	double x = 0.898989;
	printf("sigmoid de %f = %f\n", x, sigmoid(x));
	printf("dérivé de sigmoid de %f = %f\n", x, sigmoid_prime(x));

	double m1[1][4] = {{1.0, 3.0, 4.0, 5.0}};
	double m2[4][2] = {{1.0, 0.0}, 
			{6.0, 9.0}, 
			{3.0, 9.0}, 
			{2.0, 0.0}};
	double mul[1][2];
	matrix_product(1, 4, 2, m1, m2, mul);
	for (int i = 0; i < 2; i++)
		printf("%f\n", mul[0][i]);

	double output[1][10];
	double input[1][784];
	generateRand(1, 784, input);
	feedforward(&network, input, output);

}
