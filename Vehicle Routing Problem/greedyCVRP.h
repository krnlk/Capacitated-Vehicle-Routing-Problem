#pragma once

#include "cVRPSolution.h"

//
class greedyCVRP : public cVRPSolution
{
	std::vector<int> pointsLeftToVisit;
	int currentCapacity;
	int currentPoint;

	void findNextPoint(instanceFile instanceFile);

	public:
		void generateASolution(instanceFile instanceFile);
		void ignoreInitialAlgorithmParameters(std::ifstream& experimentFile);
};

