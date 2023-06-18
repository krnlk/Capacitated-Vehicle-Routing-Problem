#include <iostream>
#include <fstream>
#include <windows.h>

#include "environment.h"
#include "geneticCVRP.h"
#include "greedyCVRP.h"
#include "randomCVRP.h"
#include "filepath.h"
#include "pseudoRandomGeneration.h"

// Used for tracking time.
long long int read_QPC()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return((long long int)count.QuadPart);
}

// Open a file with the instances and solve them using genetic algorithm.
// The files are loaded one after another - when all the iterations of an algorithm for an instance are finished,
// then and only then will the new instance file be loaded.
void environment::geneticAlgorithmExperiment(filepath filepath) {
	geneticCVRP geneticCVRP;
	std::ofstream generationResultsFile;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	experimentFile.open(filepath.getInitialisationFile()); // Open input and output files.
	testResultsFile.open(filepath.getOutputFile());
	generationResultsFile.open(filepath.getGenerationResultsFile());

	if (experimentFile.is_open() && testResultsFile.is_open() && generationResultsFile.is_open()) { // If all files have been opened succesfully.
		geneticCVRP.setAlgorithmParameters(experimentFile); // Sets algorithm parameters for the experiment.

		testResultsFile << "instance file;instance repeats;best known route cost;best found route cost;best found route path;average error;average time [s];average best solution" << std::endl;
		generationResultsFile << "file name;iteration;generation;minimum specimen cost so far;maximum specimen cost so far;average specimen cost;minimum specimen cost this generation; maximum specimen cost this generation" << std::endl;

		while (!experimentFile.eof()) {
			this->geneticAlgorithmInstanceFileRepetitions(filepath, geneticCVRP, generationResultsFile);
		}
	}
	else { // If any of the files can't be reached/the directory doesn't exist.
		this->geneticOrRandomAlgorithmFileOpenError(filepath, generationResultsFile);
	}

	experimentFile.close(); // Close input and output files.
	testResultsFile.close();
	generationResultsFile.close();
}

// A full set of repetitions of the geneticCVRP algorithm for one instance file.
// Amount of repetitions is defined by the instanceRepeats value in the experiment file.
void environment::geneticAlgorithmInstanceFileRepetitions(filepath filepath, geneticCVRP& geneticCVRP, std::ofstream& generationResultsFile) {
	experimentFile >> instanceFileName >> instanceRepeats;
	instanceFile.loadInstanceData(filepath.getInstancesPath(), instanceFileName);
	geneticCVRP.setInstanceFile(instanceFile); // Set file used by algorithm.

	if (instanceFile.getIsFileOpen()) {
		overallBestSolutionFound = INT_MAX;

		averageError = 0.0;
		averageTime = 0.0;
		averageSolution = 0.0;

		for (int i = 0; i < instanceRepeats; i++) { // Log separate results for each repeat of an instance.
			geneticCVRP.clearInfo(); // Clear best found solution and its order from the previous run.
			start = read_QPC(); // Start tracking time.
			geneticCVRP.mainAlgorithmLoop(generationResultsFile, i + 1);
			elapsed = read_QPC() - start; // Stop tracking time.
			// After the algorithm is over, save the results to a file.
			averageError += (((double)geneticCVRP.getBestFoundSolutionTotalCost() - instanceFile.getOptimalValue()) / (double)instanceFile.getOptimalValue());
			averageTime += (elapsed / (double)frequency);
			averageSolution += geneticCVRP.getBestFoundSolutionTotalCost();

			if (geneticCVRP.getBestFoundSolutionTotalCost() < overallBestSolutionFound) {
				overallBestSolutionFound = geneticCVRP.getBestFoundSolutionTotalCost();
				bestFoundRoutePath = geneticCVRP.getBestFoundSolutionPointOrder();
			}

			std::cout << "Iteration number " << i + 1 << " of instance file " << instanceFileName << " has finished." << std::endl;
		}

		averageError = averageError / (double)instanceRepeats;
		averageTime = averageTime / (double)instanceRepeats;
		averageSolution = averageSolution / (double)instanceRepeats;

		testResultsFile << instanceFileName << ";" << instanceRepeats << ";" << instanceFile.getOptimalValue() << ";" << overallBestSolutionFound
			<< ";" << bestFoundRoutePath << ";" << averageError * 100.00 << "%" << ";" << averageTime << ";" << averageSolution << std::endl;
	}
}

