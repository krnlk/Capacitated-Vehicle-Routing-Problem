#include <iostream>
#include <sstream>
#include <algorithm>

#include "instanceFile.h"
#include "geneticIOData.h"

// Get the cost of the solution with the lowest found cost.
int geneticIOData::getBestFoundSolutionTotalCost()
{
	return bestFoundSolutionTotalCost;
}

// Get the point order of the solution with the lowest found cost
// in string format, with white spaces to separate points.
std::string geneticIOData::getBestFoundSolutionPointOrder() {
	std::stringstream stringifiedPointOrder;
	for (auto iterator = bestFoundSolutionPointOrder.begin(); iterator != bestFoundSolutionPointOrder.end(); iterator++) {
		if (iterator != bestFoundSolutionPointOrder.begin()) {
			stringifiedPointOrder << " ";
		}
		stringifiedPointOrder << *iterator;
	}
	return stringifiedPointOrder.str();
}

// Read algorithm parameters from a file (assuming specific format).
void geneticIOData::setAlgorithmParameters(std::ifstream& experimentFile)
{
	std::string textLine;
	char crossoverUsed, mutationUsed;
	double crossoverProbability, mutationProbability;

	std::getline(experimentFile, textLine); // Skip unneeded info.

	std::getline(experimentFile, textLine);
	selectUsed = textLine.substr(14)[0]; // Trim out beginning of the line to get to mutation.

	std::getline(experimentFile, textLine);
	crossoverUsed = textLine.substr(17)[0]; // Trim out beginning of the line to get to crossover.

	std::getline(experimentFile, textLine);
	mutationUsed = textLine.substr(16)[0]; // Trim out beginning of the line to get to mutation.

	std::getline(experimentFile, textLine);
	crossoverProbability = std::stod(textLine.substr(24)); // Trim out beginning of the line to get to crossover probability and convert it to integer.

	std::getline(experimentFile, textLine);
	mutationProbability = std::stod(textLine.substr(23)); // Trim out beginning of the line to get to mutation probability and convert it to integer.

	std::getline(experimentFile, textLine);
	numberOfSpecimenInAGeneration = stoi(textLine.substr(36)); // Trim out beginning of the line to get to number of specimen in a generation and convert it to integer.

	std::getline(experimentFile, textLine);
	maxNumberOfGenerations = stoi(textLine.substr(24)); // Trim out beginning of the line to get to number of generations and convert it to integer.

	std::getline(experimentFile, textLine); // Skip unneeded info.

	geneticCVRP.setGeneticParameters(selectUsed, crossoverUsed, mutationUsed, crossoverProbability, mutationProbability, numberOfSpecimenInAGeneration);
}

// Update the information about the new best found solution.
void geneticIOData::setBestSpecimenInfo(int bestSpecimenIndex) {
	bestFoundSolutionTotalCost = geneticCVRP.getSpecimenTotalCostByIndex(bestSpecimenIndex);
	bestFoundSolutionPointOrder = geneticCVRP.getBestFoundSolutionPointOrderByIndex(bestSpecimenIndex);
}

//
void geneticIOData::setInstanceFile(const instanceFile& instanceFile) {
	this->loadedInstanceFile = instanceFile;
}

// Used in rank selection.
void geneticIOData::calculateSumOfRanks() {
	sumOfRanks = 0;

	for (int i = 0; i < numberOfSpecimenInAGeneration; i++) {
		sumOfRanks += i + 1;
	}

	geneticCVRP.setSumOfRanks(sumOfRanks);
}

// Main genetic algorithm loop.
void geneticIOData::mainAlgorithmLoop(std::ofstream& experimentResultsFile, int iteration) {
	loadedInstanceFile.calculateDistancesBetweenPoints();

	geneticCVRP.setInstanceFile(loadedInstanceFile);

	for (int i = 0; i < loadedInstanceFile.dimension; i++) {
		bestFoundSolutionPointOrder.push_back(i); // Start with adding all points in the order they are presented in in the instance file.
	}

	if (selectUsed == 'k') {
		calculateSumOfRanks(); // If the select is rank selection, calculate sum of ranks for all points.
	}

	geneticCVRP.generateInitialSpecimen(); // Generate the initial generation of specimen.
	analyseNewGeneration(0, experimentResultsFile, iteration); // Find best and worst specimen of the initial generation.

	for (int i = 0; i < maxNumberOfGenerations; i++) {
		geneticCVRP.generationLoop();
		analyseNewGeneration(i + 1, experimentResultsFile, iteration); // Find best specimen.
	}

	loadedInstanceFile.dealocateDistancesBetweenPoints();
}

