#pragma once
#include <string>

#include "Filepath.h"
#include "environment.h"

// This class serves as a user interface that is used to access other features of the application.
class menu
{
	filepath filepath; // Used to store data about setting files & paths leading to them.
	environment environment; // Used to store data of each experiment.

	char userInput; // Character used as user input to select options from the menu.
	std::string changeInput; // String used as user input for file names.

	bool exitProgram = false; // Marks whether the "Exit the program" option has been selected.

	void printMainMenu(); // Print the main menu options.
	void mainMenuSwitch(); // Read user's input and choose one from the main menu options based on it (or terminate the program).

	void startGeneticCVRP(); // Start the cVRP experiment using genetic algorithm.
	void startRandomCVRP(); // Start the cVRP experiment using random algorithm.
	void startGreedyCVRP(); // Start the cVRP experiment using greedy algorithm.
	void checkSettings(); // Print current file settings.

	void changesDialogue(); // Menu for changing files.
	void printChangesMenu(); // Print the options of menu for changing files.
	void changesMenuSwitch(bool &exitChangeSettings); // Read user's input and choose one of the options from menu for changing files based on it (or terminate the program).

	void changeInitialisationPath(); // Change the initialisation file path.
	void changeIniitalisationFileName(); // Change the initialisation file name.
	void changeInstancePath(); // Change the path of the instance files.
	void changeOutputPath(); // Change the path of the output files.
	void changeOutputFileName(); // Change the name of the output file.
	void changeGenerationResultsPath(); // Change the path of the generation results output file.
	void changeGenerationResultsFileName(); // Change the name of the generation results output file.

public:
	void startDialogue(); // Main menu.
};