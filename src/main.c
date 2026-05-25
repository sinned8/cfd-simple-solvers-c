#include <stdio.h>
#include <solvers.h>

int DEFAULT_GRID_SIZEX = 50 , DEFAULT_TIMESTEPS = 25 , DEFAULT_GRID_SIZEY = 50; // nx, nt
double DEFAULT_DELTA_T = 0.025, DEFAULT_VIS = 0.07; // delta t , viscosity(nu)

// maybe include default wave speed and viscosity constants

void solverSelector(int c);
double inputSelect();

int main()
{


    // display welcome message, have user select which solver they want to use then ask info for grid points, time steps
    // and any other solver specific variables
    // Note: For now ICs for 1d are s.t u = 2 between 0.5 & 1 , u = 1 everywhere else


    int choice;
    printf("\t-= Simple Fluid Solvers by Dennis Dust =-");
    printf("\n\nPlease select one of the following solvers: "
           "\n1) 1D Linear Convection"
           "\n2) 1D Burgers' Equation"
           "\n3) 2D Poisson Equation"
           "\nSelection... ");
    scanf("%d", &choice);
    getchar();
    solverSelector(choice);

}

void solverSelector(int c)
{


    char choice;
    int gridSizeX , gridSizey , timeSteps;
    double deltaT,viscosity;


    switch (c)
    {
        case 1:
            printf("\n\t1D Linear Convection has been selected.");

            printf("\nWould you like parameters different than the defaults of:"
                   "\nGRID SIZE: 50"
                   "\nTIME STEPS: 25"
                   "\nDELTA T: 0.025?"
                   "\n(Y/N): ");
            scanf("%c", &choice);
            getchar();
            if (choice == 'Y' || choice == 'y')
            {
                printf("\nEnter the grid size: ");
                scanf("%d", &gridSizeX);
                printf("\nEnter the time steps: ");
                scanf("%d", &timeSteps);
                printf("\nEnter the delta T: ");
                scanf("%lf", &deltaT);
                runLinearConvection(gridSizeX, timeSteps, deltaT);
            }
            else runLinearConvection(DEFAULT_GRID_SIZEX, DEFAULT_TIMESTEPS, DEFAULT_DELTA_T);


            break;

        case 2:
            printf("\n\t1D Burgers equation has been selected.");
            printf("\nWould you like parameters different than the defaults of:"
                   "\nGRID SIZE: 50"
                   "\nTIME STEPS: 25"
                   "\nVISCOSITY: 0.07?"
                   "\n(Y/N): ");
            scanf("%c", &choice);
            getchar();
            if (choice == 'Y' || choice == 'y')
            {
                printf("\nEnter the grid size: ");
                scanf("%d", &gridSizeX);
                printf("\nEnter the time steps: ");
                scanf("%d", &timeSteps);
                printf("\nEnter the viscosity: ");
                scanf("%lf", &viscosity);
                run1DBurgersEquation(gridSizeX, timeSteps, viscosity);
            }
            else run1DBurgersEquation(DEFAULT_GRID_SIZEX, DEFAULT_TIMESTEPS, DEFAULT_VIS);
            break;
        case 3:
            printf("\n\t2D Poisson Equation has been selected.");

            printf("\nWould you like parameters different than the defaults of:"
                   "\nGRID SIZE X: 50"
                   "\nGRID SIZE Y: 50"
                   "\nTIME STEPS: 25"
                   "\n(Y/N): ");
            scanf("%c", &choice);
            getchar();
            if (choice == 'Y' || choice == 'y')
            {
                printf("\nEnter the grid size x: ");
                scanf("%d", &gridSizeX);
                printf("\nEnter the grid size y: ");
                scanf("%d", &gridSizey);
                printf("\nEnter the delta T: ");
                scanf("%d", &timeSteps);
                run2DPoissonEquation(gridSizeX, gridSizey, timeSteps);
            }
            else run2DPoissonEquation(DEFAULT_GRID_SIZEX, DEFAULT_GRID_SIZEY, DEFAULT_TIMESTEPS);


            default:
                break;
    }
}

