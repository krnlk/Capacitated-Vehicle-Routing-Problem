#include <random>
#include <iostream>

#include "pseudoRandomGeneration.h"

// Returns a pseudo random value with uniform integer distribution using Mercenne Twister algorithm.
int getRand(const int& A, const int& B) {
    static std::random_device randDev;
    static std::mt19937 twister(randDev()); 
    static std::uniform_int_distribution<int> dist;

    dist.param(std::uniform_int_distribution<int>::param_type(A, B));
    return dist(twister);
}

// Returns a pair of unique pseudo random values with uniform integer distribution using Mercenne Twister algorithm.
// First value in the pair will be no greater than the second pair.
std::pair<int, int> getRandUniquePair(const int& a, const int& b)
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