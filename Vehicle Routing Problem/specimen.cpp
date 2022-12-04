#include <algorithm>

#include "specimen.h"
#include "instanceFile.h"
#include <random>

specimen::specimen() {
}

specimen::~specimen() {

}

// Copy constructor for generating offspring.
specimen::specimen(const specimen& old, instanceFile instanceFile) {
    //TODO: copy a list?

    totalCost = old.totalCost;
}

int specimen::getTotalCost() {
    return totalCost;
}

// Calculate the total cost of the specimen by travelling from point to point until a capacity limit is reached
// and then returning to depot. Repeat the process until there are no more points left.
int specimen::calculateTotalCost(instanceFile instanceFile) {
    totalCost = 0;
    int currentCapacity = 0;

    for (int i = 1; i < instanceFile.dimension; ) {
        if (currentCapacity + instanceFile.pointsVector[pointOrder[i]].getDemand() <= instanceFile.maxCapacity) { // If there's capacity for the next point.
            if (currentCapacity == 0) totalCost += instanceFile.distanceBetweenPoints[0][pointOrder[i]]; // Get distance between depot and first point on a new subroute.
            else totalCost += instanceFile.distanceBetweenPoints[pointOrder[i - 1]][pointOrder[i]]; // Get distance between previous point and current point in a subroute.

            currentCapacity += instanceFile.pointsVector[pointOrder[i]].getDemand(); // Increase capacity for current point.
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

void specimen::setPointOrder(std::vector<int>& pointOrder) {
    this->pointOrder = pointOrder;
}

void specimen::clearPointOrder() {
    pointOrder.clear();
}

//
int specimen::getPoint(int index)
{
    return pointOrder[index];
}

// Generates a random route by shuffling the order of points around.
void specimen::generateRandomRoute(instanceFile instanceFile, std::vector<int>& pointOrder) {
    unsigned rnd = 0; // Creates a random formula for shuffling. //TODO maybe move it to calling function from specimen.cpp, since there's a new seed being initialised each time?

    std::shuffle(pointOrder.begin() + 1, pointOrder.end(), std::default_random_engine(rnd)); // Shuffle the order of points on a route (don't shuffle depot).
    

    for (int i = 0; i < instanceFile.dimension; i++) {
        this->pointOrder.push_back(pointOrder[i]);
    }
    
    totalCost = calculateTotalCost(instanceFile); //TODO maybe remove totalCost = part
}

void specimen::mutateSwap(instanceFile instanceFile) {
    int index1, index2;

    index1 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the first point. Don't swap first point (depot).
    index2 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the second point. Don't swap first point (depot).

    while (index1 == index2) index2 = rand() % (instanceFile.dimension - 1) + 1;

    std::swap(pointOrder[index1], pointOrder[index2]);
}

// Swap the order of all elements between two random elements in a specimen (including those two elements).
void specimen::mutateInvert(instanceFile instanceFile) {
    int index1, index2;

    index1 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the first point. Don't swap first point (depot).
    index2 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the second point. Don't swap first point (depot).

    while (index1 == index2) index2 = rand() % (instanceFile.dimension - 1) + 1;

    if (index1 > index2) std::swap(index1, index2);

    while (index1 < index2) {
        std::swap(pointOrder[index1], pointOrder[index2]);
        index1++;
        index2--;
    }
}

std::vector<int> specimen::getPointsOnARoute(instanceFile instanceFile) {
    std::vector<int> pointsOnARoute;

    int currentCapacity = 0;

    pointsOnARoute.push_back(0);

    for (int i = 1; i < instanceFile.dimension; ) {
        if (currentCapacity + instanceFile.pointsVector[pointOrder[i]].getDemand() <= instanceFile.maxCapacity) { // If there's capacity for the next point.
            pointsOnARoute.push_back(pointOrder[i]);
            currentCapacity += instanceFile.pointsVector[pointOrder[i]].getDemand(); // Increase capacity for current point.
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

//TODO verify
void specimen::setPointLocations(instanceFile instanceFile) {
    if (pointLocationSet == true) return; //

    pointLocation.assign(instanceFile.dimension, 0); // Fill values

    for (int i = 0; i < instanceFile.dimension; i++) {
        pointLocation[pointOrder[i]] = i; // Point i can be found at an i spot in the array.  
    }

    pointLocationSet = true; // 
}