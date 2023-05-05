#include <random>
#include <fstream>
#include <iostream>

#include "randomCVRP.h"

// Generate a solution by pseudorandomly selecting a point that isn't on the route yet and adding it to the route.
// Each point is equally likely to be selected (ignoring pseudorandom aspects).
// The process concludes when all points have been added to a route.
void randomCVRP::generateASolution(instanceFile instanceFile, std::ofstream& experimentResultsFile, int iteration, int rnd) {
	instanceFile.calculateDistancesBetweenPoints();

	totalCost = 0;
	pointOrder.clear();

	for (int i = 0; i < instanceFile.dimension; i++) {
		pointOrder.push_back(i);
	}

	std::shuffle(pointOrder.begin() + 1, pointOrder.end(), std::default_random_engine(rnd)); // Shuffle the order of points on a route (don't shuffle depot).
	totalCost = calculateTotalCost(instanceFile); 	
	
	experimentResultsFile << instanceFile.getFileName() << ";" << iteration + 1 << ";" << totalCost << std::endl;

	instanceFile.dealocateDistancesBetweenPoints();
}

// Ignore initial parmeters read from a genetic algorithm initialisation file.
void randomCVRP::ignoreInitialAlgorithmParameters(std::ifstream& experimentFile)
{
	std::string textLine;

	for (int i = 0; i < 9; i++) {
		std::getline(experimentFile, textLine); // Skip unneeded info.
	}
}