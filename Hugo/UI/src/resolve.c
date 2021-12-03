#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define MAX_SIZE 12

void print_grid(int grid[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			printf("%d", grid[i][j]);
		printf("\n");
	}
}

void initialisation_array(int grid[9][9])
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			grid[i][j] = 0;
}

void input_sudoku(char path[], int grid[9][9])
{
	FILE* file = NULL;

	file = fopen(path, "r");

	if (file != NULL)
	{
		char str[MAX_SIZE] = "";

		for (int i = 0; i < 9 && fgets(str, MAX_SIZE, file) != NULL; i++)
			for (int j = 0; j < 9; j++)
			{
				if (str[j] >= 49 && str[j] <= 57)
					grid[i][j] = str[j] - 48;
			}

		fclose(file);
	}
	else
		errx(1, "Error : the path does not exist");
}

void output_sudoku(int grid[9][9], char* path)
{
	FILE* file = NULL;

	file = fopen(path, "w");

	if (file != NULL)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
				fprintf(file, "%d", grid[i][j]);
			fprintf(file, "\n");
		}

		fclose(file);
	}
	else
		errx(1, "Error : cannot create the file");
}

int isSafe(int grid[9][9], int row, int col, int num)
{
	for (int x = 0; x <= 8; x++)
		if (grid[row][x] == num)
			return 0;

	for (int x = 0; x <= 8; x++)
		if (grid[x][col] == num)
			return 0;

	int startRow = row - row % 3,
		startCol = col - col % 3;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (grid[i + startRow][j +
				startCol] == num)
				return 0;

	return 1;
}

int BruteSolve(int grid[9][9], int row, int col)
{
	// Check if we have reached the  8th row and 9th column
	// We return 1 (True) to avoid further backtracking 
	if (row == 8 && col == 9)
		return 1;
	// Check if we are at the 9th column to pass at the begening of 
	// the  next row 
	if (col == 9)
	{
		row++;
		col = 0;
	}

	// Check if the current position already contain a value (another than 0)
	if (grid[row][col] > 0)
		return BruteSolve(grid, row, col + 1);

	for (int num = 1; num <= 9; num++)
	{
		// Check if it safe to place to place a number between 1 
	// and 9 in the current position
	// We iterate for the next column
		if (isSafe(grid, row, col, num) == 1)
		{
			grid[row][col] = num;
			// Checking for next possibility with next column 
			if (BruteSolve(grid, row, col + 1) == 1)
				return 1;
		}
		// If num is not the good number we remove it and 
		// we restart with an other number
		grid[row][col] = 0;
	}
	return 0;
}

int resolve()
{
	int grid[9][9];

	initialisation_array(grid);
	input_sudoku("../data/sudoku.txt", grid);
	// Resolve the grid
	if (BruteSolve(grid, 0, 0) == 1)
	{
		output_sudoku(grid, "../data/result_grid.txt");
		return 1;
	}
	// No solution exists
	else
		return 0;
}
