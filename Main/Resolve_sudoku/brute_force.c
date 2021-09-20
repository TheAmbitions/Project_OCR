#include "isSafe.h"

int BruteSolve (int grid[9][9], int row, int col)
{
	if (row == 8 && col == 9)
		return 1;
	if (col == 9)
	{
		row++;
          	col = 0;
       	}

 if (grid[row][col] > 0)
	         return BruteSolve(grid, row, col + 1);
  
     for (int num = 1; num <= 9; num++)
     {
		if (isSafe(grid, row, col, num)==1)
		{
			grid[row][col] = num;
			if (BruteSolve (grid, row, col + 1)==1)
				return 1;
		}
		grid[row][col] = 0;
     }
     return 0;
}

