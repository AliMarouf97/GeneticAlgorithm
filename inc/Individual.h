/**
 * @file Individual.h
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief Individual Class is a part of the Genetic Algorithm library
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

using namespace std;

typedef unsigned char byte;

template <class T>
class Individual
{
public:
    /**
     * @brief Construct a new Individual object
     *
     * @param fitnessFunction defines by user 'double fitness(T& chromosome)'
     * @param initialChromosome
     * @param age
     */
    Individual(function<double(T &)> fitnessFunction, T &initialChromosome, uint64_t age = 0)
    {
        this->fitnessFunction = fitnessFunction;
        this->chromosome = initialChromosome;
        computeFitness();
        setAge(age);
    }
    Individual(function<double(T &)> fitnessFunction, vector<byte> &initialChromosome, uint64_t age = 0)
    {
        this->fitnessFunction = fitnessFunction;
        assert((initialChromosome.size() == sizeof(chromosome)) && "Vector size does not equal structure size.");
        memcpy(&chromosome, initialChromosome.data(), initialChromosome.size());
        computeFitness();
        setAge(age);
    }
    ~Individual() {}

    double getFitness()
    {
        return fitness;
    }

    void computeFitness()
    {
        fitness = fitnessFunction(chromosome);
    }

    byte *getChromosomePointer()
    {
        return (byte *)&chromosome;
    }

    T getChromosome()
    {
        return chromosome;
    }

    uint64_t getAge() { return age; }
    void setAge(uint64_t age) { this->age = age; }
    void increaseAge() { ++age; }

    bool operator<(Individual b)
    {
        if (getFitness() > b.getFitness())
            return true;
        if (getFitness() < b.getFitness())
            return false;
        return getAge() > b.getAge();
    }

protected:
    function<double(T &)> fitnessFunction;
    T chromosome;
    double fitness;
    uint64_t age;
};

#endif