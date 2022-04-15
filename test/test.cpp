/**
 * @file test.cpp
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief test GA
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "GeneticAlgorithm.h"

using namespace std;

char password[] = "ALI";
char elements[] = "ALI";

const int p_size = sizeof(password) - 1; // -1 to ignore '\0' char
const int e_size = sizeof(elements) - 1; // -1 to ignore '\0' char

struct Chromosome
{
    byte c[p_size];
};

double fitnessFunction(Chromosome &chromosome)
{
    double scoure = 0;
    for (int i = 0; i < p_size; i++)
    {
        char c = elements[chromosome.c[i] % e_size];
        if (c == password[i])
            ++scoure;
    }

    return scoure;
}

// test Macros
#define MN 0
#define MX 2

const uint8_t MOD = RANGE_ENCODER(MN, MX);

#define MY_DECODER(chromosome) ((int)RANGE_DECODER(chromosome, MOD, MN))

int main()
{
    GeneticAlgorithm<Chromosome> ga(fitnessFunction, true, -1, 2, 15);
    ga.initializePopulation(500);
    ga.terminationConditions.setFitnessGoal(p_size);
    auto bestIndividual = ga.solve();

    // return 0;
    for (int i = 0; i < p_size; i++)
    {
        char c = elements[MY_DECODER(bestIndividual.getChromosome().c[i])];
        if (c != password[i])
            return 1;
    }

    return 0;
}