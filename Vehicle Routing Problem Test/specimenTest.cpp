#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "../Vehicle Routing Problem/specimen.h"

class SpecimenTest : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
	}

	instanceFile instanceFile;
	specimen specimen;
};

TEST_F(SpecimenTest, MutateSwapTest) {

}

TEST_F(SpecimenTest, MutateSwap1Test) {

}

TEST_F(SpecimenTest, MutateSwap2Test) {

}

TEST_F(SpecimenTest, MutateSwap3Test) {

}