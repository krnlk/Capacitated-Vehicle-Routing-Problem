#pragma once
#include <list>

#include "instanceFile.h"

// This class represents a solution in the cVRP in its most basic form.
class cVRPSolution
{
	public:
		std::vector<int> pointOrder; // A vector containing the order of points in this solution.
		int totalCost; // Total cost of a solution.

		std::vector<int> pointLocation; // Contains information about index position of each point in the solution. 
										// For example:
										// pointLocation[4] = 6 
										// means that point number 4 can be found in pointOrder[6].
		bool pointLocationSet = false; // Information about whether points have had their locations assigned in the pointLocation vector.

		int getTotalCost(); // Get total cost of this solution.
		int calculateTotalCost(instanceFile instanceFile); // Calculate the total cost of this solution.
		std::string getPointOrder(); // Get point order of this solution saved to a string variable.

		void setPointOrder(std::vector<int>& pointOrder); // Set point order of this solution.

		void clearPointOrder(); // Clear point order of this solution.
		int getPointByIndex(int index); // Get a point from this solution by index.
	
		void generateRandomRoute(instanceFile instanceFile, std::vector<int>& pointOrder); // Generate a random route by shuffling the order of points around.
		std::vector<int> getPointsOnARoute(instanceFile instanceFile); // Construct a full route (with subroutes).
		void setPointLocations(instanceFile instanceFile); // Set each point's location if it hasn't been set already.
};

