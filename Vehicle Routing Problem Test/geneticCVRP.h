#pragma once
#include <fstream>

#include "../Vehicle Routing Problem/specimen.h"

// This class represents a genetic implementation of the cVRP algorithm.
class geneticCVRP
{
public:
	instanceFile currentInstanceFile; // Instance file for which new solutions (specimen) are being created.
	std::vector<int> bestFoundSolutionPointOrder; // Vector of the points from the best found route.
	int bestFoundSolutionTotalCost = INT_MAX; // Best found cost of a solution.
	int worstFoundSolutionTotalCost = 0; // Worst found cost of a solution.

	int generationsSinceNewBestSolutionFound = 0; // Counter used to measure how many generations have passed since a new best solution has been found.

	int fitnessFunction; // Whether this is used or not depends on the select, used for measuring the value of fitness function.
	int sumOfRanks; // Used in rank selection only.

	char selectUsed; // Character used to determine selection opeator used.
	char crossoverUsed; // Character used to determine crossover operator used.
	char mutationUsed; // Character used to determine mutation operator used.
	double crossoverProbability; // Value used to represent a chance for a crossover to happen.
	double mutationProbability; // Value used to represent a chance for a mutation to happen.
	int numberOfSpecimenInAGeneration; // Value used to represent a number of specimen per generation.
	int maxNumberOfGenerations; // Value used to represent the maximum amount of generations that will be simulated.

	std::vector<specimen> allCurrentSpecimen; // Vector of all specimen in the current generation.
	std::vector<specimen> newGenerationOfSpecimen; // Vector of all specimen of the new generation (that's still being generated).

	void setBestSpecimenInfo(int bestSpecimenIndex); // Update the information about the new best found solution.

	void calculateSumOfRanks(); // Used in rank selection.

	void generateInitialSpecimen(); // Randomly generate initial specimen.

	void analyseNewGeneration(int generationNumber, std::ofstream& experimentResultsFile, int iteration); // Find best specimen in the new generation.

	// Save information about average solution and potential new best solutions in this generation.
	void findBestSpecimenInAGeneration(int index, int& averageSolutionTotalCost, int& currentGenerationBestFoundSolutionTotalCost, int& bestSpecimenIndex, bool& newBestSpecimenFound);

	void findWorstSpecimenInAGeneration(int index, int& currentGenerationWorstFoundSolutionTotalCost); // Save information about worst total cost in this generation.

	void createNewOffspring(); // Generates a new generation of specimen.

	void calculateFitnessFunction(); // Calculates fitness function for roulette wheel and stochastic universal sampling methods.

	std::pair<int, int> selection(); // Returns a pair of point indexes, used to select parents.

	std::pair<int, int> rankSelectionIndexes(); // Select a pair of indexes using rank selection.
	int rankSelection(); // Return a single parent index using rank selection.

	std::pair<int, int> randomSelection(); // Select a pair of indexes using random selection.

	std::pair<int, int> stochasticUniversalSamplingIndexes(); // Select a pair of indexes using stochastic universal sampling selection.

	std::pair<int, int> tournamentSelectionIndexes(); // Select a pair of indexes using tournament selection.
	int tournamentSelection(); // Return a single parent index using tournament selection.

	std::pair<int, int> rouletteWheelSelectionIndexes(); // Select a pair of indexes using roulette wheel selection.
	int rouletteWheelSelection(int parentIndex, double& totalSpecimenValueSum, double probability); // Return a single parent index using rank selection.

	void selectCrossover(int firstParentIndex, int secondParentIndex); // Selects crossover based on data loaded from a file.

	void cycleCrossover(int firstParentIndex, int secondParentIndex); // Generate offsping by using cycles between two parents.

	specimen orderedCrossover(int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff); // Generate offspring by using ordered crossover.

	void partiallyMappedCrossoverCutoffsSetup(int firstParentIndex, int secondParentIndex); // Setting up cutoff points for partially mapped crossover selection.
	specimen partiallyMappedCrossover(int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff); // Generate offspring by using partially mapped crossover.

	int findValueInsideParent(int valueIndex, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff, int recursion); // Return the index where a value resides inside a parent.

	void mutate(); // Handle mutation of all offspring in new generation.
	void mutateSwapOnSpecimen(int specimenIndex); // Mutation that swaps two random points in a specimen. Repeats multiple times.
	void mutateInvertOnSpecimen(int specimenIndex); // Mutation that swaps all points between two points in a route. Only one mutation is performed per specimen.

	int getBestFoundSolutionTotalCost() const; // Get the cost of the solution with the lowest found cost.
	std::string getBestFoundSolutionPointOrder() const; // Get the point order of the solution with the lowest found cost.

	void setAlgorithmParameters(std::ifstream& experimentParametersFile); // Read algorithm parameters from a file (assuming specific format).
	void setInstanceFile(instanceFile instanceFile); // Set the instance file for this object.

	void clearInfo(); // Clean up all information about best found solution and all the specimen.

	void mainAlgorithmLoop(std::ofstream& experimentResultsFile, int iteration); // Main genetic algorithm loop.
};
