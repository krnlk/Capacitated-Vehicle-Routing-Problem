#pragma once

#include "cVRPSolution.h"

class greedyCVRP : public cVRPSolution
{
	std::vector<int> pointsLeftToVisit;
	int currentCapacity;
	int currentPoint;
public:
	void findNextPoint(instanceFile instanceFile);
	void generateASolution(instanceFile instanceFile);
	void ignoreInitialAlgorithmParameters(std::ifstream& experimentFile);
};

