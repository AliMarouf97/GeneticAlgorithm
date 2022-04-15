/**
 * @file GeneticAlgorithm.h
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief GeneticAlgorithm Class is the main class of the Genetic Algorithm library
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GENETICALGORITHMS_H
#define GENETICALGORITHMS_H

#include "Individual.h"
#include "TerminationConditions.h"
#include <vector>
#include <memory>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <functional>
#include <chrono>
using namespace std;

#define GET_BIT(a, i) ((a >> i) & 1)
#define SET_BIT(a, i) (a |= (1UL << i))
#define RESET_BIT(a, i) (a &= ~(1UL << i))
#define CHANGE_BIT(a, i, b) (a = (a & ~(1UL << i)) | ((b > 0) << i))
#define FLIP_BIT(a, b) ((a) ^= (1ULL << (b)))
#define RANDOM(mx) (rand() % (mx))

// 10 ^ p
#define TEN(p) ((1e##p))

#define RANGE_ENCODER(mn, mx) (1LL + (int64_t)(mx) - (int64_t)(mn))
#define RANGE_DECODER(x, MOD, mn) (((x) % MOD) + (mn))

#define RANGE_ENCODER_F(mn, mx, POINTS) (RANGE_ENCODER(mn, mx - 1) * (uint64_t)(TEN(POINTS)))
#define RANGE_DECODER_F(x, MOD, mn, POINTS) (((x) % MOD) / TEN(POINTS) + (mn))

enum Selection
{
    Fast,
    RouletteWheel,
    MixSelection
};
enum Crossover
{
    Uniform,
    OnePoint,
    TwoPoint,
    MixCrossover
};

template <class T>
class GeneticAlgorithm
{
public:
    /**
     * @brief Construct a new Genetic Algorithm object
     *
     * @tparam T the type of the chromosome structure
     * @param fitnessFunction defines by user 'double fitness(T& chromosome)'
     * @param maximizeFitness true to Maximize the fitness, false to Minimize the fitness
     * @param maxGeneration put -1 for infinite generations
     * @param MutationPercentage Mutation percentage in the new generation
     * @param ElitePercentage Elite percentage in the population
     * @param selection parent selection method: 'Fast, RouletteWheel, MixSelection'
     * @param crossover crossover method: 'Uniform, OnePoint, TwoPoint, MixCrossover'
     */
    GeneticAlgorithm(function<double(T &)> fitnessFunction, bool maximizeFitness = true, uint64_t maxGeneration = 500, double MutationPercentage = 1.5, double ElitePercentage = 15.0, Selection selection = Fast, Crossover crossover = Uniform);
    ~GeneticAlgorithm();

    T generateRandomChromosome();
    // Initialize population from predefined vector
    void initializePopulation(vector<Individual<T>> &population);
    // Initialize population with random chromosomes
    void initializePopulation(int size);

    // Solve the problem and return the best individual found
    Individual<T> solve();
    // Set elite percentage in the population
    void setElitePercentage(double ElitePercentage);
    // Biased random number generation for small numbers
    int biasedRandom(int maxOutput);

    // Set parent selection method: 'Fast, RouletteWheel, MixSelection'
    void setParentSelectionMethod(Selection selection) { this->selection = selection; }
    // Get the current parent selection method, 'Fast, RouletteWheel, MixSelection'
    Selection getParentSelectionMethod() { return selection; }

    // Set crossover method: 'Uniform, OnePoint, TwoPoint, MixCrossover'
    void setCrossoverMethod(Crossover crossover) { this->crossover = crossover; }
    // Get the current crossover method, 'Uniform, OnePoint, TwoPoint, MixCrossover'
    Crossover getCrossoverMethod() { return crossover; }

    /**
     * @brief Chromosome Fix Function is used to fix the chromosome when unwanted genes appear during the process of crossing over
     *
     * @tparam T the type of the chromosome structure
     * @param chromosomeFixFunction defines by user 'void fix(T& chromosome)'
     */
    void setChromosomeFixFunction(function<void(T &)> chromosomeFixFunction);
    // Enable / Disable chromosome fix function
    void setChromosomeFixFunctionStatus(bool status);
    // Check if the chromosome fix function is enabled
    bool isChromosomeFixFunctionEnabled() { return chromosomeFixFunctionStatus; }

    // Set the Kick Out Age (dying of old age)
    void setKickOutAge(int kickOutAge, bool exceptBestOne = true)
    {
        this->kickOutAge = kickOutAge;
        kickOutExceptBest = exceptBestOne;
    }

    // Terminate the solving process when one of the conditions occur
    TerminationConditions terminationConditions;

