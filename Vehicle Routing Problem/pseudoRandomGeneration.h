#pragma once

int getRand(const int& a, const int& b); // Returns a pseudo random value with uniform integer distribution using Mercenne Twister algorithm.

// Returns a pair of pseudo random values, where first value in the pair will be no greater than the second pair.
std::pair<int, int> getRandUniquePair(const int& a, const int& b);