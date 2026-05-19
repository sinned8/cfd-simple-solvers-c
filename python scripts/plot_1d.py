import sys
import matplotlib.pyplot as plt
import numpy
import pandas as pd

#maybe have switch case statement here that evals for each different solver


csv_file = sys.argv[1]
solver_type = sys.argv[2]


if solver_type == 'l':
    dataframe = pd.read_csv(csv_file, header=None)
    u = dataframe.to_numpy().ravel()
    x = numpy.linspace(0, 2, len(u))
    plt.xlabel("x")
    plt.plot(x, u, marker="o", markersize=5, label="Computational",color="C0")
    plt.title("1D Linear Convection")
    plt.grid(True)
    plt.show()
else:
    dataframe = pd.read_csv(csv_file, header=None)
    u = dataframe.to_numpy().ravel()
    x = numpy.linspace(0, 2 * numpy.pi, len(u))
    plt.figure(figsize=(11, 7), dpi=100)
    plt.plot(x, u, marker="o", markersize=5, label="Computational",color="C0")
    plt.xlim(0, 2 * numpy.pi)
    plt.ylim(0, 10)
    plt.xlabel("x")
    plt.title("1D Burgers equation")
    plt.grid(True)
    plt.show()