// Print out error messages to mark files which couldn't be properly opened.
// Mandatory to pass ofstream as an argument using a reference, as it has no copy constructor.
// Genetic and random algorithms only.
void environment::geneticOrRandomAlgorithmFileOpenError(filepath filepath, std::ofstream& generationResultsFile) {
	if (!experimentFile.is_open()) {
		std::cout << "Initialisation file " << filepath.getInitialisationFile() << " couldn't be opened." << std::endl;
	}

	if (!testResultsFile.is_open()) {
		std::cout << "Output file " << filepath.getOutputFile() << " couldn't be created or opened." << std::endl;
	}

	if (!generationResultsFile.is_open()) {
		std::cout << "Generation results file " << filepath.getGenerationResultsFile() << " couldn't be created or opened." << std::endl;
	}
}

// Open a file with the instances and solve them using a basic pseudorandom algorithm.
// The files are loaded one after another - when all the iterations of an algorithm for an instance are finished,
// then and only then will the new instance file be loaded.
void environment::randomAlgorithmExperiment(filepath filepath) {
	randomCVRP randomCVRP;
	std::ofstream iterationResultsFile;
	unsigned rnd = time(NULL) * getRand(0, 9999); // Create a random formula for shuffling.

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	experimentFile.open(filepath.getInitialisationFile()); // Open input and output files.
	testResultsFile.open(filepath.getOutputFile());
	iterationResultsFile.open(filepath.getGenerationResultsFile());

	if (experimentFile.is_open() && testResultsFile.is_open() && iterationResultsFile.is_open()) { // If all files have been opened succesfully.
		randomCVRP.ignoreInitialAlgorithmParameters(experimentFile);

		testResultsFile << "instance file;instance repeats;best known route cost;best found route cost;best found route path;average error;average time [s];average best solution" << std::endl;
		iterationResultsFile << "file name;iteration;generation;minimum solution cost so far;maximum solution cost so far;average solution cost;minimum solution cost this generation; maximum solution cost this generation" << std::endl;

		while (!experimentFile.eof()) {
			this->randomAlgorithmInstanceFileRepetitions(filepath, randomCVRP, iterationResultsFile);
		}
	}
	else { // If any of the files can't be reached/the directory doesn't exist.
		this->geneticOrRandomAlgorithmFileOpenError(filepath, iterationResultsFile);
	}

	experimentFile.close(); // Close input and output files.
	testResultsFile.close();
	iterationResultsFile.close();
}

