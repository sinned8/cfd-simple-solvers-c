# Simple Solvers in C - Summer 2026

A small C-based numerical solver project for learning basic computational fluid dynamics methods.

This project is inspired by the Barba Group CFD Python lessons, but the goal is to build an original C solver framework rather than directly translate the notebooks line by line. The C code handles the numerical computation, while Python is used only for plotting and visualization.

## Current Features

- Menu-driven terminal interface
- 1D linear convection solver - animated
- 1D Burgers' equation solver - animated
- 2D Poisson equation solver - animated
- Basic grid and memory utilities
- CSV output from C
- Python plotting from generated CSV data
- Solver-specific plot formatting


## Implemented Solvers

### 1D Linear Convection

Uses a finite difference method with forward difference in time and backward difference in space.

### 1D Burgers' Equation

Uses a finite difference method with:

- nonlinear convection
- diffusion/viscosity
- periodic boundary conditions

### 2D Poisson Equation

Uses a finite difference method to solve for a pressure-like scalar field in a 2D domain.

Unlike the 1D solvers, this is not a time-marching wave problem. The solution is found iteratively by updating each grid point using its neighboring values and the source term.

The result shows a smooth pressure surface with high-pressure and low-pressure regions caused by the source term.

## Requirements

### C

A C compiler and CMake are required. This project is currently developed using CLion.

### Python

Python is used only for plotting.

Required packages:

```bash
pip install numpy pandas matplotlib
```

## Running the Project

Build and run the C program from the project root directory.

The program displays a solver menu:

```text
-= Simple Fluid Solvers by Dennis Dust =-

1) 1D Linear Convection
2) 1D Burgers' Equation
3) 2D Poisson Equation
```

For 1D solvers, the program runs the simulation and writes the final result to:

```text
outputs/output.csv
```

For 2D Solvers, depending on the type, the simulation is ran and the output per iteration is stored in files like "pressure_iterations" 
```text
outputs/pressure_iterations/p_0001.csv
```
Where p_0001.csv would denote the first iteration ran.


Then the Python plotting script displays the solution.

## Notes

The plotting script changes behavior depending on which solver is selected.

Examples:

- Linear convection is plotted over `[0, 2]`
- Burgers' equation is plotted over `[0, 2π]`
- 2D Poisson equation is plotted over `[0, 2]` for x and `[0, 1]` for y

## Planned Improvements

- Separate output files for each solver
- Better input validation
- Analytical solution comparisons
- Error calculations
- Grid refinement studies
- Runtime scaling tests
- Additional 1D and 2D solvers
- More documentation for each method

## References and Acknowledgements

This project uses the following materials as learning references:

- Barba Group, *CFD Python: 12 Steps to Navier-Stokes*  
  https://github.com/barbagroup/CFDPython

- Lorena A. Barba and collaborators, CFD Python educational materials  
  https://lorenabarba.com/blog/cfd-python-12-steps-to-navier-stokes/

- John D. Anderson Jr., *Computational Fluid Dynamics: The Basics with Applications*  
  Used for theoretical background and conservation-law review.


## Author

Dennis Dust
