#pragma once
#include <string>

#include "Filepath.h"

// This class serves as a user interface that is used to access other features of the application.
class menu
{
public:
	Filepath filepath;
	char userInput; // Character used as user input to select options from the menu.
	bool exitProgram = false; // Marks whether the "Exit the program" option has been selected.

	void loadSettings(); // If a settings file exists, load input & output file names.

	void startDialogue(); // Main menu.
	void changesDialogue(); // Menu for changing files.
};