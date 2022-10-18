# Racer

This package is the C++ source code for the _RACER_ simulation-based game. See the `GettingStarted.md` document in the documentation
directory to read about the architecture of this project.

## Development

### Requirements

- CMake (Version 3.11 or greater)
- A C++20 compliant compiler (Not all C++20 features are required)

### Initial Setup

Steps for building for the first time:

> For Windows users: see `SETUP_Windows.md` instead for first-time setup

- `cd game`
- `mkdir build`
- `cmake -B build`
- `cmake --build build

### Building

After the initial setup, only the `cmake --build build` step is necessary to build the application. (`cmake -B build` is only necessary
if changes to `CMakeLists.txt` are made.

The game will be built and once successfully built, a green message will indicate the path it has been built to.

The executable must be run from the `game` directory. For example: `.\build\Debug\RACER.exe` on windows, and `./build/RACER` on MacOS.
