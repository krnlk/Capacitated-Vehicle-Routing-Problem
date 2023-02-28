#include <algorithm>
#include <random>

#include "specimen.h"
#include "instanceFile.h"

// Mutation operator - swap.
// Swap the order of two randomly selected points in this specimen.
void specimen::mutateSwap(instanceFile instanceFile) {
    int index1, index2;

    index1 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the first point. Don't swap first point (depot).
    index2 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the second point. Don't swap first point (depot).

    while (index1 == index2) index2 = rand() % (instanceFile.dimension - 1) + 1; // Make sure the swap doesn't happen between the same point.

    std::swap(pointOrder[index1], pointOrder[index2]); // Swap order of points.
}

// Mutation operator - invert.
// Swap the order of all points between two randomly selected points in this specimen (including those two points).
void specimen::mutateInvert(instanceFile instanceFile) {
    int index1, index2;

    index1 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the first point. Don't swap first point (depot).
    index2 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the second point. Don't swap first point (depot).

    while (index1 == index2) index2 = rand() % (instanceFile.dimension - 1) + 1; // Make sure the invert doesn't happen between the same point.

    if (index1 > index2) std::swap(index1, index2); // index1 should be the earlier point on the route.

    while (index1 < index2) { // Keep swapping points until the indexes "meet" each other.
        std::swap(pointOrder[index1], pointOrder[index2]);
        index1++;
        index2--;
    }
}