#include "input_sudoku.h"

//je fais la déclaration ici parce que g la flemme de le faire dans autre fichier pour ce cas
void print_grid(int grid[9][9]);

void print_grid(int grid[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			printf("%d", grid[i][j]);
		printf("\n");
	}
}

int main()
{
	int grid[9][9], i, j;
	
	initialisation_array(grid);
	input_sudoku("sudoku_test.txt", grid);

	print_grid(grid);

	return 0;
}
