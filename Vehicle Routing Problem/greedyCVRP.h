#pragma once

#include "cVRPSolution.h"

// This class represents a greedy implementation of the cVRP algorithm.
class greedyCVRP : public cVRPSolution
{
	std::vector<int> pointsLeftToVisit; // Vector of points that still aren't a part of any subroute.
	int currentCapacity; // Value representing capacity of the current subroute.
	int currentPoint; // Value representing index of the currently visited point.

	void findNextPoint(const instanceFile& instanceFile); // Find the next best point to go to using greedy algorithm.

public:
	void generateASolution(instanceFile instanceFile); // Generate a solution to an instance file using greedy cVRP algorithm.
	void ignoreInitialAlgorithmParameters(std::ifstream& experimentFile); // Skip initialisation data that's not needed in greedy algorithm.
};

