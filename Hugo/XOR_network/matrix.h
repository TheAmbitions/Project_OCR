#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

void transpose(size_t line, size_t col, double m[line][col], double mt[col][line]);
void matrix_sum(size_t line, size_t col, double m1[line][col], double m2[line][col], double add[line][col]);
void matrix_product(size_t linem1, size_t linem2, size_t colm2, double m1[linem1][linem2], double m2[linem2][colm2], double mul[linem1][colm2]);

#endif
