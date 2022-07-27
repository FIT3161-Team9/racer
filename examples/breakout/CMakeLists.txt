cmake_minimum_required(VERSION 3.11)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# set the project name and version
project(Breakout VERSION 1.0)

if(MSVC)
  # warning level 4 and all warnings as errors
  add_compile_options(/W4 /WX /EHsc)
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

# include_directories(../../engine) subdirs(../../engine)

add_executable(Breakout main.cpp)

# Link executable to required SFML modules
target_include_directories(Breakout PUBLIC ../../ ../../)
target_link_libraries(Breakout sfml-graphics EnTT::EnTT)
