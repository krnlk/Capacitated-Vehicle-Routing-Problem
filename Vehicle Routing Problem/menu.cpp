#include <iostream>
#include <conio.h>
#include <fstream>

#include "menu.h"
#include "environment.h"

// If a settings file exists, load the name of the file with the initialisation data
// and the names of the output files.
void menu::loadSettings() {
	std::ifstream settingsFile;

	settingsFile.open(settingsFileName); // Attempt to open a file.

	if (settingsFile) { // If file exists.
		settingsFile >> initialisationFileName >> outputFileName >> generationResultsFile; // Load output file names.
	}

	// Go to main menu.
	startDialogue();
}

// Print current algorithm parameters.
void menu::printSettings()
{
}

// Main menu.
void menu::startDialogue() {
	environment environment; // Used to store data of each experiment.

	while (!exitProgram) { // As long as "Exit the program" option hasn't been selected, show the main menu after one of the options' output finishes.
		// Options dialogue.
		std::cout << "(1) Start the genetic cVRP algorithm." << std::endl;
		std::cout << "(2) Check initialisation & output files." << std::endl;
		std::cout << "(3) Change the initialisation data file." << std::endl;
		std::cout << "(4) Change test results output file name." << std::endl;
		std::cout << "(5) Change generation results output file name." << std::endl;
		std::cout << "(6) Start the random cVRP algorithm." << std::endl;
		std::cout << "(7) Start the greedy cVRP algorithm." << std::endl;
		std::cout << "(e) Exit the program." << std::endl;

		std::cout << std::endl << std::endl << std::endl; // Spacing after options dialogue.

		userInput = _getch(); // Read input from the keyboard without the user needing to press enter.

		switch (userInput) { // Perform an action selected by user input.
		case '1': // Start the genetic cVRP algorithm.
			std::cout << "Beginning experiment using genetic algorithm. Data will be loaded from " << initialisationFileName << " file and saved to files "
				<< outputFileName << " and " << generationResultsFile << std::endl;
			environment.geneticAlgorithmExperiment(initialisationFileName, outputFileName, generationResultsFile);
			std::cout << "The experiment has finished." << std::endl;
			break;

		case '2': // Check initialisation & output files.
			std::cout << "Currently initialisation data is loaded from a file named " << initialisationFileName << "." << std::endl;
			std::cout << "Experiment results are currently being saved to a file named " << outputFileName << "." << std::endl;
			std::cout << "Results for each generation are currently being saved to a file named " << generationResultsFile << "." << std::endl;
			break;

		case '3': // Change the initialisation data file.
			changeInitialisationFileName();
			break;

		case '4': // Change test results output file name.
			changeOutputFileName();
			break;

		case '5': // Change generation results output file name.
			changeGenerationResultsFileName();
			break;

		case '6': // Start the random cVRP algorithm.
			std::cout << "Beginning experiment using random algorithm. Data will be loaded from " << initialisationFileName << " file and saved to files "
				<< outputFileName << " and " << generationResultsFile << std::endl;
			environment.randomAlgorithmExperiment(initialisationFileName, outputFileName, generationResultsFile);
			std::cout << "The experiment has finished." << std::endl;
			break;

		case '7': // Start the greedy cVRP algorithm.
			std::cout << "Beginning experiment using greedy algorithm. Data will be loaded from " << initialisationFileName << " file and saved to files "
				<< outputFileName << "." << std::endl;
			environment.greedyAlgorithmExperiment(initialisationFileName, outputFileName);
			std::cout << "The experiment has finished." << std::endl;
			break;

		case 'e': // Exit the program.
			exitProgram = true;
			break;

		default: // If the user didn't select any of the options above, show main menu and ask for input again.
			break;
		}

		std::cout << std::endl << std::endl << std::endl; // Spacing between main menu user input and option output.
	}
}

// Change the name of the initialisation file (where instances and algorithm parameters will be loaded from).
void menu::changeInitialisationFileName()
{
	std::cout << "What should be the new initialization file's name?" << std::endl;
	std::cin >> initialisationFileName;
	std::cout << "Initialization file has been changed to: " << initialisationFileName << std::endl;
}

// Change the name of the output file.
void menu::changeOutputFileName()
{
	std::cout << "What should be the file name of the output file?" << std::endl;
	std::cin >> outputFileName;
	std::cout << "Output file name has been changed to: " << outputFileName << std::endl;
}

// Change the name of the generation results file.
void menu::changeGenerationResultsFileName()
{
	std::cout << "What should be the file name of the generation results output file?" << std::endl;
	std::cin >> generationResultsFile;
	std::cout << "Generation results output file name has been changed to: " << generationResultsFile << std::endl;
}