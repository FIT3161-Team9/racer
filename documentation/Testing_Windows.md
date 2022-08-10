
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

After you've done the above the first time, you can build and run tests from the build
directory with:

- `cmake --build .`
- `ctest`

Displaying the results of the test:
 - `ctest --output-on-failure`
 OR
 - `.\Debug\RACER_tests.exe`