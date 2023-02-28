#pragma once
#include <string>

// This class serves as a user interface that is used to access other features of the application.
class menu
{
public:
	std::string settingsFileName = "settings.ini"; // Name of the file where earlier experiment file names are stored.
	std::string initialisationFileName = "experiment.txt"; // Name of the file containing experiment input parameters.
	std::string outputFileName = "testResults.txt"; // Name of the file containing experiment output.
	std::string generationResultsFile = "generationResults.txt"; // (In genetic algorithm) Name of the file containing experiment generation output.
	
	char userInput; // Character used as user input to select options from the main menu.
	bool exitProgram = false; // Marks whether the "Exit the program" option has been selected.

	void loadSettings(); // If a settings file exists, load input & output file names.
	void printSettings(); // Print current algorithm parameters.

	void startDialogue(); // Main menu.

	void changeInitialisationFileName(); // Change the name of the initialisation file.
	void changeOutputFileName(); // Change the name of the output file.
	void changeGenerationResultsFileName(); // Change the name of the generation results file.
};