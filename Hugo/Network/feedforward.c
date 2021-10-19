#include <stddef.h>
#include "feedforward.h"
#include "sigmoid.h"
#include "matrix.h"

#include <stdio.h>

void create_values(size_t line2, size_t col2, double m1[line2], double w[line2][col2], double b[col2], double val[col2])
{
	size_t j, k;

	for (j = 0; j < col2; j++)
	{
		val[j] = 0;
		for (k = 0; k < line2; k++)
		{
			//printf("OK, line = %li --> k = %li, j = %li\n", line2, k, j);
			val[j] += m1[k] * w[k][j];
		}
		val[j] += b[j];
		val[j] = sigmoid(val[j]);
	}
}

void feedforward(Network *net, double input[net->inputsize], double output[net->outputsize])
{
	//input layer to hidden layer
	create_values(net->inputsize, net->hiddensize, input, net->w1, net->b1, net->values);

	// hidden layer to output layer
	create_values(net->hiddensize, net->outputsize, net->values, net->w2, net->b2, output);
}
