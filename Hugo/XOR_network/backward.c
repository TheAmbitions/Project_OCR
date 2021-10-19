#include <stddef.h>
#include "sigmoid.h"
#include "backward.h"
#include "matrix.h"

void adjust_weights(size_t line, size_t col, double m1[line], double m2[col], double w[line][col])
{
	size_t i, j;

	for (i = 0; i < line; i++)
	{
		for (j = 0; j < col; j++)
			w[i][j] += m1[i] * m2[j];
	}
}

//To respect the condition of 80 words/ligne:
//x -> input array
//o -> ouptput array
//y -> expected array
void backward(Network *net, double x[net->inputsize], double o[net->outputsize], double y[net->outputsize])
{
	//update errors of the output layer
	size_t i;
	for (i = 0; i < net->outputsize; i++)
		o[i] = (y[i] - o[i]) * sigmoid_prime(o[i]);

	//update errors of the hidden layer
	double w2_t[net->outputsize][net->hiddensize];
	transpose(net->hiddensize, net->outputsize, net->w2, w2_t);

	double z2_delta[net->hiddensize];
	matrix_product(1, net->outputsize, net->hiddensize, o, w2_t, z2_delta);
	
	for (i = 0; i < net->hiddensize; i++)
		z2_delta[i] = z2_delta[i] * sigmoid_prime(net->values[i]);

	//update w1 of the network
	adjust_weights(net->inputsize, net->hiddensize, x, z2_delta, net->w1);

	//update w2 of the network
	adjust_weights(net->hiddensize, net->outputsize, net->values, o, net->w2);
}
