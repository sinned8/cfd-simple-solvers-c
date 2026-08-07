# Simple Solvers in C - Summer 2026

A small C-based numerical solver project for learning basic computational fluid dynamics methods.

This project is inspired by the Barba Group CFD Python lessons, but the goal is to build an original C solver framework rather than directly translate the notebooks line by line. The C code handles the numerical computation, while Python is used only for plotting and visualization.

## Current Features

- Menu-driven terminal interface
- 1D linear convection solver - animated
- 1D Burgers' equation solver - animated
- 2D Poisson equation solver - animated
- 2D Lid-Driven Cavity flow solver - animated
- 2D Channel Flow solver - animated 
- Grid Refinement Study for 2D Lid-Driven Cavity flow
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

### 2D Lid-Driven Cavity Flow

Solves the incompressible 2D Navier-Stokes equations for a lid-driven cavity flow. The top boundary moves horizontally, while the remaining walls are stationary no-slip boundaries. The solver updates the horizontal velocity `u`, vertical velocity `v`, and pressure field `p` over time.

The method uses finite differences with:

- backward differences for the nonlinear convection terms
- central differences for pressure gradients
- central differences for diffusion terms
- a pressure Poisson equation to enforce incompressibility

The simulation saves pressure, `u` velocity, and `v` velocity fields to CSV files at each timestep. These outputs are then animated in Python using pressure contours with velocity streamlines to show the formation of the cavity vortex.

### 2D Channel Flow
Simulates pressure-driven channel flow using the 2D incompressible Navier-Stokes equations.

The solver uses finite difference methods to update the horizontal velocity `u`, vertical velocity `v`, and pressure field `p`. A pressure Poisson equation is solved at each timestep to help enforce incompressibility.

Main features:

- Periodic boundary conditions in the x-direction
- No-slip wall boundary conditions at the top and bottom
- Constant forcing term to drive the flow through the channel
- Pressure Poisson iterations for pressure correction
- CSV output for pressure, u-velocity, and v-velocity fields
- Python visualization for velocity magnitude and vector fields

This solver represents flow through a horizontal channel where the fluid is pushed forward by a constant body force while viscosity and wall boundaries shape the velocity profile.

## Grid Refinement Study
The grid-refinement study runs the cavity solver on three uniformly refined grids:

- 41x41

- 81x81

- 161x161

With grid spacings of 0.0500, 0.0250, 0.0125 respectively.

The refinement ration is r = 2 and the measured quantity is the horizontal velocity at x = 1.0 and y = 0.5

The observed order is calculated using:

$p=\frac{\ln\left|\dfrac{f_3 - f_2}{f_2 - f_1}\right|}{\ln(r)}$

where $f_3$ is the coarse grid result, $f_2$ is the fine grid result, and $f_1$ is the fine grid result.

### Current GRS Results
Using a shared time step value of 0.00025:

| Grid Size | u(1.0,0.5) |
|-----------|:----------:|
| 41x41     | -0.117460  |
| 81x81     | -0.117047  |
| 161x161   | -0.116822  |

The resulting refinement ratio is:
${\left|\dfrac{f_3 - f_2}{f_2 - f_1}\right|}{\approx1.836}$

and therefore the observed order is approximately:
$p\approx0.88$

Since the convection and time stepping component I used are 1st order accurate the observed order is reasonably close to the expected order.


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
4) 2D Lid-Driven Cavity Flow with Navier-Stokes
5) 2D Channel Flow with Navier-Stokes
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
- 2D Lid-Driven Cavity Flow solver is plotted over `[0, 2]` for x and `[0, 2]` for y
- 2D Channel Flow solver is plotted over `[0, 2]` for x and `[0, 2]` for y

## Planned Improvements

- Separate output files for each solver
- Better input validation
- Analytical solution comparisons
- Error calculations
- Grid refinement studies
- Runtime scaling tests
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
