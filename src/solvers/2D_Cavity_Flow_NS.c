#include <solvers.h>
#include "grid.h"
#include <math.h>
#include <stdio.h>

#include "io.h"


double **bracketDiscretizedPressurePoisson(double **b,double rho, double dt,
                                           double **u,double **v,double dx,double dy,
                                           int nx,int ny);

double **pressurePoisson(double **p, double ** pn,double ** b, double dx, double dy,
                         int nx,int ny);


double max_change(double **u, double **v, double **un, double **vn, int nx, int ny);

double maxPressureChange(double **p, double **pn, int nx, int ny);

double run2DCavityFlow(int nx, int ny,int nt, bool GRS)
{

    int xmin = 0, xmax = 2, ymin = 0, ymax = 2,frameNumber = 0;
    double dx = compute_dx(nx,xmin,xmax);
    double dy = compute_dy(ny,ymin,ymax);
    double dx2 = dx * dx;
    double dy2 = dy * dy;

    //dt needs to change for grid refinement study (GRS) 181x181 since D would be > 1/2
    double rho = 1, nu = 0.1 , dt = 0.001 , max_change_val = 1;
    char solverType = 'C';


    if (GRS)
    {
        dt = 0.00025;
    }

    double **u = allocate_2d_array(ny,nx);
    double **v = allocate_2d_array(ny,nx);
    double **p = allocate_2d_array(ny,nx);
    double **b = allocate_2d_array(ny,nx);
    double **un = allocate_2d_array(ny,nx);
    double **vn = allocate_2d_array(ny,nx);
    double **pn = allocate_2d_array(ny,nx);
    copy_2d_array(p,pn,ny,nx);

    clear_2d_pressure_iteration_folder();
    clear_2d_u_velocity_folder();
    clear_2d_v_velocity_folder();



    //since dt would be different for last GRS nt does not represent the same amount of simulated time
    //so instead we can loop until the velocity field stops changing significantly -> becomes "steady"
    if (GRS)
    {


        int iteration = 0,max_iterations = 50000;
        double change_rate = max_change_val / dt;

        printf("\nnx = %d, dx = %f, dt = %f\n", nx, dx, dt);
        //this loop then relies on max_change
        while ((change_rate > 0.0001 || iteration < 2) && iteration < max_iterations)
        {
            copy_2d_array(u,un,ny,nx);
            copy_2d_array(v,vn,ny,nx);

            b = bracketDiscretizedPressurePoisson(b,rho,dt,u,v,dx,dy,nx,ny);
            p = pressurePoisson(p,pn,b,dx,dy,nx,ny);

            if (iteration % 100 == 0)
            {
                printf("\nGrid size %d: iteration = %d, max change = %.8e , change rate = %.8e",
                       nx, iteration, max_change_val,change_rate);
            }
            //velo is stored in un & vn so when max_change is very small
            //flow is relatively "steady" ( <10^-6 or smth)
            for (int i=1; i<ny - 1; i++)
            {
                for (int j=1; j<nx - 1; j++)
                {

                    u[i][j] = (un[i][j] - (un[i][j] * (dt/dx) *(un[i][j] - un[i][j-1]))
                                - (vn[i][j]* (dt/dy) *(un[i][j] - un[i-1][j]))
                                - ((dt/ (2*rho*dx)) * (p[i][j+1] - p[i][j-1]) )
                                + nu * ( (dt / dx2 ) * (un[i][j+1] - (2 * un[i][j]) + un[i][j-1] )
                                + ((dt/dy2) * (un[i+1][j] - (2*un[i][j]) + un[i-1][j]) ) )  );

                    v[i][j] = (vn[i][j] - (un[i][j] * (dt/dx) *(vn[i][j] - vn[i][j-1]))
                                - (vn[i][j] * (dt/dy) *(vn[i][j] - vn[i-1][j]))
                                - ((dt/ (2*rho*dy)) * (p[i+1][j] - p[i-1][j]) )
                                + nu * ( (dt / dx2 ) * (vn[i][j+1] - (2 * vn[i][j]) + vn[i][j-1] )
                                + ((dt/dy2) * (vn[i+1][j] - (2*vn[i][j]) + vn[i-1][j]) ) ) );

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

            //max change here after applying ICs and BCs
            max_change_val = max_change(u,v,un,vn,nx,ny);
            change_rate = max_change_val / dt;
            //check
            if (!isfinite(max_change_val))
            {
                printf("\nNaN/Inf detected at iteration %d\n", iteration);
                break;
            }
            ++iteration;
        }
        //write the value of u at x = 1 , y = 0.5 to Grid_Refinement_Study
        int i = (ny - 1) / 4;
        int j = (nx - 1) / 2;

        double fval = u[i][j];

        free_2d_array(u,ny);
        free_2d_array(v,ny);
        free_2d_array(b,ny);
        free_2d_array(p,ny);
        free_2d_array(un,ny);
        free_2d_array(vn,ny);
        free_2d_array(pn,ny);
        return fval;
    }

    if (!GRS)
    {
        for (int n = 0; n < nt; n++)
        {
            copy_2d_array(u,un,ny,nx);
           copy_2d_array(v,vn,ny,nx);

           b = bracketDiscretizedPressurePoisson(b,rho,dt,u,v,dx,dy,nx,ny);
           p = pressurePoisson(p,pn,b,dx,dy,nx,ny);

           //velo is stored in un & vn so when max_change = max(|uij- unij|, |vij - vnij|) is very small
           //flow is relatively "steady" ( <10^-6 or smth)
           for (int i=1; i<ny - 1; i++)
           {
               for (int j=1; j<nx - 1; j++)
               {
                   u[i][j] = (un[i][j] - (un[i][j] * (dt/dx) *(un[i][j] - un[i][j-1]))
                               - (vn[i][j]* (dt/dy) *(un[i][j] - un[i-1][j]))
                               - ((dt/ (2*rho*dx)) * (p[i][j+1] - p[i][j-1]) )
                               + nu * ( (dt / dx2 ) * (un[i][j+1] - (2 * un[i][j]) + un[i][j-1] )
                               + ((dt/dy2) * (un[i+1][j] - (2*un[i][j]) + un[i-1][j]) ) )  );

                   v[i][j] = (vn[i][j] - (un[i][j] * (dt/dx) *(vn[i][j] - vn[i][j-1]))
                               - (vn[i][j] * (dt/dy) *(vn[i][j] - vn[i-1][j]))
                               - ((dt/ (2*rho*dy)) * (p[i+1][j] - p[i-1][j]) )
                               + nu * ( (dt / dx2 ) * (vn[i][j+1] - (2 * vn[i][j]) + vn[i][j-1] )
                               + ((dt/dy2) * (vn[i+1][j] - (2*vn[i][j]) + vn[i-1][j]) ) ) );

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

           //disable this if were doing a GRS, so func needs another var to say were doing a GRS
           if (n % 20 == 0)
           {
               generate_2d_pressure_iteration_csv(p,ny,nx,frameNumber);
               generate_2d_u_velocity_csv(u,ny,nx,frameNumber);
               generate_2d_v_velocity_csv(v,ny,nx,frameNumber);
               frameNumber++;
           }


        }
        free_2d_array(u,ny);
        free_2d_array(v,ny);
        free_2d_array(b,ny);
        free_2d_array(p,ny);
        free_2d_array(un,ny);
        free_2d_array(vn,ny);
        free_2d_array(pn,ny);

    }


    plot2d_csv(solverType);
    return 1;


}


double **bracketDiscretizedPressurePoisson(double **b,double rho, double dt,
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
    }

    return b;
}

double **pressurePoisson(double **p, double ** pn,double ** b, const double dx, const double dy,
                         const int nx, const int ny)
{

    //solved iteratively, CD 5 point stencil , fixed number if iterations
    //need to change this for GRS, or list as limitation. Finer grid may require more passes compared to coarse grid...
    double dx2 = dx * dx;
    double dy2 = dy * dy;
    const int max_iterations = 10000;
    const double pressure_tolerance = 1e-6;
    //fixed mem allocation -> moved **pn allocation & freeing into run2DCavityFlow() func
    //decreases the need to realoc a new massive array every timestep...
    //now running until the change in pressure is sufficiently small...
    for (int it = 0; it < max_iterations; it++)
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

        double pressure_change = maxPressureChange(p,pn,nx,ny);
        if (pressure_change < pressure_tolerance)
        {
            break;
        }
        if (it == max_iterations)
        {
            printf("\nWarning: pressure did not converge.");
            printf("\nFinal pressure change: %.8e\n", pressure_change);
        }

    }

    return p;
}

//this is being called each time step
double max_change(double **u, double **v, double **un, double **vn, int nx, int ny)
{
    //searches the entire grid for the velocity value that
    //changed the most during the most recent time step.
    //umax uij - unij & vmax vij - vnij
    //max_change = fmax(umax,vmax)

    double largest_change = 0.0;
    for (int i = 1; i < ny - 1 ; ++i)
    {
        for (int j = 1; j < nx - 1; ++j)
        {
            double du = fabs(u[i][j] - un[i][j]);
            double dv = fabs(v[i][j] - vn[i][j]);

            double local_change = fmax(du, dv);
            largest_change = fmax(largest_change, local_change);
        }
    }
    return largest_change;
}



double maxPressureChange(double **p, double **pn, int nx, int ny)
{
    double largest_change = 0.0;
    for (int i = 1; i < ny - 1 ; ++i)
    {
        for (int j = 1; j < nx - 1; ++j)
        {
            double dp = fabs(p[i][j] - pn[i][j]);
            largest_change = fmax(largest_change, dp);
        }
    }
    return largest_change;
}