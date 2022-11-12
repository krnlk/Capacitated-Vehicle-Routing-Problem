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

int specimen::getNumberOfSubroutes() {
    return subroutes.size();
}

int specimen::getNumberOfPointsInASubroute(int subrouteIndex)
{
    std::list<route>::iterator it = subroutes.begin();
    advance(it, subrouteIndex);
    return it->getSize();
}

route specimen::getSubroute(int subrouteIndex)
{
    std::list<route>::iterator it = subroutes.begin();
    advance(it, subrouteIndex);
    return *it;
}

// Generates a random route by shuffling the order of points around.
void specimen::generateRandomRoute(instanceFile instanceFile, int* pointOrder) {
    unsigned rnd = 0; // Creates a random formula for shuffling.

    std::shuffle(pointOrder + 1, pointOrder + instanceFile.dimension, std::default_random_engine(rnd)); // Shuffle the order of points on a route (don't shuffle depot).
    totalCost = calculateTotalCost(instanceFile, pointOrder);
}

// Calculate the total cost of the specimen by travelling from point to point until a capacity limit is reached
// and then returning to depot. Repeat the process until there are no more points left.
int specimen::calculateTotalCost(instanceFile instanceFile, int* pointOrder) {
    totalCost = 0;
    int currentSubrouteCapacity = 0;
    route subroute;

    for (int i = 1; i < instanceFile.dimension; ) { // Start from i = 1 to ignore the depot.
        subroute = createANewSubroute(instanceFile, i, pointOrder);
        subroutes.push_back(subroute); // Add a new subroute.
        totalCost += subroute.getCost(); //think about standardizing getcost and iterating the index
    }

    return totalCost;
}


route specimen::createANewSubroute(instanceFile instanceFile, int& currentPointIndex, int* pointOrder) {
    route newSubroute;
    newSubroute.generateRoute(instanceFile, currentPointIndex, pointOrder);
    return newSubroute;
}

// Remove points from a subroute.
void specimen::removePoints(instanceFile instanceFile, int amountOfPointsLeftToDelete, int* deletedPoints)
{
    std::list<route>::iterator routesIt;

    for (routesIt = subroutes.begin(); routesIt != subroutes.end() && amountOfPointsLeftToDelete > 0; ++routesIt) {
        totalCost -= routesIt->removePoints(instanceFile, amountOfPointsLeftToDelete, deletedPoints); // Remove a point and change total cost of the specimen.
    }
}

// Add missing points to a specimen.
void specimen::addPoint(instanceFile instanceFile, int amountOfPointsLeftToAdd, int* addedPoints)
{
    std::list<route>::iterator routesIt;

    std::list<route>::iterator bestInsertionIt; // Best insertion's route.
    std::pair<std::list<int>::iterator, int> bestInsertion; // Best insertion's iterator & value.
    std::pair<std::list<int>::iterator, int> newInsertion;

    for (int i = 0; i < amountOfPointsLeftToAdd; i++) { // For every point that needs to be added.
        bestInsertion = make_pair(subroutes.begin()->getIterator(), INT_MAX); // New point being added - reset best insertion.

        for (routesIt = subroutes.begin(); routesIt != subroutes.end() && amountOfPointsLeftToAdd > 0; ++routesIt) { // Find the best place to insert a point.
            if (routesIt->getCapacity() + instanceFile.pointsArray[addedPoints[i]].getDemand() <= instanceFile.maxCapacity) // Only check the subroute if it has the capacity to fit the point.
                newInsertion = routesIt->findPointInsertionIndex(instanceFile, amountOfPointsLeftToAdd, addedPoints[i]);
            if (newInsertion.second > bestInsertion.second) { // If a better insertion point has been found.
                bestInsertionIt = routesIt; // Update the route with the best insertion point.
                bestInsertion = newInsertion; // Update the position and value of the new best insertion spot.
            }
        }

        if (bestInsertion.second == INT_MAX) { // If point doesn't fit anywhere, create a new route for it.
            addNewSubroute(instanceFile, addedPoints[i]);
        }
        else { // If the point fits somewhere in the existing subroutes.
            bestInsertionIt->addPoint(instanceFile, bestInsertion); // Insert a point into the route with the "best" spot.
        }
    }

    //TODO probably remove it later
    /*
    for (routesIt = subroutes.begin(); routesIt != subroutes.end() && amountOfPointsLeftToAdd > 0; ++routesIt) { //fix the addedpoints0 placeholder
        if (routesIt->getCapacity() + instanceFile.pointsArray[addedPoints[0]].getDemand() <= instanceFile.maxCapacity) // Only check the subroute if it has the capacity to fit the point.
            routesIt->addPoints(instanceFile, amountOfPointsLeftToAdd, addedPoints);
    }
    */
}

