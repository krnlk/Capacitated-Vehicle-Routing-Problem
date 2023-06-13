#pragma once
#include <string>
#include <vector>

#include "point.h"

// This class represents a single instance file, along with all its data.
class instanceFile
{
	std::string fileName; // Name of the instance file.
	int optimalValue; // Best known value of the cVRP solution for this instance file.
	int maxCapacity; // Subroute's maximum capacity for this instance file.
	bool isOpen; // Boolean value representing whether instance file is open or not.

	void loadInstanceDataSuccess(std::ifstream& instanceFile); // Load data from an instance file, assuming a specific file structure.

public:
	int dimension; // Number of points in this instance file (including depot).
	std::vector<point> pointsVector; // Vector of objects represeting instance file points.
	int** distanceBetweenPoints; // Two dimensional array containing distance between any two points in the instance file.

	std::string getFileName() const; // Returns file name of this instance file.
	int getOptimalValue() const; // Returns best known value of an cVRP algorithm for this instance file.
	int getMaxCapacity() const; // Returns subroute's maximum capacity for this instance file.
	bool getIsFileOpen() const; // Returns information about whether instance file is open or not.

	void loadInstanceData(std::string path, std::string filename); // Open an instance file and load its data.

	void clearInstanceData(); // Clear information about last read instance file's points.
	void printInstanceData(); // Print information about this instance file. Mostly used for testing.

	void calculateDistancesBetweenPoints(); // Calculate distances between any two given points in this file and save them in a dynamic, two-dimensional array.
	void dealocateDistancesBetweenPoints(); // Deallocate the memory in two-dimensional array containing distances between any two given points in this file.

	void printDistancesBetweenPoints(); // For testing purposes only.
};

