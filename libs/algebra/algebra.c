#include "algebra.h"
#include <gsl/gsl_matrix.h>

gsl_matrix_float *ortho(float left, float right, float bottom, float top)
{
    gsl_matrix_float *mat = gsl_matrix_float_alloc(4, 4);
    gsl_matrix_float_set_identity(mat);

    gsl_matrix_float_set(mat, 0, 0, 2 / (right - left));
    gsl_matrix_float_set(mat, 1, 1, 2 / (top - bottom));
    gsl_matrix_float_set(mat, 2, 2, -1);
    gsl_matrix_float_set(mat, 3, 0, -(right + left) / (right - left));
    gsl_matrix_float_set(mat, 3, 1, -(top + bottom) / (top - bottom));

    return mat;
}
