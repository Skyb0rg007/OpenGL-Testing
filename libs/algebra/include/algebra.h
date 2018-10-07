#ifndef ALGEBRA_H_INCLUDED
#define ALGEBRA_H_INCLUDED

#include <gsl/gsl_matrix.h>

gsl_matrix_float *ortho(float left, float right, float bottom, float top);

#endif
