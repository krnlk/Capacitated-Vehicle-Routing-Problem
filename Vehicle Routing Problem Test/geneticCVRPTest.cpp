#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "../Vehicle Routing Problem/geneticCVRP.h"

class geneticCVRPTest : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		instanceFile.calculateDistancesBetweenPoints();
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
	geneticCVRP geneticCVRP;
};

TEST_F(geneticCVRPTest, GetBestFoundSolutionTotalCostTest) {

}

TEST_F(geneticCVRPTest, GetBestFoundSolutionPointOrderTest) {

}

TEST_F(geneticCVRPTest, SetAlgorithmParametersTest) {

}

TEST_F(geneticCVRPTest, ClearInfoTest) {

}

TEST_F(geneticCVRPTest, SetBestSpecimenInfoTest) {

}

TEST_F(geneticCVRPTest, CalculateSumOfRanksTest) {

}

TEST_F(geneticCVRPTest, GenerateInitialSpecimenTest) {

}

TEST_F(geneticCVRPTest, AnalyseNewGenerationTest) {

}

TEST_F(geneticCVRPTest, FindBestSpecimenInAGenerationTest) {

}

TEST_F(geneticCVRPTest, FindWorstSpecimenInAGenerationTest) {

}

TEST_F(geneticCVRPTest, CreateNewOffspringTest) {

}

TEST_F(geneticCVRPTest, CalculateFitnessFunctionTest) {

}

TEST_F(geneticCVRPTest, MainAlgorithmLoopTest) {

}

TEST_F(geneticCVRPTest, Test) {

}
