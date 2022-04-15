/**
 * @file furniture_manufacturer.cpp
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief solving a maximization problem using GA
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include "GeneticAlgorithm.h"
using namespace std;

// A furniture manufacturer makes two types of furniture – chairs and sofas.
// The production of the sofas and chairs requires three operations – carpentry, finishing, and upholstery.
// Manufacturing a chair requires 3 hours of carpentry, 9 hours of finishing, and 2 hours of upholstery.
// Manufacturing a sofa requires 2 hours of carpentry, 4 hours of finishing, and 10 hours of upholstery.
// The factory has allocated at most 66 labor hours for carpentry, 180 labor hours for finishing, and 200 labor hours for upholstery.
// The profit per chair is $90 and the profit per sofa is $75.
// How many chairs and how many sofas should be produced each day to maximize the profit?

// ranges
// 66/3 = 22, 180/9 = 20, 200/2 = 100 => at most 20 chairs
// 66/2 = 33, 180/4 = 45, 200/10 = 20 => at most 20 sofas

struct Chromosome
{
    byte chairs;
    byte sofas;
};

double fitnessFunction(Chromosome chromosome)
{
    int chairs = chromosome.chairs % 21; // [0,20]
    int sofas = chromosome.sofas % 21;   // [0,20]

    int carpentry = 3 * chairs + 2 * sofas;
    int finishing = 9 * chairs + 4 * sofas;
    int upholstery = 2 * chairs + 10 * sofas;

    if (carpentry > 66 || finishing > 180 || upholstery > 200) // if out of range
        return -1e9;                                           // return very low fitness

    return 90 * chairs + 75 * sofas;
}

int main()
{
    GeneticAlgorithm<Chromosome> ga(fitnessFunction, true, 100, 2, 20);
    ga.initializePopulation(100);
    auto bestFound = ga.solve();
    cout << "Best: Chairs = " << bestFound.getChromosome().chairs % 21 << ", Sofas = " << bestFound.getChromosome().sofas % 21 << ", $ = " << bestFound.getFitness() << "\n";
    // Best: Chairs = 10, Sofas = 18, $ = 2250
    return 0;
}