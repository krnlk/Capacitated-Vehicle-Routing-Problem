#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "geneticCVRP.h"

class geneticCVRPTest : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		instanceFile.calculateDistancesBetweenPoints();
		geneticCVRP.setInstanceFile(instanceFile);

		geneticCVRP.bestFoundSolutionTotalCost = INT_MAX;
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
	geneticCVRP geneticCVRP;
	specimen specimen1, specimen2, specimen3;
};

// Tests a bunch of methods at once.
TEST_F(geneticCVRPTest, GeneralGeneticTest) {
	int bestSpecimenIndex;
	int averageSolutionTotalCost = 0;
	int currentGenerationWorstFoundSolutionTotalCost = 0;
	int currentGenerationBestFoundSolutionTotalCost = INT_MAX;
	bool newBestSpecimenFound = false;

	for (int i = 0; i < geneticCVRP.currentInstanceFile.dimension; i++) {
		geneticCVRP.bestFoundSolutionPointOrder.push_back(0);
	}

	std::vector<int> pointOrder1 = { 0, 2, 6, 8, 15, 12, 10, 14, 5, 13, 9, 7, 11, 4, 3, 1 };
	std::string pointOrder1Stringified = "0 2 6 8 15 12 10 14 5 13 9 7 11 4 3 1";
	specimen1.setPointOrder(pointOrder1);
	std::vector<int> pointOrder2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	specimen2.setPointOrder(pointOrder2);
	std::vector<int> pointOrder3 = { 0, 15, 1, 2, 14, 13, 3, 4, 12, 11, 5, 6, 10, 9, 7, 8 };
	specimen3.setPointOrder(pointOrder3);

	geneticCVRP.allCurrentSpecimen.push_back(specimen2);
	geneticCVRP.allCurrentSpecimen.push_back(specimen1);
	geneticCVRP.allCurrentSpecimen.push_back(specimen3);

	for (int i = 0; i < 3; i++) {
		geneticCVRP.findBestSpecimenInAGeneration(i, averageSolutionTotalCost, currentGenerationBestFoundSolutionTotalCost, bestSpecimenIndex, newBestSpecimenFound);
		geneticCVRP.findWorstSpecimenInAGeneration(i, currentGenerationWorstFoundSolutionTotalCost);
		if (newBestSpecimenFound) {
			geneticCVRP.setBestSpecimenInfo(bestSpecimenIndex);
		}
	}

	EXPECT_EQ(geneticCVRP.allCurrentSpecimen.size(), 3);
	EXPECT_EQ(geneticCVRP.allCurrentSpecimen[1].getTotalCost(), 450);
	EXPECT_EQ(geneticCVRP.getBestFoundSolutionTotalCost(), currentGenerationBestFoundSolutionTotalCost);
	EXPECT_EQ(geneticCVRP.getBestFoundSolutionTotalCost(), 450);
	EXPECT_EQ(bestSpecimenIndex, 1);
	EXPECT_EQ(geneticCVRP.bestFoundSolutionPointOrder, pointOrder1);
	EXPECT_EQ(geneticCVRP.getBestFoundSolutionPointOrder(), pointOrder1Stringified);
	EXPECT_EQ(geneticCVRP.worstFoundSolutionTotalCost, 644);

	geneticCVRP.clearInfo();

	std::vector<int> pointOrder4 = {};
	std::string pointOrder4Stringified = "";
	EXPECT_EQ(geneticCVRP.bestFoundSolutionPointOrder, pointOrder4);
	EXPECT_EQ(geneticCVRP.getBestFoundSolutionPointOrder(), pointOrder4Stringified);
	EXPECT_EQ(geneticCVRP.getBestFoundSolutionTotalCost(), INT_MAX);
	EXPECT_EQ(geneticCVRP.worstFoundSolutionTotalCost, 0);
	EXPECT_EQ(geneticCVRP.allCurrentSpecimen.size(), 0);
	EXPECT_EQ(geneticCVRP.newGenerationOfSpecimen.size(), 0);
}

TEST_F(geneticCVRPTest, SetAlgorithmParametersTest) {
	std::ifstream experimentFile;
	experimentFile.open("Experiments/experiment.txt");
	geneticCVRP.setAlgorithmParameters(experimentFile);
	EXPECT_EQ(geneticCVRP.selectUsed, 't');
	EXPECT_EQ(geneticCVRP.crossoverUsed, 'p');
	EXPECT_EQ(geneticCVRP.mutationUsed, 'i');
	EXPECT_EQ(geneticCVRP.crossoverProbability, 0.8);
	EXPECT_EQ(geneticCVRP.mutationProbability, 0.2);
	EXPECT_EQ(geneticCVRP.numberOfSpecimenInAGeneration, 3);
	EXPECT_EQ(geneticCVRP.maxNumberOfGenerations, 10);
	experimentFile.close();

	experimentFile.open("Experiments/experimentROS.txt");
	geneticCVRP.setAlgorithmParameters(experimentFile);
	EXPECT_EQ(geneticCVRP.selectUsed, 'r');
	EXPECT_EQ(geneticCVRP.crossoverUsed, 'o');
	EXPECT_EQ(geneticCVRP.mutationUsed, 's');
	EXPECT_EQ(geneticCVRP.crossoverProbability, 0.8);
	EXPECT_EQ(geneticCVRP.mutationProbability, 0.2);
	EXPECT_EQ(geneticCVRP.numberOfSpecimenInAGeneration, 100);
	EXPECT_EQ(geneticCVRP.maxNumberOfGenerations, 1000);
	experimentFile.close();
}

