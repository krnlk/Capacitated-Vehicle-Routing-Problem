#include <iostream>
#include <conio.h>
#include <fstream>

#include "menu.h"
#include "environment.h"

// Main menu.
void menu::startDialogue() {
	this->filepath.loadSettingsFile(); // Load initial settings for files.

	while (!exitProgram) { // As long as "Exit the program" option hasn't been selected, show the main menu after one of the options' output finishes.
		// Options dialogue.
		this->printMainMenu();
		this->mainMenuSwitch();
	}
}

// Prints options for main menu.
void menu::printMainMenu()
{
	std::cout << "(1) Start the genetic cVRP algorithm." << std::endl;
	std::cout << "(2) Start the random cVRP algorithm." << std::endl;
	std::cout << "(3) Start the greedy cVRP algorithm." << std::endl;
	std::cout << "(4) Change settings." << std::endl;
	std::cout << "(?) Check current settings." << std::endl;
	std::cout << "(e) Exit the program." << std::endl;

	std::cout << std::endl << std::endl << std::endl; // Spacing after options dialogue.
}

// Read user's input and choose one from the main menu options based on it (or terminate the program).
void menu::mainMenuSwitch()
{
	userInput = _getch(); // Read input from the keyboard without the user needing to press enter.

	switch (userInput) { // Perform an action selected by user input.
	case '1': // Start the genetic cVRP algorithm.
		this->startGeneticCVRP();
		break;

	case '2': // Start the random cVRP algorithm.
		this->startRandomCVRP();
		break;

	case '3': // Start the greedy cVRP algorithm.
		this->startGreedyCVRP();
		break;

	case '4': // Change settings.
		this->changesDialogue();
		break;

	case '?': // Check current settings.
		this->checkSettings();
		break;

	case 'e': // Exit the program.
		exitProgram = true;
		break;

	default: // If the user didn't select any of the options above, show main menu and ask for input again.
		break;
	}

	std::cout << std::endl << std::endl << std::endl; // Spacing between main menu user input and option output.
}

// Start the cVRP experiment using genetic algorithm.
void menu::startGeneticCVRP()
{
	std::cout << "Beginning experiment using genetic algorithm." << std::endl;
	std::cout << "Data will be loaded from " << this->filepath.getInitialisationFileName()
		<< " file in " << this->filepath.getInitialisationPath() << " directory " << std::endl;
	std::cout << "and saved to files " << this->filepath.getOutputFileName() << " in " << this->filepath.getOutputPath()
		<< " directory and " << this->filepath.getGenerationResultsFileName() << " in " << this->filepath.getGenerationResultsPath()
		<< " directory." << std::endl;
	environment.geneticAlgorithmExperiment(this->filepath);
	std::cout << "The experiment has finished." << std::endl;
}

// Start the cVRP experiment using random algorithm.
void menu::startRandomCVRP()
{
	std::cout << "Beginning experiment using random algorithm." << std::endl;
	std::cout << "Data will be loaded from " << this->filepath.getInitialisationFileName()
		<< " file in " << this->filepath.getInitialisationPath() << " directory and saved to files " << this->filepath.getOutputFileName()
		<< " in " << this->filepath.getOutputPath() << "directory and " << this->filepath.getGenerationResultsFileName()
		<< " in " << this->filepath.getGenerationResultsPath() << " directory." << std::endl;
	environment.randomAlgorithmExperiment(this->filepath);
	std::cout << "The experiment has finished." << std::endl;
}

// Start the cVRP experiment using greedy algorithm.
void menu::startGreedyCVRP()
{
	std::cout << "Beginning experiment using greedy algorithm." << std::endl;
	std::cout << "Data will be loaded from " << this->filepath.getInitialisationFileName() << " file in "
		<< this->filepath.getInitialisationPath() << " directory and saved to file " << this->filepath.getOutputFileName()
		<< " in " << this->filepath.getOutputPath() << " directory." << std::endl;
	environment.greedyAlgorithmExperiment(this->filepath);
	std::cout << "The experiment has finished." << std::endl;
}

// Print current file settings.
void menu::checkSettings()
{
	std::cout << "Currently initialisation data is loaded from a file named " << this->filepath.getInitialisationFileName()
		<< " in " << this->filepath.getInitialisationPath() << " directory." << std::endl;
	std::cout << "Experiment results are currently being saved to a file named " << this->filepath.getOutputFileName()
		<< " in " << this->filepath.getOutputPath() << " directory." << std::endl;
	std::cout << "Results for each generation are currently being saved to a file named " << this->filepath.getGenerationResultsFileName()
		<< " in " << this->filepath.getGenerationResultsPath() << " directory." << std::endl;
}

