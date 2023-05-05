#pragma once
#include <string>
#include <fstream>

#include "instanceFile.h"
#include "Filepath.h"

// This class contains information about the experiment files and results of experiments.
// It is also used to save the results in a file.
class environment
{
	instanceFile instanceFile;
	std::string instanceFileName;
	std::ifstream experimentFile;
	std::ofstream testResultsFile;

	long long int frequency, start, elapsed; // For tracking time.
	int instanceRepeats;

	double averageError;
	double averageTime;
	double averageSolution;

	int overallBestSolutionFound;
	std::string bestFoundRoutePath;
	double bestError;

	public:
		void geneticAlgorithmExperiment(Filepath filepath);
		void randomAlgorithmExperiment(Filepath filepath);
		void greedyAlgorithmExperiment(Filepath filepath);
};

