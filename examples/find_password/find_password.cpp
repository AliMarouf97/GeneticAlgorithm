/**
 * @file find_password.cpp
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

// Your friend discovered a security hole in his neighbor's wireless network,
// which allows him to determine the length of the password
// and test any combination of passwords that comes to his mind.
// Moreover, he can measure the time it takes to verify the password,
// which allows him to estimate the number of correct characters in each word.
// Your friend wants you to help him find the password as soon as possible
// because he is tired of waiting.

char password[] = "Ali Marouf ali marouf ALI MAROUF 2022";

const int p_size = sizeof(password) - 1; // -1 to ignore '\0' char

// The chromosome represents the password.
struct Chromosome
{
    char c[p_size];
};

double fitnessFunction(Chromosome &chromosome)
{
    double scoure = 0;
    for (int i = 0; i < p_size; i++)
    {
        char c = chromosome.c[i];
        if (c == password[i])
            ++scoure;
    }

    return scoure;
}

int main()
{
    GeneticAlgorithm<Chromosome> ga(fitnessFunction, true, 1000, 1.5, 20);
    ga.initializePopulation(2000);
    ga.terminationConditions.setFitnessGoal(p_size);
    auto bestIndividual = ga.solve();
    printf("Best: c = %.*s", p_size, bestIndividual.getChromosome().c);
    // cout << "Best: c = " << bestIndividual.getChromosome().c << "\n";

    return 0;
}