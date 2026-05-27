#include "grid.h"
#include <stdlib.h>
#include <string.h>

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

double **allocate_2d_array(const int ny, const int nx)
{
    double **array = (double**) malloc(ny * sizeof(double*));
    for (int i = 0; i < ny; i++)
    {
        array[i] = (double*) calloc(nx , sizeof(double));
    }
    return array;
}

void free_2d_array(double **array,double ny)
{
    for (int i = 0; i < ny; i++)
        free(array[i]);
    free(array);
}

double compute_dx(double nx, int xmin, int xmax)
{
    return ((xmax - xmin) / ( nx - 1));
}

double compute_dy(double ny, int ymin, int ymax)
{
    return ((ymax - ymin) / (ny - 1));
}

double **copy_2d_array( double **arraycp,double **arraypst,int ny, int nx)
{
        for (int i = 0; i < ny; i++)
        {
            arraypst[i] = malloc(nx * sizeof(double));
            memcpy(arraypst[i],arraycp[i],nx * sizeof(double));
        }
    return arraypst;
}