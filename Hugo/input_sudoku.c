#include <stdio.h>
#include <stdlib.h>
#include <err.h>

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
		int currentChar = fgetc(file);
		int j = -1;
		int count = 0;

		for (int i = 0; i < 81; i++)
		{
			if (i % 9 == 0)
			{
				count = 0;
				j++;
			}
			
			
			if (currentChar >= 49 && currentChar <= 57)
			       grid[j][count] = currentChar - 48;

			currentChar = fgetc(file);
			count++;
		}

		fclose(file);
	}
	else
		errx(1, "Error : the path does not exist");	
}

