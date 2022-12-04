#include <iostream>
#include <sstream>
#include <algorithm>

#include "geneticCVRP.h"
#include "specimen.h"

geneticCVRP::~geneticCVRP() {
}

void geneticCVRP::setAlgorithmParameters(std::ifstream &experimentFile)
{
	std::string textLine;

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
}

int geneticCVRP::getBestFoundSolutionTotalCost()
{
	return bestFoundSolutionTotalCost;
}

std::string geneticCVRP::getBestFoundSolutionPointOrder() {
	std::stringstream ss;
	for (auto it = bestFoundSolutionPointOrder.begin(); it != bestFoundSolutionPointOrder.end(); it++) {
		if (it != bestFoundSolutionPointOrder.begin()) {
			ss << " ";
		}
		ss << *it;
	}
	return ss.str();
}

void geneticCVRP::setBestSpecimenInfo(instanceFile instanceFile, int bestSpecimenIndex) {
	bestFoundSolutionTotalCost = allCurrentSpecimen[bestSpecimenIndex].getTotalCost();

	for (int i = 0; i < instanceFile.dimension; i++) {
		bestFoundSolutionPointOrder[i] = allCurrentSpecimen[bestSpecimenIndex].getPoint(i);
	}
}

void geneticCVRP::clearInfo() {
	bestFoundSolutionPointOrder.clear();
	bestFoundSolutionTotalCost = INT_MAX;

	allCurrentSpecimen.clear();
	newGenerationOfSpecimen.clear();
}

//
void geneticCVRP::calculateSumOfRanks(){
	sumOfRanks = 0;

	for (int i = 0; i < numberOfSpecimenInAGeneration; i++) {
		sumOfRanks += i + 1;
	}
}

bool compareSpecimen(specimen firstSpecimen, specimen secondSpecimen) {
	return (firstSpecimen.getTotalCost() < secondSpecimen.getTotalCost());
}

// Main algorithm loop.
void geneticCVRP::mainAlgorithmLoop(instanceFile instanceFile, std::ofstream& experimentResultsFile) {
	for (int i = 0; i < instanceFile.dimension; i++) {
		bestFoundSolutionPointOrder.push_back(i); // Placeholder.
	}

	if (selectUsed == 'k') calculateSumOfRanks();

	instanceFile.calculateDistancesBetweenPoints();
	generateInitialSpecimen(instanceFile); // Note: this generation isn't checked for the best specimen.
	analyseNewGeneration(instanceFile, 0, experimentResultsFile); //

	for (int i = 0; i < maxNumberOfGenerations; i++) { 
		if (selectUsed == 'k') std::sort(allCurrentSpecimen.begin(), allCurrentSpecimen.end(), compareSpecimen); //
		createNewOffspring(instanceFile);
		mutate(instanceFile);
		// find best specimen
		analyseNewGeneration(instanceFile, i + 1, experimentResultsFile);
	}

	instanceFile.dealocateDistancesBetweenPoints();
}

//
void geneticCVRP::analyseNewGeneration(instanceFile instanceFile, int generationNumber, std::ofstream& experimentResultsFile) {
	int bestSpecimenIndex; 
	int averageSolutionTotalCost = 0; // Average of total costs of all solution in a particular generation.
	int currentGenerationWorstFoundSolutionTotalCost = 0;
	int bestFoundSolutionTotalCost = INT_MAX; // Total cost of the best solution found (current generation only).
	bool newBestSpecimenFound = false; // Used for tracking if a new best specimen has been found in this generation.

	for (int j = 0; j < numberOfSpecimenInAGeneration; j++) { // Check for a new best solution.
		allCurrentSpecimen[j].calculateTotalCost(instanceFile);
		averageSolutionTotalCost += allCurrentSpecimen[j].getTotalCost();

		if (allCurrentSpecimen[j].getTotalCost() < bestFoundSolutionTotalCost) {
			bestFoundSolutionTotalCost = allCurrentSpecimen[j].getTotalCost();

			if (allCurrentSpecimen[j].getTotalCost() < this->bestFoundSolutionTotalCost) {
				this->bestFoundSolutionTotalCost = allCurrentSpecimen[j].getTotalCost();
				bestSpecimenIndex = j;
				newBestSpecimenFound = true;
			}
		}

		if (allCurrentSpecimen[j].getTotalCost() > currentGenerationWorstFoundSolutionTotalCost) {
			currentGenerationWorstFoundSolutionTotalCost = allCurrentSpecimen[j].getTotalCost();
			if (worstFoundSolutionTotalCost < currentGenerationWorstFoundSolutionTotalCost) {
				worstFoundSolutionTotalCost = currentGenerationWorstFoundSolutionTotalCost;
			}
		}
	}

	if (newBestSpecimenFound) setBestSpecimenInfo(instanceFile, bestSpecimenIndex);

	experimentResultsFile << instanceFile.fileName << ";" << "aaa" << ";" << generationNumber << ";" << 
		bestFoundSolutionTotalCost << ";" << currentGenerationWorstFoundSolutionTotalCost << ";" <<
		averageSolutionTotalCost / numberOfSpecimenInAGeneration << std::endl;
}

