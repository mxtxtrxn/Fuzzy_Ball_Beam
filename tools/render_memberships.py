"""Render the membership functions to a PNG without an interactive GUI.

Run from the repository root:

    python tools/render_memberships.py

The output is written to artifacts/membership_functions.png so it can be
opened in the desktop's browser.
"""

import os
import sys

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from tests.test_fuzzy_tables import POS, SP, THETA, trapmf


OUTPUT = os.path.join("artifacts", "membership_functions.png")
COLORS = {"NM": "r", "NS": "y", "ZO": "g", "PS": "c", "PM": "b"}


def plot_sets(ax, universe, table, title):
    for label, params in table.items():
        values = [trapmf(x, *params) for x in universe]
        ax.plot(universe, values, COLORS[label], linewidth=1.5, label=label)
    ax.set_title(title)
    ax.legend()
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)


def main():
    os.makedirs("artifacts", exist_ok=True)
    fig, axes = plt.subplots(nrows=3, figsize=(8, 9))

    position_axis = np.arange(0, 31, 1)
    theta_axis = np.arange(45, 181, 5)

    plot_sets(axes[0], position_axis, POS, "Ball position")
    plot_sets(axes[1], position_axis, SP, "Set point position")
    plot_sets(axes[2], theta_axis, THETA, "Theta output")

    plt.tight_layout()
    plt.savefig(OUTPUT, dpi=120)
    print("Saved " + OUTPUT)


if __name__ == "__main__":
    main()