// Find best specimen in the new generation and check, if it's better than the best found specimen across all previous generations.
// Find the worst cost of a specimen in new generation.
void geneticIOData::analyseNewGeneration(int generationNumber, std::ofstream& experimentResultsFile, int iteration) {
	int averageSolutionTotalCost = 0; // Average of total costs of all solution in a particular generation.
	int currentGenerationWorstFoundSolutionTotalCost = 0;
	int currentGenerationBestFoundSolutionTotalCost = INT_MAX; // Total cost of the best solution found (current generation only).
	bool newBestSpecimenFound = false; // Used for tracking if a new best specimen has been found in this generation.

	for (int j = 0; j < numberOfSpecimenInAGeneration; j++) { // Check for a new best solution.
		findBestSpecimenInAGeneration(j, averageSolutionTotalCost, currentGenerationBestFoundSolutionTotalCost, bestSpecimenIndex, newBestSpecimenFound);
		findWorstSpecimenInAGeneration(j, currentGenerationWorstFoundSolutionTotalCost);
	}

	if (newBestSpecimenFound) { // If a new best total cost has been found.
		setBestSpecimenInfo(bestSpecimenIndex); // Save its information.
	}

	geneticCVRP.setWorstFoundSolutionTotalCost(worstFoundSolutionTotalCost);

	// Save information to generation results file.
	experimentResultsFile << loadedInstanceFile.getFileName() << ";" << iteration << ";" << generationNumber << ";" <<
		bestFoundSolutionTotalCost << ";" << worstFoundSolutionTotalCost << ";" <<
		averageSolutionTotalCost / numberOfSpecimenInAGeneration << ";" <<
		currentGenerationBestFoundSolutionTotalCost << ";" << currentGenerationWorstFoundSolutionTotalCost << std::endl;
}

// Save information about average solution and potential new best solutions in this generation.
void geneticIOData::findBestSpecimenInAGeneration(int index, int& averageSolutionTotalCost, int& currentGenerationBestFoundSolutionTotalCost, int& bestSpecimenIndex, bool& newBestSpecimenFound) {
	int specimenTotalCost;
	specimenTotalCost = geneticCVRP.getSpecimenTotalCostByIndex(index);
	
	geneticCVRP.calculateSpecimenTotalCostByIndex(index);
	averageSolutionTotalCost += specimenTotalCost;

	if (specimenTotalCost < currentGenerationBestFoundSolutionTotalCost) { // Check if current specimen is best in current generation.
		currentGenerationBestFoundSolutionTotalCost = specimenTotalCost;

		if (specimenTotalCost < bestFoundSolutionTotalCost) { // If current generation best specimen is better than best specimen across all generations.
			bestFoundSolutionTotalCost = specimenTotalCost;
			bestSpecimenIndex = index;
			newBestSpecimenFound = true;
		}
	}
}

// Save information about worst total cost in this generation.
void geneticIOData::findWorstSpecimenInAGeneration(int index, int& currentGenerationWorstFoundSolutionTotalCost) {
	int specimenTotalCost;
	specimenTotalCost = geneticCVRP.getSpecimenTotalCostByIndex(index);

	if (specimenTotalCost > currentGenerationWorstFoundSolutionTotalCost) { // Check if current specimen is worst in current generation.
		currentGenerationWorstFoundSolutionTotalCost = specimenTotalCost;

		if (worstFoundSolutionTotalCost < currentGenerationWorstFoundSolutionTotalCost) {
			worstFoundSolutionTotalCost = currentGenerationWorstFoundSolutionTotalCost;
		}
	}
}

// Clean up all information about best found solution and all the specimen.
void geneticIOData::clearInfo() {
	bestFoundSolutionPointOrder.clear();
	bestFoundSolutionTotalCost = INT_MAX;
	worstFoundSolutionTotalCost = 0;

	geneticCVRP.clearAllSpecimen();
}