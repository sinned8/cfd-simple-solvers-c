#include <solvers.h>
#include "grid.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"



void run2DCavityFlowGridRefinement()
{

    // f1 fine f2 medium f3 coarse
    double f1 = 0.0, f2 = 0.0, f3 = 0.0,r = 2.0;


    printf("\nStarting coarse grid run");
    f3 = run2DCavityFlow(41,41,500,true);
    printf("\nCoarse grid run done.");
    printf("\nStored coarse result in f3: %.10f\n", f3);

    printf("\nStarting medium grid run...");
    f2 = run2DCavityFlow(81,81,500,true);
    printf("\nMedium grid run done.");
    printf("\nStored medium result in f2: %.10f\n", f2);

    printf("\nStarting fine grid run...");
    f1 = run2DCavityFlow(161,161,500,true);
    printf("\nFine grid run done.");
    printf("\nStored fine result in f1: %.10f\n", f1);


    printf("\n\t-=Lid Driven Cavity Grid Refinement=-");
    printf("\nMeasurement u at: x = 1.0 y = 0.5");
    printf("\nGrid size\tFvalue");
    printf("\n41x41\t\t%f",f3);
    printf("\n81x81\t\t%f",f2);
    printf("\n161x161\t\t%f",f1);
    printf("\nRefinement ratio: %f",r);
    double ratio = fabs((f3 - f2) / (f2 - f1));
    double p = log(ratio) / log(r);
    printf("\nObserved order: %.6f\n", p);

}