#ifndef NETWORK_H
#define NETWORK_H

//values -> neuron's values of the hidden layer
//b1 -> biases of the hidden layer
//b2 -> biases of the ouptut layer
//w1 -> weights between the input layer and the hidden layer
//w2 -> weights between the hidden layer and the output layer
typedef struct Network
{
	int inputsize;
	int outputsize;
	int hiddensize;
	double values[1][15];
	double b1[1][15];
	double b2[1][10];
	double w1[784][15];
	double w2[15][10];	
	
} Network;

void init_network(Network *network);
void network();

#endif
