#include "algorithm_1.h"
#include "specimen.h"

algorithm_1::~algorithm_1() {
	delete bestFoundSolutionPointOrder;
	delete allCurrentSpecimen;
	delete newGenerationOfSpecimen;
}

void algorithm_1::setAlgorithmParameters(std::ifstream &experimentFile)
{
	std::string textLine;

	std::getline(experimentFile, textLine); // Skip unneeded info.

	std::getline(experimentFile, textLine);
	crossoverUsed = textLine.substr(17); // Trim out beginning of the line to get to crossover.

	std::getline(experimentFile, textLine);
	mutationUsed = textLine.substr(16); // Trim out beginning of the line to get to mutation.

	std::getline(experimentFile, textLine);
	crossoverProbability = stoi(textLine.substr(24)); // Trim out beginning of the line to get to crossover probability and convert it to integer.

	std::getline(experimentFile, textLine);
	mutationProbability = stoi(textLine.substr(23)); // Trim out beginning of the line to get to mutation probability and convert it to integer.

	std::getline(experimentFile, textLine);
	numberOfSpecimenInAGeneration = stoi(textLine.substr(36)); // Trim out beginning of the line to get to number of specimen in a generation and convert it to integer.

	std::getline(experimentFile, textLine);
	maxNumberOfGenerations = stoi(textLine.substr(24)); // Trim out beginning of the line to get to number of generations and convert it to integer.

	std::getline(experimentFile, textLine); // Skip unneeded info.
}

int algorithm_1::getBestFoundSolutionTotalCost()
{
	return bestFoundSolutionTotalCost;
}

int* algorithm_1::getBestFoundSolutionPointOrder()
{
	return bestFoundSolutionPointOrder;
}

void algorithm_1::calculateDistancesBetweenPoints(instanceFile instanceFile) {
	distanceBetweenPoints = new int* [instanceFile.dimension];

	for (int i = 0; i < instanceFile.dimension; i++) {
		distanceBetweenPoints[i] = new int [instanceFile.dimension];

		for (int j = 0; j < instanceFile.dimension; j++) {
			distanceBetweenPoints[i][j] = instanceFile.pointsArray[i].getDistanceToAnotherPoint(instanceFile.pointsArray[j]);
		}
	}
}

void algorithm_1::dealocateDistancesBetweenPoints(instanceFile instanceFile) {
	for (int i = 0; i < instanceFile.dimension; i++) {
			delete[] distanceBetweenPoints[i];
	}

	delete[] distanceBetweenPoints;
}

// Main algorithm loop.
void algorithm_1::mainAlgorithmLoop(instanceFile instanceFile) {
	int bestSpecimenIndex;
	bestFoundSolutionPointOrder = new int[instanceFile.dimension];
	bool newBestSpecimenFound;

	generateInitialSpecimen(instanceFile);

	for (int i = 0; i < maxNumberOfGenerations; i++) {
		newBestSpecimenFound = false;

		createNewOffspring(instanceFile);
		mutate(instanceFile);
		//TODO tournament too?
		// find best specimen

		for (int i = 0; i < numberOfSpecimenInAGeneration; i++) { // Check for a new best solution.
			if (allCurrentSpecimen[i].getTotalCost() < bestFoundSolutionTotalCost) {
				bestFoundSolutionTotalCost = allCurrentSpecimen[i].getTotalCost();
				bestSpecimenIndex = i;
				newBestSpecimenFound = true;
			}
		}
		
		//if (newBestSpecimenFound) setBestSpecimenInfo(bestSpecimenIndex);
	}

	delete bestFoundSolutionPointOrder;
}

// Randomly generate initial specimen.
void algorithm_1::generateInitialSpecimen(instanceFile instanceFile) {
	int* pointOrder = new int[instanceFile.dimension];

	for (int i = 0; i < instanceFile.dimension; i++) {
		pointOrder[i] = i;
	}

	allCurrentSpecimen = new specimen[numberOfSpecimenInAGeneration];

	for (int i = 0; i < numberOfSpecimenInAGeneration; i++) {
		allCurrentSpecimen[i].generateRandomRoute(instanceFile, pointOrder);
	}
}

//TODO: make a better crossover algorithm later, this is temp
// Select two parents A and B. Select a random subroute in each parent and remove those points in the other parent.
// Point by point, insert the missing points into spots with minimal cost.
/*
void algorithm_1::crossover(instanceFile instanceFile) {
	for (int i = 0; i < amountOfSpecimenInAGeneration; i++) {
		//sort stuff here later?
		//allSpecimen[i].
	}

	int firstParentIndex, secondParentIndex; // Parents which will be used for crossover.
	specimen* newGenerationOfSpecimen = new specimen[amountOfSpecimenInAGeneration];

	for (int i = 0; i < amountOfSpecimenInAGeneration / 2; i++) { // Halved, because each pair of parents produces two offspring.
		firstParentIndex = rand() % amountOfSpecimenInAGeneration; // Randomly select first parent.
		secondParentIndex = rand() % amountOfSpecimenInAGeneration; // Randomly select second parent.

		newGenerationOfSpecimen[i] = createOffspring(instanceFile, firstParentIndex, secondParentIndex);
		newGenerationOfSpecimen[i + 1] = createOffspring(instanceFile, secondParentIndex, firstParentIndex);
	}

	allCurrentSpecimen = newGenerationOfSpecimen; // Update the offspring to be the new generation.
	delete newGenerationOfSpecimen; // Dealocate unused memory.
}
*/

