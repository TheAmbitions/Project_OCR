#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "network.h"
#include "feedforward.h"
#include "backward.h"

#include <string.h>

void generateBiases(size_t n, double b[n])
{
	size_t i;
	int rand_mid = RAND_MAX / 2;

	for (i = 0; i < n; i++)
	{
		double sign;
		if (rand() >= rand_mid)
			sign = 1.0;
		else
			sign = -1.0;
		b[i] = sign * ((double)rand() / RAND_MAX);
	}
}

void generateWeights(size_t n, size_t p, double w[n][p])
{
	size_t i,j;
	int rand_mid = RAND_MAX / 2;

	for (i = 0; i < n; i++)
		for (j = 0; j < p; j++)
		{
			double sign;
			if (rand() >= rand_mid)
				sign = 1.0;
			else
				sign = -1.0;
			w[i][j] = sign * ((double) rand() / RAND_MAX);
		}
}

void init_network(Network *net)
{
	net->inputsize = INPUTSIZE;
	net->outputsize = 1;
	net->hiddensize = HIDDENSIZE;

	generateBiases(HIDDENSIZE, net->b1);
	generateBiases(1, net->b2);
	generateWeights(INPUTSIZE, HIDDENSIZE, net->w1);
	generateWeights(HIDDENSIZE, net->outputsize, net->w2);
}

void train_network(Network *net, double x[4][2], double y[4], unsigned long max_it)
{
	size_t i, j;
	for (i = 0; i < max_it; i++)
	{
		for (j = 0; j < 4; j++)
		{
			double o[1];
			double input[2] = {x[j][0], x[j][1]};
			double y_layer[1] = {y[j]};
			feedforward(net, input, o);
			backward(net, input, o, y_layer);
		}
	}
}

void train(Network* network)
{
	double input[4][2] = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
	double expected[4] = { 0, 1, 1, 0 };

	printf("Before training:\n");
	size_t i;
	double o[1];
	double inputlayer[2];

	for (i = 0; i < 4; i++)
	{
		inputlayer[0] = input[i][0];
		inputlayer[1] = input[i][1];
		feedforward(network, inputlayer, o);
		printf("%f XOR %f -> expected value: %f | network value: %f\n", input[i][0], input[i][1], expected[i], o[0]);
	}

	unsigned long max_it = 1000;
	printf("\n");
	printf("-----------------------\n");
	printf("training...\n");
	train_network(network, input, expected, max_it);
	printf("-----------------------\n");
	printf("\n");

	printf("After training (%lu iterations):\n", max_it);
	for (i = 0; i < 4; i++)
	{
		inputlayer[0] = input[i][0];
		inputlayer[1] = input[i][1];
		feedforward(network, inputlayer, o);
		printf("%f XOR %f -> expected value: %f | network value: %f\n", input[i][0], input[i][1], expected[i], o[0]);
	}

	printf("\n");
	printf("-----------------------\n");
	printf("training...\n");
	train_network(network, input, expected, max_it);
	printf("-----------------------\n");
	printf("\n");

	printf("After training (%lu iterations):\n", 2 * max_it);
	for (i = 0; i < 4; i++)
	{
		inputlayer[0] = input[i][0];
		inputlayer[1] = input[i][1];
		feedforward(network, inputlayer, o);
		printf("%f XOR %f -> expected value: %f | network value: %f\n", input[i][0], input[i][1], expected[i], o[0]);
	}
}

void save(Network* net, char path[])
{
	FILE* file = NULL;

	file = fopen(path, "w");

	if (file != NULL)
	{
		size_t i, j;
		for (i = 0; i < INPUTSIZE; i++)
		{
			for (j = 0; j < HIDDENSIZE; j++)
			{
				fprintf(file, "%f\n", net->w1[i][j]);
			}
		}

		for (i = 0; i < HIDDENSIZE; i++)
		{
			for (j = 0; j < OUTPUTSIZE; j++)
			{
				fprintf(file, "%f\n", net->w2[i][j]);
			}
		}

		for (i = 0; i < HIDDENSIZE; i++)
			fprintf(file, "%f\n", net->b1[i]);

		for (i = 0; i < OUTPUTSIZE; i++)
			fprintf(file, "%f\n", net->b2[i]);

		fclose(file);
	}
	else
		errx(1, "Error : cannot create the file");
}

