#pragma once
#include <string>

// This class contains about the experiment files and results of experiments.
// It is also used to save the results in a file.
class environment
{
	std::string instanceFileName;
	int bestKnownRouteCost;
	int bestFoundRouteCost;
	std::string bestFoundRoutePath;
	double error;
	long long int time;

public:
	void beginExperiment(std::string experimentFileName, std::string outputFileName);
};

