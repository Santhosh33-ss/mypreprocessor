# Custom C Preprocessor (`mypreprocessor`)

A modular C preprocessor implemented in C that processes `.c` source files and generates an expanded `.i` output file.

## Features
- **Comment Removal:** Strips both single-line (`//`) and multi-line (`/* ... */`) comments while preserving code formatting and line structure.
- **File Inclusion (`#include`):** Replaces `#include "header.h"` and `#include <header.h>` directives with the contents of the referenced header file.
- **Macro Substitution (`#define`):**
  - Supports simple object-like macros (e.g., `#define PI 3.14`).
  - Supports function-like macros with arguments (e.g., `#define SQUARE(x) ((x)*(x))`).

## Project Structure
- `main.c` — Command-line argument validation, file I/O, and main lexical scanning loop.
- `comment_removal.c` — Handles stripping single-line and multi-line comments.
- `file_inclusion.c` — Resolves and copies local and system header contents into the `.i` output.
- `macro_handler.c` — Stores macro definitions and expands simple and parameterized macros.
- `preprocessor.h` — Shared data structures and function prototypes.
- `Makefile` — Automates compilation and cleanup.

## Build & Run
1. **Compile the project:**
   ```bash
   make
