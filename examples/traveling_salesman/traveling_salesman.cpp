/**
 * @file traveling_salesman.cpp
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief solving a minimization problem using GA
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "..\inc\GeneticAlgorithm.h"
#include <iostream>
#include <chrono>
using namespace std;

// The traveling salesman problem (TSP) asks the following question:
// "Given a list of cities and the distances between each pair of cities,
// what is the shortest possible route that visits each city exactly once and returns to the origin city?"

// In this version, the distance between each pair of cities is the length of the line segment connecting them.

pair<int, int> citysLocation[] = {
    {-10, 10},
    {-11, 20},
    {22, 20},
    {-151, -25},
    {-211, 24},
    {-66, -32},
    {-34, 54},
    {-43, 76},
    {-62, -78},
    {-55, 99},
    {221, -12},
    {12, 20},
    {124, -30},
};

const int sz = sizeof(citysLocation) / sizeof(pair<int, int>);

// The chromosome represents the order of the cities.
struct Chromosome
{
    byte city[sz];
};

void fixChromosome(Chromosome &chromosome)
{
    vector<int> pos[sz];
    for (int i = 0; i < sz; i++)
    {
        int city = chromosome.city[i] % sz;
        pos[city].push_back(i);
    }

    vector<int> missing, changePos;

    // Find duplicate and missing items.
    for (int i = 0; i < sz; i++)
        if (pos[i].size() > 1) // duplicate item
        {
            random_shuffle(pos[i].begin(), pos[i].end());
            for (int j = 0; j < pos[i].size() - 1; j++)
                changePos.push_back(pos[i][j]);
        }
        else if (pos[i].empty()) // missing item
            missing.push_back(i);

    random_shuffle(missing.begin(), missing.end());

    // Replace duplicate items with missing items.
    for (int i = 0; i < missing.size(); i++)
    {
        int p = changePos[i];
        chromosome.city[p] = missing[i];
    }
}

#define DIS(c1, c2) sqrt(pow(c1.first - c2.first, 2) + pow(c1.second - c2.second, 2))

double fitnessFunction(Chromosome &chromosome)
{
    double score = 0;
    int firstCity = chromosome.city[0] % sz;
    int lastCity = firstCity;
    for (int i = 1; i < sz; i++)
    {
        int city = chromosome.city[i] % sz;
        score += DIS(citysLocation[city], citysLocation[lastCity]);
        lastCity = city;
    }
    score += DIS(citysLocation[firstCity], citysLocation[lastCity]);

    return score;
}

string myDecoder(byte data[])
{
    string ans;

    for (int i = 0; i < sz; i++)
    {
        char c = 'A' + data[i] % sz;
        ans.push_back(c);
    }
    return ans;
}

GeneticAlgorithm<Chromosome> ga(fitnessFunction, false, -1, 4, 30);

int main()
{
    ga.setParentSelectionMethod(Selection::MixSelection);
    ga.setCrossoverMethod(Crossover::MixCrossover);
    ga.setChromosomeFixFunction(fixChromosome);
    ga.setKickOutAge(20, false);
    ga.terminationConditions.setFitnessGoal(1010);
    // ga.terminationConditions.setMaxGeneration(3000);
    ga.terminationConditions.setMaxRunningTime_ms(10000); // 10 s
    ga.terminationConditions.setMaxIterations(500);
    ga.initializePopulation(500);

    auto t_start = chrono::high_resolution_clock::now();
    auto bestFound = ga.solve();
    auto t_end = chrono::high_resolution_clock::now();

    cout << "Best: c = " << myDecoder(bestFound.getChromosome().city) << "\t, Fitness: " << bestFound.getFitness() << "\t, Age: " << bestFound.getAge() << "\n";
    // Best: c = BALCKMIFDEJHG , Fitness: 1009.3 , Age: 0
    double elapsed_time_ms = chrono::duration<double, milli>(t_end - t_start).count();
    cout << "elapsed time: " << elapsed_time_ms << " ms\n";
    return 0;
}