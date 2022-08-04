cmake_minimum_required(VERSION 3.11)

# Output compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Make sure dll's output to the same place as binaries
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

project(engine)

add_library(engine app.cpp)

# Add FetchContent module
include(FetchContent)

# Fetch SFML
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Fetch EnTT
FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v3.10.1)
FetchContent_MakeAvailable(EnTT)

# Add Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1)

# This is so we don't override compiler settings on windows (as per GoogleTest
# docs)
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Link executable to required SFML modules
target_link_libraries(engine sfml-graphics EnTT::EnTT)

enable_testing()

add_executable(engine_tests ./tests/tests.cpp)
target_link_libraries(engine_tests sfml-graphics EnTT::EnTT GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(engine_tests)
