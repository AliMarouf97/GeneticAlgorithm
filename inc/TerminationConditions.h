/**
 * @file TerminationConditions.h
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief TerminationConditions Class is a part of the Genetic Algorithm library
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef TERMINATIONCONDITIONS_H
#define TERMINATIONCONDITIONS_H

#include <chrono>
using namespace std;

class TerminationConditions
{
public:
    // Terminate the solving process after reaching the fitness goal
    void setFitnessGoal(double goal);
    // Terminate the solving process after %maxRunningTime_ms% milliseconds
    void setMaxRunningTime_ms(int maxRunningTime_ms);
    // Terminate the solving process after reaching %maxIterations% (no improvement in the population for %maxIterations% )
    void setMaxIterations(uint64_t maxIterations);
    // Terminate the solving process after reaching %maxGeneration%, put -1 for infinite generations
    void setMaxGeneration(uint64_t maxGeneration);

    bool enableGool = false;
    bool enableTime = false;
    bool enableIterations = false;

    template <class T>
    friend class GeneticAlgorithm;

private:
    double goal;
    int maxRunningTime_ms;
    bool maximizeFitness;
    uint64_t maxIterations;
    uint64_t iterations = 0;
    double lastFitness;
    bool isFirstTime = true;
    uint64_t maxGeneration = 200;
    chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();

    void setStartTime(chrono::high_resolution_clock::time_point startTime);
    void setStartTime();
    bool checkTimeCondition();
    bool checkFitnessGoalCondition(double bestFitness);
    bool checkIterationsCondition(double bestFitness);
};

#endif