#include <random>
#include <fstream>

#include "greedyCVRP.h"
#include "instanceFile.h"

#include <iostream>

void greedyCVRP::findNextPoint(instanceFile instanceFile) {
    int lowestCostPointIndex = -1;
    int lowestCostPoint = 0;
    int lowestCost = INT_MAX;

    for (int i = 0; i < pointsLeftToVisit.size(); i++) {
        if (instanceFile.distanceBetweenPoints[currentPoint][pointsLeftToVisit[i]] < lowestCost 
            && currentCapacity + instanceFile.pointsVector[pointsLeftToVisit[i]].getWeight() <= instanceFile.maxCapacity) {
            lowestCostPointIndex = i;
            lowestCostPoint = pointsLeftToVisit[i];
            lowestCost = instanceFile.distanceBetweenPoints[currentPoint][pointsLeftToVisit[i]];
        }
    }

    if (lowestCostPointIndex == -1) {
        totalCost += instanceFile.distanceBetweenPoints[currentPoint][0];
        currentPoint = 0;
        currentCapacity = 0;
        findNextPoint(instanceFile);
    }
    else {
        pointOrder.push_back(lowestCostPoint);
        currentPoint = lowestCostPoint;
        currentCapacity += instanceFile.pointsVector[pointsLeftToVisit[lowestCostPointIndex]].getWeight();
        totalCost += lowestCost;
        pointsLeftToVisit.erase(pointsLeftToVisit.begin() + lowestCostPointIndex);
    }
}

//
void greedyCVRP::generateASolution(instanceFile instanceFile) {
    instanceFile.calculateDistancesBetweenPoints();
    unsigned rnd = 0; // Creates a random formula for shuffling.

    totalCost = 0;
    pointOrder.clear();
    currentCapacity = 0;
    currentPoint = 0;

    for (int i = 1; i < instanceFile.dimension; i++) {
        pointsLeftToVisit.push_back(i);
    }

    pointOrder.push_back(0);
    currentPoint = 0;

    for (int i = 1; i < instanceFile.dimension; i++) {
        findNextPoint(instanceFile);
    }

    if (currentPoint != 0) {
        totalCost += instanceFile.distanceBetweenPoints[currentPoint][0];
        currentPoint = 0;
        currentCapacity = 0;
    }

    instanceFile.dealocateDistancesBetweenPoints();
}

void greedyCVRP::ignoreInitialAlgorithmParameters(std::ifstream& experimentFile)
{
    std::string textLine;

    for (int i = 0; i < 9; i++) {
        std::getline(experimentFile, textLine); // Skip unneeded info.
    }
}