// Randomly generate initial specimen.
void geneticCVRP::generateInitialSpecimen(instanceFile instanceFile) {
	std::vector<int> pointOrder;
	specimen specimen;

	for (int i = 0; i < instanceFile.dimension; i++) { // Generate a point order that will be shuffled between subsequent speciment.
		pointOrder.push_back(i);
	}

	for (int i = 0; i < numberOfSpecimenInAGeneration; i++) {
		specimen.generateRandomRoute(instanceFile, pointOrder);
		allCurrentSpecimen.push_back(specimen);
		specimen.clearPointOrder();
	}
}

// Selects crossover based on data loaded from a file.
void geneticCVRP::selectCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex) {
	int leftCutoff, rightCutoff; // Cutoffs are used to determine an allel.

	switch (crossoverUsed) { // Select crossover that's currently used in the experiment.
		case 'c':
			cycleCrossover(instanceFile, offspringIndex, firstParentIndex, secondParentIndex);
			break;

		case 'o':
			leftCutoff = rand() % (instanceFile.dimension - 1) + 1; // Avoid leftCutoff value being 0.
			do {
				rightCutoff = rand() % (instanceFile.dimension - 1) + 1;
			} while (rightCutoff == leftCutoff); // Make sure first and second cutoff are different values. 

			if (leftCutoff > rightCutoff) std::swap(leftCutoff, rightCutoff); // Second cutoff should be a bigger value than the first cutoff.

			newGenerationOfSpecimen.push_back(orderedCrossover(instanceFile, firstParentIndex, secondParentIndex, leftCutoff, rightCutoff));
			newGenerationOfSpecimen.push_back(orderedCrossover(instanceFile, secondParentIndex, firstParentIndex, leftCutoff, rightCutoff));
			break;

		case 'p':
			leftCutoff = rand() % (instanceFile.dimension - 1) + 1; // Avoid leftCutoff value being 0.

			do {
				rightCutoff = rand() % (instanceFile.dimension - 1) + 1;
			} while (rightCutoff == leftCutoff); // Make sure first and second cutoff are different values. 

			if (leftCutoff > rightCutoff) std::swap(leftCutoff, rightCutoff); // Second cutoff should be a bigger value than the first cutoff.

			newGenerationOfSpecimen.push_back(partiallyMappedCrossover(instanceFile, firstParentIndex, secondParentIndex, leftCutoff, rightCutoff));
			newGenerationOfSpecimen.push_back(partiallyMappedCrossover(instanceFile, secondParentIndex, firstParentIndex, leftCutoff, rightCutoff));
			break;

		default:
			break;
	}
}

