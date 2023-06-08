#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "geneticCVRP.h"

class geneticCVRPTestMutation : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		geneticCVRP.setInstanceFile(instanceFile);
		instanceFile.calculateDistancesBetweenPoints();
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
	geneticCVRP geneticCVRP;
	specimen specimen;
};

TEST_F(geneticCVRPTestMutation, MutateSwapOnSpecimenTest1) {
	std::pair<int, int> mutationPair = { 1, 5 };
	std::vector<int> pointOrder = { 0, 1, 15, 2, 14, 3, 13, 4, 12, 5, 11, 6, 10, 7, 9, 8 };
	std::vector<int> newPointOrder = { 0, 3, 15, 2, 14, 1, 13, 4, 12, 5, 11, 6, 10, 7, 9, 8 };
	specimen.setPointOrder(pointOrder);
	geneticCVRP.allCurrentSpecimen.push_back(specimen);
	geneticCVRP.allCurrentSpecimen[0].mutateSwap(mutationPair);

	EXPECT_EQ(geneticCVRP.allCurrentSpecimen[0].pointOrder, newPointOrder);
}

TEST_F(geneticCVRPTestMutation, MutateInvertOnSpecimenTest1) {
	std::pair<int, int> mutationPair = { 1, 5 };
	std::vector<int> pointOrder = { 0, 1, 15, 2, 14, 3, 13, 4, 12, 5, 11, 6, 10, 7, 9, 8 };
	std::vector<int> newPointOrder = { 0, 3, 14, 2, 15, 1, 13, 4, 12, 5, 11, 6, 10, 7, 9, 8 };
	specimen.setPointOrder(pointOrder);
	geneticCVRP.allCurrentSpecimen.push_back(specimen);
	geneticCVRP.allCurrentSpecimen[0].mutateInvert(mutationPair);

	EXPECT_EQ(geneticCVRP.allCurrentSpecimen[0].pointOrder, newPointOrder);
}

TEST_F(geneticCVRPTestMutation, MutateInvertOnSpecimenTest2) {
	std::pair<int, int> mutationPair = { 1, 15 };
	std::vector<int> pointOrder = { 0, 1, 15, 2, 14, 3, 13, 4, 12, 5, 11, 6, 10, 7, 9, 8 };
	std::vector<int> newPointOrder = { 0, 8, 9, 7, 10, 6, 11, 5, 12, 4, 13, 3, 14, 2, 15, 1};
	specimen.setPointOrder(pointOrder);
	geneticCVRP.allCurrentSpecimen.push_back(specimen);
	geneticCVRP.allCurrentSpecimen[0].mutateInvert(mutationPair);

	EXPECT_EQ(geneticCVRP.allCurrentSpecimen[0].pointOrder, newPointOrder);
}

TEST_F(geneticCVRPTestMutation, MutateSwapOnSpecimenTest2) {
	std::vector<int> pointOrder = { 0, 1, 15, 2, 14, 3, 13, 4, 12, 5, 11, 6, 10, 7, 9, 8 };
	specimen.setPointOrder(pointOrder);
	geneticCVRP.allCurrentSpecimen.push_back(specimen);

	for (int i = 0; i < 1000; i++) {
		geneticCVRP.mutateSwapOnSpecimen(0);
		EXPECT_EQ(geneticCVRP.allCurrentSpecimen[0].getPointByIndex(0), 0);
	}
}

TEST_F(geneticCVRPTestMutation, MutateInvertOnSpecimenTest3) {
	std::vector<int> pointOrder = { 0, 1, 15, 2, 14, 3, 13, 4, 12, 5, 11, 6, 10, 7, 9, 8 };
	specimen.setPointOrder(pointOrder);
	geneticCVRP.allCurrentSpecimen.push_back(specimen);

	for (int i = 0; i < 1000; i++) {
		geneticCVRP.mutateInvertOnSpecimen(0);
		EXPECT_EQ(geneticCVRP.allCurrentSpecimen[0].getPointByIndex(0), 0);
	}
}
