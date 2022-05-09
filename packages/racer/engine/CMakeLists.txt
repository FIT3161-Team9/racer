
add_library(engine app.cpp)

# Find SFML shared libraries
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

# Link executable to required SFML modules
target_link_libraries(engine sfml-graphics)
