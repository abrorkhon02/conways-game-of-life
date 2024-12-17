#include "Grid.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string.h>
#include <omp.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

Grid::Grid() : height(0), width(0), printEnabled(true) {}

Grid::Grid(int h, int w)
    : height(h),
      width(w),
      currentGeneration(h, std::vector<bool>(w, false)),
      nextGeneration(h, std::vector<bool>(w, false)),
      previousGeneration(h, std::vector<bool>(w, false)),
      printEnabled(true) {}

std::pair<int, int> Grid::to2D(int p) const {
    return {p / width, p % width};
}

void Grid::initializePattern(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Attempting to open file at: " << filename << std::endl;
        std::cerr << "Error opening file!" << std::endl;
        std::cerr << "Error code: " << strerror(errno) << std::endl;
        return;
    }
    file >> height >> width;
    currentGeneration.resize(height, std::vector<bool>(width, false));
    nextGeneration.resize(height, std::vector<bool>(width, false));
    previousGeneration.resize(height, std::vector<bool>(width, false));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int cell;
            file >> cell;
            currentGeneration[i][j] = (cell == 1);
        }
    }
    file.close();
}

void Grid::print() const {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << (currentGeneration[i][j] ? 'O' : '.');
        }
        std::cout << std::endl;
    }
}

long long Grid::run(int generations, int delay_ms) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int step = 0; step < generations; ++step) {
        if (printEnabled) {
            clearScreen();
            std::cout << "Generation " << step + 1 << ":\n";
            print();
        }
        evolve();
        if (is_stable()) {
            std::cout << "\nStable configuration detected at generation " << step + 1 << ".\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

long long Grid::runSerial(int generations, int delay_ms) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int step = 0; step < generations; ++step) {
        if (printEnabled) {
            clearScreen();
            std::cout << "Generation " << step + 1 << ":\n";
            print();
        }
        evolveSerial();
        if (is_stable()) {
            std::cout << "\nStable configuration detected at generation " << step + 1 << ".\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

bool Grid::load(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }
    file >> height >> width;
    currentGeneration.resize(height, std::vector<bool>(width, false));
    nextGeneration.resize(height, std::vector<bool>(width, false));
    previousGeneration.resize(height, std::vector<bool>(width, false));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int cell;
            file >> cell;
            currentGeneration[i][j] = (cell == 1);
        }
    }
    file.close();
    return true;
}

bool Grid::save(const std::string &filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }
    file << height << " " << width << "\n";
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            file << (currentGeneration[i][j] ? 1 : 0) << " ";
        }
        file << "\n";
    }
    file.close();
    return true;
}

void Grid::setSize(int h, int w) {
    height = h;
    width = w;
    currentGeneration.resize(height, std::vector<bool>(width, false));
    nextGeneration.resize(height, std::vector<bool>(width, false));
    previousGeneration.resize(height, std::vector<bool>(width, false));
}

int Grid::getHeight() const { return height; }
int Grid::getWidth() const { return width; }

void Grid::setCell(int x, int y, bool state) {
    if (x >= 0 && x < height && y >= 0 && y < width) {
        currentGeneration[x][y] = state;
    } else {
        std::cerr << "Error: Coordinates out of bounds.\n";
    }
}

void Grid::setCell(int p, bool state) {
    auto coords = to2D(p);
    setCell(coords.first, coords.second, state);
}

bool Grid::getCell(int x, int y) const {
    if (x >= 0 && x < height && y >= 0 && y < width) {
        return currentGeneration[x][y];
    }
    std::cerr << "Error: Coordinates out of bounds.\n";
    return false;
}

bool Grid::getCell(int p) const {
    auto coords = to2D(p);
    return getCell(coords.first, coords.second);
}

void Grid::addGlider(int x, int y) {
    std::vector<std::pair<int, int>> glider = {
        {0,1}, {1,2}, {2,0}, {2,1}, {2,2}
    };
    for (const auto &p : glider) {
        setCell(x + p.first, y + p.second, true);
    }
}

void Grid::addToad(int x, int y) {
    std::vector<std::pair<int, int>> toad = {
        {0,1}, {0,2}, {0,3}, {1,0}, {1,1}, {1,2}
    };
    for (const auto &p : toad) {
        setCell(x + p.first, y + p.second, true);
    }
}

void Grid::addBeacon(int x, int y) {
    std::vector<std::pair<int, int>> beacon = {
        {0,0}, {0,1}, {1,0}, {1,1}, {2,2}, {2,3}, {3,2}, {3,3}
    };
    for (const auto &p : beacon) {
        setCell(x + p.first, y + p.second, true);
    }
}

void Grid::addRPentomino(int x, int y) {
    std::vector<std::pair<int, int>> rpentomino = {
        {0,1}, {0,2}, {1,0}, {1,1}, {2,1}
    };
    for (const auto &p : rpentomino) {
        setCell(x + p.first, y + p.second, true);
    }
}

void Grid::setPrintEnabled(bool enabled) {
    printEnabled = enabled;
}

// Serial Implementation
int Grid::countLiveNeighborsSerial(int x, int y) const {
    int liveCount = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
                liveCount += currentGeneration[nx][ny];
            }
        }
    }
    return liveCount;
}

void Grid::evolveSerial() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int liveNeighbors = countLiveNeighborsSerial(i, j);
            if (currentGeneration[i][j]) {
                nextGeneration[i][j] = (liveNeighbors == 2 || liveNeighbors == 3);
            } else {
                nextGeneration[i][j] = (liveNeighbors == 3);
            }
        }
    }
    previousGeneration = currentGeneration;
    currentGeneration.swap(nextGeneration);
}

// Parallel Implementation
int Grid::countLiveNeighbors(int x, int y) const {
    int liveCount = 0;
    #pragma omp parallel for reduction(+:liveCount)
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
                liveCount += currentGeneration[nx][ny];
            }
        }
    }
    return liveCount;
}

void Grid::evolve() {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int liveNeighbors = countLiveNeighbors(i, j);
            if (currentGeneration[i][j]) {
                nextGeneration[i][j] = (liveNeighbors == 2 || liveNeighbors == 3);
            } else {
                nextGeneration[i][j] = (liveNeighbors == 3);
            }
        }
    }
    previousGeneration = currentGeneration;
    currentGeneration.swap(nextGeneration);
}

bool Grid::is_stable() {
    return previousGeneration == currentGeneration;
}

void Grid::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    int result = system("clear");
    (void)result;
#endif
}