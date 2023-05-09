#pragma once
#include <fstream>

#include "cVRPSolution.h"

// This class represents a greedy implementation of the cVRP algorithm.
class randomCVRP : public cVRPSolution
{	
	public: 
		void generateASolution(instanceFile instanceFile, std::ofstream& experimentResultsFile, int iteration, int rnd); // Generate a solution by pseudorandomly selecting a point that isn't on the route yet and adding it to the route.
		void ignoreInitialAlgorithmParameters(std::ifstream& experimentFile); // Ignore initial parmeters read from a genetic algorithm initialisation file.
};

