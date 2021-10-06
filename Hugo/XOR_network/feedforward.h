#ifndef FEEDFORWARD_H
#define FEEDFORWARD_H

#include "network.h"

void feedforward(Network *net, double input[1][net->inputsize], double output[1][net->outputsize]);

#endif
