#ifndef TRAIN_H
#define TRAIN_H

#include <stddef.h>
#include "network.h"

void initialization_layer(Network* net, char path_in[], char path_e[], double x[INPUTSIZE], double o[OUTPUTSIZE], double e[OUTPUTSIZE], int back);
void train_network(Network *net, size_t nb, double input_layer[INPUTSIZE], double expect[OUTPUTSIZE], double out[OUTPUTSIZE]);

#endif