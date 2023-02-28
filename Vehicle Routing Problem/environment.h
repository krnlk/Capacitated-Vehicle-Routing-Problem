#pragma once
#include <string>

#include "instanceFile.h"

// This class contains about the experiment files and results of experiments.
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
	void geneticAlgorithmExperiment(std::string experimentFileName, std::string outputFileName, std::string generationResultsFileName);
	void randomAlgorithmExperiment(std::string experimentFileName, std::string outputFileName, std::string iterationResultsFileName);
	void greedyAlgorithmExperiment(std::string experimentFileName, std::string outputFileName);
};

