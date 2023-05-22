#pragma once

#include <fstream>

#include "specimen.h"
#include "geneticParameters.h"

// This class represents a genetic implementation of the cVRP algorithm.
class geneticCVRP
{
	instanceFile loadedInstanceFile;
	geneticParameters geneticParameters;

	std::vector<specimen> allCurrentSpecimen; // Vector of all specimen in the current generation.
	std::vector<specimen> newGenerationOfSpecimen; // Vector of all specimen of the new generation (that's still being generated).

	int fitnessFunction; // Whether this is used or not depends on the select, used for measuring the value of fitness function.
	int sumOfRanks; // Used in rank selection only.
	int worstFoundSolutionTotalCost = 0; // Worst found cost of a solution.

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

	void partiallyMappedCrossoverCutoffsSetup( int firstParentIndex, int secondParentIndex); // Setting up cutoff points for partially mapped crossover selection.
	specimen partiallyMappedCrossover(int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff); // Generate offspring by using partially mapped crossover.

	int findValueInsideParent(int valueIndex, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff, int recursion); // Return the index where a value resides inside a parent.

	void mutate(); // Handle mutation of all offspring in new generation.
	void mutateSwapOnSpecimen(int specimenIndex); // Mutation that swaps two random points in a specimen. Repeats multiple times.
	void mutateInvertOnSpecimen(int specimenIndex); // Mutation that swaps all points between two points in a route. Only one mutation is performed per specimen.
	
public:
	void calculateSpecimenTotalCostByIndex(int index);

	int getSpecimenTotalCostByIndex(int index);
	std::vector<int> getBestFoundSolutionPointOrderByIndex(int index);

	void setInstanceFile(const instanceFile& instanceFile);
	//
	void setGeneticParameters(char selectUsed, char crossoverUsed, char mutationUsed, double crossoverProbability, double mutationProbability, int numberOfSpecimenInAGeneration);
	void setWorstFoundSolutionTotalCost(int worstFoundSolutionTotalCost);
	void setSumOfRanks(int sumOfRanks);


	void clearAllSpecimen();
	void generateInitialSpecimen(); // Randomly generate initial specimen.
	void generationLoop();
	void createNewOffspring(); // Generates a new generation of specimen.
};

