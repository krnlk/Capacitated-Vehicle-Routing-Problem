#pragma once

#include <fstream>

#include "specimen.h"

class geneticCVRP
{
	std::vector<int> bestFoundSolutionPointOrder;
	int bestFoundSolutionTotalCost = INT_MAX;
	int worstFoundSolutionTotalCost = 0;

	int fitnessFunction; // Whether this is used or not depends on the select.
	int sumOfRanks; // Used in rank selection only.
	
	char selectUsed;
	char crossoverUsed;
	char mutationUsed;
	double crossoverProbability; 
	double mutationProbability; 
	int numberOfSpecimenInAGeneration; 
	int maxNumberOfGenerations; 
	
	std::vector<specimen> allCurrentSpecimen;
	std::vector<specimen> newGenerationOfSpecimen;

public: 	
	~geneticCVRP();

	void setAlgorithmParameters(std::ifstream &experimentParametersFile);

	void saveGenerationResults(std::ifstream& experimentResultsFile);

	int getBestFoundSolutionTotalCost();

	void calculateSumOfRanks();

	std::string getBestFoundSolutionPointOrder();

	void clearInfo();

	void setBestSpecimenInfo(instanceFile instanceFile, int bestSpecimenIndex);

	//bool compareSpecimen(specimen firstSpecimen, specimen secondSpecimen);

	void mainAlgorithmLoop(instanceFile instanceFile, std::ofstream& experimentResultsFile);
	void analyseNewGeneration(instanceFile instanceFile, int generationNumber, std::ofstream& experimentResultsFile);

	void generateInitialSpecimen(instanceFile instanceFile);

	void selectCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex);
	void crossover(instanceFile instanceFile, int offSpringIndex, int firstParentIndex, int secondParentIndex);

	void cycleCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex);
	specimen orderedCrossover(instanceFile instanceFile, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff);
	int findValueInsideParent(int valueIndex, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff, int recursion);
	specimen partiallyMappedCrossover(instanceFile instanceFile, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff);

	void mutate(instanceFile instanceFile);
	void mutateSwap(instanceFile instanceFile, int specimenIndex);
	void mutateInvert(instanceFile instanceFile, int specimenIndex);
	
	std::pair<int, int> selection(instanceFile instanceFile);

	std::pair<int, int> randomSelection(instanceFile instanceFile);
	int rankSelection(instanceFile instanceFile);
	std::pair<int, int> stochasticUniversalSamplingSelection(instanceFile instanceFile);
	int tournamentSelection(instanceFile instanceFile);
	//int rouletteWheelSelection(instanceFile instanceFile);
	int rouletteWheelSelection(instanceFile instanceFile, int parentIndex, double &totalSpecimenValueSum, double probability);

	void createNewOffspring(instanceFile instanceFile);

	void calculateFitnessFunction();
};

