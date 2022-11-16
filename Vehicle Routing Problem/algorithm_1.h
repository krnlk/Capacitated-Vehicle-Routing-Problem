#pragma once

#include <fstream>

#include "specimen.h"

class algorithm_1
{
	int* bestFoundSolutionPointOrder;
	int bestFoundSolutionTotalCost = INT_MAX;
	
	char crossoverUsed;
	char mutationUsed;
	float crossoverProbability = 0.0; //use later
	float mutationProbability = 1.0; //use later
	int numberOfSpecimenInAGeneration = 10; //TODO: currently hardcoded to 10, change later, should probably stay an even number
	int maxNumberOfGenerations = 5; //TODO: currently hardcoded to 5, change later
	
	specimen* allCurrentSpecimen;
	specimen* newGenerationOfSpecimen;

public: 	
	~algorithm_1();

	void setAlgorithmParameters(std::ifstream &experimentFile);

	int getBestFoundSolutionTotalCost();
	int* getBestFoundSolutionPointOrder();

	void setBestSpecimenInfo(instanceFile instanceFile, int bestSpecimenIndex);

	void mainAlgorithmLoop(instanceFile instanceFile);
	void generateInitialSpecimen(instanceFile instanceFile);
	void selectCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex);
	void crossover(instanceFile instanceFile, int offSpringIndex, int firstParentIndex, int secondParentIndex);
	void cycleCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex);
	specimen orderedCrossover(instanceFile instanceFile, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff);
	void partiallyMappedCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff);
	//specimen createCrossoverOffspring(instanceFile instanceFile, int firstParentIndex, int secondParentIndex);
	void mutate(instanceFile instanceFile);
	void mutateSwap(instanceFile instanceFile, int specimenIndex);
	void mutateInvert(instanceFile instanceFile, int specimenIndex);
	void tournament();

	void createNewOffspring(instanceFile instanceFile);
};

