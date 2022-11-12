#pragma once
#include <list>

#include "instanceFile.h"
#include "route.h"

// This class represents a single specimen in a generation.
class specimen
{
	std::list<route> subroutes;
	int totalCost;

public:
	specimen();
	specimen(const specimen& old, instanceFile instanceFile); // Copy constructor
	~specimen();

	int getTotalCost();
	int getNumberOfSubroutes();
	int getNumberOfPointsInASubroute(int subrouteIndex);
	route getSubroute(int subrouteIndex);
	void generateRandomRoute(instanceFile instanceFile, int* pointOrder);
	int calculateTotalCost(instanceFile instanceFile, int* pointOrder);
	route createANewSubroute(instanceFile instanceFile, int& currentPointIndex, int* pointOrder);
	void removePoints(instanceFile instanceFile, int amountOfPointsLeftToDelete, int* deletedPoints);
	void addPoint(instanceFile instanceFile, int amountOfPointsLeftToAdd, int* addedPoints);
	void mutateSwap(instanceFile instanceFile);
	void mutateInvert(instanceFile instanceFile);
	void addNewSubroute(instanceFile instanceFile, int newPoint);
};

