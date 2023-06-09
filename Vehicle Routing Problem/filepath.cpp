#include <iostream>
#include <fstream>

#include "Filepath.h"

// Returns the relative directory and the file name of the settings file.
std::string filepath::getSettingsFile() const
{
	return this->settingsPath + this->settingsFileName;
}

// Returns the relative directory of the initialisation file.
std::string filepath::getInitialisationPath() const
{
	return this->initialisationPath;
}

// Returns the file name of the initialisation file.
std::string filepath::getInitialisationFileName() const
{
	return this->initialisationFileName;
}

// Returns the relative directory and the file name of the initialisation file.
std::string filepath::getInitialisationFile() const
{
	return this->initialisationPath + this->initialisationFileName;
}

// Returns the relative directory where the instance files are located.
std::string filepath::getInstancesPath() const
{
	return this->instancesPath;
}

// Returns the relative directory of the output file.
std::string filepath::getOutputPath() const
{
	return this->outputPath;
}

// Returns the file name of the output file.
std::string filepath::getOutputFileName() const
{
	return this->outputFileName;
}

// Returns the relative directory and the file name of the output file.
std::string filepath::getOutputFile() const
{
	return this->outputPath + this->outputFileName;
}

// Returns the relative directory of the generation results output file.
std::string filepath::getGenerationResultsPath() const
{
	return this->generationResultsPath;
}

// Returns the file name of the generation results output file.
std::string filepath::getGenerationResultsFileName() const
{
	return this->generationResultsFileName;
}

// Returns the relative directory and the file name of the generation results output file.
std::string filepath::getGenerationResultsFile() const
{
	return this->generationResultsPath + this->generationResultsFileName;
}

// Sets the relative directory of the initialisation file.
void filepath::setInitialisationPath(std::string path)
{
	this->initialisationPath = path;
}

// Sets the file name of the initialisation file.
void filepath::setInitialisationFileName(std::string fileName)
{
	this->initialisationFileName = fileName;
}

// Sets the relative directory where the instance files are located.
void filepath::setInstancesPath(std::string path)
{
	this->instancesPath = path;
}

// Sets the relative directory of the output file.
void filepath::setOutputPath(std::string path)
{
	this->outputPath = path;
}

// Sets the file name of the output file.
void filepath::setOutputFileName(std::string fileName)
{
	this->outputFileName = fileName;
}

// Sets the relative directory of the generation results output file.
void filepath::setGenerationResultsPath(std::string path)
{
	this->generationResultsPath = path;
}

// Sets the relative directory of the generation results output file.
void filepath::setGenerationResultsFileName(std::string fileName)
{
	this->generationResultsFileName = fileName;
}

// Load the paths and names of files needed from a settings file. 
// If no such file exists, create it and instantiate it with default settings found in this class.
void filepath::loadSettingsFile()
{
	std::fstream settingsFile;

	settingsFile.open(this->getSettingsFile());

	if (settingsFile.is_open()) { // If a settings file has been succesfully opened.
		this->loadSettingsFileSuccess(settingsFile);
	}
	else { // If a settings file doesn't exist or couldn't be opened, create a new one, open it and save it with default settings.
		this->loadSettingsFileError();
	}
}

// Load information from the settings file, assuming a specific format.
void filepath::loadSettingsFileSuccess(std::fstream& settingsFile) {
	std::getline(settingsFile, this->initialisationPath);
	std::getline(settingsFile, this->initialisationFileName);
	std::getline(settingsFile, this->instancesPath);
	std::getline(settingsFile, this->outputPath);
	std::getline(settingsFile, this->outputFileName);
	std::getline(settingsFile, this->generationResultsPath);
	std::getline(settingsFile, this->generationResultsFileName);

	this->initialisationPath = this->initialisationPath.substr(22);
	this->initialisationFileName = this->initialisationFileName.substr(27); // Trim out beginning of the line to get to value.
	this->instancesPath = instancesPath.substr(17); // Trim out beginning of the line to get to value.
	this->outputPath = outputPath.substr(14); // Trim out beginning of the line to get to value.
	this->outputFileName = outputFileName.substr(19); // Trim out beginning of the line to get to value.
	this->generationResultsPath = generationResultsPath.substr(26); // Trim out beginning of the line to get to value.
	this->generationResultsFileName = generationResultsFileName.substr(31); // Trim out beginning of the line to get to value.

	settingsFile.close();
}

// Create a new settings file, open it, save default settings into it and then close it.
void filepath::loadSettingsFileError() {
	std::ofstream newSettingsFile; // Create a new settings file.
	newSettingsFile.open(this->getSettingsFile());

	newSettingsFile << "INITIALISATION_PATH : " + this->initialisationPath << std::endl
		<< "INITIALISATION_FILE_NAME : " + this->initialisationFileName << std::endl
		<< "INSTANCES_PATH : " + this->instancesPath << std::endl
		<< "OUTPUT_PATH : " + this->outputPath << std::endl
		<< "OUTPUT_FILE_NAME : " + this->outputFileName << std::endl
		<< "GENERATION_RESULTS_PATH : " + this->generationResultsPath << std::endl
		<< "GENERATION_RESULTS_FILE_NAME : " + this->generationResultsFileName << std::endl;

	newSettingsFile.close();
}
