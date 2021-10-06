void transpose(int line, int col, double m[line][col], double mt[col][line])
{
	int i,j;

	for (i = 0; i < line; i++)
		for (j = 0; j < col; j++)
			mt[j][i] = m[i][j];
}

void matrix_sum(int line, int col, double m1[line][col], double m2[line][col], double add[line][col])
{
	int i, j;

	for (i = 0; i < line; i++)
		for (j = 0; j < col; j++)
			add[i][j] = m1[i][j] + m2[i][j];
}

void matrix_product(int linem1, int linem2, int colm2, double m1[linem1][linem2], double m2[linem2][colm2], double mul[linem1][colm2])
{
	int i, j, k;
	
	for (i = 0; i < linem1; i++)
		for (j = 0; j < colm2; j++)
		{
			mul[i][j] = 0;
			for (k = 0; k < linem2; k++)
				mul[i][j] += m1[i][k] * m2[k][j];
		}
}