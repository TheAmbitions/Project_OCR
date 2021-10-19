#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

#define INPUTSIZE 2
#define HIDDENSIZE 4
#define OUTPUTSIZE 1

typedef struct Hidden
{
	size_t size;
	double values[1][10];
} Hidden;

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
void network();

#endif
