#ifndef NETWORK_H
#define NETWORK_H

typedef struct Network
{
	int inputsize;
	int outputsize;
	int hiddensize;
	double biases[1][15];
	double w1[784][15];
	double w2[15][10];	
	
} Network;

void init_network(Network *network);
void network();

#endif
