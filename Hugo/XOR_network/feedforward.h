#ifndef FEEDFORWARD_H
#define FEEDFORWARD_H

#include "network.h"

void feedforward(Network *net, double input[net->inputsize], double output[net->outputsize]);

#endif
