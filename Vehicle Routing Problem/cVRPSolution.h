#pragma once
#include <list>

#include "instanceFile.h"

// This class represents a solution in the cVRP in its most basic form.
class cVRPSolution
{
public:
	std::vector<int> pointOrder; // A vector containing the order of points in this specimen.
	int totalCost; // Total cost of a specimen.

	std::vector<int> pointLocation; // Contains information about index position of each point in the specimen. For example pointLocation[4] = 6 means that point number 4 can be found in pointOrder[6].
	bool pointLocationSet = false; //

	int getTotalCost(); // Get total cost of this solution.
	int calculateTotalCost(instanceFile instanceFile); // Calculate the total cost of this solution.
	std::string getPointOrder();

	void setPointOrder(std::vector<int>& pointOrder);

	void clearPointOrder();
	int getPointByIndex(int index);
	
	void generateRandomRoute(instanceFile instanceFile, std::vector<int>& pointOrder);
	std::vector<int> getPointsOnARoute(instanceFile instanceFile); // For testing only.
	void setPointLocations(instanceFile instanceFile);
};

