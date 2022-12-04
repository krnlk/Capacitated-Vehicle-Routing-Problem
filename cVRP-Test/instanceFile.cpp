#include <iostream>
#include <fstream>
#include <string>

#include "instanceFile.h"


instanceFile::~instanceFile() {
	//delete[] this->pointsArray;
}

void instanceFile::loadInstanceData(std::string filename) {
	std::ifstream instanceFile;
	std::string textLine;
	std::string textWord1, textWord2, textWord3;
	int textNumber1, textNumber2, textNumber3;
	point point;

	fileName = filename;

	instanceFile.open(filename);

	if (instanceFile.is_open()) {
			std::getline(instanceFile, textLine); // Skip unneeded info.

			std::getline(instanceFile, textLine);
			if (textLine.find("Optimal value: ") != -1) {
				textLine = textLine.substr(textLine.find("Optimal value: ") + 15); // Trim out beginning of the line to get to optimal value;
				textLine = textLine.erase(textLine.find(")"));
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
				pointsVector[i].setDemand(textNumber2);
				pointsVector[i].setIndex(i);
				//std::cout << pointsVector[i].getDemand() << std::endl;
			}
			instanceFile.close();
	}
	else {
		std::cout << "Error" << std::endl;
	}
	//std::cout << "Instance dimension: " << dimension << std::endl;
}

void instanceFile::clearInstanceData() {
	pointsVector.clear();
}

void instanceFile::saveResultsToFile() {

}

//TODO update
void instanceFile::printInstanceData() {
	std::cout << "Optimal value is: " << optimalValue << std::endl;
	std::cout << "Maximum capacity per route is: " << maxCapacity << std::endl;
	std::cout << "This instance files consists of " << dimension << " points." << std::endl;

	std::cout << "The depot is placed in coordinates (" << pointsVector[0].getPositionX() << ", " << pointsVector[0].getPositionY() 
		<< "). Its demand is: " << pointsVector[0].getDemand() << std::endl << std::endl;

	for (int i = 1; i < dimension; i++) {
		std::cout << "The point number " << i << " is placed in coordinates (" << pointsVector[i].getPositionX() << ", " << pointsVector[i].getPositionY() 
			<< "). Its demand is: " << pointsVector[i].getDemand() << std::endl;
	}
}

void instanceFile::calculateDistancesBetweenPoints() {
	distanceBetweenPoints = new int* [dimension];

	for (int i = 0; i < dimension; i++) {
		distanceBetweenPoints[i] = new int[dimension];

		for (int j = 0; j < dimension; j++) {
			distanceBetweenPoints[i][j] = pointsVector[i].getDistanceToAnotherPoint(pointsVector[j]);
		}
	}
}

void instanceFile::dealocateDistancesBetweenPoints() {
	for (int i = 0; i < dimension; i++) {
		delete[] distanceBetweenPoints[i];
	}

	delete[] distanceBetweenPoints;
}