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
	crossoverUsed = textLine.substr(17)[0]; // Trim out beginning of the line to get to crossover.

	std::getline(experimentFile, textLine);
	mutationUsed = textLine.substr(16)[0]; // Trim out beginning of the line to get to mutation.

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

void algorithm_1::setBestSpecimenInfo(instanceFile instanceFile, int bestSpecimenIndex) {
	bestFoundSolutionTotalCost = allCurrentSpecimen[bestSpecimenIndex].getTotalCost();

	for (int i = 0; i < instanceFile.dimension; i++) {
		bestFoundSolutionPointOrder[i] = allCurrentSpecimen[bestSpecimenIndex].getPoint(i);
	}
}

// Main algorithm loop.
void algorithm_1::mainAlgorithmLoop(instanceFile instanceFile) {
	int bestSpecimenIndex;
	bestFoundSolutionPointOrder = new int[instanceFile.dimension];
	bool newBestSpecimenFound;

	instanceFile.calculateDistancesBetweenPoints();
	generateInitialSpecimen(instanceFile); // Note: this generation isn't checked for the best specimen.

	for (int i = 0; i < maxNumberOfGenerations; i++) { 
		newBestSpecimenFound = false; // Used for tracking if a new best specimen has been found in this generation.

		createNewOffspring(instanceFile);
		mutate(instanceFile);
		//TODO tournament too?
		// find best specimen

		for (int i = 0; i < numberOfSpecimenInAGeneration; i++) { // Check for a new best solution.
			allCurrentSpecimen[i].calculateTotalCost(instanceFile);

			if (allCurrentSpecimen[i].getTotalCost() < bestFoundSolutionTotalCost) {
				bestFoundSolutionTotalCost = allCurrentSpecimen[i].getTotalCost();
				bestSpecimenIndex = i;
				newBestSpecimenFound = true;
			}
		}
		
		if (newBestSpecimenFound) setBestSpecimenInfo(instanceFile, bestSpecimenIndex);
	}

	delete bestFoundSolutionPointOrder;
	instanceFile.dealocateDistancesBetweenPoints();
}

// Randomly generate initial specimen.
void algorithm_1::generateInitialSpecimen(instanceFile instanceFile) {
	int* pointOrder = new int[instanceFile.dimension];

	for (int i = 0; i < instanceFile.dimension; i++) { // Generate a point order that will be shuffled between subsequent speciment.
		pointOrder[i] = i;
	}

	allCurrentSpecimen = new specimen[numberOfSpecimenInAGeneration];

	for (int i = 0; i < numberOfSpecimenInAGeneration; i++) {
		allCurrentSpecimen[i].generateRandomRoute(instanceFile, pointOrder);
	}

	delete[] pointOrder;
}

// Selects crossover based on data loaded from a file.
void algorithm_1::selectCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex) {
	int leftCutoff, rightCutoff; // Cutoffs are used to determine an allel.

	switch (crossoverUsed) { // Select crossover that's currently used in the experiment.
		case 'c':
			cycleCrossover(instanceFile, offspringIndex, firstParentIndex, secondParentIndex);
			break;

		case 'o':

			leftCutoff = rand() % instanceFile.dimension;
			do {
				rightCutoff = rand() % instanceFile.dimension;
			} while (rightCutoff == leftCutoff); // Make sure first and second cutoff are different values. 

			if (leftCutoff > rightCutoff) std::swap(leftCutoff, rightCutoff); // Second cutoff should be a bigger value than the first cutoff.

			newGenerationOfSpecimen[offspringIndex] = orderedCrossover(instanceFile, firstParentIndex, secondParentIndex, leftCutoff, rightCutoff);
			newGenerationOfSpecimen[offspringIndex + 1] = orderedCrossover(instanceFile, secondParentIndex, firstParentIndex, leftCutoff, rightCutoff);
			break;

		case 'p':
			leftCutoff = rand() % instanceFile.dimension;
			do {
				rightCutoff = rand() % instanceFile.dimension;
			} while (rightCutoff == leftCutoff); // Make sure first and second cutoff are different values. 

			if (leftCutoff > rightCutoff) std::swap(leftCutoff, rightCutoff); // Second cutoff should be a bigger value than the first cutoff.

			partiallyMappedCrossover(instanceFile, offspringIndex, firstParentIndex, secondParentIndex, leftCutoff, rightCutoff);
			break;

		default:
			break;
	}
}

