#pragma once

// This class represents a single point in an instance file. The point with index 0 is always agreed to represent a depot.
class point
{
	int index;
	int positionX;
	int positionY;
	int demand;
	bool alreadyVisited;

public: 
	int getIndex();
	int getPositionX();
	int getPositionY();
	int getDemand();
	void setPosition(int positionX, int positionY);
	void setIndex(int index);
	void setDemand(int demand);
	int getDistanceToAnotherPoint(point anotherPoint);
};

