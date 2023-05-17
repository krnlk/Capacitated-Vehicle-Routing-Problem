#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "../Vehicle Routing Problem/greedyCVRP.h"
#include "../Vehicle Routing Problem/greedyCVRP.cpp"

class greedyCVRPTest : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		instanceFile.calculateDistancesBetweenPoints();
		greedyCVRP.generateASolution(instanceFile);
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
	greedyCVRP greedyCVRP;
};

TEST_F(greedyCVRPTest, GenerateASolutionTest) {
	EXPECT_EQ(greedyCVRP.getTotalCost(), 497);

	std::vector<int> greedyPoints = { 0, 6, 0, 1, 10, 13, 0, 2, 0, 4, 11, 0, 7, 5, 9, 0, 12, 15, 0, 14, 3, 0, 8, 0 };
	EXPECT_EQ(greedyCVRP.getPointsOnARoute(instanceFile), greedyPoints);
}

TEST_F(greedyCVRPTest, SubroutesCapacityTest) {
	int subroute1Capacity = instanceFile.pointsVector[6].getWeight();
	int subroute2Capacity = instanceFile.pointsVector[1].getWeight() + instanceFile.pointsVector[10].getWeight() + instanceFile.pointsVector[13].getWeight();
	int subroute3Capacity = instanceFile.pointsVector[2].getWeight();
	int subroute4Capacity = instanceFile.pointsVector[4].getWeight() + instanceFile.pointsVector[11].getWeight();
	int subroute5Capacity = instanceFile.pointsVector[7].getWeight() + instanceFile.pointsVector[5].getWeight() + instanceFile.pointsVector[9].getWeight();
	int subroute6Capacity = instanceFile.pointsVector[12].getWeight() + instanceFile.pointsVector[15].getWeight();
	int subroute7Capacity = instanceFile.pointsVector[14].getWeight() + instanceFile.pointsVector[3].getWeight();
	int subroute8Capacity = instanceFile.pointsVector[8].getWeight();

	EXPECT_LE(subroute1Capacity, instanceFile.getMaxCapacity());
	EXPECT_LE(subroute2Capacity, instanceFile.getMaxCapacity());
	EXPECT_LE(subroute3Capacity, instanceFile.getMaxCapacity());
	EXPECT_LE(subroute4Capacity, instanceFile.getMaxCapacity());
	EXPECT_LE(subroute5Capacity, instanceFile.getMaxCapacity());
	EXPECT_LE(subroute6Capacity, instanceFile.getMaxCapacity());
	EXPECT_LE(subroute7Capacity, instanceFile.getMaxCapacity());
	EXPECT_LE(subroute8Capacity, instanceFile.getMaxCapacity());
}

TEST_F(greedyCVRPTest, Test) {

}
