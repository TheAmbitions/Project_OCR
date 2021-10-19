#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

#define INPUTSIZE 784
#define HIDDENSIZE 15
#define OUTPUTSIZE 10

//values -> neuron's values of the hidden layer
//b1 -> biases of the hidden layer
//b2 -> biases of the ouptut layer
//w1 -> weights between the input layer and the hidden layer
//w2 -> weights between the hidden layer and the output layer
typedef struct Network
{
	size_t inputsize;
	size_t outputsize;
	size_t hiddensize;
	double values[HIDDENSIZE];
	double b1[HIDDENSIZE];
	double b2[OUTPUTSIZE];
	double w1[INPUTSIZE][HIDDENSIZE];
	double w2[HIDDENSIZE][OUTPUTSIZE];	
	
} Network;

void init_network(Network *network);
size_t eval(double o[OUTPUTSIZE]);
void network();

#endif