// This crossover generates two offspring from two parents by randomly selecting a route in each parent,
// then removing those points from the other parent. After that, removed points are readded by checking all possible positions
// for the best spot and inserting each point in its respective spot. 
void algorithm_1::crossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex) {
	newGenerationOfSpecimen[offspringIndex] = createCrossoverOffspring(instanceFile, firstParentIndex, secondParentIndex);
	newGenerationOfSpecimen[offspringIndex + 1] = createCrossoverOffspring(instanceFile, secondParentIndex, firstParentIndex);
}

// This crossover generates two offspring from two parents by finding cycles in parents, then copying cycle 1 from parent 1, cycle 2 from parent 2 etc.
void algorithm_1::cycleCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex) {
	newGenerationOfSpecimen[offspringIndex] = createCrossoverOffspring(instanceFile, firstParentIndex, secondParentIndex);
	newGenerationOfSpecimen[offspringIndex + 1] = createCrossoverOffspring(instanceFile, secondParentIndex, firstParentIndex);
}

// This crossover generates two offspring from two parents by selecting a range of points in one parent and copying it over to the child in the same order,
// while the remaining points in child are filled out by inserting non-repeating points from parent 2 in order.
void algorithm_1::orderedCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex) {
	newGenerationOfSpecimen[offspringIndex] = createCrossoverOffspring(instanceFile, firstParentIndex, secondParentIndex);
	newGenerationOfSpecimen[offspringIndex + 1] = createCrossoverOffspring(instanceFile, secondParentIndex, firstParentIndex);
}

// This crossover generates two offspring from two parents by selecting a random crossover range in both parents, swapping those points,
// defining a mapping relationship (to avoid repeating points in offspring) and then mapping remaining points in offspring to make sure nothing repeats.
void algorithm_1::partiallyMappedCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex) {
	newGenerationOfSpecimen[offspringIndex] = createCrossoverOffspring(instanceFile, firstParentIndex, secondParentIndex);
	newGenerationOfSpecimen[offspringIndex + 1] = createCrossoverOffspring(instanceFile, secondParentIndex, firstParentIndex);
}

// Create a single offspring from two parents by selecting a random subroute in the second parent and removing those points in the first parent.
specimen algorithm_1::createCrossoverOffspring(instanceFile instanceFile, int firstParentIndex, int secondParentIndex) {
	specimen offspring(allCurrentSpecimen[firstParentIndex]); // Create an offspring that starts off as a deep copy of the first parent.
	int randomRouteIndex = rand() % allCurrentSpecimen[secondParentIndex].getNumberOfSubroutes(); // Select a random subroute from the second parent.

	route deletedSubroute = allCurrentSpecimen[secondParentIndex].getSubroute(randomRouteIndex); // Select a subroute that should have its points removed.

	int amountOfPointsLeftToDelete = deletedSubroute.getSize();
	int* deletedPoints = new int[amountOfPointsLeftToDelete]; // Add points from a subroute to an array
	deletedSubroute.copyPoints(deletedPoints);

	offspring.removePoints(instanceFile, amountOfPointsLeftToDelete, deletedPoints);
	offspring.addPoint(instanceFile, amountOfPointsLeftToDelete, deletedPoints);

	// TODO: Make sure to check if this offspring's totalCost is lower than the best found one so far.
	delete deletedPoints;
	return offspring;
}


void algorithm_1::mutate(instanceFile instanceFile) {
	int probability; // 
	
	for (int i = 0; i < numberOfSpecimenInAGeneration; i++) { // For each offspring.
		probability = rand() % 100;

		if (probability / 100.0 < mutationProbability) { //TODO select mutation here based on config files
			mutateSwap(instanceFile, i); // Mutate the specimen with index i.
			//mutateInvert(instanceFile, i);
		}

		else {} // Otherwise do nothing.
	}

}

// Swap two random points in a specimen.
void algorithm_1::mutateSwap(instanceFile instanceFile, int specimenIndex) {
	int amountOfMutations = numberOfSpecimenInAGeneration; //TODO subject to change

	for (int i = 0; i < amountOfMutations; i++) { // Mutate a specimen with index specimenIndex i times.
		allCurrentSpecimen[specimenIndex].mutateSwap(instanceFile);
	}
}

// Swap all points between two points in a route. Only one mutation is performed per specimen.
void algorithm_1::mutateInvert(instanceFile instanceFile, int specimenIndex) {
		allCurrentSpecimen[specimenIndex].mutateInvert(instanceFile);
}

void algorithm_1::tournament() {

}

void algorithm_1::createNewOffspring(instanceFile instanceFile) {
	int firstParentIndex = -1, secondParentIndex = -1; // Indexes of the two parents.
	int probability = rand() % 100;
	newGenerationOfSpecimen = new specimen[numberOfSpecimenInAGeneration];

	for (int i = 0; i < numberOfSpecimenInAGeneration / 2; i++) // Both mutation and crossover each generate 2 offspring.
	{
		firstParentIndex = rand() % numberOfSpecimenInAGeneration; // Randomly select first parent;
		secondParentIndex = rand() % numberOfSpecimenInAGeneration; // Randomly select second parent.

		while (firstParentIndex == secondParentIndex) secondParentIndex = rand() % numberOfSpecimenInAGeneration; // Make sure parents are different specimen;

		if (probability / 100.0 < crossoverProbability) { // Produce two offspring from two parents using crossover.
			crossover(instanceFile, i, firstParentIndex, secondParentIndex); 
		}
		else { // Take two parents without modifying them. TODO: maybe fix shallow copy?
			newGenerationOfSpecimen[i] = allCurrentSpecimen[firstParentIndex]; 
			newGenerationOfSpecimen[i + 1] = allCurrentSpecimen[secondParentIndex];
		}
	}
}