private:
    double MutationPercentage;
    double ElitePercentage;
    int EliteSize;
    int kickOutAge;
    bool kickOutExceptBest;
    vector<Individual<T>> population;
    function<double(T &)> fitnessFunction;
    bool maximizeFitness;
    int getPercentageSize(double Percentage);

    // Cross over
    inline Individual<T> mate(Individual<T> &parent1, Individual<T> &parent2);
    Individual<T> uniform(Individual<T> &parent1, Individual<T> &parent2);
    Individual<T> onePoint(Individual<T> &parent1, Individual<T> &parent2);
    Individual<T> twoPoint(Individual<T> &parent1, Individual<T> &parent2);

    Crossover crossover;
    Selection selection;

    /// RouletteWheel Selection
    double fitnessSum;
    vector<double> fitnessValues;
    void prepareRouletteWheelSelection();
    /**
     * @brief Select parents using Roulette Wheel selection
     * @note Time complexity: O(log n), where n is the size of the population
     *
     * @param[out] p1 parent1 index
     * @param[out] p2 parent2 index
     */
    void rouletteWheelSelection(int &p1, int &p2);

    /// Fast Selection
    int goodRange, elite, populationSize;
    void prepareFastSelection();
    /**
     * @brief Select parents using Fast selection
     * @note Time complexity: O(1)
     *
     * @param[out] p1 parent1 index
     * @param[out] p2 parent2 index
     */
    void fastSelection(int &p1, int &p2);

    /// Fix Chromosome
    bool chromosomeFixFunctionStatus;
    function<void(T &)> chromosomeFixFunction;
};

template <class T>
GeneticAlgorithm<T>::GeneticAlgorithm(function<double(T &)> fitnessFunction, bool maximizeFitness, uint64_t maxGeneration, double MutationPercentage, double ElitePercentage, Selection selection, Crossover crossover)
{
    assert((fitnessFunction != nullptr) && "No fitness function");
    this->MutationPercentage = MutationPercentage;
    this->fitnessFunction = fitnessFunction;
    this->maximizeFitness = maximizeFitness;
    setElitePercentage(ElitePercentage);
    setParentSelectionMethod(selection);
    setCrossoverMethod(crossover);
    terminationConditions.setMaxGeneration(maxGeneration);
    kickOutAge = -1;
    chromosomeFixFunctionStatus = false;
}

template <class T>
GeneticAlgorithm<T>::~GeneticAlgorithm<T>()
{
}

template <class T>
T GeneticAlgorithm<T>::generateRandomChromosome()
{
    const int sz = sizeof(T);
    T newChromosome;
    byte *p = (byte *)&newChromosome;

    for (int i = 0; i < sz; i++)
        p[i] = rand() % 256;

    if (chromosomeFixFunctionStatus)
        chromosomeFixFunction(newChromosome);

    return newChromosome;
}

template <class T>
void GeneticAlgorithm<T>::setElitePercentage(double ElitePercentage)
{
    this->ElitePercentage = ElitePercentage;
}

template <class T>
int GeneticAlgorithm<T>::getPercentageSize(double Percentage)
{
    if (Percentage == 0)
        return 0;
    assert((!population.empty()) && "No population yet.");
    return (int)ceil(Percentage * population.size() / 100);
}

template <class T>
void GeneticAlgorithm<T>::initializePopulation(vector<Individual<T>> &population)
{
    this->population = population;
    srand((unsigned int)time(0));
    assert((population.size() > 10) && "Population size must be greater than 10.");
    populationSize = population.size();
}

