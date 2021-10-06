#include "feedforward.h"
#include "sigmoid.h"
#include "matrix.h"

void feedforward(Network *net, double input[1][net->inputsize], double output[1][net->outputsize])
{
	//input layer to hidden layer
	matrix_product(1, net->inputsize, net->hiddensize, input, net->w1, net->values);
	matrix_sum(1, net->hiddensize, net->b1, net->values, net->values);

	int j;
	for (j = 0; j < net->hiddensize; j++)
		net->values[0][j] = sigmoid(net->values[0][j]);

	// hidden layer to output layer
	matrix_product(1, net->hiddensize, net->outputsize, net->values, net->w2, output);
	matrix_sum(1, net->outputsize, net->b2, output, output);

	for (j = 0; j < net->outputsize; j++)
		output[0][j] = sigmoid(output[0][j]);

}
