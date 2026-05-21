#include <stdio.h>
#include <solvers.h>
#include <grid.h>
#include <stdlib.h>

#include "io.h"

void runLinearConvection(int gridSize, int timeSteps, double deltaT)
{

    const char solverType = 'l';
    double dx = (float)2 / (gridSize - 1);
    double c = 1; // default wavespeed of one
    const int n1 = 0.5/dx;
    const int n2 = 1/dx + 1;

    double *u = allocate_1d_array(gridSize);
    double *un = allocate_1d_array(gridSize);

    // set u = 2 between 0.5 and 1 per our ICs
    for (int i = 0; i < gridSize; i++)
    {
        if ( n1 < i && i <= n2 )
        {
            u[i] = 2;
        }
        else u[i]=1;
    }

    //plotting u(x) at t=0 w/ ICs
    generate_1d_csv(u,gridSize);


    // FD numerical scheme FwD in time and BwD in space
    for (int n = 0; n < timeSteps; n++)
    {
        un = copy_1d_array(u,un,gridSize);
        for (int i = 1; i < gridSize; i++)
        {
            u[i] =  un[i] - (c * deltaT/dx) * (un[i] - un[i-1]);
        }
        // just plotting every time step since its a basic simulation / not much data
        write_1d_row(u,gridSize);
    }

    const char *output = "outputs/output.csv";
    printf("Writing to file: %s\n", output);
    plot1d_csv(output,solverType,deltaT);

    free(u);
    free(un);

}
