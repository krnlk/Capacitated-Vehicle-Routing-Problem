#include <random>
#include <fstream>
#include <iostream>
#include <sstream>

#include "randomCVRP.h"
#include "pseudoRandomGeneration.h"


// Get the cost of the solution with the lowest found cost.
int randomCVRP::getBestFoundSolutionTotalCost() const
{
	return bestFoundSolutionTotalCost;
}

// Get the point order of the solution with the lowest found cost
// in string format, with white spaces to separate points.
std::string randomCVRP::getBestFoundSolutionPointOrder() const {
	std::stringstream stringifiedPointOrder;
	for (auto iterator = bestFoundSolutionPointOrder.begin(); iterator != bestFoundSolutionPointOrder.end(); iterator++) {
		if (iterator != bestFoundSolutionPointOrder.begin()) {
			stringifiedPointOrder << " ";
		}
		stringifiedPointOrder << *iterator;
	}
	return stringifiedPointOrder.str();
}

// Set the instance file for this object.
void randomCVRP::setInstanceFile(instanceFile instanceFile)
{
	this->currentInstanceFile = instanceFile;
}

// Clean up all information about best found solution and all the specimen.
void randomCVRP::clearInfo() {
	bestFoundSolutionPointOrder.clear();
	bestFoundSolutionTotalCost = INT_MAX;
	worstFoundSolutionTotalCost = 0;
}

// Main algorithm loop, mimicking how genetic algorithm works.
void randomCVRP::mainAlgorithmLoop(std::ofstream& experimentResultsFile, int iteration) {

	double averageSolutionTotalCost;
	int currentGenerationBestFoundSolutionTotalCost;
	std::vector<int> currentGenerationBestFoundSolutionPointOrder;
	int currentGenerationWorstFoundSolutionTotalCost;

	currentInstanceFile.calculateDistancesBetweenPoints(); // Set up distances between points.

	pointOrder.clear();
	for (int i = 0; i < currentInstanceFile.dimension; i++) { // Add all points to a route.
		pointOrder.push_back(i);
	}

	for (int i = 0; i < this->maxNumberOfGenerations; i++) { // i is the number of current "generation"
		unsigned rnd = time(NULL) * getRand(0, 9999); // Create a random formula for shuffling.

		averageSolutionTotalCost = 0;
		currentGenerationBestFoundSolutionTotalCost = INT_MAX;
		currentGenerationWorstFoundSolutionTotalCost = 0;

		for (int j = 0; j < this->numberOfSpecimenPerGeneration; j++) { // j is the number of current "specimen"
			this->generateASolution(experimentResultsFile, iteration, rnd);

			if (totalCost < currentGenerationBestFoundSolutionTotalCost) {
				currentGenerationBestFoundSolutionTotalCost = totalCost;
				currentGenerationBestFoundSolutionPointOrder = pointOrder;
			}

			if (totalCost > currentGenerationWorstFoundSolutionTotalCost) {
				currentGenerationWorstFoundSolutionTotalCost = totalCost;
			}

			averageSolutionTotalCost += totalCost;
		}

		if (currentGenerationBestFoundSolutionTotalCost < bestFoundSolutionTotalCost) {
			bestFoundSolutionTotalCost = currentGenerationBestFoundSolutionTotalCost;
			bestFoundSolutionPointOrder = currentGenerationBestFoundSolutionPointOrder;
		}

		if (currentGenerationWorstFoundSolutionTotalCost > worstFoundSolutionTotalCost) {
			worstFoundSolutionTotalCost = currentGenerationWorstFoundSolutionTotalCost;
		}

		// Save information to generation results file.
		experimentResultsFile << currentInstanceFile.getFileName() << ";" << iteration << ";" << i << ";" <<
			bestFoundSolutionTotalCost << ";" << worstFoundSolutionTotalCost << ";" <<
			averageSolutionTotalCost / this->numberOfSpecimenPerGeneration << ";" <<
			currentGenerationBestFoundSolutionTotalCost << ";" << currentGenerationWorstFoundSolutionTotalCost << std::endl;
	}

	currentInstanceFile.dealocateDistancesBetweenPoints();
}

// Generate a solution by pseudorandomly selecting a point that isn't on the route yet and adding it to the route.
// Each point is equally likely to be selected (ignoring pseudorandom aspects).
// The process concludes when all points have been added to a route.
void randomCVRP::generateASolution(std::ofstream& experimentResultsFile, int iteration, int rnd) {
	totalCost = 0;

	std::shuffle(pointOrder.begin() + 1, pointOrder.end(), std::default_random_engine(rnd)); // Shuffle the order of points on a route (don't shuffle depot).
	totalCost = calculateTotalCost(currentInstanceFile); // Calculate the cost of the randomly generated route.	
}

// Ignore initial parmeters read from a genetic algorithm initialisation file
// (thus making it possible to use the same instance file format as for the genetic and random implementations).
void randomCVRP::ignoreInitialAlgorithmParameters(std::ifstream& experimentFile)
{
	std::string textLine;

	for (int i = 0; i < 6; i++) {
		std::getline(experimentFile, textLine); // Skip unneeded info.
	}

	std::getline(experimentFile, textLine);
	this->numberOfSpecimenPerGeneration = stoi(textLine.substr(36)); // Trim out beginning of the line to get to number of specimen in a generation and convert it to integer.

	std::getline(experimentFile, textLine);
	this->maxNumberOfGenerations = stoi(textLine.substr(24)); // Trim out beginning of the line to get to number of generations and convert it to integer.

	std::getline(experimentFile, textLine); // Skip unneeded info.
}