/**
 * @file TerminationConditions.cpp
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief TerminationConditions Class is a part of the Genetic Algorithm library
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "TerminationConditions.h"

void TerminationConditions::setFitnessGoal(double goal)
{
    this->goal = goal;
    enableGool = true;
}

void TerminationConditions::setMaxRunningTime_ms(int maxRunningTime_ms)
{
    this->maxRunningTime_ms = maxRunningTime_ms;
    enableTime = true;
}

void TerminationConditions::setMaxIterations(uint64_t maxIterations)
{
    this->maxIterations = maxIterations;
    enableIterations = true;
}

void TerminationConditions::setMaxGeneration(uint64_t maxGeneration)
{
    this->maxGeneration = maxGeneration;
}

void TerminationConditions::setStartTime(chrono::high_resolution_clock::time_point startTime)
{
    this->startTime = startTime;
}

void TerminationConditions::setStartTime()
{
    startTime = chrono::high_resolution_clock::now();
}

bool TerminationConditions::checkTimeCondition()
{
    if (!enableTime)
        return false;

    auto endTime = chrono::high_resolution_clock::now();
    int elapsedTime_ms = chrono::duration<float, milli>(endTime - startTime).count();
    return elapsedTime_ms >= maxRunningTime_ms;
}

bool TerminationConditions::checkFitnessGoalCondition(double bestFitness)
{
    if (!enableGool)
        return false;
    if (maximizeFitness)
        return bestFitness >= goal;
    return bestFitness <= goal;
}

bool TerminationConditions::checkIterationsCondition(double bestFitness)
{
    if (!enableIterations)
        return false;

    if (isFirstTime)
    {
        isFirstTime = false;
        lastFitness = bestFitness;
        iterations = 0;
        return false;
    }

    if ((maximizeFitness) ? bestFitness > lastFitness : bestFitness < lastFitness)
    {
        lastFitness = bestFitness;
        iterations = 0;
        return false;
    }

    ++iterations;
    return iterations >= maxIterations;
}
