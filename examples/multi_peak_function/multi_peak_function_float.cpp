/**
 * @file multi_peak_function_float.cpp
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief solving a maximization problem using GA
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include "../inc/GeneticAlgorithm.h"
using namespace std;

// In the range [-205, 205] what is the maximum value of the function $f(x) = x * (0.4 + sin(x / 2))$.

double fitnessFunction(float chromosome)
{
    float x = chromosome;

    if (isnan(x) || isinf(x)) // if bad chromosome
        return -1e9;          // return very low fitness

    if (x > 205 || x < -205) // if out of range [-205,205]
        return -1e9;         // return very low fitness

    return x * (0.4 + sin(x / 2));
}

int main()
{
    GeneticAlgorithm<float> ga(fitnessFunction, true, 1500, 2, 30);
    ga.initializePopulation(500);
    ga.terminationConditions.setMaxIterations(200);
    auto best = ga.solve();
    std::cout << "Best: f(" << best.getChromosome() << ") = " << best.getFitness() << "\n";
    // Best: f(204.231) = 285.904
    return 0;
}