#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

typedef struct Network
{
	size_t inputsize;
	size_t outputsize;
	size_t hiddensize;
	double values[1][10];
	double b1[1][10];
	double b2[1][1];
	double w1[2][10];
	double w2[10][1];	
	
} Network;

void init_network(Network *network);
void network();

#endif
