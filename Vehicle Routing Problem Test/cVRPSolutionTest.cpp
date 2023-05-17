#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"

class cVRPSolutionTest : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		instanceFile.calculateDistancesBetweenPoints();
		cVRPSolution.pointOrder = { 0, 2, 6, 8, 15, 12, 10, 14, 5, 13, 9, 7, 11, 4, 3, 1 };
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
};

TEST_F(cVRPSolutionTest, TotalCostTest) {
	cVRPSolution.totalCost = cVRPSolution.calculateTotalCost(instanceFile);

	EXPECT_EQ(cVRPSolution.getTotalCost(), 450);
}

TEST_F(cVRPSolutionTest, GetPointByIndexTest) {
	int point1, point16;

	point1 = cVRPSolution.getPointByIndex(0);
	point16 = cVRPSolution.getPointByIndex(15);

	EXPECT_EQ(point1, 0);
	EXPECT_EQ(point16, 1);
}

TEST_F(cVRPSolutionTest, ChangePointOrderTest) {
	std::vector<int> pointOrder = { 0, 2, 6, 8, 15, 12, 10, 14, 5, 13, 9, 7, 11, 4, 3, 1 };
	
	EXPECT_EQ(cVRPSolution.pointOrder, pointOrder);
	cVRPSolution.clearPointOrder();
	
	EXPECT_EQ(cVRPSolution.pointOrder.size(), 0);
	
	cVRPSolution.setPointOrder(pointOrder);
	
	EXPECT_EQ(cVRPSolution.pointOrder, pointOrder);
	EXPECT_EQ(cVRPSolution.pointOrder.size(), 16);
}

TEST_F(cVRPSolutionTest, SetPointLocationTest) {
	cVRPSolution.setPointLocations(instanceFile);
	std::vector<int> pointLocation = { 0, 15, 1, 14, 13, 8, 2, 11, 3, 10, 6, 12, 5, 9, 7, 4 };

	EXPECT_EQ(cVRPSolution.pointLocation, pointLocation);
}

TEST_F(cVRPSolutionTest, PointsOnARouteTest) {
	std::vector<int> pointsOnARoute = { 0, 2, 0, 6, 0, 8, 0, 15, 12, 10, 0, 14, 5, 0, 13, 9, 7, 0, 11, 4, 0, 3, 1, 0 };
	EXPECT_EQ(cVRPSolution.getPointsOnARoute(instanceFile), pointsOnARoute);
}