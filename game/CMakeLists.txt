cmake_minimum_required(VERSION 3.11)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Make sure dll's get outputted to the same directory as executables
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

# Output compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# set the project name and version
project(RACER VERSION 1.0)

if(MSVC)
  # warning level 4 and all warnings as errors
  add_compile_options(/W4 /EHsc /bigobj)
else()
  # lots of warnings and all warnings as errors
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

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

# Fetch json
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(RACER main.cpp)

# Link executable to required SFML modules
target_include_directories(RACER PUBLIC ../ ../)
target_link_libraries(RACER sfml-graphics EnTT::EnTT
                      nlohmann_json::nlohmann_json)

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

enable_testing()

add_executable(RACER_tests ./tests/tests.cpp)
target_include_directories(RACER_tests PUBLIC ../ ../)
target_link_libraries(RACER_tests sfml-graphics EnTT::EnTT GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(RACER_tests)
