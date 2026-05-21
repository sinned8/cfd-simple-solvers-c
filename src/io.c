#include <io.h>
#include <stdio.h>
#include <stdlib.h>



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