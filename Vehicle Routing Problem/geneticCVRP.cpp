#include <iostream>
#include <sstream>
#include <algorithm>

#include "geneticCVRP.h"
#include "geneticParameters.h"
#include "specimen.h"
#include "pseudoRandomGeneration.h"

// Returns true if the first specimen has a smaller total cost than the second one.
// Used when sorting specimen.
bool compareSpecimen(specimen firstSpecimen, specimen secondSpecimen) {
	return (firstSpecimen.getTotalCost() < secondSpecimen.getTotalCost());
}

void geneticCVRP::calculateSpecimenTotalCostByIndex(int index) {
	allCurrentSpecimen[index].calculateTotalCost(loadedInstanceFile);
}

///
int geneticCVRP::getSpecimenTotalCostByIndex(int index) {
	return allCurrentSpecimen[index].getTotalCost();
}

std::vector<int> geneticCVRP::getBestFoundSolutionPointOrderByIndex(int index) {
	std::vector<int> bestFoundSolutionPointOrder;

	for (int i = 0; i < loadedInstanceFile.dimension; i++) {
		bestFoundSolutionPointOrder.push_back(allCurrentSpecimen[index].getPointByIndex(i));
	}

	return bestFoundSolutionPointOrder;
}

//
void geneticCVRP::setInstanceFile(const instanceFile& instanceFile) {
	this->loadedInstanceFile = instanceFile;
}

void geneticCVRP::setGeneticParameters(char selectUsed, char crossoverUsed, char mutationUsed, double crossoverProbability, double mutationProbability, int numberOfSpecimenInAGeneration) {
	this->geneticParameters.selectUsed = selectUsed;
	this->geneticParameters.crossoverUsed = crossoverUsed;
	this->geneticParameters.mutationUsed = mutationUsed;
	this->geneticParameters.crossoverProbability = crossoverProbability;
	this->geneticParameters.mutationProbability = mutationProbability;
	this->geneticParameters.numberOfSpecimenInAGeneration = numberOfSpecimenInAGeneration;
}

void geneticCVRP::setWorstFoundSolutionTotalCost(int worstFoundSolutionTotalCost) {
	this->worstFoundSolutionTotalCost = worstFoundSolutionTotalCost;
}

void geneticCVRP::setSumOfRanks(int sumOfRanks) {
	this->sumOfRanks = sumOfRanks;
}

void geneticCVRP::clearAllSpecimen() {
	allCurrentSpecimen.clear();
	newGenerationOfSpecimen.clear();
}

void geneticCVRP::generationLoop() {
	if (geneticParameters.selectUsed == 'k') // For each generation in rank selection.
	{
		std::sort(allCurrentSpecimen.begin(), allCurrentSpecimen.end(), compareSpecimen); // If the select is rank selection, rank specimen accordingly.
	}
	createNewOffspring(); // Create a new generation of specimen.
	mutate(); // Mutate new specimen.
}

// Randomly generate initial specimen.
void geneticCVRP::generateInitialSpecimen() {
	std::vector<int> pointOrder;
	specimen specimen;

	for (int i = 0; i < loadedInstanceFile.dimension; i++) { // Generate a point order that will be shuffled between subsequent speciment.
		pointOrder.push_back(i);
	}

	for (int i = 0; i < geneticParameters.numberOfSpecimenInAGeneration; i++) { // For each specimen, generate a random route.
		specimen.generateRandomRoute(loadedInstanceFile, pointOrder);
		allCurrentSpecimen.push_back(specimen);
		specimen.clearPointOrder();
	}
}