/*
route specimen::calculateSubrouteCost(instanceFile instanceFile, int& currentPointIndex, int* pointOrder) {
    int currentCapacity = 0;
    int currentCost = 0;

    currentCost += instanceFile.pointsArray[0].getDistanceToAnotherPoint(instanceFile.pointsArray[pointOrder[currentPointIndex]]); // Get distance from depot to the first point on a subroute.
    currentCapacity += instanceFile.pointsArray[pointOrder[currentPointIndex]].getDemand(); // Get the capacity of the first point on a subroute.
    currentPointIndex++; // Move onto the next point in the subroute.

    while (currentPointIndex < instanceFile.dimension) {
        if (currentCapacity + instanceFile.pointsArray[pointOrder[currentPointIndex]].getDemand() <= instanceFile.maxCapacity) { // If the next point on the subroute still meets capacity limitations.
            currentCapacity += instanceFile.pointsArray[pointOrder[currentPointIndex]].getDemand();
            currentCost += instanceFile.pointsArray[pointOrder[currentPointIndex - 1]].getDistanceToAnotherPoint(instanceFile.pointsArray[pointOrder[currentPointIndex]]);
            currentPointIndex++;
        }

        else { // If the next point on the subroute doesn't meet capacity limitations.
            currentCost += instanceFile.pointsArray[pointOrder[currentPointIndex - 1]].getDistanceToAnotherPoint(instanceFile.pointsArray[0]);
            route route(currentPointIndex, currentCapacity, currentCost);
            return route;
        }
    }

    // This part should only be reached if the route ran out of points before it ran out of capacity.
    currentCost += instanceFile.pointsArray[pointOrder[currentPointIndex - 1]].getDistanceToAnotherPoint(instanceFile.pointsArray[0]);
    route route(currentPointIndex, currentCapacity, currentCost);
    currentPointIndex++;
    return route;
}
*/

// Swap two random elements in a specimen.
void specimen::mutateSwap(instanceFile instanceFile) {
    std::list<int>::iterator it1;
    std::list<int>::iterator it2;

    std::list<route>::iterator routesIt1 = subroutes.begin();
    std::list<route>::iterator routesIt2;

    int firstElementIndex = rand() % instanceFile.dimension;
    int secondElementIndex = rand() % instanceFile.dimension;

    while (secondElementIndex == firstElementIndex) secondElementIndex = rand() % instanceFile.dimension; // Ensure that two different points will be swapped.

    if (firstElementIndex > secondElementIndex) std::swap(firstElementIndex, secondElementIndex);

    // Find the correct subroutes.
    while (routesIt1->getSize() <= firstElementIndex) {
        firstElementIndex -= routesIt1->getSize();
        secondElementIndex -= routesIt1->getSize();

        routesIt1++;
    }

    it1 = routesIt1->findElement(firstElementIndex);

    while (routesIt2->getSize() <= secondElementIndex) {
        secondElementIndex -= routesIt2->getSize();

        routesIt2++;
    }

    it2 = routesIt2->findElement(secondElementIndex);

    // Swap the pointers
    std::list<int>::iterator tmp = it1;
    it1 = it2;
    it2 = tmp;

    // Swap values for routes.
}

// Swap two random elements in a specimen.
/*
void specimen::mutateSwap(instanceFile instanceFile) {
    std::list<int>::iterator it1;
    std::list<int>::iterator it2;

    std::list<route>::iterator routesIt1 = subroutes.begin();
    std::list<route>::iterator routesIt2;

    int firstElementIndex = rand() % instanceFile.dimension;
    int secondElementIndex = rand() % instanceFile.dimension;

    while (secondElementIndex == firstElementIndex) secondElementIndex = rand() % instanceFile.dimension; // Ensure that two different points will be swapped.

    if (firstElementIndex > secondElementIndex) std::swap(firstElementIndex, secondElementIndex);

    // Find the correct subroutes.
    while (routesIt1->getSize() <= firstElementIndex) {
        firstElementIndex -= routesIt1->getSize();
        secondElementIndex -= routesIt1->getSize();

        routesIt1++;
    }

    it1 = routesIt1->findElement(firstElementIndex);

    while (routesIt2->getSize() <= secondElementIndex) {
        secondElementIndex -= routesIt2->getSize();

        routesIt2++;
    }

    it2 = routesIt2->findElement(secondElementIndex);

    // Swap the pointers
    std::list<int>::iterator tmp = it1;
    it1 = it2;
    it2 = tmp;
}
*/

// Swap the order of all elements between two random elements in a specimen (including those two elements).
void specimen::mutateInvert(instanceFile instanceFile) {
    /*
    std::list<int>::iterator it1;
    std::list<int>::iterator it2;

    std::list<route>::iterator routesIt1 = subroutes.begin();
    std::list<route>::iterator routesIt2;

    int firstElementIndex = rand() % instanceFile.dimension;
    int secondElementIndex = rand() % instanceFile.dimension;

    while (secondElementIndex == firstElementIndex) secondElementIndex = rand() % instanceFile.dimension; // Ensure that two different points will be swapped.

    if (firstElementIndex > secondElementIndex) std::swap(firstElementIndex, secondElementIndex);

    // Find the correct subroutes.
    while (routesIt1->getSize() <= firstElementIndex) {
        firstElementIndex -= routesIt1->getSize();
        secondElementIndex -= routesIt1->getSize();

        routesIt1++;
    }

    it1 = routesIt1->findElement(firstElementIndex);

    while (routesIt2->getSize() <= secondElementIndex) {
        secondElementIndex -= routesIt2->getSize();

        routesIt2++;
    }

    it2 = routesIt2->findElement(secondElementIndex);

    // Swap the pointers
    std::list<int>::iterator tmp = it1;
    it1 = it2;
    it2 = tmp;
    */
}


void specimen::addNewSubroute(instanceFile instanceFile, int newPoint) {
    route newRoute;
    subroutes.push_back(newRoute);
    totalCost += newRoute.getCost();
}