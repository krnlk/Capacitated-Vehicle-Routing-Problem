#pragma once
class Point
{
	int positionX;
	int positionY;
	int demand;

	int getPositionX();
	int getPositionY();
	int getDemand();
	void setPosition(int positionX, int positionY);
	void setDemand(int demand);
};

