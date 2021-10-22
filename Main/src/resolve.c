#include "input_sudoku.h"
#include "output_sudoku.h"
#include "brute_force.h"

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

void resolve()
{
	int grid[9][9];

	initialisation_array(grid);
	input_sudoku("../data/sudoku_test.txt", grid);

	// Draw the grid before its resolution 
	print_grid(grid);

	//Résolution du sudoku
	// Resolve the grid
	if (BruteSolve(grid, 0, 0) == 1)
	{
		output_sudoku(grid);
		printf("\n");
		// Print the grid solution with Ok if a solution exist
		print_grid(grid);
	}
	// No solution exists
	else
		printf("No solution exists\n");
}
