#include <iostream>
#include <fstream>
#include <windows.h>

#include "geneticCVRP.h"
#include "environment.h"
#include "instanceFile.h"

long long int read_QPC() // Used for tracking time.
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return((long long int)count.QuadPart);
}

// Opens a file with the instances.
// The files are loaded one after another - when all the iterations of an algorithm for an instance are finished,
// then and only then will the new instance file be loaded.
void environment::beginExperiment(std::string experimentFileName, std::string outputFileName, std::string generationResultsFileName) {
	std::ifstream experimentFile;
	std::ofstream testResultsFile;
	std::ofstream generationResultsFile;

	instanceFile instanceFile;
	geneticCVRP geneticCVRP;
	int instanceRepeats;

	double averageTime; 
	double averageSolution;
	int overallBestSolutionFound;

	long long int frequency, start, elapsed; // For tracking time.
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency); 


	experimentFile.open(experimentFileName); // Open input and output files.
	testResultsFile.open(outputFileName);
	generationResultsFile.open(generationResultsFileName);

	if (experimentFile.is_open() && testResultsFile.is_open() && generationResultsFile.is_open()) { // If all files have been opened succesfully.
		geneticCVRP.setAlgorithmParameters(experimentFile); // Sets algorithm parameters for the experiment.

		testResultsFile << "instance file;instance repeats;best known route cost;best found route cost;best found route path;average error;average time[unit];average best solution" << std::endl;
		generationResultsFile << "file name;iteration;generation;minimum specimen cost;maximum specimen cost;average specimen cost" << std::endl;

		while (!experimentFile.eof()){
			experimentFile >> instanceFileName >> instanceRepeats;
			instanceFile.loadInstanceData(instanceFileName);

			overallBestSolutionFound = INT_MAX;

			averageError = 0.0;
			averageTime = 0.0;
			averageSolution = 0.0;

			for (int i = 0; i < instanceRepeats; i++) { // Log separate results for each repeat of an instance.
				geneticCVRP.clearInfo(); // Clear best found solution and its order from the previous run.
				start = read_QPC(); // Start tracking time.
				geneticCVRP.mainAlgorithmLoop(instanceFile, generationResultsFile);
				elapsed = read_QPC() - start; // Stop tracking time.
				// After the algorithm is over, save the results to a file.
				std::cout << averageError << " " << geneticCVRP.getBestFoundSolutionTotalCost() << 
					" " << instanceFile.optimalValue << " " << (geneticCVRP.getBestFoundSolutionTotalCost() - instanceFile.optimalValue) / (double) instanceFile.optimalValue << std::endl;
				averageError = averageError + ((geneticCVRP.getBestFoundSolutionTotalCost() - instanceFile.optimalValue) / (double) instanceFile.optimalValue);
				averageTime += elapsed;
				averageSolution += geneticCVRP.getBestFoundSolutionTotalCost();

				if (geneticCVRP.getBestFoundSolutionTotalCost() < overallBestSolutionFound) {
					overallBestSolutionFound = geneticCVRP.getBestFoundSolutionTotalCost();
					bestFoundRoutePath = geneticCVRP.getBestFoundSolutionPointOrder();
				}

				std::cout << "Iteration number " << i + 1 << " of instance file " << instanceFileName << " has finished." << std::endl;
			}

			averageError = averageError / instanceRepeats;
			averageTime = averageTime / instanceRepeats;
			averageSolution = averageSolution / instanceRepeats;

			testResultsFile << instanceFileName << ";" << instanceRepeats << ";" << instanceFile.optimalValue << ";" << overallBestSolutionFound
				<< ";" << bestFoundRoutePath << ";" << averageError * 100.00 << "%" << ";" << averageTime << ";" << averageSolution << std::endl;
		}
	}

	experimentFile.close(); // Close input and output files.
	testResultsFile.close();
	generationResultsFile.close();
}
