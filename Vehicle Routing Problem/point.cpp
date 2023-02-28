#include <algorithm>
#include <math.h>
#include <cmath>

#include "point.h"

// Get index of this point.
int point::getIndex() {
	return this->index;
}

// Get X coordinate of this point.
int point::getPositionX() {
	return this->positionX;
}

// Get Y coordinate of this point.
int point::getPositionY() {
	return this->positionY;
}

// Get weight of this point.
int point::getWeight() {
	return this->weight;
}

// Set X and Y coordinates of this point.
void point::setPosition(int positionX, int positionY) {
	this->positionX = positionX;
	this->positionY = positionY;
}

// Set index of this point.
void point::setIndex(int index) {
	this->index = index;
}

// Set weight of this point.
void point::setWeight(int weight) {
	this->weight = weight;
}

// Calculate distance from this point to another point. 
int point::getDistanceToAnotherPoint(point anotherPoint) {
	double distanceX = pow(std::max(this->positionX, anotherPoint.getPositionX()) - std::min(this->positionX, anotherPoint.getPositionX()), 2);
	double distanceY = pow(std::max(this->positionY, anotherPoint.getPositionY()) - std::min(this->positionY, anotherPoint.getPositionY()), 2);

	double totalDistanceBeforeRound = pow(distanceX + distanceY, 0.5);

	// Values after decimal point 0 <= x < 0.5 are rounded down. 
	// Values after decimal point 0.5 <= x < 1.0 are rounded up.
	int totalDistance = std::round(totalDistanceBeforeRound);

	return totalDistance;
}