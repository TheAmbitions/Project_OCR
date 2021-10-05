#include <stddef.h>
#include "sigmoid.h"
#include "backward.h"
#include "matrix.h"

//To respect the condition of 80 words/ligne:
//x -> input array
//o -> ouptput array
//y -> expected array
void backward(Network *net, double x[1][net->inputsize], double o[1][net->outputsize], double y[1][net->outputsize])
{
	//update errors of the output layer
	double o_delta[1][net->outputsize];

	size_t i;
	for (i = 0; i < net->outputsize; i++)
		o_delta[0][i] = (y[0][i] - o[0][i]) * sigmoid_prime(o[0][i]);

	//update errors of the hidden layer
	double w2_t[net->outputsize][net->hiddensize];
	transpose(net->hiddensize, net->outputsize, net->w2, w2_t);

	double z2_delta[1][net->hiddensize];
	matrix_product(1, net->outputsize, net->hiddensize, o_delta, w2_t, z2_delta);
	
	for (i = 0; i < net->hiddensize; i++)
		z2_delta[0][i] = z2_delta[0][i] * 0.1 * sigmoid_prime(net->values[0][i]);

	//update w1 of the network
	double input_t[net->inputsize][1];
	transpose(1, net->inputsize, x, input_t);

	double w1_newvalue[net->inputsize][net->hiddensize];
	matrix_product(net->inputsize, 1, net->hiddensize, input_t, z2_delta, w1_newvalue);
	matrix_sum(net->inputsize, net->hiddensize, net->w1, w1_newvalue, net->w1);

	//update w2 of the network
	double values_t[net->hiddensize][1];
	transpose(1, net-> hiddensize, net->values, values_t);
	
	double w2_newvalue[net->hiddensize][net->outputsize];
	matrix_product(net->hiddensize, 1, net->outputsize, values_t, o_delta, w2_newvalue);
	matrix_sum(net->hiddensize, net->outputsize, net->w2, w2_newvalue, net->w2);
}
