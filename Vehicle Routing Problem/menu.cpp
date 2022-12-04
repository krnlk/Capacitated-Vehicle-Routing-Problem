#include <iostream>
#include <conio.h>
#include <fstream>

#include "menu.h"
#include "environment.h"

// If a settings file exists, load the name of the file with the initialisation info 
// (as well as the name of the output file).
void menu::loadSettings() {
	std::ifstream settingsFile;

	settingsFile.open(settingsFileName);
	if (settingsFile) { // If file exists.
		settingsFile >> initialisationFileName >> outputFileName >> generationResultsFile;
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
		std::cout << "(5) Change test results output file name." << std::endl;
		std::cout << "(6) Change generation results output file name." << std::endl;
		std::cout << "(e) Exit the program." << std::endl;

		std::cout << std::endl << std::endl << std::endl;

		//std::cin >> userInput;
		userInput = _getch();

		switch (userInput) {
		case '1':
			std::cout << "Beginning experiment. Data will be loaded from " << initialisationFileName << " file and saved to files "
				<< outputFileName << " and " << generationResultsFile << std::endl;
			environment.beginExperiment(initialisationFileName, outputFileName, generationResultsFile);
			std::cout << "The experiment has finished." << std::endl;
			break;

		case '2':
			std::cout << "Not yet implemented." << std::endl;
			break;

		case '3':
			std::cout << "Currently initialisation data is loaded from a file named " << initialisationFileName << "." << std::endl;
			std::cout << "Experiment results are currently being saved to a file named " << outputFileName << "." << std::endl;
			std::cout << "Results for each generation are currently being saved to a file named " << generationResultsFile << "." << std::endl;
			break;

		case '4':
			changeInitialisationFileName();
			break;

		case '5':
			changeOutputFileName();
			break;

		case '6':
			changeGenerationResultsFileName();
			break;

		case 'e':
			exitProgram = true;
			break;

		default:
			break;
		}

		std::cout << std::endl << std::endl << std::endl;
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

// Changes the name of the generation results file.
void menu::changeGenerationResultsFileName()
{
	std::cout << "What should be the file name of the generation results output file?" << std::endl;
	std::cin >> generationResultsFile;
	std::cout << "Generation results output file name has been changed to: " << generationResultsFile << std::endl;
}