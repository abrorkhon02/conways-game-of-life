#include "CLI.h"
#include <iostream>

void CLI::run() {
    Grid world;
    world.initializePattern("./start_pattern.txt");
    bool printEnabled;
    
    std::cout << "Enable printing after each generation? (1 for yes, 0 for no): ";
    std::cin >> printEnabled;
    world.setPrintEnabled(printEnabled);
    
    int delay_ms;
    std::cout << "Enter delay in milliseconds between generations: ";
    std::cin >> delay_ms;

    const int generations = 20;
    
    // Run serial version
    std::cout << "\nRunning serial version...\n";
    long long serialTime = world.runSerial(generations, delay_ms);
    std::cout << "Serial time: " << serialTime << " ms\n";

    // Reset the world state
    world.initializePattern("./start_pattern.txt");

    // Run parallel version
    std::cout << "\nRunning parallel version...\n";
    long long parallelTime = world.run(generations, delay_ms);
    std::cout << "Parallel time: " << parallelTime << " ms\n";
    
    // Calculate speedup
    double speedup = static_cast<double>(serialTime) / parallelTime;
    std::cout << "Speedup: " << speedup << "x\n";

    // Save final state
    const std::string saveFile = "world_state.txt";
    if (world.save(saveFile)) {
        std::cout << "\nWorld saved to " << saveFile << ".\n";
    } else {
        std::cerr << "Error saving world to " << saveFile << ".\n";
    }
}