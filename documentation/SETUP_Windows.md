# First-Time Setup for Windows Users

> You will need Visual Studio 2019 installed

There is an extra step for first time setup on Windows machines

**Create the build directory**:

- `cd game`
- `mkdir build`

**Create the build system**:

- `cmake -B build`

**Build the code**:

- `cmake --build build`

**Move the DLLs**:

- Move `sfml-audio-d-2.dll`, `sfml-graphics-d-2.dll`, `sfml-networks-d-2.dll`, & `sfml-window-d-2.dll`
  from `build/deps/sfml-build/lib` to `build/Debug`
