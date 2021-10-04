#include <math.h>

double sigmoid(double x)
{
	return 1.0 / (1.0 + exp(x));
}

double sigmoid_prime(double x)
{
	return x * (1.0 - x);
}
