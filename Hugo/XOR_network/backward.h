#ifndef BACKWARD_H
#define BACKWARD_H

#include "network.h"

void backward(Network *net, double x[1][net->inputsize], double o[1][net->outputsize], double y[1][net->outputsize]);

#endif
