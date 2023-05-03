#pragma once
#include <string>

class Filepath
{
public:
	std::string settingsPath = ""; // Path to the settings file.
	std::string settingsFileName = "settings.ini"; // Name of the file where earlier experiment file names are stored.

	std::string initialisationPath = "Experiments/"; // Path to the experiment initialisation data file.
	std::string initialisationFileName = "experiment.txt"; // Name of the file containing experiment input parameters.

	std::string instancesPath = "Instances/"; // Path to the directory containing cVRP instances.

	std::string outputPath = "Results/"; // Path to the output file.
	std::string outputFileName = "testResults.txt"; // Name of the file containing experiment output.

	std::string generationResultsPath = "Results/GeneticGenerations/"; // (Genetic algorithm) Path to the output file for each generation.
	std::string generationResultsFileName = "generationResults.txt"; // (Genetic algorithm) Name of the file containing experiment generation output.

	std::string getSettingsFile();
	std::string getInitialisationFile();
	std::string getInstancesPath();
	std::string getOutputFile();
	std::string getGenerationResultsFile();

	void loadSettingsFile();

	void setInitialisationPath(std::string path);
	void setInitialisationFileName(std::string fileName);

	void setInstancesPath(std::string path);

	void setOutputPath(std::string path);
	void setOutputFileName(std::string fileName);

	void setGenerationResultsPath(std::string path);
	void setGenerationResultsFileName(std::string fileName);
};

