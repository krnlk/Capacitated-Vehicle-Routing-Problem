#pragma once
#include "point.h"

class instanceFile
{
	int bestKnownSolution;
	int maxCapacity;
	int dimension;
	Point points[];
	void loadInstanceData();
	void saveResultsToFile();
};

