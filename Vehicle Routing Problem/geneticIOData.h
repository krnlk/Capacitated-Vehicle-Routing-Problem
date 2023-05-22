#pragma once

#include <fstream>

#include "specimen.h"
#include "instanceFile.h"
#include "geneticCVRP.h"

// This class represents input and output data needed for running cVRP algorithm.
// Input data contains experiment variables (mutation used, probabilities, etc).
// Output data contains output data (best specimen value, average specimen value, time, etc).
class geneticIOData
{
	instanceFile loadedInstanceFile;
	geneticCVRP geneticCVRP;

	int bestSpecimenIndex;
	std::vector<int> bestFoundSolutionPointOrder; // Vector of the points from the best found route.
	int bestFoundSolutionTotalCost = INT_MAX; // Best found cost of a solution.
	int worstFoundSolutionTotalCost = 0; // Worst found cost of a solution.

	int fitnessFunction; // Whether this is used or not depends on the select, used for measuring the value of fitness function.
	int sumOfRanks; // Used in rank selection only.

	char selectUsed; // Character used to determine selection opeator used.
	int numberOfSpecimenInAGeneration; // Value used to represent a number of specimen per generation.
	int maxNumberOfGenerations; // Value used to represent the maximum amount of generations that will be simulated.

	void calculateSumOfRanks(); // Used in rank selection.

	// Save information about average solution and potential new best solutions in this generation.
	void findBestSpecimenInAGeneration(int index, int& averageSolutionTotalCost, int& currentGenerationBestFoundSolutionTotalCost, int& bestSpecimenIndex, bool& newBestSpecimenFound);

	void findWorstSpecimenInAGeneration(int index, int& currentGenerationWorstFoundSolutionTotalCost); // Save information about worst total cost in this generation.

	void analyseNewGeneration(int generationNumber, std::ofstream& experimentResultsFile, int iteration); // Find best specimen in the new generation.

public: 
	int getBestFoundSolutionTotalCost(); // Get the cost of the solution with the lowest found cost.
	std::string getBestFoundSolutionPointOrder(); // Get the point order of the solution with the lowest found cost.

	void setAlgorithmParameters(std::ifstream& experimentParametersFile); // Read algorithm parameters from a file (assuming specific format).
	void setBestSpecimenInfo(int bestSpecimenIndex); // Update the information about the new best found solution.
	void setInstanceFile(const instanceFile& instanceFile);

	void mainAlgorithmLoop(std::ofstream& experimentResultsFile, int iteration); // Main genetic algorithm loop.
	void clearInfo(); // Clean up all information about best found solution and all the specimen.
};

