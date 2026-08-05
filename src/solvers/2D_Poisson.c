#include <stdio.h>
#include <solvers.h>
#include "grid.h"
#include <math.h>

#include "io.h"

void run2DPoissonEquation(int nx, int ny,int nt)
{
    const int xmin = 0, xmax = 2, ymin = 0, ymax = 1;
    double dx = compute_dy(nx, xmin, xmax);
    double dy = compute_dy(ny, ymin, ymax);
    double dx2 = dx * dx;
    double dy2 = dy * dy;
    char solverType = 'p';
    int iterationNum = 1;

    // Initialize all the 2d arrays
    double **p = allocate_2d_array(ny, nx);
    double **pd = allocate_2d_array(ny, nx);
    double ** b = allocate_2d_array(ny, nx);

    //handling initial spikes in source term b inside the domain
    const int bi1 = ny / 4 , bi2 = 3 * ny / 4 , bj1  = nx / 4 , bj2 = 3 * nx / 4;
    b[bi1][bj1] = 100 , b[bi2][bj2] = -100;

    clear_2d_pressure_iteration_folder();
    //initial time loop
    for (int it = 0; it < nt; it++)
    {
        pd = copy_2d_array(p,pd,ny,nx);

        //space loop for x and y
        for (int i = 1; i < ny - 1; i++)
        {
            for (int j = 1; j < nx - 1; j++)
            {
                p[i][j] = ( ( ((pd[i][j+1] + pd[i][j-1]) * dy2) + ((pd[i+1][j] + pd[i-1][j]) * dx2)
                    - (b[i][j] * dx2 * dy2 ))
                    / (2 * (dx2 + dy2 ) ));
            }
        }

        // given boundary conditions
        for (int j = 0; j < nx; j++)
        {
            p[0][j] = 0.0;
            p[ny - 1][j] = 0.0;
        }
        for (int i = 0; i < ny; i++)
        {
            p[i][0] = 0.0;
            p[i][nx - 1] = 0.0;
        }


        if (it % 2 == 0)
        {
            generate_2d_pressure_iteration_csv(p,ny,nx,iterationNum);
            iterationNum++;
        }
    }

    plot2d_csv(solverType);
    free_2d_array(p,ny);
    free_2d_array(b,ny);
    free_2d_array(pd,ny);







}
