#include <stdio.h>
#include <solvers.h>

int DEFAULT_GRID_SIZE = 41 , DEFAULT_TIMESTEPS = 25; // nx, nt
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
           "\nSelection... ");
    scanf("%d", &choice);
    getchar();
    solverSelector(choice);

}

void solverSelector(int c)
{


    char choice;
    int gridSize, timeSteps;
    double deltaT,viscosity;


    switch (c)
    {
        case 1:
            printf("\n\t1D Linear Convection has been selected.");

            printf("\nWould you like parameters different than the defaults of:"
                   "\nGRID SIZE: 41"
                   "\nTIME STEPS: 25"
                   "\nDELTA T: 0.025?"
                   "\n(Y/N): ");
            scanf("%c", &choice);
            getchar();
            if (choice == 'Y' || choice == 'y')
            {
                printf("\nEnter the grid size: ");
                scanf("%d", &gridSize);
                printf("\nEnter the time steps: ");
                scanf("%d", &timeSteps);
                printf("\nEnter the delta T: ");
                scanf("%lf", &deltaT);
                runLinearConvection(gridSize, timeSteps, deltaT);
            }
            else runLinearConvection(DEFAULT_GRID_SIZE, DEFAULT_TIMESTEPS, DEFAULT_DELTA_T);


            break;

        case 2:
            printf("\n\t1D Burgers equation has been selected.");
            printf("\nWould you like parameters different than the defaults of:"
                   "\nGRID SIZE: 41"
                   "\nTIME STEPS: 25"
                   "\nVISCOSITY: 0.07?"
                   "\n(Y/N): ");
            scanf("%c", &choice);
            getchar();
            if (choice == 'Y' || choice == 'y')
            {
                printf("\nEnter the grid size: ");
                scanf("%d", &gridSize);
                printf("\nEnter the time steps: ");
                scanf("%d", &timeSteps);
                printf("\nEnter the viscosity: ");
                scanf("%lf", &viscosity);
                run1DBurgersEquation(gridSize, timeSteps, viscosity);
            }
            else run1DBurgersEquation(DEFAULT_GRID_SIZE, DEFAULT_TIMESTEPS, DEFAULT_VIS);
            break;


            default:
                break;
    }
}

