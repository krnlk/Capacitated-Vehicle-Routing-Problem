#include <iostream>
#include <fstream>
#include <string>

#include "instanceFile.h"

// Returns file name of this instance file.
std::string instanceFile::getFileName() const
{
	return this->fileName;
}

// Returns best known value of an cVRP algorithm for this instance file.
int instanceFile::getOptimalValue() const
{
	return this->optimalValue;
}

// Returns subroute's maximum capacity for this instance file.
int instanceFile::getMaxCapacity() const
{
	return this->maxCapacity;
}

// Returns information about whether instance file is open or not.
bool instanceFile::getIsFileOpen() const
{
	return this->isOpen;
}

// Open an instance file and load its data.
void instanceFile::loadInstanceData(std::string path, std::string filename) {
	std::ifstream instanceFile;

	fileName = filename;

	instanceFile.open(path + filename);

	if (instanceFile.is_open()) { // If the instance file has been succesfully opened.
		this->isOpen = true;
		this->loadInstanceDataSuccess(instanceFile);
	}
	else { // If the instance file couldn't be found or opened.
		this->isOpen = false;
		std::cout << "Error - couldn't open the instance file " << filename << "." << std::endl;
		std::cout << "Skipping ahead to the next instance file." << std::endl;
	}
}

// Load data from an instance file, assuming a specific file structure.
void instanceFile::loadInstanceDataSuccess(std::ifstream& instanceFile) {
	std::string textLine;
	std::string textWord1, textWord2, textWord3;
	int textNumber1, textNumber2, textNumber3;
	point point;

	std::getline(instanceFile, textLine); // Skip unneeded info.

	std::getline(instanceFile, textLine);
	if (textLine.find("Optimal value: ") != -1) {
		textLine = textLine.substr(textLine.find("Optimal value: ") + 15); // Trim out beginning of the line to get to optimal value;
		if (textLine.find(")") != -1) { 
			textLine = textLine.erase(textLine.find(")")); 
		}
		optimalValue = stoi(textLine); // Convert from string to integer.
	}

	std::getline(instanceFile, textLine); // Skip unneeded info.

	std::getline(instanceFile, textLine);
	textLine = textLine.substr(12); // 12 to remove "DIMENSION : ".
	dimension = stoi(textLine); // Convert from string to integer.

	std::getline(instanceFile, textLine); // Skip unneeded info.

	std::getline(instanceFile, textLine);
	textLine = textLine.substr(11); // 11 to remove "CAPACITY : ".
	maxCapacity = stoi(textLine); // Convert from string to integer.

	std::getline(instanceFile, textLine); // Skip unneeded info.

	pointsVector.clear(); // Clear before filling.

	for (int i = 0; i < dimension; i++) {
		instanceFile >> textNumber1 >> textNumber2 >> textNumber3;
		point.setPosition(textNumber2, textNumber3);
		pointsVector.push_back(point);
	}

	std::getline(instanceFile, textLine); // Finish the previous line.
	std::getline(instanceFile, textLine); // Skip unneeded info.

	for (int i = 0; i < dimension; i++) {
		instanceFile >> textNumber1 >> textNumber2;
		pointsVector[i].setWeight(textNumber2);
		pointsVector[i].setIndex(i);
	}
	instanceFile.close();
}

// Clear information about last read instance file's points.
void instanceFile::clearInstanceData() {
	pointsVector.clear();
}

// Print information about this instance file.
// Mostly used for testing.
void instanceFile::printInstanceData() {
	std::cout << "Optimal value is: " << optimalValue << std::endl;
	std::cout << "Maximum capacity per route is: " << maxCapacity << std::endl;
	std::cout << "This instance files consists of " << dimension << " points." << std::endl;

	std::cout << "The depot is placed in coordinates (" << pointsVector[0].getPositionX() << ", " << pointsVector[0].getPositionY()
		<< "). Its demand is: " << pointsVector[0].getWeight() << std::endl << std::endl;

	for (int i = 1; i < dimension; i++) {
		std::cout << "The point number " << i << " is placed in coordinates (" << pointsVector[i].getPositionX() << ", " << pointsVector[i].getPositionY()
			<< "). Its demand is: " << pointsVector[i].getWeight() << std::endl;
	}
}

// Calculate distances between any two given points in this file and save them in a dynamic, two-dimensional array.
void instanceFile::calculateDistancesBetweenPoints() {
	distanceBetweenPoints = new int* [dimension];

	for (int i = 0; i < dimension; i++) {
		distanceBetweenPoints[i] = new int[dimension];

		for (int j = 0; j < dimension; j++) {
			distanceBetweenPoints[i][j] = pointsVector[i].getDistanceToAnotherPoint(pointsVector[j]);
		}
	}
}

// Deallocate the memory in two-dimensional array containing distances between any two given points in this file.
void instanceFile::dealocateDistancesBetweenPoints() {
	for (int i = 0; i < dimension; i++) {
		delete[] distanceBetweenPoints[i];
	}

	delete[] distanceBetweenPoints;
}

// Print the array of distances between any two given points.
// Testing only.
void instanceFile::printDistancesBetweenPoints() {
	std::cout << std::endl;

	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			std::cout << distanceBetweenPoints[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
