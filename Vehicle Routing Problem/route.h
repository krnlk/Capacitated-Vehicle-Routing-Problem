#pragma once
#include <list>
#include "point.h"
#include "instanceFile.h"

// This class represents one of the routes that together create a solution of a cVRP instance file.
class route
{
	std::list<int> pointsOnARoute;
	//int routeStartIndex;
	//int routeEndIndex;
	int capacity;
	int cost;

public: 
	route();
	route(instanceFile instanceFile, int newPoint);

	int getCapacity();
	int getCost();
	int getSize();
	void copyPoints(int* arrayOfPoints);
	std::list<int>::iterator getIterator();

	int removePoints(instanceFile instanceFile, int amountOfPointsLeftToDelete, int* deletedPoints);
	//std::pair<int, int> findPointInsertionIndex(instanceFile instanceFile, int amountOfPointsLeftToAdd, int addedPoint);
	std::pair<std::list<int>::iterator, int> findPointInsertionIndex(instanceFile instanceFile, int amountOfPointsLeftToAdd, int addedPoint);
	void addPoint(instanceFile instanceFile, std::pair<std::list<int>::iterator, int> addedPoint);
	
	void generateRoute(instanceFile instanceFile, int& currentPointIndex, int* pointOrder);
	std::list<int>::iterator findElement(int index);
};

