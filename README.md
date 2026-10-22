# Lights: Hexagonal Power Puzzle

For an exercise in the C++ Object-Oriented Programming 2 course, we built a hexagonal puzzle game in which the goal is to light up every bulb on the board. Each bulb has arms pointing in different directions, and clicking a bulb rotates it. Power flows from the central bulb to any neighbor whose arms face each other. This repository showcases the implementation built using C++23 and the SFML 3 framework, with a strict separation between game logic and graphics.

## 🌟 Key Features

* **Rotation Puzzle Gameplay:** Rotate bulbs by 60° at a time to connect them to the central power source. A level is solved once every bulb on the board is lit.
* **Live Power Propagation:** After every rotation, power is recalculated across the whole board and bulbs light up or turn off instantly.
* **Three Growing Levels:** The hexagonal board gets bigger from level to level, with a success message after each level and at the end of the game.
* **Generic Boards:** Any board size defined in a level file is supported, and the board is automatically scaled and centered to fit the window.
* **Data-Driven Levels:** Levels are plain text files loaded in order from a playlist, so new levels can be added without recompiling.

## 🏗️ Implementation Details

This project was designed with a heavy focus on clean architecture and object-oriented design:

* **Separated Layers:** The logic layer (`Node`, `Graph`, `Board`, `BFS`) has zero knowledge of SFML, and the view layer (`LineShape`, `PointShape`, `BoardRenderer`, `View`) contains no game logic. All visual updates flow through a single `BoardRenderer::sync()` call.
* **Graph Model:** The board is a graph of bulbs stored in a vector, with a bidirectional adjacency list describing which bulbs are physical neighbors.
* **BFS Power Propagation:** After every rotation, a BFS runs from the central bulb. Power passes between two bulbs only if both of them have an arm pointing toward the other.
* **Runtime Adjacency:** Neighbor relationships are not stored in the level files. They are computed at runtime from the physical distance and angle between bulbs, and the angle is mapped to one of six hex directions.
* **SFML Shapes:** Each bulb is a `PointShape` (a center circle with up to six arms) that inherits from `sf::Drawable` and `sf::Transformable`, so the bulb and its arms rotate together.
* **Error Handling:** Exceptions are caught in `main` and written to `log.txt`.

## 👥 Authors

* **Benji Mandel:** Graphics and user interface, including the shapes, board rendering, and the game window.
* **Yitzhak Rose:** Game logic, including the graph, board, level loading, adjacency computation, and power propagation.

## 🛠️ Build Instructions

This project uses **CMake** for its build system.

### Prerequisites

* C++23 compiler
* CMake (3.26+)
* SFML 3

### Building

Clone the repository and build using CMake:

```bash
git clone https://github.com/yitzhak-rose/lights-puzzle-game.git
cd lights-puzzle-game

mkdir build
cd build

# Generate build files and compile
cmake ..
cmake --build .

```

### Running

Once built, you can run the executable directly from the build directory:

```bash
./oop2_ex04

```

## 🎮 Controls

* **Left Click:** Rotate a bulb clockwise
* **Right Click:** Rotate a bulb counter-clockwise
* **Close Button (X):** Exit the game

## ⚖️ License

**All Rights Reserved**

The source code in this repository is provided for viewing and portfolio purposes only. You are welcome to compile and play the game for personal entertainment. However, you may not copy, modify, distribute, or use this code for any other purpose.
