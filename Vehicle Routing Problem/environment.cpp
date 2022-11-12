#include <iostream>
#include <fstream>
#include <windows.h>

#include "algorithm_1.h"
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
void environment::beginExperiment(std::string experimentFileName, std::string outputFileName) {
	std::ifstream experimentFile;
	std::ofstream resultsFile;
	instanceFile instanceFile;
	algorithm_1 algorithm_1;
	int instanceRepeats;

	long long int frequency, start, elapsed; // For tracking time.
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency); 


	experimentFile.open(experimentFileName); // Open input and output files.
	resultsFile.open(outputFileName);
	
	if (experimentFile.is_open() && resultsFile.is_open()) { // If both files have been opened succesfully.
		algorithm_1.setAlgorithmParameters(experimentFile); // Sets algorithm parameters for the experiment.

		resultsFile << "instanceFile;instanceRepeats;bestKnownRouteCost;bestFoundRouteCost;bestFoundRoutePath;error;time";
		

		while (!experimentFile.eof()){
			experimentFile >> instanceFileName >> instanceRepeats;
			instanceFile.loadInstanceData(instanceFileName);

			instanceFile.printInstanceData();

			for (int i = 0; i < instanceRepeats; i++) { // Log separate results for each repeat of an instance.
				start = read_QPC(); // Start tracking time.
				algorithm_1.mainAlgorithmLoop(instanceFile);
				elapsed = read_QPC() - start; // Stop tracking time.
				// After the algorithm is over, save the results to a file.
				error = (algorithm_1.getBestFoundSolutionTotalCost() - instanceFile.optimalValue) / instanceFile.optimalValue;

				resultsFile << instanceFileName << ";" << instanceRepeats << ";" << instanceFile.optimalValue << ";" << algorithm_1.getBestFoundSolutionTotalCost()
					<< ";" << bestFoundRoutePath << ";" << error << "%" << ";" << elapsed;
			}
		}
	}

	experimentFile.close(); // Close input and output files.
	resultsFile.close();
}
