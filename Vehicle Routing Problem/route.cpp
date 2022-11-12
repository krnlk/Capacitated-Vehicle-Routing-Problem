#include "route.h"
#include "instanceFile.h"

route::route()
{
    this->capacity = 0;
    this->cost = 0;
}

route::route(instanceFile instanceFile, int newPoint) {
    pointsOnARoute.push_back(newPoint);
    this->capacity += instanceFile.pointsArray[newPoint].getDemand();
    this->cost = instanceFile.pointsArray[0].getDistanceToAnotherPoint(instanceFile.pointsArray[newPoint])
        + instanceFile.pointsArray[newPoint].getDistanceToAnotherPoint(instanceFile.pointsArray[0]);
}

int route::getCapacity()
{
    return capacity;
}

int route::getCost()
{
    return cost;
}

int route::getSize()
{
    return pointsOnARoute.size();
}

void route::copyPoints(int* arrayOfPoints)
{
    std::list<int>::iterator it;
    int i = 0;

    for (it = pointsOnARoute.begin(); it != pointsOnARoute.end(); ++it) {
        arrayOfPoints[i] = *it;
    }
}

std::list<int>::iterator route::getIterator()
{
    return pointsOnARoute.begin();
}

// Used to generate an initial route. Keeps adding points to a route until either 
// there are no more points left or there's no more capacity to add the next point in sequence. 
void route::generateRoute(instanceFile instanceFile, int& currentPointIndex, int* pointOrder) {
    pointsOnARoute.push_back(pointOrder[currentPointIndex]);
    cost += instanceFile.pointsArray[0].getDistanceToAnotherPoint(instanceFile.pointsArray[pointOrder[currentPointIndex]]); // Get distance from depot to the first point on a subroute.
    capacity += instanceFile.pointsArray[pointOrder[currentPointIndex]].getDemand(); // Get the capacity of the first point on a subroute.
    currentPointIndex++; // Move onto the next point in the subroute.

    while (currentPointIndex < instanceFile.dimension) {
        if (capacity + instanceFile.pointsArray[pointOrder[currentPointIndex]].getDemand() <= instanceFile.maxCapacity) { // If the next point on the subroute still meets capacity limitations.
            pointsOnARoute.push_back(pointOrder[currentPointIndex]);
            capacity += instanceFile.pointsArray[pointOrder[currentPointIndex]].getDemand();
            cost += instanceFile.pointsArray[pointOrder[currentPointIndex - 1]].getDistanceToAnotherPoint(instanceFile.pointsArray[pointOrder[currentPointIndex]]);
            currentPointIndex++;
        }

        else { // If the next point on the subroute doesn't meet capacity limitations.
            cost += instanceFile.pointsArray[pointOrder[currentPointIndex - 1]].getDistanceToAnotherPoint(instanceFile.pointsArray[0]);
            return;
        }
    }

    // This part should only be reached if the route ran out of points before it ran out of capacity.
    cost += instanceFile.pointsArray[pointOrder[currentPointIndex - 1]].getDistanceToAnotherPoint(instanceFile.pointsArray[0]);
    currentPointIndex++;
    return;
}

int route::removePoints(instanceFile instanceFile, int amountOfPointsLeftToDelete, int* deletedPoints) {
    std::list<int>::iterator it;
    std::list<int>::iterator helpIt;

    int originalCost = cost; // Cost of the subroute before removing a point.

    //TODO consider ignoring first point since it's a depot
    for (it = pointsOnARoute.begin(); it != pointsOnARoute.end() && amountOfPointsLeftToDelete > 0; ++it) { // For every point on a route.
        for (int i = 0; i < amountOfPointsLeftToDelete; i++) { // Check if the point is one of the points that should be removed.
            if (*it == deletedPoints[i]) {
                if (pointsOnARoute.begin() == pointsOnARoute.end()) { cost = 0; } // TODO: fix later

                else {
                    if (it != pointsOnARoute.begin() && it != pointsOnARoute.end()) {// If the deleted point isn't the first or last point in a subroute.
                        cost -= instanceFile.pointsArray[*std::prev(it)].getDistanceToAnotherPoint(instanceFile.pointsArray[*it]) // Remove cost of moving from previous point to removed point.
                            - instanceFile.pointsArray[*it].getDistanceToAnotherPoint(instanceFile.pointsArray[*std::next(it)]) // Remove cost of moving from removed point to next point.
                            + instanceFile.pointsArray[*std::prev(it)].getDistanceToAnotherPoint(instanceFile.pointsArray[*std::next(it)]); // Add cost of moving from previous to next point.
                    }

                    if (it != pointsOnARoute.begin()) { // If the deleted point is the first point in a subroute.
                        cost -= instanceFile.pointsArray[0].getDistanceToAnotherPoint(instanceFile.pointsArray[*it])
                            - instanceFile.pointsArray[*it].getDistanceToAnotherPoint(instanceFile.pointsArray[*std::next(it)])
                            + instanceFile.pointsArray[0].getDistanceToAnotherPoint(instanceFile.pointsArray[*std::next(it)]);

                    }

                    if (it != pointsOnARoute.end()) { // If the deleted point is the last point in a subroute.
                        cost -= instanceFile.pointsArray[*it].getDistanceToAnotherPoint(instanceFile.pointsArray[0])
                            - instanceFile.pointsArray[*std::prev(it)].getDistanceToAnotherPoint(instanceFile.pointsArray[*it])
                            + instanceFile.pointsArray[*std::prev(it)].getDistanceToAnotherPoint(instanceFile.pointsArray[0]);

                    }
                }

                capacity -= instanceFile.pointsArray[*it].getDemand();

                it = pointsOnARoute.erase(it); // Remove the element from the list.

                std::swap(deletedPoints[i], deletedPoints[amountOfPointsLeftToDelete - 1]); // Remove deleted elements out of range of the array.
                amountOfPointsLeftToDelete--;
                i--; // To negate incrementation, since order of deleted points has been swapped.
            }
        }
    }

    return originalCost - cost; // Return how much the cost has changed.
}

