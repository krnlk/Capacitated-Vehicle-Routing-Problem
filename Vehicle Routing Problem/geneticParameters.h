#pragma once



class geneticParameters
{
public:
	char selectUsed; // Character used to determine selection opeator used.
	
	char crossoverUsed; // Character used to determine crossover operator used.
	
	char mutationUsed; // Character used to determine mutation operator used.
	
	double crossoverProbability; // Value used to represent a chance for a crossover to happen.
	
	double mutationProbability; // Value used to represent a chance for a mutation to happen.
	
	int numberOfSpecimenInAGeneration; // Value used to represent a number of specimen per generation.
};

