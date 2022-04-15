/**
 * @file multi_peak_function_float2.cpp
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

// Number of digits after floating point
#define POINTS 3

// Save MOD to const int to reduce recalculating operations.
const uint32_t MOD = RANGE_ENCODER_F(MN, MX, POINTS);

#define MY_DECODER(chromosome) ((double)RANGE_DECODER_F(chromosome, MOD, MN, POINTS))

// double MY_DECODER(uint32_t chromosome)
// {
//     return ((double) RANGE_DECODER_F(chromosome, MOD, MN, POINTS));
// }

double fitnessFunction(uint32_t &chromosome)
{
    double x = MY_DECODER(chromosome);
    return x * (0.4 + sin(x / 2));
}

int main()
{
    GeneticAlgorithm<uint32_t> ga(fitnessFunction, true, 1500, 2, 30);
    ga.initializePopulation(300);
    ga.terminationConditions.setMaxIterations(150);
    auto best = ga.solve();
    cout << "Best: f(" << MY_DECODER(best.getChromosome()) << ") = " << best.getFitness() << "\n";
    // Best: f(204.231) = 285.904
    return 0;
}