// A full set of repetitions of the randomCVRP algorithm for one instance file.
// Amount of repetitions is defined by the instanceRepeats value in the experiment file.
void environment::randomAlgorithmInstanceFileRepetitions(filepath filepath, randomCVRP& randomCVRP, std::ofstream& iterationResultsFile) {
	experimentFile >> instanceFileName >> instanceRepeats;
	instanceFile.loadInstanceData(filepath.getInstancesPath(), instanceFileName);
	randomCVRP.setInstanceFile(instanceFile); // Set file used by algorithm.

	overallBestSolutionFound = INT_MAX;

	averageError = 0.0;
	averageTime = 0.0;
	averageSolution = 0.0;

	for (int i = 0; i < instanceRepeats; i++) { // Log separate results for each repeat of an instance.
		randomCVRP.clearInfo();
		start = read_QPC(); // Start tracking time.
		randomCVRP.mainAlgorithmLoop(iterationResultsFile, i);
		elapsed = read_QPC() - start; // Stop tracking time.
		// After the algorithm is over, save the results to a file.
		averageError += (((double)randomCVRP.getBestFoundSolutionTotalCost() - instanceFile.getOptimalValue()) / (double)instanceFile.getOptimalValue());
		averageTime += (elapsed / (double)frequency);
		averageSolution += randomCVRP.getBestFoundSolutionTotalCost();

		if (randomCVRP.getTotalCost() < overallBestSolutionFound) {
			overallBestSolutionFound = randomCVRP.getBestFoundSolutionTotalCost();
			bestFoundRoutePath = randomCVRP.getStringifiedPointOrder();
		}
	}

	averageError = averageError / (double)instanceRepeats;
	averageTime = averageTime / (double)instanceRepeats;
	averageSolution = averageSolution / (double)instanceRepeats;

	testResultsFile << instanceFileName << ";" << instanceRepeats << ";" << instanceFile.getOptimalValue() << ";" << overallBestSolutionFound
		<< ";" << bestFoundRoutePath << ";" << averageError * 100.00 << "%" << ";" << averageTime << ";" << averageSolution << std::endl;
}

// Open a file with the instances and solve them using greedy algorithm.
// The files are loaded one after another - when all the iterations of an algorithm for an instance are finished,
// then and only then will the new instance file be loaded.
void environment::greedyAlgorithmExperiment(filepath filepath) {
	greedyCVRP greedyCVRP;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	experimentFile.open(filepath.getInitialisationFile()); // Open input and output files.
	testResultsFile.open(filepath.getOutputFile());

	if (experimentFile.is_open() && testResultsFile.is_open()) { // If all files have been opened succesfully.
		greedyCVRP.ignoreInitialAlgorithmParameters(experimentFile); // 

		testResultsFile << "instance file;best known route cost;found route cost;found route path;error;time [s]" << std::endl;

		while (!experimentFile.eof()) {
			greedyAlgorithmInstanceFileRepetitions(filepath, greedyCVRP);
		}
	}
	else { // If any of the files can't be reached/the directory doesn't exist.
		this->greedyAlgorithmFileOpenError(filepath);
	}

	experimentFile.close(); // Close input and output files.
	testResultsFile.close();
}

// Print out error messages to mark files which couldn't be properly opened.
// Mandatory to pass ofstream as an argument using a reference, as it has no copy constructor.
// Greedy algorithm only.
void environment::greedyAlgorithmFileOpenError(filepath filepath) {
	if (!experimentFile.is_open()) {
		std::cout << "Initialisation file " << filepath.getInitialisationFile() << " couldn't be opened." << std::endl;
	}

	if (!testResultsFile.is_open()) {
		std::cout << "Output file " << filepath.getOutputFile() << " couldn't be created or opened." << std::endl;
	}
}

// A full set of repetitions of the greedyCVRP algorithm for one instance file.
// Amount of repetitions is defined by the instanceRepeats value in the experiment file.
void environment::greedyAlgorithmInstanceFileRepetitions(filepath filepath, greedyCVRP& greedyCVRP)
{
	experimentFile >> instanceFileName >> instanceRepeats;
	instanceFile.loadInstanceData(filepath.getInstancesPath(), instanceFileName);

	overallBestSolutionFound = INT_MAX;

	averageError = 0.0;
	averageTime = 0.0;

	start = read_QPC(); // Start tracking time.
	greedyCVRP.generateASolution(instanceFile);
	elapsed = read_QPC() - start; // Stop tracking time.

	averageError = (((double)greedyCVRP.getTotalCost() - instanceFile.getOptimalValue()) / (double)instanceFile.getOptimalValue());
	averageTime = (elapsed / (double)frequency);

	testResultsFile << instanceFileName << ";" << instanceFile.getOptimalValue() << ";" << greedyCVRP.getTotalCost()
		<< ";" << greedyCVRP.getStringifiedPointOrder() << ";" << averageError * 100.00 << "%" << ";" << averageTime << std::endl;
}
