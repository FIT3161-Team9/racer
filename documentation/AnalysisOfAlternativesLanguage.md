# Analysis of Alternatives - Programming Language

This document provides an analysis of alternatives for the programming language to be used in the project.

The goal of this document is to list and analyse the options we have for which programming language to use. We analysis each option against a fixed list of criteria.

## Criteria

This section lists the criteria we have for the programming language to be used.

1. **Speed**: The selected language should be fast enough to run the game smoothly
2. **Documentation**: We want a well documented language, so we can solve issues easily
3. **Familiar**: We may want a language that the team is familiar with (?)
4. **Tools**: We want a language with good development tools
5. **Libraries**: We want a language with appropriate graphics libraries

## Option 1: `C++`

### Criteria 1: Speed

C++ is very fast as it is a compiled language that has had years of optimisations imlemented in the compiler.

### Criteria 2: Documentation

C++ doesn't really have great documentation.

### Criteria 3: Familiar

Most of the team is familiar with C++ (?)

### Criteria 4: Tools

C++ tooling isn't _great_. Most editors don't handle autocompletion etc. very well. Package management is an absolute nightmare, as there isn't a standard solution. Build tools have the same problem.

### Criteria 5: Libraries

C++ has native bindings to OpenGL and Vulkan graphics libraries.

## Option 2: `Java`

### Criteria 1: Speed

Java is slow (relative to compiled languages), because it runs in the JVM.

### Criteria 2: Documentation

Java is relatively well [documented](https://docs.oracle.com/en/java/).

### Criteria 3: Familiar

Most of the team is familiar with Java (?)

### Criteria 4: Tools

Java tooling isn't _great_. You have to use either Eclipse or IntelliJ to develop, and package management isn't great.

### Criteria 5: Libraries

Java isn't well known for it's graphics libraries.

## Option 3: `Typescript`

### Criteria 1: Speed

Typescript is quite slow (compared to Rust & C++) as it runs in the browser.

### Criteria 2: Documentation

Typescript is quite well [documented](https://www.typescriptlang.org/docs/).

### Criteria 3: Familiar

Most of the team is familiar with Typescript (?)

### Criteria 4: Tools

Java tooling isn't _great_. You have to use either Eclipse or IntelliJ to develop, and package management isn't great.

### Criteria 5: Libraries

Typescript runs (as Javascript) in the browser, so it is well suited to graphics (we can just use the Canvas API, or some 2D graphics library).

## Option 4: `Rust`

### Criteria 1: Speed

Rust is very fast, as it is a compiled language.

### Criteria 2: Documentation

Rust is really well [documented](https://www.typescriptlang.org/docs/).

### Criteria 3: Familiar

Only Harrison is familiar with Rust.

### Criteria 4: Tools

Rust's tools are great. It's super easy to get started and manage packages.

### Criteria 5: Libraries

Rust has a large ecosystem of libraries, as well as bindings to most C and C++ bindings.

# Preferences

| Language   | Preference (Harrison) | Preference (Chenchen) |
| ---------- | --------------------- | --------------------- |
| C++        | 3                     | 3                     |
| Java       | 4                     | 4                     |
| Typescript | 2                     | 1                     |
| Rust       | 1                     | 2                     |
