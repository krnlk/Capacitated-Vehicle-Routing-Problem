#include <random>
#include <fstream>

#include <iostream>

#include "randomCVRP.h"

void randomCVRP::generateASolution(instanceFile instanceFile, std::ofstream& experimentResultsFile, int iteration, int rnd) {
	instanceFile.calculateDistancesBetweenPoints();

	totalCost = 0;
	pointOrder.clear();

	for (int i = 0; i < instanceFile.dimension; i++) {
		pointOrder.push_back(i);
	}

	std::shuffle(pointOrder.begin() + 1, pointOrder.end(), std::default_random_engine(rnd)); // Shuffle the order of points on a route (don't shuffle depot).
	totalCost = calculateTotalCost(instanceFile); 	
	
	if (instanceFile.dimension == 16) std::cout << getPointOrder() << " " << pointOrder.size() << " " << pointLocation.size() << std::endl;

	experimentResultsFile << instanceFile.fileName << ";" << iteration + 1 << ";" << totalCost << std::endl;

	instanceFile.dealocateDistancesBetweenPoints();
}

// Ignores initial parmeters read from a genetic algorithm initialisation file.
void randomCVRP::ignoreInitialAlgorithmParameters(std::ifstream& experimentFile)
{
	std::string textLine;

	for (int i = 0; i < 9; i++) {
		std::getline(experimentFile, textLine); // Skip unneeded info.
	}
}