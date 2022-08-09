# Generate Docs

This project uses Doxygen to generate documentation html. The `engine` directory and the `game` directory
support generating docs.

### Prerequisites

Ensure you have Doxygen installed

### Generating Docs

**For the engine**:

- `cd engine`
- `doxygen ./Doxyfile`

Then open `engine/generated_docs/html/index.html` in your browser.

**For the game**:

- `cd game`
- `doxygen ./Doxyfile`

Then open `game/generated_docs/html/index.html` in your browser.