void load(Network* net, char path[])
{
	FILE* file = NULL;

	file = fopen(path, "r");

	if (file != NULL)
	{
		const int MAX_SIZE = 20;
		//char str[MAX_SIZE];
		char* str = (char*)malloc(MAX_SIZE);
		size_t i, j;
		for (i = 0; i < INPUTSIZE; i++)
		{
			for (j = 0; j < HIDDENSIZE; j++) 
			{
				str = fgets(str, MAX_SIZE, file);
				net->w1[i][j] = atof(str);
			}
		}

		for (i = 0; i < HIDDENSIZE; i++)
		{
			for (j = 0; j < OUTPUTSIZE; j++)
			{
				str = fgets(str, MAX_SIZE, file);
				net->w2[i][j] = atof(str);
			}
		}

		for (i = 0; i < HIDDENSIZE; i++)
		{
			str = fgets(str, MAX_SIZE, file);
			net->b1[i] = atof(str);
		}

		for (i = 0; i < OUTPUTSIZE; i++)
		{
			str = fgets(str, MAX_SIZE, file);
			net->b2[i] = atof(str);
		}

		free(str);
		fclose(file);
	}
	else
		errx(1, "Error : the path does not exist");
}

int eval(double o)
{
	if (o < 0.5)
		return 0;
	else
		return 1;
}

void network()
{
	/*double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	double expected[4] = {0, 1, 1, 0};*/

	Network network = {0, 0, 0, {}, {}, {}, {}, {} };
	init_network(&network);

	double input_layer[2];
	double o[1];

	char input1;
	char input2;

	for (;;)
	{
		printf("Enter a key: ");
		input1 = getchar();
		if (input1 == 'q')
			break;
		else if (input1 == 't')
			train(&network);
		else if (input1 == 'l')
			load(&network, "../data/save.txt");
		else if (input1 == 's')
			save(&network, "../data/save.txt");
		else if (input1 == '1' || input1 == '0')
		{
			printf("Enter a second key: ");
			input2 = getchar();
			input2 = getchar();
			if (input2 == '1' || input2 == '0') 
			{
				input_layer[0] = (double)(input1 - 48);
				input_layer[1] = (double)(input2 - 48);
				feedforward(&network, input_layer, o);
				printf("%i XOR %i = %i\n", (input1 - 48), (input2 - 48), eval(o[0]));
			}
			else 
			{
				printf("Value not accepted (value = 0 or 1).\n");
			}
		}
		else 
		{
			printf("Value not accepted:\n");
			printf("Accepted: 0 or 1\n");
			printf("Accepted: q -> quit\n");
			printf("Accepted: t -> train\n");
			printf("Accepted: l -> load\n");
			printf("Accepted: s -> save\n");
		}

		input1 = getchar();
		printf("\n");
	}
	//train(&network);
	//load(&network, "save.txt");

	/*printf("Before training:\n");
	size_t i;
	double o[1];
	double inputlayer[2];

	for (i = 0; i < 4; i++)
	{
		inputlayer[0] = input[i][0];
		inputlayer[1] = input[i][1];
		feedforward(&network, inputlayer, o);
		printf("%f XOR %f -> expected value: %f | network value: %f\n", input[i][0], input[i][1], expected[i], o[0]);
	}

	unsigned long max_it = 1000;
	printf("\n");
	printf("-----------------------\n");
	printf("training...\n");
	train_network(&network, input, expected, max_it);
	printf("-----------------------\n");
	printf("\n");
	
	printf("After training (%lu iterations):\n", max_it);
	for (i = 0; i < 4; i++)
	{
		inputlayer[0] = input[i][0];
		inputlayer[1] = input[i][1];
		feedforward(&network, inputlayer, o);
		printf("%f XOR %f -> expected value: %f | network value: %f\n", input[i][0], input[i][1], expected[i], o[0]);
	}
	
	printf("\n");
	printf("-----------------------\n");
	printf("training...\n");
	train_network(&network, input, expected, max_it);
	printf("-----------------------\n");
	printf("\n");
	
	printf("After training (%lu iterations):\n", 2 * max_it);
	for (i = 0; i < 4; i++)
	{
		inputlayer[0] = input[i][0];
		inputlayer[1] = input[i][1];
		feedforward(&network, inputlayer, o);
		printf("%f XOR %f -> expected value: %f | network value: %f\n", input[i][0], input[i][1], expected[i], o[0]);
	}

	save(&network, "save.txt");*/
}
