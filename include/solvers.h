#ifndef SOLVERS_H
#define SOLVERS_H
#include <stdbool.h>

void runLinearConvection(int nx, int nt,double dt);

void run1DBurgersEquation(int nx, int nt,double vis);

void run2DPoissonEquation(int nx, int ny,int nt);

double run2DCavityFlow(int nx, int ny,int nt, bool GRS);

void run2DChannelFlow(int nx, int ny,int nt);

void run2DCavityFlowGridRefinement();

#endif
