#include <iostream>
#include <fstream>

#include "menu.h"
#include "environment.h"

// If a settings file exists, load the name of the file with the initialisation info 
// (as well as the name of the output file).
void menu::loadSettings() {
	std::ifstream settingsFile;

	settingsFile.open(settingsFileName);
	if (settingsFile) { // If file exists.
		settingsFile >> initialisationFileName >> outputFileName;
	}

	startDialogue();
}

// Prints current algorithm parameters.
void menu::printSettings()
{
}

// Main menu.
void menu::startDialogue() {
	environment environment; // Used to store data of each experiment.

	while (!exitProgram) {
		std::cout << "(1) Start the cVRP algorithm." << std::endl;
		std::cout << "(2) Check the program parameters." << std::endl;
		std::cout << "(3) Check initialisation & output files." << std::endl;
		std::cout << "(4) Change the initialisation data file." << std::endl;
		std::cout << "(5) Change output file name." << std::endl;
		std::cout << "(e) Exit the program." << std::endl;

		std::cin >> userInput;

		switch (userInput) {
		case '1':
			environment.beginExperiment(initialisationFileName, outputFileName);
			break;

		case '2':
			std::cout << "Not yet implemented." << std::endl;
			break;

		case '3':
			std::cout << "Currently initialisation data is loaded from a file named " << initialisationFileName << "." << std::endl;
			std::cout << "Experiment results are currently being saved to a file named " << outputFileName << "." << std::endl;
			break;

		case '4':
			changeInitialisationFileName();
			break;

		case '5':
			changeOutputFileName();
			break;

		case 'e':
			exitProgram = true;
			break;

		default:
			break;
		}
	}
}

// Changes the name of the initialisation file (where instances and algorithm parameters will be loaded from).
void menu::changeInitialisationFileName()
{
	std::cout << "What should be the new initialization file's name?" << std::endl;
	std::cin >> initialisationFileName;
	std::cout << "Initialization file has been changed to: " << initialisationFileName << std::endl;
}

// Changes the name of the output file.
void menu::changeOutputFileName()
{
	std::cout << "What should be the file name of the output file?" << std::endl;
	std::cin >> outputFileName;
	std::cout << "Output file name has been changed to: " << outputFileName << std::endl;
}

