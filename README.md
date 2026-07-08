# Custom-Analog-Circuit-Simulation-Engine

> **NOTE:** Please be aware that this is still under active development and thus many things are still subject to change. Furthermore, as of writing this optimization is less of a priority at this moment compared to getting it functional. Despite this, smart architectual designs are still being made to the best of my ability.

## Overview
This project is a custom-built, lightweight SPICE (Simulation Program with Integrated Circuit Emphasis) simulator developed from scratch in C++. Designed for academic and engineering exploration, the application allows users to define, solve, and visualize complex linear electronic circuits. By combining efficient numerical methods with an intuitive graphical interface, it provides deep insight into circuit behaviors across different domains.

---

## Key Features

* **Multi-Domain Simulation Engine:**
	* **Operating Point (DC) Analysis:** Computes the steady-state voltages and currents of the circuit.
  * **Transient Analysis:** Solves time-dependent circuit behaviors to simulate time-varying waveforms via numerical integration.
  * **Small-Signal (AC) Analysis:** Evaluates the frequency response of the circuit over a specified spectrum.
* **Advanced Numerical Solvers:** Leverages the **Eigen C++ template library** to handle high-performance matrix and vector operations. Circuit equations are formulated using **Modified Nodal Analysis (MNA)**, ensuring accurate handling of independent/dependent voltage and current sources, resistors, and reactive components.
* **Interactive Data Visualization:** Integrates **QCustomPlot** (Qt-based) to plot real-time simulation data, delivering clear, interactive graphs for transient waveforms and frequency response curves.

---
## Technical Stack

* **Core Language:** C++ (Object-Oriented Design)
* **Math Engine:** Eigen Library (Dense/Sparse matrix solvers)
* **GUI & Plotting:** Qt Framework & QCustomPlot
* **Analysis Methods:** Modified Nodal Analysis (MNA), Companion Models & Numerical Integration (for transient analysis)



