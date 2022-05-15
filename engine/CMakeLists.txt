
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

# Fetch EnTT
FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v3.10.1
)
FetchContent_MakeAvailable(EnTT)

# Link executable to required SFML modules
target_link_libraries(engine sfml-graphics EnTT::EnTT)