// Generates a new generation of specimen by either taking crossover of two randomly selected parents as two children
// or by taking two of the randomly selected parents as the children outright.
// This process is repeated until there are numberOfSpecimenInAGeneration specimen in the new generation.
void geneticCVRP::createNewOffspring() {
	int probability = getRand(0, 99);

	newGenerationOfSpecimen.clear(); // Clear the values.

	if (geneticParameters.selectUsed == 'w' || geneticParameters.selectUsed == 's') { 	// If the selection is either roulette wheel or stochastic universal sampling.
		calculateFitnessFunction();
	}

	for (int i = 0; i < geneticParameters.numberOfSpecimenInAGeneration / 2; i++) // Both using crossover and just taking the parents generate 2 offspring.
	{
		std::pair<int, int> parentIndexes = selection();

		probability = getRand(0, 99);

		if (probability / 100.0 < geneticParameters.crossoverProbability) { // Produce two offspring from two parents using crossover.
			selectCrossover(parentIndexes.first, parentIndexes.second);
		} 
		else { // Take two parents without modifying them, but guarantee they'll mutate.
			newGenerationOfSpecimen.push_back(allCurrentSpecimen[parentIndexes.first]);
			newGenerationOfSpecimen.push_back(allCurrentSpecimen[parentIndexes.second]);

			newGenerationOfSpecimen[2 * i].obligatoryMutation = true;
			newGenerationOfSpecimen[2 * i].pointLocationSet = false;
			newGenerationOfSpecimen[2 * i + 1].obligatoryMutation = true;
			newGenerationOfSpecimen[2 * i + 1].pointLocationSet = false;
		}
	}

	allCurrentSpecimen = newGenerationOfSpecimen; // Replace it with the new generation by copying pointer's value.
}

// Calculates fitness function for roulette wheel and stochastic universal sampling methods.
void geneticCVRP::calculateFitnessFunction() {
	fitnessFunction = 0;

	for (int i = 0; i < allCurrentSpecimen.size(); i++) {
		// Get the difference between worst solution point cost and this specimen's cost.
		fitnessFunction += worstFoundSolutionTotalCost - allCurrentSpecimen[i].getTotalCost();
	}
}

// Returns a pair of point indexes, used to select parents.
std::pair<int, int> geneticCVRP::selection() {
	std::pair<int, int> parentIndexes;

	double firstProbability, secondProbability;
	double totalSpecimenValueSum;

	switch (geneticParameters.selectUsed) { // Select one of selection methods.
	case 'k': // Rank selection.
		return rankSelectionIndexes();
		break;

	case 'r': // Random selection.
		return randomSelection();
		break;

	case 's': // Stochastic universal sampling.
		return stochasticUniversalSamplingIndexes();
		break;

	case 't': // Tournament selection.
		return tournamentSelectionIndexes();
		break;

	case 'w': // Roulette wheel selection.
		return rouletteWheelSelectionIndexes();
		break;

	default:
		break;
	}
}

// Select a pair of indexes using rank selection.
std::pair<int, int> geneticCVRP::rankSelectionIndexes() {
	std::pair<int, int> parentIndexes;

	parentIndexes.first = rankSelection();
	do {
		parentIndexes.second = rankSelection();
	} while (parentIndexes.first == parentIndexes.second);
	return parentIndexes;
}

// Return a single parent index using rank selection.
int geneticCVRP::rankSelection() {
	int parentIndex = 0;
	double totalRankSum = 0.0;

	double probability = getRand(0, 9999);
	probability /= 10000.0;

	while (parentIndex < geneticParameters.numberOfSpecimenInAGeneration) {
		totalRankSum += (geneticParameters.numberOfSpecimenInAGeneration - parentIndex) / (double)sumOfRanks; // Due to previous sorting.
		if (totalRankSum > probability) {
			return parentIndex;
		}
		parentIndex++;
	}
}

// Select a pair of indexes using random selection.
std::pair<int, int> geneticCVRP::randomSelection() {
	return getRandUniquePair(0, geneticParameters.numberOfSpecimenInAGeneration - 1);
}

