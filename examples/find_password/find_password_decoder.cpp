/**
 * @file find_password_decoder.cpp
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief solving a maximization problem using GA
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include "..\inc\GeneticAlgorithm.h"

using namespace std;

// Your friend discovered a security hole in his neighbor's wireless network,
// which allows him to determine the length of the password
// and test any combination of passwords that comes to his mind.
// Moreover, he can measure the time it takes to verify the password,
// which allows him to estimate the number of correct characters in each word.
// Your friend wants you to help him find the password as soon as possible
// because he is tired of waiting.

// He also discovered a way to determine the used characters to speed up the search process.

char password[] = "Ali Marouf ali marouf ALI MAROUF 2022";
char elements[] = "ALIMROUFalimrouf 20";

const int p_size = sizeof(password) - 1; // -1 to ignore '\0' char
const int e_size = sizeof(elements) - 1; // -1 to ignore '\0' char

// The chromosome represents the index of each element of the password.
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

string myDecoder(byte data[])
{
    string ans;

    for (int i = 0; i < p_size; i++)
    {
        char c = elements[data[i] % e_size];
        ans.push_back(c);
    }
    return ans;
}

int main()
{
    GeneticAlgorithm<Chromosome> ga(fitnessFunction, true, 1000, 2, 15);
    ga.initializePopulation(500);
    ga.terminationConditions.setFitnessGoal(p_size);
    auto bestIndividual = ga.solve();
    cout << "Best: c = " << myDecoder(bestIndividual.getChromosome().c) << "\n";

    return 0;
}