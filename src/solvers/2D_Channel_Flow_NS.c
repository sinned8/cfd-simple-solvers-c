#include <solvers.h>
#include "grid.h"
#include <math.h>
#include "io.h"

double **sourceDiscretizedPressurePoisson(double **b,double rho, double dt,
                                           double **u,double **v,double dx,double dy,
                                           int nx,int ny);

double **pressurePoissonPeriodic(double **p,double **pn, double ** b, double dx, double dy,
                         int nx,int ny);

void run2DChannelFlow(int nx, int ny,int nt)
{
    int xmin = 0, xmax = 2, ymin = 0, ymax = 2,frameNumber = 0;
    double dx = compute_dx(nx,xmin,xmax);
    double dy = compute_dy(ny,ymin,ymax);
    double dx2 = dx * dx;
    double dy2 = dy * dy;


    double rho = 1, nu = 0.1 , dt = 0.01, F = 1.0;
    char solverType = 'H';

    double **u = allocate_2d_array(ny,nx);
    double **v = allocate_2d_array(ny,nx);
    double **p = allocate_2d_array(ny,nx);
    double **b = allocate_2d_array(ny,nx);
    double **un = allocate_2d_array(ny,nx);
    double **vn = allocate_2d_array(ny,nx);
    double **pn = allocate_2d_array(ny,nx);



    clear_2d_pressure_iteration_folder();
    clear_2d_u_velocity_folder();
    clear_2d_v_velocity_folder();

    for (int n = 0; n < nt; n++)
    {
        copy_2d_array(u,un,ny,nx);
        copy_2d_array(v,vn,ny,nx);

        b = sourceDiscretizedPressurePoisson(b,rho,dt,u,v,dx,dy,nx,ny);
        p = pressurePoissonPeriodic(p,pn,b,dx,dy,nx,ny);

        for (int i=1; i<ny - 1; i++)
        {
            for (int j=1; j<nx - 1; j++)
            {
                u[i][j] = (un[i][j] - (un[i][j] * (dt/dx) *(un[i][j] - un[i][j-1]))
                            - (vn[i][j]* (dt/dy) *(un[i][j] - un[i-1][j]))
                            - ((dt/ (2*rho*dx)) * (p[i][j+1] - p[i][j-1]) )
                            + nu * ( (dt / dx2 ) * (un[i][j+1] - (2 * un[i][j]) + un[i][j-1] )
                            + ((dt/dy2) * (un[i+1][j] - (2*un[i][j]) + un[i-1][j]) ) )
                            + (F * dt));

                v[i][j] = (vn[i][j] - (un[i][j] * (dt/dx) *(vn[i][j] - vn[i][j-1]))
                            - (vn[i][j] * (dt/dy) *(vn[i][j] - vn[i-1][j]))
                            - ((dt/ (2*rho*dy)) * (p[i+1][j] - p[i-1][j]) )
                            + nu * ( (dt / dx2 ) * (vn[i][j+1] - (2 * vn[i][j]) + vn[i][j-1] )
                            + ((dt/dy2) * (vn[i+1][j] - (2*vn[i][j]) + vn[i-1][j]) ) ) );
            }

            //Periodic BC u @ x = 2
            u[i][nx-1] = (un[i][nx-1] - (un[i][nx-1] * (dt/dx) *(un[i][nx-1] - un[i][nx-2]))
                            - (vn[i][nx-1]* (dt/dy) *(un[i][nx-1] - un[i-1][nx-1]))
                            - ((dt/ (2*rho*dx)) * (p[i][0] - p[i][nx-2]) )
                            + nu * ( (dt / dx2 ) * (un[i][0] - (2 * un[i][nx-1]) + un[i][nx-2] )
                            + ((dt/dy2) * (un[i+1][nx-1] - (2*un[i][nx-1]) + un[i-1][nx-1]) ) )
                            + (F * dt));

            //Periodic BC u @ x = 0
            u[i][0] = (un[i][0] - (un[i][0] * (dt/dx) *(un[i][0] - un[i][nx-2]))
                            - (vn[i][0]* (dt/dy) *(un[i][0] - un[i-1][0]))
                            - ((dt/ (2*rho*dx)) * (p[i][1] - p[i][nx-1]) )
                            + nu * ( (dt / dx2 ) * (un[i][1] - (2 * un[i][0]) + un[i][nx-1] )
                            + ((dt/dy2) * (un[i+1][0] - (2*un[i][0]) + un[i-1][0]) ) )
                            + (F * dt));

            // Periodic BC v @ x = 2
            v[i][nx-1] = (vn[i][nx-1] - (un[i][nx-1] * (dt/dx) *(vn[i][nx-1] - vn[i][nx-2]))
                            - (vn[i][nx-1] * (dt/dy) *(vn[i][nx-1] - vn[i-1][nx-1]))
                            - ((dt/ (2*rho*dy)) * (p[i+1][nx-1] - p[i-1][nx-1]) )
                            + nu * ( (dt / dx2 ) * (vn[i][0] - (2 * vn[i][nx-1]) + vn[i][nx-2] )
                            + ((dt/dy2) * (vn[i+1][nx-1] - (2*vn[i][nx-1]) + vn[i-1][nx-1]) ) ) );

            // Periodic BC v @ x = 0
            v[i][0] = (vn[i][0] - (un[i][0] * (dt/dx) *(vn[i][0] - vn[i][nx-2]))
                            - (vn[i][0] * (dt/dy) *(vn[i][0] - vn[i-1][0]))
                            - ((dt/ (2*rho*dy)) * (p[i+1][0] - p[i-1][0]) )
                            + nu * ( (dt / dx2 ) * (vn[i][1] - (2 * vn[i][0]) + vn[i][nx-1] )
                            + ((dt/dy2) * (vn[i+1][0] - (2*vn[i][0]) + vn[i-1][0]) ) ));

            //Wall BC: u,v = 0 @ y = 0,2
            for (int j = 0; j < nx; j++)
            {
                u[0][j] = 0;
                u[ny-1][j] = 0;
                v[0][j] = 0;
                v[ny-1][j] = 0;
            }
        }

        if (n % 2 == 0)
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
    free_2d_array(pn,ny);
}


double **sourceDiscretizedPressurePoisson(double **b,double rho, double dt,
                                           double **u,double **v,double dx,double dy,
                                           int nx,int ny)
{
    for (int i=1; i<ny - 1; i++)
    {

        for (int j=1; j<nx - 1; j++)
        {
            b[i][j] = rho * (1/dt * ( (u[i][j+1] - u[i][j-1]) / (2 * dx) +
                                        (v[i+1][j] - v[i-1][j] ) / (2 * dy) )
                            - (pow(((u[i][j+1] - u[i][j-1]) / (2 * dx)) ,2))
                            - 2 * ((u[i+1][j] - u[i-1][j]) / (2 * dy) * (v[i][j+1] - v[i][j-1]) / (2 * dx))
                            - (pow(((v[i+1][j] - v[i-1][j] ) / (2 * dy)) ,2)));

        }

        // Periodic BC for pressure Poisson source term @ x = 2
        b[i][nx-1] = rho * (1/dt * ( (u[i][0] - u[i][nx-2]) / (2 * dx) +
                                    (v[i+1][nx-1] - v[i-1][nx-1] ) / (2 * dy) )
                        - (pow(((u[i][0] - u[i][nx-2]) / (2 * dx)) ,2))
                        - 2 * ((u[i+1][nx-1] - u[i-1][nx-1]) / (2 * dy) * (v[i][0] - v[i][nx-2]) / (2 * dx))
                        - (pow(((v[i+1][nx-1] - v[i-1][nx-1] ) / (2 * dy)) ,2)));

        // Periodic BC for pressure Poisson source term @ x = 0
        b[i][0] = rho * (1/dt * ( (u[i][1] - u[i][nx-1]) / (2 * dx) +
                                    (v[i+1][0] - v[i-1][0] ) / (2 * dy) )
                        - (pow(((u[i][1] - u[i][nx-1]) / (2 * dx)) ,2))
                        - 2 * ((u[i+1][0] - u[i-1][0]) / (2 * dy) * (v[i][1] - v[i][nx-1]) / (2 * dx))
                        - (pow(((v[i+1][0] - v[i-1][0] ) / (2 * dy)) ,2)));
    }
    return b;
}


double **pressurePoissonPeriodic(double **p,double **pn,double ** b, double dx, double dy,
                         int nx,int ny)
{
    const int nit = 50;

    double dx2 = dx * dx;
    double dy2 = dy * dy;

    for (int it = 0; it < nit; it++)
    {
        copy_2d_array(p,pn,ny,nx);

        for (int i=1; i<ny - 1; i++)
        {
            for (int j=1; j<nx - 1; j++)
            {
                p[i][j] = ( ( ( (pn[i][j+1] + pn[i][j-1]) * dy2 + (pn[i+1][j] + pn[i-1][j]) * dx2)
                / (2 * (dx2 + dy2)) )
                - ((dx2 * dy2) / (2 * (dx2 + dy2) ))
                * b[i][j] );

            }

            //Periodic BC for Pressure @ x = 2
            p[i][nx-1] = ( ( ( (pn[i][0] + pn[i][nx-2]) * dy2 + (pn[i+1][nx-1] + pn[i-1][nx-1]) * dx2)
            / (2 * (dx2 + dy2)) )
            - ((dx2 * dy2) / (2 * (dx2 + dy2) ))
            * b[i][nx-1] );

            //Periodic BC for Pressure @ x = 0
            p[i][0] = ( ( ( (pn[i][1] + pn[i][nx-1]) * dy2 + (pn[i+1][0] + pn[i-1][0]) * dx2)
            / (2 * (dx2 + dy2)) )
            - ((dx2 * dy2) / (2 * (dx2 + dy2) ))
            * b[i][0] );
        }

        // Wall boundary conditions for pressure
        for (int j = 0; j < nx; j++)
        {
            p[0][j] = p[1][j];
            p[ny - 1][j] = p[ny - 2][j];
        }
    }

    return p;
}