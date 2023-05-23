#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "../Vehicle Routing Problem/geneticCVRP.h"

class geneticCVRPTestSelection : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		instanceFile.calculateDistancesBetweenPoints();
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
	geneticCVRP geneticCVRP;
};

TEST_F(geneticCVRPTestSelection, SelectionTest) {

}

TEST_F(geneticCVRPTestSelection, RankSelectionIndexesTest) {

}

TEST_F(geneticCVRPTestSelection, RankSelectionTest) {

}

TEST_F(geneticCVRPTestSelection, RandomSelectionTest) {

}

TEST_F(geneticCVRPTestSelection, StochasticUniversalSamplingIndexesTest) {

}

TEST_F(geneticCVRPTestSelection, StochasticUniversalSamplingTest) {

}

TEST_F(geneticCVRPTestSelection, TournamentSelectionIndexesTest) {

}

TEST_F(geneticCVRPTestSelection, TournamentSelectionTest) {

}

TEST_F(geneticCVRPTestSelection, RouletteWheelSelectionIndexesTest) {

}

TEST_F(geneticCVRPTestSelection, RouletteWheelSelectionTest) {

}

TEST_F(geneticCVRPTestSelection, Test) {

}
