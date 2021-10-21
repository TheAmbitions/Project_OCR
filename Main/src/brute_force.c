#include "isSafe.h"

int BruteSolve (int grid[9][9], int row, int col)
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
		if (isSafe(grid, row, col, num)==1)
		{
			grid[row][col] = num;
			// Checking for next possibility with next column 
			if (BruteSolve (grid, row, col + 1)==1)
				return 1;
		}
		// If num is not the good number we remove it and 
		// we restart with an other number
		grid[row][col] = 0;
     }
     return 0;
}

