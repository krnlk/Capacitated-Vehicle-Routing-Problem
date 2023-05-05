#include <iostream>
#include <fstream>

#include "Filepath.h"

std::string Filepath::getSettingsFile()
{
	return this->settingsPath + this->settingsFileName;
}

std::string Filepath::getInitialisationPath()
{
	return this->initialisationPath;
}

std::string Filepath::getInitialisationFileName()
{
	return this->initialisationFileName;
}

std::string Filepath::getInitialisationFile()
{
	return this->initialisationPath + this->initialisationFileName;
}

std::string Filepath::getInstancesPath()
{
	return this->instancesPath;
}

std::string Filepath::getOutputPath()
{
	return this->outputPath;
}

std::string Filepath::getOutputFileName()
{
	return this->outputFileName;
}

std::string Filepath::getOutputFile()
{
	return this->outputPath + this->outputFileName;
}

std::string Filepath::getGenerationResultsPath()
{
	return this->generationResultsPath;
}

std::string Filepath::getGenerationResultsFileName()
{
	return this->generationResultsFileName;
}

std::string Filepath::getGenerationResultsFile()
{
	return this->generationResultsPath + this->generationResultsFileName;
}

void Filepath::setInitialisationPath(std::string path)
{
	this->initialisationPath = path;
}

void Filepath::setInitialisationFileName(std::string fileName)
{
	this->initialisationFileName = fileName;
}

void Filepath::setInstancesPath(std::string path)
{
	this->instancesPath = path;
}

void Filepath::setOutputPath(std::string path)
{
	this->outputPath = path;
}

void Filepath::setOutputFileName(std::string fileName)
{
	this->outputFileName = fileName;
}

void Filepath::setGenerationResultsPath(std::string path)
{
	this->generationResultsPath = path;
}

void Filepath::setGenerationResultsFileName(std::string fileName)
{
	this->generationResultsFileName = fileName;
}

// Load the paths and names of files needed from a settings file. 
// If no such file exists, create it and instantiate it with default settings found in this class.
void Filepath::loadSettingsFile()
{
	std::fstream settingsFile;

	settingsFile.open(this->getSettingsFile());

	if (settingsFile.is_open()) {
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
	else { // If a settings file doesn't exist or couldn't be opened, create a new one, open it and save it with default settings.
		std::ofstream newSettingsFile;
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
}
