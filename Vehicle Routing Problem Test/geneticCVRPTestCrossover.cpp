#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "../Vehicle Routing Problem/geneticCVRP.h"

class geneticCVRPTestCrossover : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		instanceFile.calculateDistancesBetweenPoints();
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
	geneticCVRP geneticCVRP;
};

TEST_F(geneticCVRPTestCrossover, SelectCrossoverTest) {

}

TEST_F(geneticCVRPTestCrossover, CycleCrossoverTest) {

}

TEST_F(geneticCVRPTestCrossover, OrderedCrossoverTest) {

}

TEST_F(geneticCVRPTestCrossover, PartiallyMappedCrossoverCutoffsSetupTest) {

}

TEST_F(geneticCVRPTestCrossover, PartiallyMappedCrossoverTest) {

}

TEST_F(geneticCVRPTestCrossover, FindValueInsideParentTest) {

}

TEST_F(geneticCVRPTestCrossover, Test) {

}