// This crossover generates two offspring from two parents by finding cycles in parents, then copying cycle 1 from parent 1, cycle 2 from parent 2 etc.
void geneticCVRP::cycleCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex) {
	std::vector<int> cycles; // Vector used for marking which point belongs to which cycle.
	int pointsVisited = 1; // Depot is skipped.
	int currentPoint = 1; // Index of the point that's used to start a new cycle.
	int nextPoint; // Next point in the cycle.
	int firstPoint; // First point in a cycle - reaching it again will complete the cycle.
	int currentCycle = 1; // Number of the current cycle.

	int* firstParentPointLocation = new int[instanceFile.dimension]; // Stores indexes of first parent's visited points.

	for (int i = 0; i < instanceFile.dimension; i++) {
		cycles.push_back(-1); // -1 means that this point doesn't belong to any cycle (yet).
		firstParentPointLocation[allCurrentSpecimen[firstParentIndex].getPoint(i)] = i; // Point i can be found at an i spot in the array.  
	}

	while (pointsVisited < instanceFile.dimension) { // Find all cycles. Don't visit the depot.
		while (cycles[currentPoint] != -1) currentPoint++; // Increment currentPoint until the new current point hasn't been a part of any previous cycle.
		cycles[currentPoint] = currentCycle;
		nextPoint = allCurrentSpecimen[secondParentIndex].getPoint(currentPoint); // Drop down. //TODO describe better
		pointsVisited++;

		while (nextPoint != allCurrentSpecimen[firstParentIndex].getPoint(currentPoint)) { //
			cycles[firstParentPointLocation[nextPoint]] = currentCycle; //
			nextPoint = allCurrentSpecimen[secondParentIndex].getPoint(firstParentPointLocation[nextPoint]); // Drop down. //TODO describe better
			pointsVisited++;
		}

		currentCycle++;
	}

	specimen child1, child2;
	std::vector<int> child1Points;
	std::vector<int> child2Points;

	child1Points.push_back(0);
	child2Points.push_back(0);

	for (int i = 1; i < instanceFile.dimension; i++) {
		if (cycles[i] % 2 == 1) { // Odd cycles.
			child1Points.push_back(allCurrentSpecimen[firstParentIndex].getPoint(i));
			child2Points.push_back(allCurrentSpecimen[secondParentIndex].getPoint(i));
		}
		else { // Even cycles.
			child1Points.push_back(allCurrentSpecimen[secondParentIndex].getPoint(i));
			child2Points.push_back(allCurrentSpecimen[firstParentIndex].getPoint(i));
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
specimen geneticCVRP::orderedCrossover(instanceFile instanceFile, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff) {
	specimen specimen; //TODO implement saving pointLocations
	std::vector<int> childPoints(instanceFile.dimension, 0); //TODO potential memory leak, check later
	int* pointLocation = new int[instanceFile.dimension]; // Stores indexes of visited points.
	int childPoint;

	childPoints[0] = 0; // Set depot.	

	for (int i = 0; i < instanceFile.dimension; i++) {
		pointLocation[i] = -1;
	}

	for (int i = leftCutoff; i <= rightCutoff; i++) { // Copy points in the cutoff from parent 1.
		childPoint = allCurrentSpecimen[firstParentIndex].getPoint(i);
		childPoints[i] = allCurrentSpecimen[firstParentIndex].getPoint(i);
		pointLocation[childPoint] = i; // Remember location of this point.
	}

	int childIndex = rightCutoff + 1; // Index where the next point will be inserted in a child.
	int parentIndex = rightCutoff + 1;

	if (childIndex >= instanceFile.dimension) childIndex = 1;
	if (parentIndex >= instanceFile.dimension) parentIndex = 1;
	
	for (int i = 0; i < instanceFile.dimension && childIndex != leftCutoff; i++) { // Ignore depot.
		if (pointLocation[allCurrentSpecimen[secondParentIndex].getPoint(parentIndex)] == -1) // If point doesn't belong in the cutoff.
		{
			childPoints[childIndex] = allCurrentSpecimen[secondParentIndex].getPoint(parentIndex);
			childIndex++;
			if (childIndex >= instanceFile.dimension) childIndex = 1; // Start adding points at the start.
		}

		parentIndex++;
		if (parentIndex >= instanceFile.dimension) parentIndex = 1;
	}

	specimen.setPointOrder(childPoints); //
	
	delete[] pointLocation;
	return specimen;
}

// This function returns the index where 
// parentIndex refers to the index of the parent whose valueIndex originates from.
int geneticCVRP::findValueInsideParent(int valueIndex, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff, int recursion) {
	int valueInSameSpot = allCurrentSpecimen[firstParentIndex].getPoint(valueIndex); // Value in the same spot in the other parent
	int value2Index = allCurrentSpecimen[secondParentIndex].pointLocation[valueInSameSpot];

	int size = allCurrentSpecimen[firstParentIndex].pointOrder.size();

	if (leftCutoff <= value2Index && value2Index <= rightCutoff) { // If the index is part of the crossover range.
		return findValueInsideParent(value2Index, firstParentIndex, secondParentIndex, leftCutoff, rightCutoff, recursion + 1);
	}
	else return value2Index;
}

// This crossover generates two offspring from two parents by selecting a random crossover range in both parents, swapping those points,
// defining a mapping relationship (to avoid repeating points in offspring) and then mapping remaining points in offspring to make sure nothing repeats.
specimen geneticCVRP::partiallyMappedCrossover(instanceFile instanceFile, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff) {
	specimen specimen;
	allCurrentSpecimen[firstParentIndex].setPointLocations(instanceFile); // Stores indexes of first parent's points.
	allCurrentSpecimen[secondParentIndex].setPointLocations(instanceFile); // Stores indexes of first parent's points.

	std::vector<int> childPoints(instanceFile.dimension, 0); //TODO potential memory leak, check later

	int nonCopiedValue;
	int indexOfReplacedPoint;

	for (int i = leftCutoff; i <= rightCutoff; i++) { // Copy points from between the cutoffs.
		childPoints[i] = allCurrentSpecimen[firstParentIndex].getPoint(i); 
	}

	
	for (int i = leftCutoff; i <= rightCutoff; i++) {
		nonCopiedValue = allCurrentSpecimen[secondParentIndex].getPoint(i);
		if (leftCutoff > allCurrentSpecimen[firstParentIndex].pointLocation[nonCopiedValue] // A value hasn't already been copied if it can be found in parent 1 between two cutoffs.
			|| allCurrentSpecimen[firstParentIndex].pointLocation[nonCopiedValue] > rightCutoff) { // Only select a value if it hasn't already been copied.
			indexOfReplacedPoint = findValueInsideParent(i, firstParentIndex, secondParentIndex, leftCutoff, rightCutoff, 0);
			childPoints[indexOfReplacedPoint] = nonCopiedValue;

		}
	}

	for (int i = 1; i < leftCutoff; i++) { // Copy remaining points to the left of crossover range from second parent.
		if (childPoints[i] == 0) {
			childPoints[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
		}
	}

	for (int i = rightCutoff + 1; i < instanceFile.dimension; i++) { // Copy remaining points to the right of crossover range from second parent.
		if (childPoints[i] == 0) {
			childPoints[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
		}
	}

	specimen.setPointOrder(childPoints);
	return specimen;
	/*
	specimen firstChild, secondChild; //
	std::vector<int> child1Points; //TODO potential memory leak, check later
	std::vector<int> child2Points; //TODO potential memory leak, check later

	int* firstParentPointLocation = new int[instanceFile.dimension]; // Stores indexes of first parent's points.
	int* secondParentPointLocation = new int[instanceFile.dimension]; // Stores indexes of second parent's points.

	int indexOfReplacedPoint;
	int replacedPointValue;

	for (int i = 0; i < instanceFile.dimension; i++) {
		firstParentPointLocation[allCurrentSpecimen[firstParentIndex].getPoint(i)] = i; // Point can be found at an i spot in the array.  
		secondParentPointLocation[allCurrentSpecimen[secondParentIndex].getPoint(i)] = i; // Point can be found at an i spot in the array.

		child1Points.push_back(allCurrentSpecimen[firstParentIndex].getPoint(i));
		child2Points.push_back(allCurrentSpecimen[secondParentIndex].getPoint(i));
	}

	for (int i = leftCutoff; i <= rightCutoff; i++) {
		if (firstParentPointLocation[allCurrentSpecimen[secondParentIndex].getPoint(i)] < leftCutoff // If the point isn't between the cutoffs.
			|| rightCutoff < firstParentPointLocation[allCurrentSpecimen[secondParentIndex].getPoint(i)]) // Legalise offspring.

		child1Points[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
		child2Points[i] = allCurrentSpecimen[firstParentIndex].getPoint(i);
	}

	firstChild.setPointOrder(child1Points);
	secondChild.setPointOrder(child2Points);

	newGenerationOfSpecimen.push_back(firstChild);
	newGenerationOfSpecimen.push_back(secondChild);
	*/
}

void geneticCVRP::mutate(instanceFile instanceFile) {
	int probability; //
	
	for (int i = 0; i < numberOfSpecimenInAGeneration; i++) { // For each offspring.
		probability = rand() % 100;

		if (probability / 100.0 < mutationProbability || allCurrentSpecimen[i].obligatoryMutation == true) { //
			switch (mutationUsed) {
			case 's':
				mutateSwap(instanceFile, i); // Mutate the specimen with index i.
				break;

			case 'i':
				mutateInvert(instanceFile, i);
				break;

			default: 
				break;
			}
		}

		else {} // Otherwise do nothing.
	}

}

// Swap two random points in a specimen.
void geneticCVRP::mutateSwap(instanceFile instanceFile, int specimenIndex) {
	int amountOfMutations = instanceFile.dimension; // Mutate up to (10% of dimension of the instance file) times.

	for (int i = 0; i < amountOfMutations; i++) {
		allCurrentSpecimen[specimenIndex].mutateSwap(instanceFile);
	}
}

// Swap all points between two points in a route. Only one mutation is performed per specimen.
void geneticCVRP::mutateInvert(instanceFile instanceFile, int specimenIndex) {
		allCurrentSpecimen[specimenIndex].mutateInvert(instanceFile);
}

//TODO implement me
std::pair<int, int> geneticCVRP::selection(instanceFile instanceFile) {
	std::vector<int> specimenRanks;
	std::pair<int, int> parentIndexes;

	double firstProbability, secondProbability;
	double totalSpecimenValueSum;

	switch (selectUsed) {
	case 'k':
		parentIndexes.first = rankSelection(instanceFile);
		do {
			parentIndexes.second = rankSelection(instanceFile);
		} while (parentIndexes.first == parentIndexes.second);
		return parentIndexes;
		break;

	case 'r':
		return randomSelection(instanceFile);
		break;

	case 's':
		do {
			firstProbability = (rand() % 10000) / 10000.0;
			secondProbability = firstProbability >= 0.5 ? firstProbability - 0.5 : firstProbability + 0.5;
			if (firstProbability > secondProbability) std::swap(firstProbability, secondProbability);

			totalSpecimenValueSum = 0.0;
			parentIndexes.first = rouletteWheelSelection(instanceFile, 0, totalSpecimenValueSum, firstProbability);
			parentIndexes.second = rouletteWheelSelection(instanceFile, parentIndexes.first + 1, totalSpecimenValueSum, secondProbability);
		} while (parentIndexes.first == parentIndexes.second);

		return parentIndexes;
		break;

	case 't':
		parentIndexes.first = tournamentSelection(instanceFile);
		do {
			parentIndexes.second = tournamentSelection(instanceFile);
		} while (parentIndexes.first == parentIndexes.second);
		return parentIndexes;
		break;

	case 'w':
		firstProbability = (rand() % 10000) / 10000.0;

		totalSpecimenValueSum = 0.0;

		parentIndexes.first = rouletteWheelSelection(instanceFile, 0, totalSpecimenValueSum, firstProbability);
		do {
			firstProbability = (rand() % 10000) / 10000.0;
			totalSpecimenValueSum = 0.0;
			parentIndexes.second = rouletteWheelSelection(instanceFile, 0, totalSpecimenValueSum, firstProbability);
		} while (parentIndexes.first == parentIndexes.second);
		return parentIndexes;
		break;

	default: 
		break;
	}

}

std::pair<int, int> geneticCVRP::randomSelection(instanceFile instanceFile) {
	std::pair<int, int> parentIndexes;

	parentIndexes.first = rand() % numberOfSpecimenInAGeneration; // Randomly select first parent.
	do {
		parentIndexes.second = rand() % numberOfSpecimenInAGeneration; // Randomly select second parent.
	} while (parentIndexes.first == parentIndexes.second); // Make sure parents are different specimen.

	return parentIndexes;
}

//
int geneticCVRP::rankSelection(instanceFile instanceFile) {
	int parentIndex = 0;
	double totalRankSum = 0.0;

	double probability = rand() % 10000;
	probability /= 10000.0;

	while (parentIndex < numberOfSpecimenInAGeneration) {
		totalRankSum += (numberOfSpecimenInAGeneration - parentIndex) / (double) sumOfRanks; // Due to previous sorting
		if (totalRankSum > probability) {
			return parentIndex;
		}
		parentIndex++;
	}
}

//
std::pair<int, int> geneticCVRP::stochasticUniversalSamplingSelection(instanceFile instanceFile) {
	std::pair<int, int> parentIndex;
	parentIndex.first = 0;
	double totalSpecimenValueSum = 0.0;
	double firstProbability = rand() % 10000;
	firstProbability /= 10000.0;

	double secondProbability = firstProbability >= 0.5 ? firstProbability - 0.5 : firstProbability + 0.5;

	if (firstProbability > secondProbability) std::swap(firstProbability, secondProbability);

	while (totalSpecimenValueSum <= firstProbability && parentIndex.first < instanceFile.dimension) {
		std::cout << parentIndex.first << " ";
		totalSpecimenValueSum += (worstFoundSolutionTotalCost - allCurrentSpecimen[parentIndex.first].getTotalCost()) / (double) fitnessFunction;
		parentIndex.first++;
	}
	std::cout << std::endl;
	parentIndex.second = parentIndex.first;

	while (totalSpecimenValueSum <= secondProbability && parentIndex.second < instanceFile.dimension) {
		std::cout << parentIndex.second << " ";
		totalSpecimenValueSum += (worstFoundSolutionTotalCost - allCurrentSpecimen[parentIndex.first].getTotalCost()) / (double) fitnessFunction;
		parentIndex.second++;
	}
	std::cout << std::endl;

	std::cout << "<--- " << parentIndex.first << "    " << parentIndex.second << " --->" << std::endl;
	return parentIndex;
}

//
int geneticCVRP::tournamentSelection(instanceFile instanceFile) {
	int parentIndex;
	int numberOfTournamentParticipants = numberOfSpecimenInAGeneration / 10;
	int randomParticipantIndex;

	int bestParticipantValue;

	if (numberOfTournamentParticipants <= 1) {
		numberOfTournamentParticipants = 2;
	}

	// First member of the tournament
	parentIndex = rand() % numberOfSpecimenInAGeneration;
	bestParticipantValue = allCurrentSpecimen[parentIndex].getTotalCost();

	for (int i = 1; i < numberOfTournamentParticipants; i++) {
		randomParticipantIndex = rand() % numberOfSpecimenInAGeneration;
		if (allCurrentSpecimen[randomParticipantIndex].getTotalCost() < bestParticipantValue) {
			bestParticipantValue = allCurrentSpecimen[randomParticipantIndex].getTotalCost();
			parentIndex = randomParticipantIndex;
		}
	}

	return parentIndex;
}

// TODO potential bugs?
int geneticCVRP::rouletteWheelSelection(instanceFile instanceFile, int parentIndex, double &totalSpecimenValueSum, double probability) {
	/*
	int parentIndex = 0;
	double totalSpecimenValueSum = 0.0;
	double probability = rand() % 10000;
	*/
	//probability /= 10000.0;

	//std::cout << parentIndex << " " << totalSpecimenValueSum << " " << probability << std::endl;

	while (parentIndex < numberOfSpecimenInAGeneration) {
		totalSpecimenValueSum += (worstFoundSolutionTotalCost - allCurrentSpecimen[parentIndex].getTotalCost()) / (double) fitnessFunction; // Casting to double to avoid dividing integer by integer.
		if (totalSpecimenValueSum > probability) {
			return parentIndex;
		}
		parentIndex++;
	}

	// In case of rounding errors where of totalSpecimenValueSum doesn't equal 1.0 after adding all the specimen.
	return parentIndex;
}

// Generates a new generation of specimen by either taking crossover of two randomly selected parents as two children
// or by taking two of the randomly selected parents as the children outright.
// This process is repeated until there are numberOfSpecimenInAGeneration specimen in the new generation.
void geneticCVRP::createNewOffspring(instanceFile instanceFile) {

	int probability = rand() % 100; 

	newGenerationOfSpecimen.clear(); // Clear the values.

	//
	if (selectUsed == 'w' || selectUsed == 's') {
		calculateFitnessFunction();
	}

	for (int i = 0; i < numberOfSpecimenInAGeneration / 2; i++) // Both using crossover and just taking the parents generate 2 offspring.
	{
		//TODO tournament here instead?
		std::pair<int, int> parentIndexes = selection(instanceFile);

		probability = rand() % 100;

		if (probability / 100.0 < crossoverProbability) { // Produce two offspring from two parents using crossover.
			//::cout << parentIndexes.first << " " << parentIndexes.second << std::endl;
			selectCrossover(instanceFile, i, parentIndexes.first, parentIndexes.second);
		}
		else { // Take two parents without modifying them.
			newGenerationOfSpecimen.push_back(allCurrentSpecimen[parentIndexes.first]);
			newGenerationOfSpecimen.push_back(allCurrentSpecimen[parentIndexes.second]);

			//TODO fix this
			
			newGenerationOfSpecimen[2 * i].obligatoryMutation = true;
			newGenerationOfSpecimen[2 * i].pointLocationSet = false;
			newGenerationOfSpecimen[2 * i + 1].obligatoryMutation = true;
			newGenerationOfSpecimen[2 * i + 1].pointLocationSet = false;
		}
	}

	allCurrentSpecimen = newGenerationOfSpecimen; // Replace it with the new generation by copying pointer's value.
}

void geneticCVRP::calculateFitnessFunction() {
	fitnessFunction = 0;

	for (int i = 0; i < allCurrentSpecimen.size(); i++) {
		fitnessFunction += worstFoundSolutionTotalCost - allCurrentSpecimen[i].getTotalCost(); // Get the difference between worst solution point cost and this specimen's cost.
	}
}