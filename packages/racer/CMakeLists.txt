cmake_minimum_required(VERSION 3.10)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name and version
project(RACER VERSION 1.0)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX /EHsc)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory(engine)

# Add executable
add_executable(RACER main.cpp)

target_link_libraries(RACER engine)
