#pragma once
#include <list>

#include "instanceFile.h"

// This class represents a single specimen in a generation.
class specimen
{
public:

	std::vector<int> pointOrder; // A vector containing the order of points in this specimen.


	int totalCost; // Total cost of a specimen.

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
};

