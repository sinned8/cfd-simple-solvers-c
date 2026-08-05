cmake_minimum_required(VERSION 4.2)
project(Simple_Solvers_in_C____Summer_2026_Project C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(src)

add_executable(Simple_Solvers_in_C____Summer_2026_Project
        include/grid.h
        include/io.h
        include/solvers.h
        "src/solvers/1D_Burgers_equation.c"
        "src/solvers/1D_Linear_Convection.c"
        src/grid.c
        src/io.c
        src/main.c
        src/solvers/2D_Poisson.c
        src/solvers/2D_Cavity_Flow_NS.c
        src/solvers/2D_Channel_Flow_NS.c
        src/solvers/2D_Cavity_Flow_Grid_Refinement_Study.c
)
