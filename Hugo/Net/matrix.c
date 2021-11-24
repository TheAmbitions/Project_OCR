#include <stddef.h>

void transpose(size_t line, size_t col, double m[line][col], double mt[col][line])
{
	size_t i,j;

	for (i = 0; i < line; i++)
		for (j = 0; j < col; j++)
			mt[j][i] = m[i][j];
}

void matrix_product(size_t linem1, size_t linem2, size_t colm2, double m1[linem2], double m2[linem2][colm2], double mul[colm2])
{
	size_t i, j, k;
	
	for (i = 0; i < linem1; i++)
		for (j = 0; j < colm2; j++)
		{
			mul[j] = 0;
			for (k = 0; k < linem2; k++)
				mul[j] += m1[k] * m2[k][j];
		}
}
