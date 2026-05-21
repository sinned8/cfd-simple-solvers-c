import sys
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy
import pandas as pd




csv_file = sys.argv[1]
solver_type = sys.argv[2]
dt = float(sys.argv[3])






if solver_type == 'l':
    data = pd.read_csv(csv_file, header=None)


    nx = data.shape[1]
    x = numpy.arange(nx)

    fig, ax = plt.subplots()

    line, = ax.plot(x, data.iloc[0])

    ax.set_xlabel("x")
    ax.set_ylabel("u")
    ax.set_ylim(data.min().min() - 0.1, data.max().max() + 0.1)

    def update(frame):
        line.set_ydata(data.iloc[frame])

        current_time = frame * dt
        ax.set_title(f"1D Linear Convection | t = {current_time:.4f}")

        return line,

    ani = animation.FuncAnimation(
        fig,
        update,
        frames=len(data),
        interval=100,
        blit= False
    )
    plt.show()


else:

    data = pd.read_csv("outputs/output.csv", header=None)

    nx = data.shape[1]
    x = numpy.arange(nx)

    fig, ax = plt.subplots()

    line, = ax.plot(x, data.iloc[0])

    ax.set_xlabel("x")
    ax.set_ylabel("u")
    ax.set_ylim(data.min().min() - 0.1, data.max().max() + 0.1)

    def update(frame):
        line.set_ydata(data.iloc[frame])

        current_time = frame * 2 * dt
        ax.set_title(f"1D Burgers Equation | t = {current_time:.4f}")

        return line,

    ani = animation.FuncAnimation(
        fig,
        update,
        frames=len(data),
        interval=100,
        blit= False
    )
    plt.show()







