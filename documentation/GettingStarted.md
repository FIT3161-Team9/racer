# Getting Started

This repository is made up of three main components:

- The engine - a framework for creating desktop applications
- The examples - examples of how to use the engine to create desktop applications
- The game - the RACER game makes use of the engine to implement a physics simulation racing game

The engine directory and game directory support generating documentation via `doxygen`. See the
`GenerateDocs.md` document for how to do this.

# The Engine

The engine provides an API to the client app to spawn entities, attach components to them,
create systems and listen to events. The engine provides a set of base components that can
be used to render entities to the screen.

## Creating an app with the engine

1. Create a new CMake project, and add the engine directory as an include directory.
2. Create a `main.cpp` and import `<engine/engine.h>`.
3. In the main function use `engine::create_app()` to create an app, and `engine::run_app()` to
   start the main event and render loop (see the documentation of these functions for details)
4. At this point, the callback passed to `engine::run_app()` can add all systems, event systems
   and entities required for the app

## ECS

The engine uses and ECS to manage entities in the app, see (this article)[https://en.wikipedia.org/wiki/Entity_component_system] to read about what an
ECS is and how it works.

- Entities are spawned with `AppCommands::spawn()`
- Components are added to entities via `Entity::add_component()`
- Systems are added with `AppCommands::add_system()`

Additionally, the engine introduces the concept of "resources", which are basically global resources
that the game can create and mutate throughout the apps lifetime. They are added with `AppCommands::add_resource()`

### Plugins

Apps can separate their setup logic and systems via plugins. Plugins are simply functions with the
same signature as the second `engine::run_app()` parameter that get called straight away.

> The best way to understand how this works is to look at the examples.

### Systems

There are two types of systems:

- Normal systems, which run on every iteration of the main engine loop
- Event systems, which run only when the specified event occurs

Systems can specify zero or more "queries" that are then run against the ECS and the matching
entities and components are passed into the system.

Systems can also specify zero or more "resource queries" that allow them to consume any number
of resources.

> The best way to understand how this works is to look at the examples.
