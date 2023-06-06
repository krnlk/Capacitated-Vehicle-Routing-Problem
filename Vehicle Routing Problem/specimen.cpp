#include <algorithm>
#include <random>

#include "specimen.h"
#include "instanceFile.h"
#include "pseudoRandomGeneration.h"

// Mutation operator - swap.
// Swap the order of two randomly selected points in this specimen.
void specimen::mutateSwap(std::pair<int, int> mutationPair) {
    std::swap(pointOrder[mutationPair.first], pointOrder[mutationPair.second]); // Swap order of points.

}

// Mutation operator - invert.
// Swap the order of all points between two randomly selected points in this specimen (including those two points).
void specimen::mutateInvert(std::pair<int, int> mutationPair) {
    while (mutationPair.first < mutationPair.second) { // Keep swapping points until the indexes "meet" each other.
        std::swap(pointOrder[mutationPair.first], pointOrder[mutationPair.second]);
        mutationPair.first++;
        mutationPair.second--;
    }
}