#ifndef GRID_H
#define GRID_H

double compute_dx(int nx, double xmin, double xmax);
double compute_dy(int ny, double ymin, double ymax);

//1 d array
double *allocate_1d_array(int nx);
void free_1d_array(double *array);
//2d arrayint


double *copy_1d_array( double *arraycp, double*arraypst, int n);


void create_1d_grid(double *x, int nx, double xmin, double xmax);



#endif