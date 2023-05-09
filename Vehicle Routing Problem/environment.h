#pragma once
#include <string>
#include <fstream>

#include "instanceFile.h"
#include "Filepath.h"
#include "geneticCVRP.h"
#include "randomCVRP.h"
#include "greedyCVRP.h"

// This class contains information about the experiment files and results of experiments.
// It is also used to save the results in a file.
class environment
{
	instanceFile instanceFile; // Object representing the currently opened instance file.
	std::string instanceFileName; // Name of the currently opened instance file.
	std::ifstream experimentFile; // Ifstream for reading initialisation data from an experiment file.
	std::ofstream testResultsFile; // Ofstream for writing test results into a file.

	long long int frequency, start, elapsed; // For tracking time during experiments.
	int instanceRepeats; // Number of repeats of a cVRP algorithm for the current instance file.

	double averageError; // Average error across all repetitions of the current instance file (where error is defined as 
						 // found value - best known optimal value / best known optimal value.
	double averageTime; // Average time to find a cVRP solution across all repetitions of the current instance file.
	double averageSolution; // Average of all best found solution values across all repetitions of the current instance file.

	int overallBestSolutionFound; // Best (lowest) value of the solution found across all algorithm repeats on the current instance file.
	std::string bestFoundRoutePath; // A string of numbers, separated by white space, representing a complete cVRP solution,
									// where each number represents visiting a point.
	double bestError; // Smallest found difference between the best found solution value of an instance file
					  // and the best known optimal value, across all repetitions of an instance file.

	void geneticOrRandomAlgorithmFileOpenError(Filepath filepath, std::ofstream &generationResultsFile); // (Genetic & random algorithms) Print out error messages to mark files which couldn't be properly opened.
	void geneticAlgorithmInstanceFileRepetitions(Filepath filepath, geneticCVRP &geneticCVRP, std::ofstream &generationResultsFile); // A full set of repetitions of the geneticCVRP algorithm for one instance file.


	void randomAlgorithmInstanceFileRepetitions(Filepath filepath, randomCVRP &randomCVRP, std::ofstream &generationResultsFile, unsigned rnd); // A full set of repetitions of the randomCVRP algorithm for one instance file.

	void greedyAlgorithmFileOpenError(Filepath filepath); // (Greedy algorithm) Print out error messages to mark files which couldn't be properly opened.
	void greedyAlgorithmInstanceFileRepetitions(Filepath filepath, greedyCVRP &greedyCVRP); // A full set of repetitions of the greedyCVRP algorithm for one instance file.

	public:
		void geneticAlgorithmExperiment(Filepath filepath); // Run the experiment using a genetic algorithm.

		void randomAlgorithmExperiment(Filepath filepath); // Run the experiment using a random algorithm.

		void greedyAlgorithmExperiment(Filepath filepath); // Run the experiment using a greedy algorithm.
};

