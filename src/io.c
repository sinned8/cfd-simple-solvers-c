#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void generate_1d_csv(const double *u, int n)
{

    // just generating new csv file
    FILE *fp = fopen("outputs/output.csv", "w");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        fprintf(fp,"%f",u[i]);
        if (i < n - 1)
        {
            fprintf(fp,",");
        }
    }
    fprintf(fp,"\n");
    fclose(fp);



}


void write_1d_row(const double *u, const int n)
{
    // we open file, we go to new line and print date from new u into there
    FILE *fp = fopen("outputs/output.csv", "a");
    if (fp != NULL) {
        // Write new data followed by a newline
        for (int i = 0; i < n; i++)
        {
            fprintf(fp,"%f",u[i]);
            if (i < n - 1)
            {
                fprintf(fp,",");
            }
        }
        fprintf(fp,"\n");
        fclose(fp);
    }
}




void plot1d_csv(const char * filename, const char solverType, const double dt)
{

    char command[512];

    // command to run plot_1d.py and transfer over necessary variables from animation plotting
    snprintf(command, sizeof(command),
             ".venv\\Scripts\\python.exe \"python scripts\\plot_1d.py\" \"%s\" \"%c\" \"%lf\" ",
             filename,solverType,dt);

    printf("Running command:\n%s\n", command);

    int result = system(command);

    if (result != 0)
    {
        printf("Error: Python plotting script failed.\n");
    }
}


void generate_2d_pressure_iteration_csv( double **p, int ny, int nx, int frame_number)
{

    // making the file name correspond to which pressure iteration it's logging
    // e.g p_0001 would be the first pressure iteration p_0050 would be the 50th
    char filename[100];
    snprintf(filename, sizeof(filename),
             "outputs/pressure_iterations/p_%04d.csv", frame_number);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            fprintf(fp, "%lf", p[j][i]);

            if (i < nx - 1) {
                fprintf(fp, ",");
            }
        }

        fprintf(fp, "\n");
    }

    fclose(fp);

}

void plot2d_csv(char solverType)
{
    char command[512];

    // command to run plot_2d.py and transfer over necessary variables from animation plotting
    snprintf(command, sizeof(command),
             ".venv\\Scripts\\python.exe \"python scripts\\plot_2d.py\" \"%c\""
             ,solverType);

    printf("Running command:\n%s\n", command);

    int result = system(command);

    if (result != 0)
    {
        printf("Error: Python plotting script failed.\n");
    }
}


void clear_2d_pressure_iteration_folder()
{
    char filename[128];

    for (int k = 0; k < 10000; k++) {
        snprintf(filename, sizeof(filename),
                 "outputs/pressure_iterations/p_%04d.csv", k);

        remove(filename);
    }
}

void generate_2d_u_velocity_csv( double **u, int ny, int nx, int frame_number)
{
    // making the file name correspond to which pressure iteration it's logging
    // e.g u_0001 would be the velocity in the u direction for the first iteration u_0050 would be
    // the velocity ion the u direction for the 50th iteration
    char filename[100];
    snprintf(filename, sizeof(filename),
             "outputs/u_velocity_iterations/u_%04d.csv", frame_number);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            fprintf(fp, "%lf", u[j][i]);

            if (i < nx - 1) {
                fprintf(fp, ",");
            }
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}

void generate_2d_v_velocity_csv( double **v, int ny, int nx, int frame_number)
{
    // making the file name correspond to which pressure iteration it's logging
    // e.g v_0001 would be the velocity in the v direction for the first iteration v_0050 would be
    // the velocity ion the v direction for the 50th iteration
    char filename[100];
    snprintf(filename, sizeof(filename),
             "outputs/v_velocity_iterations/v_%04d.csv", frame_number);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            fprintf(fp, "%lf", v[j][i]);

            if (i < nx - 1) {
                fprintf(fp, ",");
            }
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}

void clear_2d_u_velocity_folder()
{
    char filename[128];

    for (int k = 0; k < 10000; k++) {
        snprintf(filename, sizeof(filename),
                 "outputs/u_velocity_iterations/u_%04d.csv", k);

        remove(filename);
    }
}

void clear_2d_v_velocity_folder()
{
    char filename[128];

    for (int k = 0; k < 10000; k++) {
        snprintf(filename, sizeof(filename),
                 "outputs/v_velocity_iterations/v_%04d.csv", k);

        remove(filename);
    }
}