// Select a pair of indexes using stochastic universal sampling selection.
std::pair<int, int> geneticCVRP::stochasticUniversalSamplingIndexes() {
	std::pair<int, int> parentIndexes;

	double firstProbability, secondProbability;
	double totalSpecimenValueSum;

	do {
		firstProbability = (getRand(0, 9999)) / 10000.0;
		secondProbability = firstProbability >= 0.5 ? firstProbability - 0.5 : firstProbability + 0.5;

		if (firstProbability > secondProbability) {
			std::swap(firstProbability, secondProbability);
		}

		totalSpecimenValueSum = 0.0;
		parentIndexes.first = rouletteWheelSelection(0, totalSpecimenValueSum, firstProbability);
		parentIndexes.second = rouletteWheelSelection(parentIndexes.first + 1, totalSpecimenValueSum, secondProbability);
	} while (parentIndexes.first == parentIndexes.second);

	return parentIndexes;
}

// Select a pair of indexes using tournament selection.
std::pair<int, int> geneticCVRP::tournamentSelectionIndexes() {
	std::pair<int, int> parentIndexes;

	parentIndexes.first = tournamentSelection();
	do {
		parentIndexes.second = tournamentSelection();
	} while (parentIndexes.first == parentIndexes.second);
	return parentIndexes;
}

// Return a single parent index using tournament selection.
int geneticCVRP::tournamentSelection() {
	int parentIndex;
	int numberOfTournamentParticipants = geneticParameters.numberOfSpecimenInAGeneration / 10;
	int randomParticipantIndex;

	int bestParticipantValue;

	if (numberOfTournamentParticipants <= 1) {
		numberOfTournamentParticipants = 2;
	}

	// First member of the tournament
	parentIndex = getRand(0, geneticParameters.numberOfSpecimenInAGeneration - 1);
	bestParticipantValue = allCurrentSpecimen[parentIndex].getTotalCost();

	for (int i = 1; i < numberOfTournamentParticipants; i++) {
		randomParticipantIndex = getRand(0, geneticParameters.numberOfSpecimenInAGeneration - 1);
		if (allCurrentSpecimen[randomParticipantIndex].getTotalCost() < bestParticipantValue) {
			bestParticipantValue = allCurrentSpecimen[randomParticipantIndex].getTotalCost();
			parentIndex = randomParticipantIndex;
		}
	}

	return parentIndex;
}

// Select a pair of indexes using roulette wheel selection.
std::pair<int, int> geneticCVRP::rouletteWheelSelectionIndexes() {
	std::pair<int, int> parentIndexes;

	double firstProbability, secondProbability;
	double totalSpecimenValueSum;

	firstProbability = (getRand(0, 9999)) / 10000.0;

	totalSpecimenValueSum = 0.0;

	parentIndexes.first = rouletteWheelSelection(0, totalSpecimenValueSum, firstProbability);
	do {
		firstProbability = (getRand(0, 9999)) / 10000.0;
		totalSpecimenValueSum = 0.0;
		parentIndexes.second = rouletteWheelSelection(0, totalSpecimenValueSum, firstProbability);
	} while (parentIndexes.first == parentIndexes.second);
	return parentIndexes;
}

// Return a single parent index using rank selection.
int geneticCVRP::rouletteWheelSelection(int parentIndex, double &totalSpecimenValueSum, double probability) {
	while (parentIndex < geneticParameters.numberOfSpecimenInAGeneration) {
		totalSpecimenValueSum += (worstFoundSolutionTotalCost - allCurrentSpecimen[parentIndex].getTotalCost()) / (double)fitnessFunction; // Casting to double to avoid dividing integer by integer.
		if (totalSpecimenValueSum > probability) {
			return parentIndex;
		}
		parentIndex++;
	}

	return parentIndex; // In case of rounding errors where of totalSpecimenValueSum doesn't equal 1.0 after adding all the specimen.
}

