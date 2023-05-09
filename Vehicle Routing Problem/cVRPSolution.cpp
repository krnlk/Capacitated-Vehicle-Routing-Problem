#include <algorithm>
#include <random>
#include <sstream> 
#include <iostream>

#include "cVRPSolution.h"

// Get total cost of this solution.
int cVRPSolution::getTotalCost() {
    return totalCost;
}

// Get a point from this solution by index.
int cVRPSolution::getPointByIndex(int index)
{
    return pointOrder[index];
}

// Get point order of this solution saved to a string variable.
std::string cVRPSolution::getStringifiedPointOrder() {
    std::stringstream stringifiedPointOrder;
    for (auto iterator = pointOrder.begin(); iterator != pointOrder.end(); iterator++) {
        if (iterator != pointOrder.begin()) {
            stringifiedPointOrder << " ";
        }
        stringifiedPointOrder << *iterator;
    }
    return stringifiedPointOrder.str();
}

// Construct a full route (including subroutes, which are separated by "0", marking return to the depot).
// The subroutes are determined by going through the array of visited points in order and adding up the capacity of current subroute.
// Once there's not enough capacity for the next point, insert return to depot and start constructing a new subroute.
std::vector<int> cVRPSolution::getPointsOnARoute(instanceFile instanceFile) {
    std::vector<int> pointsOnARoute;

    int currentCapacity = 0;

    pointsOnARoute.push_back(0);

    for (int i = 1; i < instanceFile.dimension; ) {
        if (currentCapacity + instanceFile.pointsVector[pointOrder[i]].getWeight() <= instanceFile.getMaxCapacity()) { // If there's capacity for the next point.
            pointsOnARoute.push_back(pointOrder[i]);
            currentCapacity += instanceFile.pointsVector[pointOrder[i]].getWeight(); // Increase capacity for current point.
            i++;
        }
        else { // If there's no capacity for next point.
            pointsOnARoute.push_back(0);
            currentCapacity = 0; // Reset capacity for the next subroute.
        }
    }

    if (currentCapacity != 0) { // Connect the last point in specimen with a depot if it hasn't been connected already.
        totalCost += instanceFile.distanceBetweenPoints[instanceFile.dimension - 1][pointOrder[0]];
        currentCapacity = 0;
        pointsOnARoute.push_back(0);
    }

    return pointsOnARoute;
}

// Set each point's location if it hasn't been set already.
void cVRPSolution::setPointLocations(instanceFile instanceFile) {
    if (pointLocationSet == true) { // If points have had their locations assigned already.
        return;
    }

    pointLocation.assign(instanceFile.dimension, 0); // Fill values.

    for (int i = 0; i < instanceFile.dimension; i++) {
        pointLocation[pointOrder[i]] = i; // Point i can be found at an i position in the array.  
    }

    pointLocationSet = true; // Mark that points have had their locations assigned already.
}

// Set point order of this solution.
void cVRPSolution::setPointOrder(std::vector<int>& pointOrder) {
    this->pointOrder = pointOrder;
}

// Clear point order of this solution.
void cVRPSolution::clearPointOrder() {
    pointOrder.clear();
}

// Calculate the total cost of this solution by travelling from point to point until a capacity limit is reached
// and then returning to the depot. Repeat the process until there are no more points left to visit.
int cVRPSolution::calculateTotalCost(instanceFile instanceFile) {
    totalCost = 0;
    int currentCapacity = 0;

    for (int i = 1; i < instanceFile.dimension; ) { // First point is a depot, therefore it's ignored. Visit all other points.
        if (currentCapacity + instanceFile.pointsVector[pointOrder[i]].getWeight() <= instanceFile.getMaxCapacity()) { // If there's capacity for the next point.
            if (currentCapacity == 0) totalCost += instanceFile.distanceBetweenPoints[0][pointOrder[i]]; // Get distance between depot and first point on a new subroute.
            else totalCost += instanceFile.distanceBetweenPoints[pointOrder[i - 1]][pointOrder[i]]; // Get distance between previous point and current point in a subroute.

            currentCapacity += instanceFile.pointsVector[pointOrder[i]].getWeight(); // Increase capacity for current point.
            i++;
        }
        else { // If there's no capacity for next point.
            totalCost += instanceFile.distanceBetweenPoints[pointOrder[i - 1]][pointOrder[0]]; // Go back from previous point to the depot and finish that subroute.
            currentCapacity = 0; // Reset capacity for the next subroute.
        }
    }

    if (currentCapacity != 0) { // Connect the last point in specimen with a depot if it hasn't been connected already.
        totalCost += instanceFile.distanceBetweenPoints[pointOrder[instanceFile.dimension - 1]][pointOrder[0]];
    }

    return totalCost;
}

// Generate a random route by shuffling the order of points around.
void cVRPSolution::generateRandomRoute(instanceFile instanceFile, std::vector<int>& pointOrder) {
    unsigned rnd = 0; // Creates a random formula for shuffling. //TODO maybe move it to calling function from specimen.cpp, since there's a new seed being initialised each time?

    std::shuffle(pointOrder.begin() + 1, pointOrder.end(), std::default_random_engine(rnd)); // Shuffle the order of points on a route (don't shuffle depot).

    for (int i = 0; i < instanceFile.dimension; i++) {
        this->pointOrder.push_back(pointOrder[i]);
    }

    totalCost = calculateTotalCost(instanceFile); 
}