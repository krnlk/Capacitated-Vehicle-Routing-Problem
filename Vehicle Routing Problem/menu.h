#pragma once
#include <string>

#include "Filepath.h"
#include "environment.h"

// This class serves as a user interface that is used to access other features of the application.
class menu
{
	Filepath filepath; // Used to store data about setting files & paths leading to them.
	environment environment; // Used to store data of each experiment.

	char userInput; // Character used as user input to select options from the menu.
	std::string changeInput; //

	bool exitProgram = false; // Marks whether the "Exit the program" option has been selected.

	void printMainMenu(); //
	void mainMenuSwitch(); //

	void startGeneticCVRP(); //
	void startRandomCVRP(); //
	void startGreedyCVRP(); //
	void checkSettings(); //

	void changesDialogue(); // Menu for changing files.
	void printChangesMenu(); //
	void changesMenuSwitch(bool& exitChangeSettings); //

	void changeInitialisationPath(); //
	void changeIniitalisationFileName(); //
	void changeInstancePath(); //
	void changeOutputPath(); //
	void changeOutputFileName(); //
	void changeGenerationResultsPath(); //
	void changeGenerationResultsFileName(); //

public:
	void startDialogue(); // Main menu.
};