template <class T>
void GeneticAlgorithm<T>::initializePopulation(int size)
{
    population.clear();
    srand((unsigned int)time(0));
    assert((size > 10) && "Population size must be greater than 10.");
    for (int i = 0; i < size; i++)
    {
        T chromosome = generateRandomChromosome();
        population.push_back(Individual<T>(fitnessFunction, chromosome));
    }

    populationSize = (int)population.size();
}

template <class T>
Individual<T> GeneticAlgorithm<T>::uniform(Individual<T> &parent1, Individual<T> &parent2)
{
    byte *chromosome1 = parent1.getChromosomePointer();
    byte *chromosome2 = parent2.getChromosomePointer();

    T newChromosome;

    byte *p = (byte *)&newChromosome;
    const int sz = sizeof(T);
    for (int i = 0; i < sz; i++)
    {
        byte b = 0;
        for (int j = 0; j < 8; j++)
        {
            double r = RANDOM(10000) / 100.0;
            if (r < MutationPercentage)
                CHANGE_BIT(b, j, RANDOM(2));
            else if (r < (100.0 - MutationPercentage) / 2.0)
                CHANGE_BIT(b, j, GET_BIT(chromosome1[i], j));
            else
                CHANGE_BIT(b, j, GET_BIT(chromosome2[i], j));
        }
        p[i] = b;
    }
    if (chromosomeFixFunctionStatus)
        chromosomeFixFunction(newChromosome);
    return Individual<T>(fitnessFunction, newChromosome);
}

template <class T>
Individual<T> GeneticAlgorithm<T>::onePoint(Individual<T> &parent1, Individual<T> &parent2)
{
    byte *chromosome1 = parent1.getChromosomePointer();
    byte *chromosome2 = parent2.getChromosomePointer();

    T newChromosome;

    byte *p = (byte *)&newChromosome;
    const int sz = sizeof(T);

    int crossPoint = RANDOM(populationSize);
    for (int i = 0; i < sz; i++)
        if (i < crossPoint)
            p[i] = chromosome1[i];
        else if (i > crossPoint)
            p[i] = chromosome2[i];
        else
        {
            byte b = 0;
            int crossBit = RANDOM(8);
            for (int j = 0; j < 8; j++)
                if (j < crossBit)
                    CHANGE_BIT(b, j, GET_BIT(chromosome1[i], j));
                else
                    CHANGE_BIT(b, j, GET_BIT(chromosome2[i], j));
            p[i] = b;
        }

    double r = RANDOM(10000) / 100.0;
    if (r < MutationPercentage / 2.0)
    {
        // Swap Mutation
        int r1 = RANDOM(sz);
        int r2 = RANDOM(sz);

        byte tmp = p[r1];
        p[r1] = p[r2];
        p[r2] = tmp;
    }
    else if (r < MutationPercentage)
    {
        /// Flip bit Mutation
        int r1 = RANDOM(sz);
        int r2 = RANDOM(8);
        FLIP_BIT(p[r1], r2);
    }

    if (chromosomeFixFunctionStatus)
        chromosomeFixFunction(newChromosome);
    return Individual<T>(fitnessFunction, newChromosome);
}

