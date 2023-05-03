#pragma once
#include <fstream>

#include "cVRPSolution.h"

class randomCVRP : public cVRPSolution
{	
	public: 
		void generateASolution(instanceFile instanceFile, std::ofstream& experimentResultsFile, int iteration, int rnd);
		void ignoreInitialAlgorithmParameters(std::ifstream& experimentFile);
};

