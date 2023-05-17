#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "../Vehicle Routing Problem/geneticCVRP.h"
//#include "../Vehicle Routing Problem/geneticCVRP.cpp"

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

TEST_F(geneticCVRPTest, Test) {

}