template <class T>
Individual<T> GeneticAlgorithm<T>::twoPoint(Individual<T> &parent1, Individual<T> &parent2)
{
    byte *chromosome1 = parent1.getChromosomePointer();
    byte *chromosome2 = parent2.getChromosomePointer();

    T newChromosome;

    byte *p = (byte *)&newChromosome;
    const int sz = sizeof(T);

    int crossPoint1 = RANDOM(populationSize);
    int crossPoint2 = RANDOM(populationSize);

    if (crossPoint1 > crossPoint2)
        swap(crossPoint1, crossPoint2);

    for (int i = 0; i < sz; i++)
        if (i < crossPoint1 || crossPoint2 < i)
            p[i] = chromosome1[i];
        else if (crossPoint1 < i && i < crossPoint2)
            p[i] = chromosome2[i];
        else
        {
            byte b = 0;
            int crossBit = RANDOM(8);
            for (int j = 0; j < 8; j++)
                if (j < crossBit)
                    CHANGE_BIT(b, j, GET_BIT(chromosome1[i], j));
                else
                    CHANGE_BIT(b, j, GET_BIT(chromosome2[i], j));
            p[i] = b;
        }

    double r = RANDOM(10000) / 100.0;
    if (r < MutationPercentage / 2.0)
    {
        // Swap Mutation
        int r1 = RANDOM(sz);
        int r2 = RANDOM(sz);

        byte tmp = p[r1];
        p[r1] = p[r2];
        p[r2] = tmp;
    }
    else if (r < MutationPercentage)
    {
        /// Flip bit Mutation
        int r1 = RANDOM(sz);
        int r2 = RANDOM(8);
        FLIP_BIT(p[r1], r2);
    }

    if (chromosomeFixFunctionStatus)
        chromosomeFixFunction(newChromosome);
    return Individual<T>(fitnessFunction, newChromosome);
}

template <class T>
inline Individual<T> GeneticAlgorithm<T>::mate(Individual<T> &parent1, Individual<T> &parent2)
{
    switch (crossover)
    {
    case Uniform:
        return uniform(parent1, parent2);
        break;
    case OnePoint:
        return onePoint(parent1, parent2);
        break;
    case TwoPoint:
        return twoPoint(parent1, parent2);
        break;
    case MixCrossover:
    default:
        int r = RANDOM(100);
        if (r < 40)
            return uniform(parent1, parent2);
        else if (r < 65)
            return onePoint(parent1, parent2);
        else
            return twoPoint(parent1, parent2);
        break;
    }
}

template <class T>
void GeneticAlgorithm<T>::setChromosomeFixFunctionStatus(bool status)
{
    assert((!(status && chromosomeFixFunction == nullptr)) && "chromosomeFixFunction not found");
    chromosomeFixFunctionStatus = status;
}

template <class T>
void GeneticAlgorithm<T>::setChromosomeFixFunction(function<void(T &)> chromosomeFixFunction)
{
    this->chromosomeFixFunction = chromosomeFixFunction;
    setChromosomeFixFunctionStatus(true);
}

template <class T>
int GeneticAlgorithm<T>::biasedRandom(int maxOutput)
{
    --maxOutput;
    const int max_r = 5000;
    double x = (RANDOM(max_r) + 1.0) / max_r; // [0.0002 , 1]
    double sig = -maxOutput / 8.517193;       // 8.517193 = -log(0.0002)
    return (int)(sig * log(x));               // Output [0,maxOutput[
}

template <class T>
void GeneticAlgorithm<T>::prepareRouletteWheelSelection()
{
    assert((populationSize > 2) && "No population yet.");
    fitnessSum = 0;
    fitnessValues.clear();
    for (int i = 0; i < populationSize; i++)
    {
        fitnessSum += population[i].getFitness();
        fitnessValues.push_back(fitnessSum);
    }
}

template <class T>
void GeneticAlgorithm<T>::rouletteWheelSelection(int &p1, int &p2)
{
    double r1 = fitnessSum * (RANDOM(5001) / 5000.0);
    double r2 = fitnessSum * (RANDOM(5001) / 5000.0);

    p1 = (int)distance(fitnessValues.begin(), lower_bound(fitnessValues.begin(), fitnessValues.end(), r1));
    p2 = (int)distance(fitnessValues.begin(), lower_bound(fitnessValues.begin(), fitnessValues.end(), r2));
}

template <class T>
void GeneticAlgorithm<T>::prepareFastSelection()
{
    assert((populationSize > 2) && "No population yet.");
    elite = getPercentageSize(ElitePercentage);
    goodRange = min((int)(1.5 * elite), populationSize);
}

