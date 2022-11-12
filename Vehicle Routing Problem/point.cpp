#include "point.h"
#include <algorithm>
#include <math.h>

int point::getIndex() {
	return this->index;
}

int point::getPositionX() {
	return this->positionX;
}

int point::getPositionY() {
	return this->positionY;
}

int point::getDemand() {
	return this->demand;
}

void point::setPosition(int positionX, int positionY) {
	this->positionX = positionX;
	this->positionY = positionY;
}

void point::setIndex(int index) {
	this->index = index;
}

void point::setDemand(int demand) {
	this->demand = demand;
}

int point::getDistanceToAnotherPoint(point anotherPoint) {
	double distanceX = pow(std::max(this->positionX, anotherPoint.getPositionX()) - std::min(this->positionX, anotherPoint.getPositionX()), 2);
	double distanceY = pow(std::max(this->positionY, anotherPoint.getPositionY()) - std::min(this->positionY, anotherPoint.getPositionY()), 2);

	double totalDistanceBeforeRound = pow(distanceX + distanceY, 0.5);
	int totalDistance = totalDistanceBeforeRound;

	return totalDistance;
}