// Selects crossover based on data loaded from a file.
void geneticCVRP::selectCrossover(int firstParentIndex, int secondParentIndex) {
	std::pair<int, int> parentIndexPair; // Setting up cutoff points for ordered crossover selection.

	switch (geneticParameters.crossoverUsed) { // Select crossover that's currently used in the experiment.
		case 'c': // Cycle crossover.
			cycleCrossover(firstParentIndex, secondParentIndex);
			break;

		case 'o': // Ordered crossover.
			parentIndexPair = getRandUniquePair(1, loadedInstanceFile.dimension - 1); // Get two different cutoff indexes.
			// Create two new specimen using ordered crossover.
			newGenerationOfSpecimen.push_back(orderedCrossover(firstParentIndex, secondParentIndex, parentIndexPair.first, parentIndexPair.second));
			newGenerationOfSpecimen.push_back(orderedCrossover(secondParentIndex, firstParentIndex, parentIndexPair.first, parentIndexPair.second));
			break;

		case 'p': // Partially mapped crossover.
			partiallyMappedCrossoverCutoffsSetup(firstParentIndex, secondParentIndex);
			break;

		default:
			break;
	}
}

// This crossover generates two offspring from two parents by finding cycles in parents, then copying cycle 1 from parent 1, cycle 2 from parent 2 etc.
void geneticCVRP::cycleCrossover(int firstParentIndex, int secondParentIndex) {
	std::vector<int> cycles; // Vector used for marking which point belongs to which cycle.
	int pointsVisited = 1; // Depot is skipped.
	int currentPoint = 1; // Index of the point that's used to start a new cycle.
	int nextPoint; // Next point in the cycle.
	int firstPoint; // First point in a cycle - reaching it again will complete the cycle.
	int currentCycle = 1; // Number of the current cycle.

	int* firstParentPointLocation = new int[loadedInstanceFile.dimension]; // Stores indexes of first parent's visited points.

	for (int i = 0; i < loadedInstanceFile.dimension; i++) {
		cycles.push_back(-1); // -1 means that this point doesn't belong to any cycle (yet).
		firstParentPointLocation[allCurrentSpecimen[firstParentIndex].getPointByIndex(i)] = i; // Point i can be found at an i spot in the array.  
	}

	while (pointsVisited < loadedInstanceFile.dimension) { // Find all cycles. Don't visit the depot.
		while (cycles[currentPoint] != -1) currentPoint++; // Increment currentPoint until the new current point hasn't been a part of any previous cycle.
		cycles[currentPoint] = currentCycle;
		nextPoint = allCurrentSpecimen[secondParentIndex].getPointByIndex(currentPoint); // 
		pointsVisited++;

		while (nextPoint != allCurrentSpecimen[firstParentIndex].getPointByIndex(currentPoint)) { //
			cycles[firstParentPointLocation[nextPoint]] = currentCycle; //
			nextPoint = allCurrentSpecimen[secondParentIndex].getPointByIndex(firstParentPointLocation[nextPoint]); //
			pointsVisited++;
		}

		currentCycle++;
	}

	specimen child1, child2; // Create children specimen.
	std::vector<int> child1Points;
	std::vector<int> child2Points;

	child1Points.push_back(0);
	child2Points.push_back(0);

	for (int i = 1; i < loadedInstanceFile.dimension; i++) {
		if (cycles[i] % 2 == 1) { // Odd cycles.
			child1Points.push_back(allCurrentSpecimen[firstParentIndex].getPointByIndex(i));
			child2Points.push_back(allCurrentSpecimen[secondParentIndex].getPointByIndex(i));
		}
		else { // Even cycles.
			child1Points.push_back(allCurrentSpecimen[secondParentIndex].getPointByIndex(i));
			child2Points.push_back(allCurrentSpecimen[firstParentIndex].getPointByIndex(i));
		}
	}
	child1.setPointOrder(child1Points);
	child2.setPointOrder(child2Points);

	newGenerationOfSpecimen.push_back(child1);
	newGenerationOfSpecimen.push_back(child2);

	delete[] firstParentPointLocation;
}

