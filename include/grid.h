#ifndef GRID_H
#define GRID_H

double compute_dx(double nx, int xmin, int xmax);
double compute_dy(double ny, int ymin, int ymax);

//1 d array
double *allocate_1d_array(int nx);
void free_1d_array(double *array);

//2d array
double **allocate_2d_array(int ny, int nx);
void free_2d_array(double **array,double ny);

double *copy_1d_array( double *arraycp, double*arraypst, int n);

double **copy_2d_array( double **arraycp,double **arraypst,int ny, int nx);




#endif