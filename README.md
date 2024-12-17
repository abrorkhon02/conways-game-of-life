# Conway's Game of Life Implementation

An efficient, object-oriented implementation of Conway's Game of Life in C++, featuring various patterns and optimization techniques.

## Authors
- Abrorkhon Azimov
- Adeola Aduroja
- Philipp Schneider
- Noah Stein

## Features

- Efficient grid representation using 2D vectors
- Support for multiple predefined patterns:
  - Glider
  - Toad
  - Beacon
  - Methuselah
  - R-Pentomino
- File I/O operations for saving and loading game states
- Command-line interface for user interaction
- Performance optimized implementation
- Support for different grid sizes
- Stability detection
- Configurable simulation speed

## Requirements

- C++ compiler with C++11 support or higher
- Standard Template Library (STL)

## Building the Project

To compile with optimizations enabled:

```bash
g++ -O3 -o game_of_life main.cpp
```

For debug build:

```bash
g++ -O0 -o game_of_life main.cpp
```

## Usage

1. Run the compiled program:
   ```bash
   ./game_of_life
   ```

2. Follow the command-line prompts to:
   - Enable/disable printing after each generation
   - Set delay between generations
   - Choose initial patterns
   - Save/load game states

## Class Structure

### Grid Class
The main class handling the game logic and state management.

Key methods:
- `void evolve()`: Advances the grid to the next generation
- `long long run(int generations, int delay_ms)`: Runs the simulation
- `bool save(string filename)`: Saves current state to file
- `bool load(string filename)`: Loads state from file
- Pattern creation methods: `addGlider()`, `addToad()`, `addBeacon()`, etc.

### CLI Class
Handles user interaction and program flow.

Key method:
- `void run()`: Executes the command-line interface

## Game Rules

Conway's Game of Life follows these rules:
1. Any live cell with fewer than 2 live neighbors dies (underpopulation)
2. Any live cell with 2 or 3 live neighbors survives
3. Any live cell with more than 3 live neighbors dies (overpopulation)
4. Any dead cell with exactly 3 live neighbors becomes alive (reproduction)

## Performance Analysis

Performance tests were conducted with different optimization levels (-O0 vs -O3) and grid sizes. The implementation shows significant performance improvements with -O3 optimization enabled.

### Grid Size Performance
Performance scales with grid size as shown in the documentation. Tests were conducted for various grid sizes up to 10000x10000 cells.

## File Formats

### Save File Format
```
height width
cell_states (0 or 1)
```

Example:
```
3 3
0 1 0
1 1 1
0 1 0
```

## Contributing

Feel free to submit issues and enhancement requests.

