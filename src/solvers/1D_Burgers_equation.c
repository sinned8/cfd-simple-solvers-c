#include <stdio.h>
#include <solvers.h>
#include <grid.h>
#include <stdlib.h>
#include <math.h>
#include "io.h"



// need a function to replace the line ufunc = lambdify((t, x, nu), u)
// this creates a callable function similar to lambdify in python, this is a numerical function that can be used when
// looping through the grid to calculate u at each space point x and  at each time point t, also passing in the viscosity constant
double burgersNumFunc(double x,double t,double nu)
{

    //used print(ccode(phi)) & print(ccode(phiprime)) in python to obtain these
    double phi = exp(-1.0/4.0*pow(-4*t + x - 2*M_PI, 2)/(nu*(t + 1))) + exp(-1.0/4.0*pow(-4*t + x, 2)/(nu*(t + 1)));
    double phiprime = -1.0/4.0*(-8*t + 2*x)*exp(-1.0/4.0*pow(-4*t + x, 2)/(nu*(t + 1)))/(nu*(t + 1)) - 1.0/4.0*(-8*t + 2*x - 4*M_PI)*exp(-1.0/4.0*pow(-4*t + x - 2*M_PI, 2)/(nu*(t + 1)))/(nu*(t + 1));

    return -2 * nu * (phiprime/phi) + 4;

}




void run1DBurgersEquation(int nx, int nt,double vis)
{
    const char solverType = 'b';
    const double dx = 2 * M_PI / (nx - 1);
    const double dt = dx * vis;
    double * u = allocate_1d_array(nx);
    double * un = allocate_1d_array(nx);

    // IC
    for (int i = 0; i < nx; i++)
    {
        double x = i * dx;
        u[i] = burgersNumFunc(x,0.0,vis);
    }

    //finite difference loop: fwd in time central d in space along with periodic boundary conditions
    for (int n = 0; n < nt; n++)
    {
        un = copy_1d_array(u,un,nx);

        for (int i = 0; i < nx-1; i++)
        {
            u[i] = un[i] - un[i] * (dt / dx) * (un[i] - un[i-1]) + vis * (dt / pow(dx,2))
            * (un[i+1] - 2 * un[i] + un[i-1]);
        }
        u[0] = un[0] - un[0] * (dt / dx) * (un[0] - un[nx-2]) + vis * (dt / pow(dx,2)) * (un[1] - 2 * un[0] + un[nx-2]);
        u[nx-1] = u[0];
    }



    generate_1d_csv(u,nx);
    const char *output = "outputs/output.csv";
    printf("Writing to file: %s\n", output);
    plot1d_csv(output,solverType);

    free(u);
    free(un);


}

