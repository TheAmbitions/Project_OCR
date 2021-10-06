#ifndef MATRIX_H
#define MATRIX_H

void transpose(int line, int col, double m[line][col], double mt[col][line]);
void matrix_sum(int line, int col, double m1[line][col], double m2[line][col], double add[line][col]);
void matrix_product(int linem1, int linem2, int colm2, double m1[linem1][linem2], double m2[linem2][colm2], double mul[linem1][colm2]);

#endif