template <class T>
void GeneticAlgorithm<T>::fastSelection(int &p1, int &p2)
{
    int r = RANDOM(100);
    p1 = (r < 65) ? RANDOM(elite) : (r < 92) ? RANDOM(goodRange)
                                             : RANDOM(populationSize);
    r = RANDOM(100);
    p2 = (r < 75) ? RANDOM(goodRange) : (r < 90) ? biasedRandom(populationSize)
                                                 : RANDOM(populationSize);
}

template <class T>
Individual<T> GeneticAlgorithm<T>::solve()
{
    assert((!population.empty()) && "No population, use initializePopulation(size)");
    cout << "Start solving...\nMax Generation: " << terminationConditions.maxGeneration << "\n";

    terminationConditions.isFirstTime = true;
    terminationConditions.maximizeFitness = maximizeFitness;
    terminationConditions.setStartTime();

    for (uint64_t generation = 0;; generation++)
    {
        // Sorting the population
        if (maximizeFitness)
            sort(population.begin(), population.end());
        else
            sort(population.begin(), population.end(),
                 [](Individual<T> a, Individual<T> b) -> bool
                 {
                     if (a.getFitness() < b.getFitness())
                         return true;
                     if (a.getFitness() > b.getFitness())
                         return false;
                     return a.getAge() > b.getAge();
                 });

        // Check termination conditions

        if (generation == terminationConditions.maxGeneration)
        {
            cout << "Done with best fitness: " << population[0].getFitness() << "\n";
            return population[0];
        }
        if (terminationConditions.checkTimeCondition())
        {
            cout << "Timeout\n";
            return population[0];
        }
        if (terminationConditions.checkFitnessGoalCondition(population[0].getFitness()))
        {
            cout << "Reached to fitness goal: " << population[0].getFitness() << "\n";
            return population[0];
        }
        if (terminationConditions.checkIterationsCondition(population[0].getFitness()))
        {
            cout << "Reached to max iterations\n";
            return population[0];
        }

        vector<Individual<T>> newGeneration;

        int elite = getPercentageSize(ElitePercentage);

        if (kickOutAge == -1) // if kickOutAge is disabled
            // Move the elite to the next generation.
            for (int i = 0; i < elite; i++)
            {
                newGeneration.push_back(population[i]);
                newGeneration[i].increaseAge();
            }
        else
        {
            // Move the young elite to the next generation.

            int added = 0;
            if (kickOutExceptBest || population[0].getAge() < kickOutAge || RANDOM(100) < 30) // Best one
            {
                newGeneration.push_back(population[0]);
                newGeneration[0].increaseAge();
                ++added;
            }

            for (int i = 1; added < elite && i < populationSize; i++)
                if (population[i].getAge() < kickOutAge || RANDOM(100) < 45)
                {
                    newGeneration.push_back(population[i]);
                    newGeneration[added].increaseAge();
                    ++added;
                }

            /// Add new individuals if adders are less than elite to keep the population size the same.
            for (int i = added; i < elite; i++)
            {
                T chromosome = generateRandomChromosome();
                newGeneration.push_back(Individual<T>(fitnessFunction, chromosome));
                ++added;
            }
        }

        // Prepare parents' selection method.
        switch (selection)
        {
        case Fast:
            prepareFastSelection();
            break;
        case RouletteWheel:
            prepareRouletteWheelSelection();
            break;
        case MixSelection:
        default:
            prepareFastSelection();
            prepareRouletteWheelSelection();
            break;
        }

        int s = populationSize - elite;
        for (int i = 0; i < s; i++)
        {
            int p1, p2;

            // Select parents
            switch (selection)
            {
            case Fast:
                fastSelection(p1, p2);
                break;
            case RouletteWheel:
                rouletteWheelSelection(p1, p2);
                break;
            case MixSelection:
            default:
                if (RANDOM(100) < 60)
                    fastSelection(p1, p2);
                else
                    rouletteWheelSelection(p1, p2);
                break;
            }

            Individual<T> offspring = mate(population[p1], population[p2]);
            newGeneration.push_back(offspring);
        }
        population = newGeneration;
        cout << "Generation: " << generation << "\tFitness: " << population[0].getFitness() << "\n";
    }
}

#endif