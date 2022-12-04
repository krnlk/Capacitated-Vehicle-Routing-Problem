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
            totalCost += instanceFile.distanceBetweenPoints[i - 1][pointOrder[0]]; // Go back from previous point to the depot and finish that subroute.
            currentCapacity = 0; // Reset capacity for the next subroute.
        }
    }
    
    if (currentCapacity != 0) { // Connect the last point in specimen with a depot if it hasn't been connected already.
        totalCost += instanceFile.distanceBetweenPoints[instanceFile.dimension - 1][pointOrder[0]];
    }

    return totalCost;
}

/*
int specimen::getNumberOfPointsInASubroute(int subrouteIndex)
{
    std::list<route>::iterator it = subroutes.begin();
    advance(it, subrouteIndex);
    return it->getSize();
}
*/

void specimen::setPointOrder(std::vector<int>& pointOrder) {
    this->pointOrder = pointOrder;
}

void specimen::clearPointOrder() {
    pointOrder.clear();
}

int specimen::getPoint(int index)
{
    return pointOrder[index];
}

/*
route specimen::getSubroute(int subrouteIndex)
{
    std::list<route>::iterator it = subroutes.begin();
    advance(it, subrouteIndex);
    return *it;
}
*/

// Generates a random route by shuffling the order of points around.
void specimen::generateRandomRoute(instanceFile instanceFile, std::vector<int>& pointOrder) {
    unsigned rnd = 0; // Creates a random formula for shuffling. //TODO maybe move it to calling function from specimen.cpp, since there's a new seed being initialised each time?

    std::shuffle(pointOrder.begin() + 1, pointOrder.end(), std::default_random_engine(rnd)); // Shuffle the order of points on a route (don't shuffle depot).
    

    for (int i = 0; i < instanceFile.dimension; i++) {
        this->pointOrder.push_back(pointOrder[i]);
    }
    
    totalCost = calculateTotalCost(instanceFile); //TODO maybe remove totalCost = part
}

/*
route specimen::createANewSubroute(instanceFile instanceFile, int& currentPointIndex, int* pointOrder) {
    route newSubroute;
    newSubroute.generateRoute(instanceFile, currentPointIndex, pointOrder);
    return newSubroute;
}
*/

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

    // Swap values for routes.
}
*/

void specimen::mutateSwap(instanceFile instanceFile) {
    int index1, index2;

    index1 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the first point. Don't swap first point (depot).
    index2 = rand() % (instanceFile.dimension - 1) + 1; // Randomly select index of the second point. Don't swap first point (depot).

    while (index1 == index2) index2 = rand() % (instanceFile.dimension - 1) + 1;

    std::swap(pointOrder[index1], pointOrder[index2]);
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

/*
void specimen::addNewSubroute(instanceFile instanceFile, int newPoint) {
    route newRoute;
    subroutes.push_back(newRoute);
    totalCost += newRoute.getCost();
}
*/