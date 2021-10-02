#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void output_sudoku(int grid[9][9])
{
	FILE* file = NULL;

	file = fopen("grid_result.txt", "w");

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
