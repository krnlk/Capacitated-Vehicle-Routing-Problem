#pragma once
#include <string>

// This class serves as a user interface that can access other features of the application.
class menu
{
public:
	std::string settingsFileName = "settings.ini";
	std::string initialisationFileName = "experiment.txt";
	std::string outputFileName = "testResults.txt";
	char userInput;
	bool exitProgram = false;

	void loadSettings();
	void printSettings();
	void startDialogue();
	void changeInitialisationFileName();
	void changeOutputFileName();
};