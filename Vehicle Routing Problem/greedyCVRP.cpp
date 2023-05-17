#include <iostream>
#include <random>
#include <fstream>

#include "greedyCVRP.h"
#include "instanceFile.h"

// Generate a solution to an instance file using greedy cVRP algorithm.
void greedyCVRP::generateASolution(instanceFile instanceFile) {
    instanceFile.calculateDistancesBetweenPoints();

    totalCost = 0; // Reset variables.
    pointOrder.clear();
    currentCapacity = 0;
    currentPoint = 0;

    for (int i = 1; i < instanceFile.dimension; i++) { // Add all the points (except for the depot) to the list of points to be visited.
        pointsLeftToVisit.push_back(i);
    }

    pointOrder.push_back(0);
    currentPoint = 0;

    for (int i = 1; i < instanceFile.dimension; i++) { // Analyse all the points (except for the depot).
        findNextPoint(instanceFile); // Find the next "best" point and add it.
    }

    if (currentPoint != 0) { // If the last subroute didn't end at the depot.
        totalCost += instanceFile.distanceBetweenPoints[currentPoint][0];
        currentPoint = 0;
        currentCapacity = 0;
    }

    instanceFile.dealocateDistancesBetweenPoints();
}

// Skip initialisation data that's not needed in greedy algorithm.
void greedyCVRP::ignoreInitialAlgorithmParameters(std::ifstream &experimentFile)
{
    std::string textLine;

    for (int i = 0; i < 9; i++) {
        std::getline(experimentFile, textLine); // Skip unneeded info.
    }
}

// Find the next best point to go to using greedy algorithm.
void greedyCVRP::findNextPoint(instanceFile instanceFile) {
    int lowestCostPointIndex = -1;
    int lowestCostPoint = 0;
    int lowestCost = INT_MAX;

    for (int i = 0; i < pointsLeftToVisit.size(); i++) { // Search for the "best" point among points that can still be visited.
        if (instanceFile.distanceBetweenPoints[currentPoint][pointsLeftToVisit[i]] < lowestCost
            && currentCapacity + instanceFile.pointsVector[pointsLeftToVisit[i]].getWeight() <= instanceFile.getMaxCapacity()) {
            lowestCostPointIndex = i;
            lowestCostPoint = pointsLeftToVisit[i];
            lowestCost = instanceFile.distanceBetweenPoints[currentPoint][pointsLeftToVisit[i]];
        }
    }

    if (lowestCostPointIndex == -1) { // If none of the remaining points can fit onto the current subroute (doesn't meet the capacity criteria).
        totalCost += instanceFile.distanceBetweenPoints[currentPoint][0]; // Go back to the depot.
        currentPoint = 0;
        currentCapacity = 0;
        findNextPoint(instanceFile);
    }

    else { // If a point that manages to meet capacity criteria has been found.
        pointOrder.push_back(lowestCostPoint); // Add it to the current subroute.
        currentPoint = lowestCostPoint;
        currentCapacity += instanceFile.pointsVector[pointsLeftToVisit[lowestCostPointIndex]].getWeight();
        totalCost += lowestCost;
        pointsLeftToVisit.erase(pointsLeftToVisit.begin() + lowestCostPointIndex);
    }
}