TEST_F(geneticCVRPTest, CalculateSumOfRanksTest) {
	std::ifstream experimentFile;
	experimentFile.open("Experiments/experimentROS.txt");
	geneticCVRP.setAlgorithmParameters(experimentFile);
	geneticCVRP.calculateSumOfRanks();

	EXPECT_EQ(geneticCVRP.sumOfRanks, 5050);
}

TEST_F(geneticCVRPTest, GenerateInitialSpecimenTest) {
	std::ifstream experimentFile;
	experimentFile.open("Experiments/experiment.txt");
	geneticCVRP.setAlgorithmParameters(experimentFile);
	experimentFile.close();

	geneticCVRP.currentInstanceFile.calculateDistancesBetweenPoints();
	geneticCVRP.generateInitialSpecimen();

	EXPECT_EQ(geneticCVRP.allCurrentSpecimen.size(), 3);
	EXPECT_EQ(geneticCVRP.allCurrentSpecimen[0].pointOrder.size(), 16);
	EXPECT_EQ(geneticCVRP.allCurrentSpecimen[1].pointOrder.size(), 16);
	EXPECT_EQ(geneticCVRP.allCurrentSpecimen[2].pointOrder.size(), 16);
}

TEST_F(geneticCVRPTest, AnalyseNewGenerationTest) {
	std::ifstream experimentFile;
	std::ofstream experimentResultsFile;
	geneticCVRP.generationsSinceNewBestSolutionFound = 0;
	experimentFile.open("Experiments/experiment2.txt");
	experimentResultsFile.open("Experiments/generationResults.txt");
	geneticCVRP.setAlgorithmParameters(experimentFile);
	experimentFile.close();

	geneticCVRP.currentInstanceFile.calculateDistancesBetweenPoints();

	for (int i = 0; i < geneticCVRP.currentInstanceFile.dimension; i++) {
		geneticCVRP.bestFoundSolutionPointOrder.push_back(0);
	}

	geneticCVRP.generateInitialSpecimen();
	
	EXPECT_EQ(geneticCVRP.allCurrentSpecimen.size(), 4);
	EXPECT_NE(geneticCVRP.allCurrentSpecimen[0].getTotalCost(), INT_MAX);
	EXPECT_NE(geneticCVRP.allCurrentSpecimen[1].getTotalCost(), INT_MAX);
	EXPECT_NE(geneticCVRP.allCurrentSpecimen[2].getTotalCost(), INT_MAX);
	EXPECT_NE(geneticCVRP.allCurrentSpecimen[3].getTotalCost(), INT_MAX);

	for (int i = 0; i < instanceFile.dimension; i++) {
		for (int j = 0; j < geneticCVRP.allCurrentSpecimen.size(); j++) {
			EXPECT_NE(geneticCVRP.allCurrentSpecimen[j].getPointByIndex(i), -1);
		}
	}

	geneticCVRP.analyseNewGeneration(0, experimentResultsFile, 0);
	experimentResultsFile.close();
}

TEST_F(geneticCVRPTest, CreateNewOffspringTest) {
	std::ifstream experimentFile;
	experimentFile.open("Experiments/experiment2.txt");
	geneticCVRP.setAlgorithmParameters(experimentFile);
	experimentFile.close();

	geneticCVRP.currentInstanceFile.calculateDistancesBetweenPoints();
	geneticCVRP.generateInitialSpecimen();

	EXPECT_EQ(geneticCVRP.allCurrentSpecimen.size(), 4);
	EXPECT_EQ(geneticCVRP.worstFoundSolutionTotalCost, 0);
	EXPECT_EQ(geneticCVRP.bestFoundSolutionTotalCost, INT_MAX);
	EXPECT_EQ(geneticCVRP.newGenerationOfSpecimen.size(), 0);

	geneticCVRP.createNewOffspring();

	EXPECT_EQ(geneticCVRP.allCurrentSpecimen.size(), 4);
	EXPECT_EQ(geneticCVRP.newGenerationOfSpecimen.size(), 4);
}

TEST_F(geneticCVRPTest, CalculateFitnessFunctionTest) {
	int currentGenerationWorstFoundSolutionTotalCost = 0;
	geneticCVRP.worstFoundSolutionTotalCost = 0;
	std::ifstream experimentFile;
	experimentFile.open("Experiments/experiment.txt");
	geneticCVRP.setAlgorithmParameters(experimentFile);
	experimentFile.close();

	specimen1.totalCost = 100;
	specimen2.totalCost = 120;
	specimen3.totalCost = 130;
	geneticCVRP.allCurrentSpecimen.push_back(specimen1);
	geneticCVRP.allCurrentSpecimen.push_back(specimen2);
	geneticCVRP.allCurrentSpecimen.push_back(specimen3);

	for (int i = 0; i < 3; i++) {
		geneticCVRP.findWorstSpecimenInAGeneration(i, currentGenerationWorstFoundSolutionTotalCost);
	}

	EXPECT_EQ(currentGenerationWorstFoundSolutionTotalCost, 130);

	geneticCVRP.calculateFitnessFunction();

	EXPECT_EQ(geneticCVRP.fitnessFunction, 40);
}

TEST_F(geneticCVRPTest, MainAlgorithmLoopTest) {
	std::ifstream experimentFile;
	std::ofstream experimentResultsFile;
	experimentFile.open("Experiments/experiment2.txt");
	geneticCVRP.setAlgorithmParameters(experimentFile);
	experimentFile.close();

	geneticCVRP.mainAlgorithmLoop(experimentResultsFile, 0);

	EXPECT_NE(geneticCVRP.getBestFoundSolutionTotalCost(), INT_MAX);
}
