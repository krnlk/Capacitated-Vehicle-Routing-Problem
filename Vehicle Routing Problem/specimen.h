#pragma once
#include <list>

#include "instanceFile.h"

// This class represents a single specimen in a generation.
class specimen
{
public:

	std::vector<int> pointOrder; // A vector containing the order of points in this specimen.

	bool obligatoryMutation = false; // 
	int totalCost; // Total cost of a specimen.

	std::vector<int> pointLocation; // Contains information about index position of each point in the specimen. For example pointLocation[4] = 6 means that point number 4 can be found in pointOrder[6].
	bool pointLocationSet = false;

	specimen();
	specimen(const specimen& old, instanceFile instanceFile); // Copy constructor
	~specimen();

	int getTotalCost();
	int calculateTotalCost(instanceFile instanceFile);
	void setPointOrder(std::vector<int>& pointOrder);

	void clearPointOrder();

	int getPoint(int index);
	void generateRandomRoute(instanceFile instanceFile, std::vector<int>& pointOrder);
	void mutateSwap(instanceFile instanceFile);
	void mutateInvert(instanceFile instanceFile);

	std::vector<int> getPointsOnARoute(instanceFile instanceFile); // For testing only.
	void setPointLocations(instanceFile instanceFile);
};

