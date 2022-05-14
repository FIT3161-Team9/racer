
add_library(engine app.cpp)

# Add FetchContent module
include(FetchContent)

# Fetch SFML
FetchContent_Declare(
 SFML
 GIT_REPOSITORY https://github.com/SFML/SFML.git
 GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)


# Link executable to required SFML modules
target_link_libraries(engine sfml-graphics)
