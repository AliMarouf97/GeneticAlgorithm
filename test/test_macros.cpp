/**
 * @file test_macros.cpp
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief test GA
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "..\inc\GeneticAlgorithm.h"

#define MN -205
#define MX 205

#define POINTS 3

const uint32_t MOD = RANGE_ENCODER_F(MN, MX, POINTS);

#define MY_DECODER(chromosome) ((double)RANGE_DECODER_F(chromosome, MOD, MN, POINTS))

double fitnessFunction(uint32_t &chromosome)
{
    double x = MY_DECODER(chromosome);
    return x * (0.4 + sin(x / 2));
}

int main()
{
    GeneticAlgorithm<uint32_t> ga(fitnessFunction, true, 4, 2, 30);
    ga.terminationConditions.setMaxIterations(150);
    ga.terminationConditions.setFitnessGoal(285);
    ga.terminationConditions.setMaxRunningTime_ms(100);
    ga.terminationConditions.setMaxGeneration(100);
    ga.setKickOutAge(10);
    ga.setElitePercentage(25);
    ga.setParentSelectionMethod(Selection::MixSelection);
    ga.setCrossoverMethod(Crossover::MixCrossover);
    ga.initializePopulation(300);
    auto best = ga.solve();
    float f = MY_DECODER(best.getChromosome());

    uint32_t x = 123456789;
    float y = MY_DECODER(x);

    double mn = 1e9, mx = -1e9;
    for (uint32_t i = 0; i < 812000; i++)
    {
        mn = min(mn, MY_DECODER(i));
        mx = max(mx, MY_DECODER(i));
    }

    if (fabs(mn - MN) > 0.01 || fabs(mx - MX) > 0.01)
        return 1;
    // std::cout << mn << " " << mx << "\n";
    // std::cout << y << "\n";
    if (fabs(y + 158.211) > 0.01)
        return 1;

    double z = fitnessFunction(x);
    // std::cout << z << "\n";
    if (fabs(z + 148.084) > 0.01)
        return 1;

    return 0;
}