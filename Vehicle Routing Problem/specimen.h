#pragma once
#include <list>

#include "instanceFile.h"

// This class represents a single specimen in a generation.
class specimen
{
	int* pointOrder; // An array containing the order of points in this specimen.
	//int* pointLocation; // An array containing information where each point is located in this particular specimen.

	int totalCost; // Total cost of a specimen.

public:
	specimen();
	specimen(const specimen& old, instanceFile instanceFile); // Copy constructor
	~specimen();

	int getTotalCost();
	int calculateTotalCost(instanceFile instanceFile);
	void setPointOrder(int* pointOrder);
	int getPoint(int index);
	void generateRandomRoute(instanceFile instanceFile, int* pointOrder);
	void mutateSwap(instanceFile instanceFile);
	void mutateInvert(instanceFile instanceFile);
};

