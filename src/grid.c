#include "grid.h"
#include <stdio.h>
#include <stdlib.h>

double *allocate_1d_array(const int nx)
{
    double *array = (double*) malloc(nx * sizeof(double));
    return array;

}

double* copy_1d_array( double *arraycp, double*arraypst, int n)
{

    for (int i = 0; i < n; i++)
    {
        arraypst[i] = arraycp[i];
    }
    return arraypst;
}
void free_1d_array(double *array)
{
    free(array);
}

