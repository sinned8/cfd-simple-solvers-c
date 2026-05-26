#include <solvers.h>
#include "grid.h"
#include <math.h>

#include "io.h"


double **bracketDiscretizedPressurePoisson(double **b,double rho, double dt,
                                           double **u,double **v,double dx,double dy,
                                           int nx,int ny);

double **pressurePoisson(double **p,double ** b, double dx, double dy,
                         int nx,int ny);




void run2DCavityFlow(int nx, int ny,int nt)
{

    int xmin = 0, xmax = 2, ymin = 0, ymax = 2,frameNumber = 0;
    double dx = compute_dx(nx,xmin,xmax);
    double dy = compute_dy(ny,ymin,ymax);

    double rho = 1, nu = 0.1 , dt = 0.001;
    char solverType = 'C';

    double **u = allocate_2d_array(ny,nx);
    double **v = allocate_2d_array(ny,nx);
    double **p = allocate_2d_array(ny,nx);
    double **b = allocate_2d_array(ny,nx);
    double **un = allocate_2d_array(ny,nx);
    double **vn = allocate_2d_array(ny,nx);

    clear_2d_pressure_iteration_folder();
    clear_2d_u_velocity_folder();
    clear_2d_v_velocity_folder();

    for (int n = 0; n < nt; n++)
    {
        copy_2d_array(u,un,ny,nx);
        copy_2d_array(v,vn,ny,nx);

        b = bracketDiscretizedPressurePoisson(b,rho,dt,u,v,dx,dy,nx,ny);
        p = pressurePoisson(p,b,dx,dy,nx,ny);

        for (int i=1; i<ny - 1; i++)
        {
            for (int j=1; j<nx - 1; j++)
            {
                u[i][j] = (un[i][j] - (un[i][j] * (dt/dx) *(un[i][j] - un[i][j-1]))
                            - (vn[i][j]* (dt/dy) *(un[i][j] - un[i-1][j]))
                            - ((dt/ (2*rho*dx)) * (p[i][j+1] - p[i][j-1]) )
                            + nu * ( (dt / pow(dx,2) ) * (un[i][j+1] - (2 * un[i][j]) + un[i][j-1] )
                            + ((dt/pow(dy,2)) * (un[i+1][j] - (2*un[i][j]) + un[i-1][j]) ) )  );

                v[i][j] = (vn[i][j] - (un[i][j] * (dt/dx) *(vn[i][j] - vn[i][j-1]))
                            - (vn[i][j] * (dt/dy) *(vn[i][j] - vn[i-1][j]))
                            - ((dt/ (2*rho*dy)) * (p[i+1][j] - p[i-1][j]) )
                            + nu * ( (dt / pow(dx,2) ) * (vn[i][j+1] - (2 * vn[i][j]) + vn[i][j-1] )
                            + ((dt/pow(dy,2)) * (vn[i+1][j] - (2*vn[i][j]) + vn[i-1][j]) ) ) );

            }
        }

        // left and right walls
        for (int i = 0; i < ny; i++)
        {
            u[i][0] = 0.0;
            u[i][nx - 1] = 0.0;

            v[i][0] = 0.0;
            v[i][nx - 1] = 0.0;
        }

        // bottom wall and top lid
        for (int j = 0; j < nx; j++)
        {
            u[0][j] = 0.0;
            u[ny - 1][j] = 1.0;

            v[0][j] = 0.0;
            v[ny - 1][j] = 0.0;
        }

        if (n % 20 == 0)
        {
            generate_2d_pressure_iteration_csv(p,ny,nx,frameNumber);
            generate_2d_u_velocity_csv(u,ny,nx,frameNumber);
            generate_2d_v_velocity_csv(v,ny,nx,frameNumber);
            frameNumber++;
        }

    }

    plot2d_csv(solverType);
    free_2d_array(u,ny);
    free_2d_array(v,ny);
    free_2d_array(b,ny);
    free_2d_array(p,ny);
    free_2d_array(un,ny);
    free_2d_array(vn,ny);

}


double **bracketDiscretizedPressurePoisson(double **b,double rho, double dt,
                                        double **u,double **v,double dx,double dy,
                                        int nx,int ny)
{

    for (int i=1; i<ny - 1; i++)
        for (int j=1; j<nx - 1; j++)
        {
            b[i][j] = rho * (1/dt * ( (u[i][j+1] - u[i][j-1]) / (2 * dx) +
                                        (v[i+1][j] - v[i-1][j] ) / (2 * dy) )
                            - (pow(((u[i][j+1] - u[i][j-1]) / (2 * dx)) ,2))
                            - 2 * ((u[i+1][j] - u[i-1][j]) / (2 * dy) * (v[i][j+1] - v[i][j-1]) / (2 * dx))
                            - (pow(((v[i+1][j] - v[i-1][j] ) / (2 * dy)) ,2)));
        }
    return b;
}

double **pressurePoisson(double **p,double ** b, const double dx, const double dy,
                         const int nx, const int ny)
{
    //nit is pseudo time variable - helps ensure a divergence free field
    const int nit = 50;
    double **pn = allocate_2d_array(ny,nx);
    copy_2d_array(p,pn,ny,nx);

    for (int it = 0; it < nit; it++)
    {
        copy_2d_array(p,pn,ny,nx);

        for (int i=1; i<ny - 1; i++)
        {
            for (int j=1; j<nx - 1; j++)
            {
                p[i][j] = ( ( ( (pn[i][j+1] + pn[i][j-1]) * pow(dy,2) + (pn[i+1][j] + pn[i-1][j]) * pow(dx,2))
                / (2 * (pow(dx,2) + pow(dy,2))) )
                - ((pow(dx,2) * pow(dy,2)) / (2 * (pow(dx,2) + pow(dy,2)) ))
                * b[i][j] );
            }
        }

        // boundary conditions for pressure
        for (int i = 0; i < ny; i++)
        {
            p[i][nx - 1] = p[i][nx - 2];
            p[i][0] = p[i][1];

        }
        for (int j = 0; j < nx; j++)
        {
            p[0][j] = p[1][j];
            p[ny - 1][j] = 0.0;
        }


    }
    return p;
}