#include <random>
#include <iostream>

#include "pseudoRandomGeneration.h"

// Returns a pseudo random value with uniform integer distribution using Mercenne Twister algorithm.
int getRand(int A, int B) {
    static std::random_device randDev; // Generate pseudo random numbers from hardware.
    static std::mt19937 mercenneTwister(randDev()); // Mercenne Twister.
    static std::uniform_int_distribution<int> dist; // Uniform integer distribution.

    dist.param(std::uniform_int_distribution<int>::param_type(A, B));
    return dist(mercenneTwister);
}

// Returns a pair of unique pseudo random values with uniform integer distribution using Mercenne Twister algorithm.
// First value in the pair will be no greater than the second pair.
std::pair<int, int> getRandUniquePair(int a, int b)
{
    std::pair<int, int> uniquePair; // Unique pair of integer values.

    uniquePair.first = getRand(a, b); // Randomly select first value.
    uniquePair.second = getRand(a, b); // Randomly select secondvalue.

    while (uniquePair.first == uniquePair.second) { // Make sure the two values are different from each other.
        uniquePair.second = getRand(a, b);
    }

    if (uniquePair.first > uniquePair.second) { // First value should be smaller than the second value.
        std::swap(uniquePair.first, uniquePair.second);
    }

    return uniquePair;
}