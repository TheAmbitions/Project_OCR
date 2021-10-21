#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define MAX_SIZE 12

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
