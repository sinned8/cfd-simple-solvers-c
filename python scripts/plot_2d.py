import glob
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation




solver_type = sys.argv[1]


if solver_type == 'p':

    files = sorted(glob.glob("outputs/pressure_iterations/p_*.csv"))

    if not files:
        raise FileNotFoundError("No pressure frame files were found.")

    first_frame = np.loadtxt(files[0], delimiter=",")
    ny, nx = first_frame.shape

    x = np.linspace(0, 2, nx)
    y = np.linspace(0, 1, ny)
    X, Y = np.meshgrid(x, y)


    zmin = float("inf")
    zmax = float("-inf")

    for file in files:
        data = np.loadtxt(file, delimiter=",")
        zmin = min(zmin, np.min(data))
        zmax = max(zmax, np.max(data))

    # Add padding so the surface does not touch the frame
    z_range = zmax - zmin
    z_pad = 0.25 * z_range

    if z_range == 0:
        z_pad = 1.0

    fig = plt.figure(figsize=(11, 7), dpi=100)
    ax = fig.add_subplot(projection="3d")

    def update(frame_index):
        ax.clear()

        p = np.loadtxt(files[frame_index], delimiter=",")

        ax.plot_surface(
            X, Y, p,
            cmap="coolwarm",
            rstride=1,
            cstride=1,
            linewidth=0,
            antialiased=False
        )

        ax.set_title(f"2D Poisson Equation - Frame {frame_index}")

        ax.set_xlim(0, 2)
        ax.set_ylim(0, 1)
        ax.set_zlim(zmin - z_pad, zmax + z_pad)

        ax.set_xlabel("x")
        ax.set_ylabel("y")
        ax.set_zlabel("p")

        ax.view_init(15, 230)

        return ax,

    ani = FuncAnimation(fig, update, frames=len(files), interval=200)

    plt.show()