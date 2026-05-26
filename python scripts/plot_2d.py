import glob
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors, cm, contour
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

elif solver_type == 'C':

    p_files = sorted(glob.glob("outputs/pressure_iterations/p_*.csv"))
    u_files = sorted(glob.glob("outputs/u_velocity_iterations/u_*.csv"))
    v_files = sorted(glob.glob("outputs/v_velocity_iterations/v_*.csv"))

    p0 = np.loadtxt(p_files[0], delimiter=",")

    ny, nx = p0.shape

    x = np.linspace(0, 2, nx)
    y = np.linspace(0, 2, ny)

    X, Y = np.meshgrid(x, y)


    p_min = min(np.loadtxt(f, delimiter=",").min() for f in p_files)
    p_max = max(np.loadtxt(f, delimiter=",").max() for f in p_files)

    if abs(p_max - p_min) < 1e-12:
        p_min -= 1.0
        p_max += 1.0

    filled_levels = np.linspace(p_min, p_max, 30)
    line_levels = np.linspace(p_min, p_max, 10)

    fig, ax = plt.subplots(figsize=(9, 7), dpi=100)

    norm = colors.Normalize(vmin=p_min, vmax=p_max)
    sm = cm.ScalarMappable(norm=norm, cmap="viridis")
    cbar = fig.colorbar(sm, ax=ax)
    cbar.set_label("Pressure")



    def update(frame):
        ax.clear()

        p = np.loadtxt(p_files[frame], delimiter=",")
        u = np.loadtxt(u_files[frame], delimiter=",")
        v = np.loadtxt(v_files[frame], delimiter=",")

        ax.contourf(X, Y, p, levels=filled_levels, cmap="viridis", alpha=0.6)
        ax.contour(X, Y, p, levels=line_levels, cmap="viridis", linewidths=1)

        ax.streamplot(
            X,
            Y,
            u,
            v,
            density=1.5,
            linewidth=1,
            arrowsize=1.2
        )

        ax.set_title(f"2D Lid-Driven Cavity Flow | Frame: {frame}")
        ax.set_xlabel("X")
        ax.set_ylabel("Y")
        ax.set_xlim(0, 2)
        ax.set_ylim(0, 2)
        ax.set_aspect("equal")


    ani = FuncAnimation(
        fig,
        update,
        frames=len(p_files),
        interval=100
    )

    plt.show()



