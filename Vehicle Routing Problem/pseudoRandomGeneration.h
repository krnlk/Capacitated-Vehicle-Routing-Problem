#pragma once

int getRand(int a, int b); // Returns a pseudo random value with uniform integer distribution using Mercenne Twister algorithm.

// Returns a pair of pseudo random values, where first value in the pair will be no greater than the second pair.
std::pair<int, int> getRandUniquePair(int a, int b);