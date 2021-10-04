#include "feedforward.h"
#include "sigmoid.h"
#include "matrix.h"

void feedforward(Network *net, double input[1][net->inputsize], double output[1][net->outputsize])
{
	//input layer to hidden layer
	matrix_product(1, net->inputsize, net->hiddensize, input, net->w1, net->biases);

	int j;
	for (j = 0; j < net->hiddensize; j++)
		net->biases[0][j] = sigmoid(net->biases[0][j]);

	// hidden layer to output layer
	matrix_product(1, net->hiddensize, net->outputsize, net->biases, net->w2, output);

	for (j = 0; j < net->outputsize; j++)
		output[0][j] = sigmoid(output[0][j]);

}
