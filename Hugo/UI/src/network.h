#ifndef NETWORK_H
#define NETWORK_H

#define SIZE 784
#define LINE 28
#define HID 30
#define OUT 10

typedef struct Network
{
        size_t inputsize;
        size_t hiddensize;
        size_t outputsize;
        double input[SIZE];
        double values[HID];
        double output[OUT];
        double b1[HID];
        double b2[OUT];
        double w1[SIZE][HID];
        double w2[HID][OUT];
}Network;

void init_network(Network *net, int is_loaded);

int apply_network(Network *net, SDL_Surface *image_surface);

void apply_training(Network* net, char* path);

#endif
