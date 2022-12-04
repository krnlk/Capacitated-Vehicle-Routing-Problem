#pragma once
#include <string>
#include <vector>

#include "point.h"

// This class represents a single instance file, along with all its data.
class instanceFile
{
public:
	std::string fileName;
	int optimalValue;
	int maxCapacity;
	int dimension;

	std::vector<point> pointsVector;
	int** distanceBetweenPoints;

	~instanceFile();
	void loadInstanceData(std::string filename);
	void clearInstanceData();
	void saveResultsToFile();
	void printInstanceData();

	void calculateDistancesBetweenPoints();
	void dealocateDistancesBetweenPoints();
};