// This crossover generates two offspring from two parents by selecting a range of points in one parent and copying it over to the child in the same order,
// while the remaining points in child are filled out by inserting non-repeating points from parent 2 in order.
specimen geneticCVRP::orderedCrossover(int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff) {
	specimen specimen;
	std::vector<int> childPoints(loadedInstanceFile.dimension, 0);
	int* pointLocation = new int[loadedInstanceFile.dimension]; // Stores indexes of visited points.
	int childPoint;

	childPoints[0] = 0; // Set depot.	

	for (int i = 0; i < loadedInstanceFile.dimension; i++) {
		pointLocation[i] = -1;
	}

	for (int i = leftCutoff; i <= rightCutoff; i++) { // Copy points in the cutoff from parent 1.
		childPoint = allCurrentSpecimen[firstParentIndex].getPointByIndex(i);
		childPoints[i] = allCurrentSpecimen[firstParentIndex].getPointByIndex(i);
		pointLocation[childPoint] = i; // Remember location of this point.
	}

	int childIndex = rightCutoff + 1; // Index where the next point will be inserted in a child.
	int parentIndex = rightCutoff + 1;

	if (childIndex >= loadedInstanceFile.dimension) {
		childIndex = 1;
	}

	if (parentIndex >= loadedInstanceFile.dimension) {
		parentIndex = 1;
	}

	for (int i = 0; i < loadedInstanceFile.dimension && childIndex != leftCutoff; i++) { // Ignore depot.
		if (pointLocation[allCurrentSpecimen[secondParentIndex].getPointByIndex(parentIndex)] == -1) // If point doesn't belong in the cutoff.
		{
			childPoints[childIndex] = allCurrentSpecimen[secondParentIndex].getPointByIndex(parentIndex);
			childIndex++;
			if (childIndex >= loadedInstanceFile.dimension) {
				childIndex = 1; // Start adding points at the start.
			}
		}

		parentIndex++;
		if (parentIndex >= loadedInstanceFile.dimension) {
			parentIndex = 1;
		}
	}

	specimen.setPointOrder(childPoints); // Set order of points in this specimen.

	delete[] pointLocation;
	return specimen;
}

// Setting up cutoff points for partially mapped crossover selection.
void geneticCVRP::partiallyMappedCrossoverCutoffsSetup(int firstParentIndex, int secondParentIndex) {
	int leftCutoff, rightCutoff; // Cutoffs are used to determine an allel.
	leftCutoff = getRand(1, loadedInstanceFile.dimension - 1); // Avoid leftCutoff value being 0.

	do {
		rightCutoff = getRand(1, loadedInstanceFile.dimension - 1);
	} while (rightCutoff == leftCutoff); // Make sure first and second cutoff are different values. 

	if (leftCutoff > rightCutoff) {
		std::swap(leftCutoff, rightCutoff); // Second cutoff should be a bigger value than the first cutoff.
	}

	newGenerationOfSpecimen.push_back(partiallyMappedCrossover(firstParentIndex, secondParentIndex, leftCutoff, rightCutoff));
	newGenerationOfSpecimen.push_back(partiallyMappedCrossover(secondParentIndex, firstParentIndex, leftCutoff, rightCutoff));
}

