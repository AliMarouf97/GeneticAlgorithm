/**
 * @file housing_development.cpp
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief solving a maximization problem using GA
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include "../inc/GeneticAlgorithm.h"
#include <chrono>
using namespace std;

// A contractor is planning to build a new housing development consisting of colonial, split-level, and ranch-style houses.
// A colonial house requires one-half acre of land, $60,000 capital and 4,000 laborhours to construct, and returns a profit of $20,000.
// A split-level house requires one-half acre of land $60,000 capital, and 3000 labor-hours to construct and returns a profit of $18,000.
// A ranch house requires 1 acre of land, $80,000 capital, and 4,000 labor-hours to construct, and returns a profit of $24,000.
// The contractor has available 30 acres of land, $3,200,000 capital, and 180,000 labor-hours.
// How many houses of each type should be constructed to maximize the contactor’s profit? What is the maximum profit.

// ranges
// 30/0.5 = 60, 3200/60 = 53, 180/4 = 45 => at most 45 colonial house
// 30/0.5 = 60, 3200/60 = 53, 180/3 = 60 => at most 53 split-level house
// 30/1 = 30, 3200/80 = 40, 180/4 = 45 => at most 30 ranch house

struct Chromosome
{
    byte colonial;
    byte split_level;
    byte ranch;
};

double fitnessFunction(Chromosome chromosome)
{
    int colonial = chromosome.colonial % 46;       // [0,45]
    int split_level = chromosome.split_level % 54; // [0,53]
    int ranch = chromosome.ranch % 31;             // [0,30]

    double acre = 0.5 * colonial + 0.5 * split_level + ranch;
    int capital = 60 * colonial + 60 * split_level + 80 * ranch;
    int labor_hours = 4 * colonial + 3 * split_level + 4 * ranch;

    if (acre > 30 || capital > 3200 || labor_hours > 180)
        return -1e9;

    return 20 * colonial + 18 * split_level + 24 * ranch;
}

int main()
{
    GeneticAlgorithm<Chromosome> ga(fitnessFunction, true, 200, 2, 30);
    ga.initializePopulation(400);
    auto bestFound = ga.solve();
    cout << "Best: colonial = " << bestFound.getChromosome().colonial % 46 << ", split_level = " << bestFound.getChromosome().split_level % 54 << ", ranch = " << bestFound.getChromosome().ranch % 31 << ", $ = " << bestFound.getFitness() << "\n";
    // Best: colonial = 20, split_level = 28, ranch = 4, $ = 1000
    // or
    // Best: colonial = 20, split_level = 20, ranch = 10, $ = 1000
    // or
    // Best: colonial = 20, split_level = 32, ranch = 1, $ = 1000
    return 0;
}