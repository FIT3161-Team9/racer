
# Testing on Windows

Running the tests on Windows involves some extra steps:

- `cd engine`
- `mkdir build`
- `cmake -B build`
- `cmake --build build`
- `cp ./build/bin/Debug/gtest.dll ./build/Debug`
- `cp ./build/bin/Debug/gtest_main.dll ./build/Debug`
- `cmake --build build`
- `./build/Debug/engine_tests.exe`
