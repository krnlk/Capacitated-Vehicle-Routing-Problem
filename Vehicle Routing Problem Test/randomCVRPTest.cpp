#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "../Vehicle Routing Problem/randomCVRP.h"

class randomCVRPTest : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		instanceFile.calculateDistancesBetweenPoints();
		// randomCVRP.generateASolution(instanceFile);
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
	randomCVRP randomCVRP;
};