// This crossover generates two offspring from two parents by randomly selecting a route in each parent,
// then removing those points from the other parent. After that, removed points are readded by checking all possible positions
// for the best spot and inserting each point in its respective spot. 
/*
void algorithm_1::crossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex) {
	newGenerationOfSpecimen[offspringIndex] = createCrossoverOffspring(instanceFile, firstParentIndex, secondParentIndex);
	newGenerationOfSpecimen[offspringIndex + 1] = createCrossoverOffspring(instanceFile, secondParentIndex, firstParentIndex);
}
*/

// This crossover generates two offspring from two parents by finding cycles in parents, then copying cycle 1 from parent 1, cycle 2 from parent 2 etc.
void algorithm_1::cycleCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex) {
	int* cycles = new int[instanceFile.dimension]; // Array used for marking which point belongs to which cycle.
	int pointsVisited = 0;
	int currentPoint = 0; // Index of the point that's used to start a new cycle.
	int nextPoint; // Next point in the cycle.
	int firstPoint; // First point in a cycle - reaching it again will complete the cycle.
	int currentCycle = 1; // Number of the current cycle.

	int* firstParentPointLocation = new int[instanceFile.dimension]; // Stores indexes of first parent's visited points.

	for (int i = 0; i < instanceFile.dimension; i++) {
		cycles[i] = -1; // -1 means that this point doesn't belong to any cycle (yet).
		firstParentPointLocation[allCurrentSpecimen[firstParentIndex].getPoint(i)] = i; // Point can be found at an i spot in the array.  
	}

	while (pointsVisited < instanceFile.dimension) { // Find all cycles.
		while (cycles[currentPoint] == -1) currentPoint++; // Increment currentPoint until the new current point hasn't been a part of any previous cycle.
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
	int* child1Points = new int[instanceFile.dimension]; //TODO potential memory leak, check later
	int* child2Points = new int[instanceFile.dimension]; //TODO potential memory leak, check later

	for (int i = 0; i < instanceFile.dimension; i++) {
		if (cycles[i] % 2 == 1) { // Odd cycles.
			child1Points[i] = allCurrentSpecimen[firstParentIndex].getPoint(i);
			child2Points[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
		}
		else { // Even cycles.
			child1Points[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
			child2Points[i] = allCurrentSpecimen[firstParentIndex].getPoint(i);
		}
	}

	child1.setPointOrder(child1Points);
	child2.setPointOrder(child2Points);

	newGenerationOfSpecimen[offspringIndex] = child1;
	newGenerationOfSpecimen[offspringIndex + 1] = child2;

	delete[] cycles;
	delete[] firstParentPointLocation;
}

// This crossover generates two offspring from two parents by selecting a range of points in one parent and copying it over to the child in the same order,
// while the remaining points in child are filled out by inserting non-repeating points from parent 2 in order.
specimen algorithm_1::orderedCrossover(instanceFile instanceFile, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff) {
	specimen specimen;
	int* childPoints = new int[instanceFile.dimension]; //TODO potential memory leak, check later
	int* pointLocation = new int[instanceFile.dimension]; // Stores indexes of visited points.

	for (int i = 0; i < instanceFile.dimension; i++) {
		pointLocation[i] = -1;
	}

	for (int i = leftCutoff; i <= rightCutoff; i++) { // Copy points in the cutoff from parent 1.
		childPoints[i] = allCurrentSpecimen[firstParentIndex].getPoint(i); 
		pointLocation[childPoints[i]] = i; // Remember location of this point.
	}

	int childIndex = rightCutoff + 1; // Index where the next point will be inserted in a child.
	int parentIndex = rightCutoff + 1;

	for (int i = 0; i < instanceFile.dimension - rightCutoff + leftCutoff; ) { // 
		if (pointLocation[allCurrentSpecimen[firstParentIndex].getPoint(parentIndex)] == -1) // If
		{
			childPoints[childIndex] = allCurrentSpecimen[secondParentIndex].getPoint(parentIndex);
			childIndex++;
			if (childIndex <= instanceFile.dimension) childIndex = 1; // Start adding points at the start.
		}

		parentIndex++;
		if (parentIndex < instanceFile.dimension) parentIndex = 1;
	}

	specimen.setPointOrder(childPoints); //

	delete[] pointLocation;
	return specimen;
}

// This crossover generates two offspring from two parents by selecting a random crossover range in both parents, swapping those points,
// defining a mapping relationship (to avoid repeating points in offspring) and then mapping remaining points in offspring to make sure nothing repeats.
void algorithm_1::partiallyMappedCrossover(instanceFile instanceFile, int offspringIndex, int firstParentIndex, int secondParentIndex, int leftCutoff, int rightCutoff) {
	specimen firstChild, secondChild; //
	int* child1Points = new int[instanceFile.dimension]; //TODO potential memory leak, check later
	int* child2Points = new int[instanceFile.dimension]; //TODO potential memory leak, check later

	/*
	int* mapping = new int[rightCutoff - leftCutoff + 1];
	int* parent1Mapping = new int[instanceFile.dimension];
	int* parent2Mapping = new int[instanceFile.dimension];

	for (int i = 0; i < leftCutoff; i++) { // Before first cutoff.
		child1Points[i] = allCurrentSpecimen[firstParentIndex].getPoint(i);
		child2Points[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
		parent1Mapping[i] = -1;
		parent2Mapping[i] = -1;
	}

	for (int i = leftCutoff; i <= rightCutoff; i++) { // Between first and second cutoff.
		child1Points[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
		child2Points[i] = allCurrentSpecimen[firstParentIndex].getPoint(i);
		parent1Mapping[i] = -1;
		parent2Mapping[i] = -1;
	}

	for (int i = rightCutoff + 1; i < instanceFile.dimension; i++) { // After second cutoff.
		child1Points[i] = allCurrentSpecimen[firstParentIndex].getPoint(i);
		child2Points[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
		parent1Mapping[i] = -1;
		parent2Mapping[i] = -1;
	}

	for (int i = leftCutoff; i <= rightCutoff; i++) { // Swap mappings to avoid repeating points.
		parent1Mapping[allCurrentSpecimen[secondParentIndex].getPoint(i)] = allCurrentSpecimen[firstParentIndex].getPoint(i);
		parent1Mapping[allCurrentSpecimen[firstParentIndex].getPoint(i)] = allCurrentSpecimen[secondParentIndex].getPoint(i);
	}

	for (int i = 0; i < leftCutoff; i++) { // Before first cutoff.
		//TODO legalise mapping by hand
		//TODO maybe better with array of locations?
	}

	for (int i = rightCutoff + 1; i < instanceFile.dimension; i++) { // After second cutoff.

	}

	firstChild.setPointOrder(child1Points); //
	secondChild.setPointOrder(child2Points); //
	*/

	int* firstParentPointLocation = new int[instanceFile.dimension]; // Stores indexes of first parent's points.
	int* secondParentPointLocation = new int[instanceFile.dimension]; // Stores indexes of second parent's points.

	for (int i = 0; i < instanceFile.dimension; i++) {
		firstParentPointLocation[allCurrentSpecimen[firstParentIndex].getPoint(i)] = i; // Point can be found at an i spot in the array.  
		secondParentPointLocation[allCurrentSpecimen[secondParentIndex].getPoint(i)] = i; // Point can be found at an i spot in the array.

		child1Points[i] = allCurrentSpecimen[firstParentIndex].getPoint(i);
		child2Points[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
	}

	for (int i = leftCutoff; i <= rightCutoff; i++) {
		child1Points[i] = allCurrentSpecimen[secondParentIndex].getPoint(i); // Exchange genetic information.
		child2Points[i] = allCurrentSpecimen[firstParentIndex].getPoint(i); 

		if (secondParentPointLocation[allCurrentSpecimen[secondParentIndex].getPoint(i)] < leftCutoff // If the point isn't between the cutoffs.
			|| rightCutoff > secondParentPointLocation[allCurrentSpecimen[secondParentIndex].getPoint(i)]) // Legalise offspring.
		{
			secondParentPointLocation[allCurrentSpecimen[secondParentIndex].getPoint(i)] = secondParentPointLocation[allCurrentSpecimen[firstParentIndex].getPoint(i)]; // Change where that point can be found.
			child2Points[secondParentPointLocation[secondParentPointLocation[allCurrentSpecimen[firstParentIndex].getPoint(i)]] = allCurrentSpecimen[firstParentIndex].getPoint(i)]; // 

			firstParentPointLocation[allCurrentSpecimen[firstParentIndex].getPoint(i)] = firstParentPointLocation[allCurrentSpecimen[secondParentIndex].getPoint(i)]; // Change where that point can be found.
			child1Points[firstParentPointLocation[firstParentPointLocation[allCurrentSpecimen[secondParentIndex].getPoint(i)]] = allCurrentSpecimen[secondParentIndex].getPoint(i)]; // 
		} 
		else { // Offspring are already legal if a point inside cutoff in parent 1 also exists inside cutoff in parent 2.
		}

		child1Points[i] = allCurrentSpecimen[secondParentIndex].getPoint(i);
		child2Points[i] = allCurrentSpecimen[firstParentIndex].getPoint(i);
	}

	firstChild.setPointOrder(child1Points);
	secondChild.setPointOrder(child2Points);

	newGenerationOfSpecimen[offspringIndex] = firstChild;
	newGenerationOfSpecimen[offspringIndex + 1] = secondChild;

	/*
	delete[] mapping;
	delete[] parent1Mapping;
	delete[] parent2Mapping;
	*/
}

// Create a single offspring from two parents by selecting a random subroute in the second parent and removing those points in the first parent.
/*
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
*/

void algorithm_1::mutate(instanceFile instanceFile) {
	int probability; // 
	
	for (int i = 0; i < numberOfSpecimenInAGeneration; i++) { // For each offspring.
		probability = rand() % 100;

		if (probability / 100.0 < mutationProbability) { //TODO select mutation here based on config files
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
void algorithm_1::mutateSwap(instanceFile instanceFile, int specimenIndex) {
	int amountOfMutations = instanceFile.dimension; // Mutate up to (10% of dimension of the instance file) times.

	for (int i = 0; i < amountOfMutations; i++) {
		allCurrentSpecimen[specimenIndex].mutateSwap(instanceFile);
	}
}

// Swap all points between two points in a route. Only one mutation is performed per specimen.
void algorithm_1::mutateInvert(instanceFile instanceFile, int specimenIndex) {
		allCurrentSpecimen[specimenIndex].mutateInvert(instanceFile);
}

//TODO implement me
void algorithm_1::tournament() {

}

// Generates a new generation of specimen by either taking crossover of two randomly selected parents as two children
// or by taking two of the randomly selected parents as the children outright.
// This process is repeated until there are numberOfSpecimenInAGeneration specimen in the new generation.
void algorithm_1::createNewOffspring(instanceFile instanceFile) {
	int firstParentIndex = -1, secondParentIndex = -1; // Indexes of the two parents.
	int probability = rand() % 100; 

	newGenerationOfSpecimen = new specimen[numberOfSpecimenInAGeneration];

	for (int i = 0; i < numberOfSpecimenInAGeneration / 2; i++) // Both using crossover and just taking the parents generate 2 offspring.
	{
		firstParentIndex = rand() % numberOfSpecimenInAGeneration; // Randomly select first parent;
		secondParentIndex = rand() % numberOfSpecimenInAGeneration; // Randomly select second parent.

		while (firstParentIndex == secondParentIndex) secondParentIndex = rand() % numberOfSpecimenInAGeneration; // Make sure parents are different specimen;

		if (probability / 100.0 < crossoverProbability) { // Produce two offspring from two parents using crossover.
			selectCrossover(instanceFile, i, firstParentIndex, secondParentIndex);
		}
		else { // Take two parents without modifying them. TODO: maybe fix shallow copy?
			newGenerationOfSpecimen[i] = allCurrentSpecimen[firstParentIndex]; 
			newGenerationOfSpecimen[i + 1] = allCurrentSpecimen[secondParentIndex];
		}
	}

	delete[] allCurrentSpecimen; // Delete old generation of specimen.
	allCurrentSpecimen = newGenerationOfSpecimen; // Replace it with the new generation by copying pointer's value.
}