// This crossover generates two offspring from two parents by selecting a random crossover range in both parents, swapping those points,
// defining a mapping relationship (to avoid repeating points in offspring) and then mapping remaining points in offspring to make sure nothing repeats.
specimen geneticCVRP::partiallyMappedCrossover(int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff) {
	specimen specimen;

	allCurrentSpecimen[firstParentIndex].setPointLocations(loadedInstanceFile); // Stores indexes of first parent's points.
	allCurrentSpecimen[secondParentIndex].setPointLocations(loadedInstanceFile); // Stores indexes of first parent's points.

	std::vector<int> childPoints(loadedInstanceFile.dimension, 0);

	int nonCopiedValue;
	int indexOfReplacedPoint;

	for (int i = leftCutoff; i <= rightCutoff; i++) { // Copy points from between the cutoffs.
		childPoints[i] = allCurrentSpecimen[firstParentIndex].getPointByIndex(i);
	}


	for (int i = leftCutoff; i <= rightCutoff; i++) {
		nonCopiedValue = allCurrentSpecimen[secondParentIndex].getPointByIndex(i);
		if (leftCutoff > allCurrentSpecimen[firstParentIndex].pointLocation[nonCopiedValue] // A value hasn't already been copied if it can be found in parent 1 between two cutoffs.
			|| allCurrentSpecimen[firstParentIndex].pointLocation[nonCopiedValue] > rightCutoff) { // Only select a value if it hasn't already been copied.
			indexOfReplacedPoint = findValueInsideParent(i, firstParentIndex, secondParentIndex, leftCutoff, rightCutoff, 0);
			childPoints[indexOfReplacedPoint] = nonCopiedValue;
		}
	}

	for (int i = 1; i < leftCutoff; i++) { // Copy remaining points to the left of crossover range from second parent.
		if (childPoints[i] == 0) {
			childPoints[i] = allCurrentSpecimen[secondParentIndex].getPointByIndex(i);
		}
	}

	for (int i = rightCutoff + 1; i < loadedInstanceFile.dimension; i++) { // Copy remaining points to the right of crossover range from second parent.
		if (childPoints[i] == 0) {
			childPoints[i] = allCurrentSpecimen[secondParentIndex].getPointByIndex(i);
		}
	}

	specimen.setPointOrder(childPoints);
	return specimen;
}

// Return the index where a value resides inside a parent.
int geneticCVRP::findValueInsideParent(int valueIndex, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff, int recursion) {
	int valueInSameSpot = allCurrentSpecimen[firstParentIndex].getPointByIndex(valueIndex); // Value in the same spot in the other parent
	int value2Index = allCurrentSpecimen[secondParentIndex].pointLocation[valueInSameSpot];

	int size = allCurrentSpecimen[firstParentIndex].pointOrder.size();

	if (leftCutoff <= value2Index && value2Index <= rightCutoff) { // If the index is part of the crossover range.
		return findValueInsideParent(value2Index, firstParentIndex, secondParentIndex, leftCutoff, rightCutoff, recursion + 1);
	}
	else return value2Index;
}

// Handle mutation of all offspring in new generation.
void geneticCVRP::mutate() {
	int probability; // Determines whether a mutation will happen or not.
	
	for (int i = 0; i < geneticParameters.numberOfSpecimenInAGeneration; i++) { // For each offspring.
		probability = getRand(0, 99);

		if (probability / 100.0 < geneticParameters.mutationProbability || allCurrentSpecimen[i].obligatoryMutation == true) { // If the specimen is a parent from previous generation, mutation will happen regardless of probability.
			switch (geneticParameters.mutationUsed) { // Select mutation to be used.
			case 's': // Swap mutation.
				mutateSwapOnSpecimen(i); // Mutate the specimen with index i.
				break;

			case 'i': // Invert mutation.
				mutateInvertOnSpecimen(i); // Mutate the specimen with index i.
				break;

			default: 
				break;
			}
		}
	}
}

// Mutation that swaps two random points in a specimen. Repeats multiple times.
void geneticCVRP::mutateSwapOnSpecimen(int specimenIndex) {
	int amountOfMutations = loadedInstanceFile.dimension; // How many times should mutations happen.
	std::pair<int, int> mutationPair; // Indexes of points which will be mutated.

	for (int i = 0; i < amountOfMutations; i++) { // Mutate multiple times.
		mutationPair = getRandUniquePair(1, loadedInstanceFile.dimension - 1); // Randomly select indices of the first and second point. Don't swap first point (depot).
		allCurrentSpecimen[specimenIndex].mutateSwap(loadedInstanceFile, mutationPair);
	}
}

// Mutation that swaps all points between two points in a route. Only one mutation is performed per specimen.
void geneticCVRP::mutateInvertOnSpecimen(int specimenIndex) {
	std::pair<int, int>	mutationPair; // Indexes of points which will be mutated.
	mutationPair = getRandUniquePair(1, loadedInstanceFile.dimension - 1); // Randomly select indices of the first and second point. Don't swap first point (depot).
	allCurrentSpecimen[specimenIndex].mutateInvert(loadedInstanceFile, mutationPair);
}