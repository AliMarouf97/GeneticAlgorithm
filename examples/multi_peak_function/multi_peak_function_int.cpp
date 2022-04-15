/**
 * @file multi_peak_function_int.cpp
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

// Define range as Macros
#define MN -205
#define MX 205

// Save MOD to const int to reduce recalculating operations.
const uint16_t MOD = RANGE_ENCODER(MN, MX);

#define MY_DECODER(chromosome) ((int)RANGE_DECODER(chromosome, MOD, MN))

// int MY_DECODER(uint16_t chromosome)
// {
//     return ((int) RANGE_DECODER(chromosome, MOD, MN));
// }

double fitnessFunction(uint16_t chromosome)
{
    double x = MY_DECODER(chromosome);
    return x * (0.4 + sin(x / 2));
}

int main()
{
    GeneticAlgorithm<uint16_t> ga(fitnessFunction, true, 1500, 2, 30);
    ga.initializePopulation(100);
    ga.terminationConditions.setMaxIterations(20);
    auto best = ga.solve();
    cout << "Best: f(" << MY_DECODER(best.getChromosome()) << ") = " << best.getFitness() << "\n";
    // Best: f(204) = 284.545
    return 0;
}