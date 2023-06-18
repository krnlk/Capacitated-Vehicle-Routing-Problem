#pragma once
#include <fstream>

#include "cVRPSolution.h"

// This class represents a greedy implementation of the cVRP algorithm.
class randomCVRP : public cVRPSolution
{
	instanceFile currentInstanceFile; // Instance file for which new solutions are being created.
	std::vector<int> bestFoundSolutionPointOrder; // Vector of the points from the best found route.
	int bestFoundSolutionTotalCost = INT_MAX; // Best found cost of a solution.
	int worstFoundSolutionTotalCost = 0; // Worst found cost of a solution.

	int numberOfSpecimenPerGeneration; // How many times should a random solution be generated.
	int maxNumberOfGenerations; // Value used to represent the maximum amount of generations that will be simulated.

	void generateASolution(std::ofstream& experimentResultsFile, int iteration, int rnd); // Generate a solution by pseudorandomly selecting a point that isn't on the route yet and adding it to the route.
public:
	int getBestFoundSolutionTotalCost() const; // Get the cost of the solution with the lowest found cost.
	
	std::string getBestFoundSolutionPointOrder() const; // Get the point order of the solution with the lowest found cost.
	void setInstanceFile(instanceFile instanceFile); // Set the instance file for this object.
	
	void ignoreInitialAlgorithmParameters(std::ifstream& experimentFile); // Ignore initial parmeters read from a genetic algorithm initialisation file.
	void clearInfo(); // Clean up all information about best found solution and all the specimen.
	
	void mainAlgorithmLoop(std::ofstream& experimentResultsFile, int iteration); // Main algorithm loop, mimicking how genethic algorithm works.
};

