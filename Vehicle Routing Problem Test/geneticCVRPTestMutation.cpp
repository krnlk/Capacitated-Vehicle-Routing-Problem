#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "../Vehicle Routing Problem/geneticCVRP.h"

class geneticCVRPTestMutation : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		instanceFile.calculateDistancesBetweenPoints();
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
	geneticCVRP geneticCVRP;
};

TEST_F(geneticCVRPTestMutation, MutateTest) {

}

TEST_F(geneticCVRPTestMutation, MutateSwapOnSpecimenTest) {

}

TEST_F(geneticCVRPTestMutation, MutateInvertOnSpecimenTest) {

}

TEST_F(geneticCVRPTestMutation, Test) {

}