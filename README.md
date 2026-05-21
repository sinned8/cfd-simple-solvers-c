# Simple Solvers in C - Summer 2026

A small C-based numerical solver project for learning basic computational fluid dynamics methods.

This project is inspired by the Barba Group CFD Python lessons, but the goal is to build an original C solver framework rather than directly translate the notebooks line by line. The C code handles the numerical computation, while Python is used only for plotting and visualization.

## Current Features

- Menu-driven terminal interface
- 1D linear convection solver - animated
- 1D Burgers' equation solver - animated
- Basic grid and memory utilities
- CSV output from C
- Python plotting from generated CSV data
- Solver-specific plot formatting

## Project Structure

```text
Simple Solvers in C - Summer 2026/
│
├── include/
│   ├── grid.h
│   ├── io.h
│   └── solvers.h
│
├── src/
│   ├── main.c
│   ├── grid.c
│   ├── io.c
│   └── solvers/
│       ├── 1DLinear Convection.c
│       └── 1D Burgers equation.c
│
├── outputs/
│   └── .gitkeep
│
├── python scripts/
│   └── plot_1d.py
│
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Implemented Solvers

### 1D Linear Convection

Uses a finite difference method with forward difference in time and backward difference in space.

### 1D Burgers' Equation

Uses a finite difference method with:

- nonlinear convection
- diffusion/viscosity
- periodic boundary conditions

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
```

After a solver is selected, the program runs the simulation, writes the final result to:

```text
outputs/output.csv
```

Then the Python plotting script displays the solution.

## Notes

The plotting script changes behavior depending on which solver is selected.

Examples:

- Linear convection is plotted over `[0, 2]`
- Burgers' equation is plotted over `[0, 2π]`

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