// Menu for changing files.
void menu::changesDialogue()
{
	bool exitChangeSettings = false;

	while (!exitChangeSettings) {
		// Options dialogue.
		this->printChangesMenu();
		this->changesMenuSwitch(exitChangeSettings);
	}
}

// Print the options from menu for changing files.
void menu::printChangesMenu()
{
	// Options dialogue.
	std::cout << "(1) Change the initialisation data path." << std::endl;
	std::cout << "(2) Change the initialisation data file." << std::endl;
	std::cout << "(3) Change the path to instance files." << std::endl;
	std::cout << "(4) Change test results output path." << std::endl;
	std::cout << "(5) Change test results output file." << std::endl;
	std::cout << "(6) Change generation results output path." << std::endl;
	std::cout << "(7) Change generation results output file." << std::endl;
	std::cout << "(8) Back to main menu." << std::endl;
	std::cout << "(?) Check current settings." << std::endl;

	std::cout << std::endl << std::endl << std::endl; // Spacing after options dialogue.
}

// Read user's input and choose one of the options from menu for changing files based on it (or terminate the program).
void menu::changesMenuSwitch(bool& exitChangeSettings)
{
	userInput = _getch(); // Read input from the keyboard without the user needing to press enter.

	switch (userInput) {
	case '1': // Change the initialisation data path.
		this->changeInitialisationPath();
		break;

	case '2': // Change the initialisation data file.
		this->changeIniitalisationFileName();
		break;

	case '3': // Change the path to instance files.
		this->changeInstancePath();
		break;

	case '4': // Change test results output path.
		this->changeOutputPath();
		break;

	case '5': // Change test results output file.
		this->changeOutputFileName();
		break;

	case '6': // Change generation results output path.
		this->changeGenerationResultsPath();
		break;

	case '7': // Change generation results output file.
		this->changeGenerationResultsFileName();
		break;

	case '8': // Back to main menu.
		exitChangeSettings = true;
		break;

	case '?': // Check current settings.
		this->checkSettings();
		break;

	default: // If the user didn't select any of the options above, show main menu and ask for input again.
		break;
	}

	std::cout << std::endl << std::endl << std::endl; // Spacing between main menu user input and option output.
}

// Change the initialisation file path.
void menu::changeInitialisationPath()
{
	std::cout << "Input new path to initialisation file: " << std::endl;
	std::cin >> changeInput;
	this->filepath.setInitialisationPath(changeInput);
	std::cout << "Initialisation file path has been changed to: " << this->filepath.getInitialisationPath() << std::endl;
}

// Change the initialisation file name.
void menu::changeIniitalisationFileName()
{
	std::cout << "Input new initialisation file name: " << std::endl;
	std::cin >> changeInput;
	this->filepath.setInitialisationFileName(changeInput);
	std::cout << "Initialisation file name has been changed to: " << this->filepath.getInitialisationFileName() << std::endl;
}

// Change the path of the instance files.
void menu::changeInstancePath()
{
	std::cout << "Input new path to instance files: " << std::endl;
	std::cin >> changeInput;
	this->filepath.setInstancesPath(changeInput);
	std::cout << "Instance files path has been changed to: " << this->filepath.getInstancesPath() << std::endl;
}

// Change the path of the output files.
void menu::changeOutputPath()
{
	std::cout << "Input new path to test results output: " << std::endl;
	std::cin >> changeInput;
	this->filepath.setOutputPath(changeInput);
	std::cout << "Test results output file path has been changed to: " << this->filepath.getOutputPath() << std::endl;
}

// Change the name of the output file.
void menu::changeOutputFileName()
{
	std::cout << "Input new test results output file name: " << std::endl;
	std::cin >> changeInput;
	this->filepath.setOutputFileName(changeInput);
	std::cout << "Test results output file name has been changed to: " << this->filepath.getOutputFileName() << std::endl;
}

// Change the path of the generation results output file.
void menu::changeGenerationResultsPath()
{
	std::cout << "Input new path to generation results output: " << std::endl;
	std::cin >> changeInput;
	this->filepath.setGenerationResultsPath(changeInput);
	std::cout << "Generation results output path has been changed to: " << this->filepath.getGenerationResultsPath() << std::endl;
}

// Change the name of the generation results output file.
void menu::changeGenerationResultsFileName()
{
	std::cout << "Input new generation results output file name: " << std::endl;
	std::cin >> changeInput;
	this->filepath.setGenerationResultsFileName(changeInput);
	std::cout << "Generation results output file name has been changed to: " << this->filepath.getGenerationResultsFileName() << std::endl;
}