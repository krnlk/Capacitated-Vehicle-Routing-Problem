#pragma once
#include <string>

// This class contains information about directories and file names of various input and output files needed by the application.
// Although there are default settings in place, they are designed to be overriden by settings inside settingsFileName file
// (unless such a file doesn't exist).
class filepath
{
	std::string settingsPath = ""; // Path to the settings file.
	std::string settingsFileName = "settings.ini"; // Name of the file where earlier experiment file names are stored.

	std::string initialisationPath = "Experiments/"; // Path to the experiment initialisation data file.
	std::string initialisationFileName = "experiment.txt"; // Name of the file containing experiment input parameters.

	std::string instancesPath = "Instances/"; // Path to the directory containing cVRP instances.

	std::string outputPath = "Results/"; // Path to the output file.
	std::string outputFileName = "testResults.txt"; // Name of the file containing experiment output.

	std::string generationResultsPath = "Results/GeneticGenerations/"; // (Genetic algorithm) Path to the output file for each generation.
	std::string generationResultsFileName = "generationResults.txt"; // (Genetic algorithm) Name of the file containing experiment generation output.

	void loadSettingsFileSuccess(std::fstream& settingsFile); // Load information from the settings file, assuming a specific format.
	void loadSettingsFileError(); // Create a new settings file, open it, save default settings into it and then close it.

public:
	std::string getSettingsFile() const; // Returns the relative directory of the settings file.

	std::string getInitialisationPath() const; // Returns the relative directory of the initialisation file.
	std::string getInitialisationFileName() const; // Returns the file name of the initialisation file.
	std::string getInitialisationFile() const; // Returns the relative directory and the file name of the initialisation file.

	std::string getInstancesPath() const; // Returns the relative directory where the instance files are located.

	std::string getOutputPath() const; // Returns the relative directory of the output file.
	std::string getOutputFileName() const; // Returns the file name of the output file.
	std::string getOutputFile() const; // Returns the relative directory and the file name of the output file.

	std::string getGenerationResultsPath() const; // Returns the relative directory of the generation results output file.
	std::string getGenerationResultsFileName() const; // Returns the file name of the generation results output file.
	std::string getGenerationResultsFile() const; // Returns the relative directory and the file name of the generation results output file.

	void setInitialisationPath(std::string path);// Sets the relative directory of the initialisation file.
	void setInitialisationFileName(std::string fileName); // Sets the file name of the initialisation file.

	void setInstancesPath(std::string path); // Sets the relative directory where the instance files are located.

	void setOutputPath(std::string path); // Sets the relative directory of the output file.
	void setOutputFileName(std::string fileName); // Sets the file name of the output file.

	void setGenerationResultsPath(std::string path); // Sets the relative directory of the generation results output file.
	void setGenerationResultsFileName(std::string fileName); // Sets the relative directory of the generation results output file.

	void loadSettingsFile(); // Load the paths and names of files needed from a settings file. 
};

