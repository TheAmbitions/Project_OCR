#ifndef BACKWARD_H
#define BACKWARD_H

#include "network.h"

void backward(Network *net, double x[net->inputsize], double o[net->outputsize], double y[net->outputsize]);

#endif
