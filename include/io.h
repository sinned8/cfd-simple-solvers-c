#ifndef IO_H
#define IO_H



//some func to generate csv from array for 1d stuff
void generate_1d_csv(const double *u, int n);

// writes a new row so we can animate
void write_1d_row(const double *u, int n);

void plot1d_csv(const char * filename,char solverType,double dt);

void generate_2d_pressure_iteration_csv(double **p, int ny, int nx,int frame_number);

void plot2d_csv(char solverType);

void clear_2d_pressure_iteration_folder();

void generate_2d_u_velocity_csv( double **u, int ny, int nx, int frame_number);

void generate_2d_v_velocity_csv( double **v, int ny, int nx, int frame_number);

void clear_2d_u_velocity_folder();

void clear_2d_v_velocity_folder();
#endif
