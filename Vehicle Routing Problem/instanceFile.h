#pragma once
#include <string>
#include <vector>

#include "point.h"

// This class represents a single instance file, along with all its data.
class instanceFile
{
	std::string fileName;
	int optimalValue;
	int maxCapacity;

public:
	int dimension;
	std::vector<point> pointsVector;
	int** distanceBetweenPoints;

	std::string getFileName();
	int getOptimalValue();
	int getMaxCapacity();

	void loadInstanceData(std::string path, std::string filename);
	void clearInstanceData();
	void printInstanceData();

	void calculateDistancesBetweenPoints();
	void dealocateDistancesBetweenPoints();

	void printDistancesBetweenPoints(); // For testing purposes only.

};