// First int is the index where a new point would be inserted, second int is the increase in cost caused by inserting that point.
// No need to check if capacity requirements will be met, as that's already done in the specimen.cpp class that's calling this one.
//std::pair<int, int> route::findPointInsertionIndex(instanceFile instanceFile, int amountOfPointsLeftToAdd, int addedPoint) {
std::pair<std::list<int>::iterator, int> route::findPointInsertionIndex(instanceFile instanceFile, int amountOfPointsLeftToAdd, int addedPoint) {
    std::list<int>::iterator it;
    std::list<int>::iterator copyIt;
    std::pair<std::list<int>::iterator, int> bestInsertion = std::make_pair(it, INT_MAX);

    int insertionCost; 
    //int index = 0;

    for (it = pointsOnARoute.begin(); it != pointsOnARoute.end(); ++it) {
        insertionCost = 0;

        if (it == pointsOnARoute.begin()) { // If the point would be inserted at the start of a route.
            insertionCost +=
                instanceFile.pointsArray[0].getDistanceToAnotherPoint(instanceFile.pointsArray[addedPoint]) // Connect depot with new first point.
                + instanceFile.pointsArray[addedPoint].getDistanceToAnotherPoint(instanceFile.pointsArray[*it]) // Connect new first point with old first point.
                - instanceFile.pointsArray[0].getDistanceToAnotherPoint(instanceFile.pointsArray[*it]); // Remove connection between depot and old first point.
        }

        if (it == pointsOnARoute.end()) { // If the point would be inserted at the end of a route.
            insertionCost +=
                instanceFile.pointsArray[*it].getDistanceToAnotherPoint(instanceFile.pointsArray[addedPoint]) // Connect old last point to the new last point.
                + instanceFile.pointsArray[addedPoint].getDistanceToAnotherPoint(instanceFile.pointsArray[0]) // Connect new last point to depot.
                - instanceFile.pointsArray[*it].getDistanceToAnotherPoint(instanceFile.pointsArray[0]); // Remove connection between old last point and depot. 
        }

        if (it != pointsOnARoute.begin() && it != pointsOnARoute.end()) { // If the point would be inserted in the middle of the route.
           insertionCost += 
               instanceFile.pointsArray[*it].getDistanceToAnotherPoint(instanceFile.pointsArray[addedPoint]) // Connect previous point to new point.
               + instanceFile.pointsArray[addedPoint].getDistanceToAnotherPoint(instanceFile.pointsArray[*std::next(it)]) // Connect new point to next point.
               - instanceFile.pointsArray[*it].getDistanceToAnotherPoint(instanceFile.pointsArray[*std::next(it)]); // Remove connection between previous point and next point.
        }

        if (insertionCost < bestInsertion.second) { // If current position in route would be better cost-wise than the previous best found position.
            //bestInsertion.first = index;
            bestInsertion.first = it;
            bestInsertion.second = insertionCost;
        }
    }

    return bestInsertion;
}

// Add a new point to a route.
void route::addPoint(instanceFile instanceFile, std::pair<std::list<int>::iterator, int> addedPoint) {
    pointsOnARoute.insert(addedPoint.first, instanceFile.pointsArray[*addedPoint.first].getIndex());
    
    // Add capacity.
    capacity += instanceFile.pointsArray[*addedPoint.first].getDemand();

    // Add cost (it has already been calculated in findPointInsertionIndex function).
    cost += addedPoint.second;

    /*
    std::list<int>::iterator it;
    std::list<int>::iterator helpIt;

    for (it = pointsOnARoute.begin(); it != pointsOnARoute.end() && amountOfPointsLeftToAdd > 0; ++it) {
        for (int i = 0; i < amountOfPointsLeftToAdd; i++) {
            if (*it == addedPoints[i]) {
                if (pointsOnARoute.begin() == pointsOnARoute.end()) {} //TODO fix later

                if (it != pointsOnARoute.begin()) {// If the deleted point isn't the first point in a subroute.
                    cost -= instanceFile.pointsArray[*std::prev(it)].getDistanceToAnotherPoint(instanceFile.pointsArray[*it]);
                }

                if (it != pointsOnARoute.end()) { // If the deleted point isn't the last point in a subroute.
                    cost -= instanceFile.pointsArray[*it].getDistanceToAnotherPoint(instanceFile.pointsArray[*std::next(it)]);

                }
                capacity -= instanceFile.pointsArray[*it].getDemand();

                it = pointsOnARoute.erase(it); // Remove the element from the list.

                std::swap(addedPoints[i], addedPoints[amountOfPointsLeftToAdd - 1]); // Remove deleted elements out of range of the array.
                amountOfPointsLeftToAdd--;
                break;
            }
        }
    }
    */
}

std::list<int>::iterator route::findElement(int index) {
    std::list<int>::iterator it = pointsOnARoute.begin();

    for (int i = 0; i < index; i++) {
        it++;
    }

    return it;
}