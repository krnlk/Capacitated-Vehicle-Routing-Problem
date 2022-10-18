#include "instanceFile.h"
#include <iostream>
#include <fstream>
#include <string>

void instanceFile::loadInstanceData() {
	std::ifstream instanceFile;
	std::string textLine;
	std::string textWord1, textWord2, textWord3;

	instanceFile.open("cos.txt");

	if (instanceFile.is_open()) {
			std::getline(instanceFile, textLine); // Skip unneeded info.

			std::getline(instanceFile, textLine);
			//trim stuff to get optimal value here


			std::getline(instanceFile, textLine); // Skip unneeded info.

			std::getline(instanceFile, textLine);
			//get info about dimensions
			//declare dimension array here

			std::getline(instanceFile, textLine); // Skip unneeded info.

			std::getline(instanceFile, textLine);
			//get info about capacity

			std::getline(instanceFile, textLine); // Skip unneeded info.

			for (int i = 0; i < this->dimension; i++) {
				instanceFile >> textWord1 >> textWord2 >> textWord3;
				this->points[textWord1].setPosition(textWord2, textWord3);
			}

			std::getline(instanceFile, textLine); // Skip unneeded info.

			for (int i = 0; i < this->dimension; i++) {
				instanceFile >> textWord1 >> textWord2;
				this->points[textWord1].setDemand(textWord2);
			}
	}
	else {
		std::cout << "Error" << std::endl;
	}
}

void instanceFile::saveResultsToFile() {

}