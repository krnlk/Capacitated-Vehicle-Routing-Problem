#pragma once

// This class represents a single point in an instance file.
class point
{
	int index; // Index of the point.
	int positionX; // X coordinate of the point in instance space. 
	int positionY; // Y coordinate of the point in instance space.
	int weight; // Weight of the point.

	public: 
		int getIndex() const; // Get index of this point.
		int getPositionX() const; // Get X coordinate of this point.
		int getPositionY() const; // Get Y coordinate of this point.
		int getWeight() const; // Get weight of this point.

		void setPosition(int positionX, int positionY); // Set X and Y coordinates of this point.
		void setIndex(int index); // Set index of this point.
		void setWeight(int weight); // Set weight of this point.

		int getDistanceToAnotherPoint(point anotherPoint) const; // Calculate distance from this point to another point. 
};

