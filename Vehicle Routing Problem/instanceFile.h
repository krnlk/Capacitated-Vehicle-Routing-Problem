#pragma once
#include <string>

#include "point.h"

// This class represents a single instance file, along with all its data.
class instanceFile
{
public:
	int optimalValue;
	int maxCapacity;
	int dimension;
	point* pointsArray;

	~instanceFile();
	void loadInstanceData(std::string filename);
	void saveResultsToFile();
	void printInstanceData();
};

