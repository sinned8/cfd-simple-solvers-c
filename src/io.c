#include <io.h>
#include <stdio.h>
#include <stdlib.h>



void generate_1d_csv(const double *u, int n)
{
    //would need to change this s.t it stores in 'outputs' folder regardless of directory path
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
    fclose(fp);



}

void plot1d_csv(const char * filename,char solverType)
{

    char command[512];

    // maybe have it st that command also has some type of variable that says where the output
    // is from the convection or burgers solver so that the graph is updated correctly for each

    snprintf(command, sizeof(command),
             ".venv\\Scripts\\python.exe \"python scripts\\plot_1d.py\" \"%s\" \"%c\"",
             filename,solverType);

    printf("Running command:\n%s\n", command);

    int result = system(command);

    if (result != 0)
    {
        printf("Error: Python plotting script failed.\n");
    }
}