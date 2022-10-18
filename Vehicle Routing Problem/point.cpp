#include "point.h"

int Point::getPositionX() {
	return this->positionX;
}

int Point::getPositionY() {
	return this->positionY;
}

int Point::getDemand() {
	return this->demand;
}

void Point::setPosition(int positionX, int positionY) {
	this->positionX = positionX;
	this->positionY = positionY;
}

void Point::setDemand(int demand) {
	this->demand = demand;
}