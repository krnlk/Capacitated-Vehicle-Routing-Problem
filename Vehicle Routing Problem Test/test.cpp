#include "pch.h"
#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/instanceFile.cpp"
#include "../Vehicle Routing Problem/specimen.h"
#include "../Vehicle Routing Problem/specimen.cpp"
#include "../Vehicle Routing Problem/point.h"
#include "../Vehicle Routing Problem/point.cpp"

#include <iostream>
#include <vector>
#include <string>
TEST(TestCaseName, TestName) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(CalculateRouteCost, routeCost) {
	/*
	instanceFile instanceFile;
	specimen specimen;

	instanceFile.pointsVector

	instanceFile.calculateDistancesBetweenPoints();

	specimen.setPointOrder();
	*/
}

TEST(DistanceBetweenPoints, sampleRoute1Distances) {
	instanceFile instanceFile;
	instanceFile.loadInstanceData("C:/Users/Kornel/source/repos/Vehicle Routing Problem/Vehicle Routing Problem Test/P-n16-k8.vrp.txt");

	instanceFile.calculateDistancesBetweenPoints();
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][2], 21);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[2][0], 21);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][6], 12);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[6][0], 12);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][8], 32);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[8][0], 32);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][15], 30);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[15][12], 6);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[12][10], 10);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[10][0], 21);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][14], 31);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[14][5], 8);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[5][0], 23);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][13], 29);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[13][9], 7);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[9][7], 10);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[7][0], 22);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][11], 28);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[11][4], 7);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[4][0], 22);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][3], 33);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[3][1], 19);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[1][0], 14);
}

TEST(DistanceBetweenPoints, sampleRoute2Instances) {
	instanceFile instanceFile;
	instanceFile.loadInstanceData("C:/Users/Kornel/source/repos/Vehicle Routing Problem/Vehicle Routing Problem Test/P-n16-k8.vrp.txt");

	instanceFile.calculateDistancesBetweenPoints();
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][6], 12);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[6][0], 12);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][8], 32);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[8][13], 10);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[13][0], 29);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][2], 21);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[2][0], 21);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][4], 22);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[4][0], 22);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][7], 22);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[7][9], 10);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[9][5], 13);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[5][0], 23);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][3], 33);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[3][14], 37);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[14][0], 31);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][1], 14);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[1][10], 7);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[10][0], 21);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][12], 30);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[12][15], 6);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[15][11], 10);
	EXPECT_EQ(instanceFile.distanceBetweenPoints[11][0], 28);

	specimen specimen;
	std::vector<int> pointOrder = { 0, 6, 8, 13, 2, 4, 7, 9, 5, 3, 14, 1, 10, 12, 15, 11 };
	specimen.setPointOrder(pointOrder);
	std::vector<int> pointsVisitedOnAnOptimalRoute = { 0, 6, 0, 8, 13, 0, 2, 0, 4, 0, 7, 9, 5, 0, 3, 14, 0, 1, 10, 0, 12, 15, 11, 0};
	EXPECT_EQ(specimen.getPointsOnARoute(instanceFile).size(), pointsVisitedOnAnOptimalRoute.size());
	EXPECT_EQ(specimen.getPointsOnARoute(instanceFile), pointsVisitedOnAnOptimalRoute);
	EXPECT_EQ(specimen.calculateTotalCost(instanceFile), 466);
}

TEST(TotalRouteCostTest, saffsa) {
	instanceFile instanceFile;
	// TODO fix path later
	instanceFile.loadInstanceData("C:/Users/Kornel/source/repos/Vehicle Routing Problem/Vehicle Routing Problem Test/P-n16-k8.vrp.txt");

	EXPECT_EQ(instanceFile.pointsVector.size(), 16);
	EXPECT_EQ(instanceFile.dimension, 16);

	instanceFile.calculateDistancesBetweenPoints();

	specimen specimen;
	std::vector<int> pointOrder = { 0, 2, 6, 8, 15, 12, 10, 14, 5, 13, 9, 7, 11, 4, 3, 1 };
	specimen.setPointOrder(pointOrder);

	std::vector<int> pointsVisitedOnAnOptimalRoute = { 0, 2, 0, 6, 0, 8, 0, 15, 12, 10, 0, 14, 5, 0, 13, 9, 7, 0, 11, 4, 0, 3, 1, 0}; //24 size

	EXPECT_EQ(specimen.calculateTotalCost(instanceFile), 450);

	EXPECT_EQ(specimen.getPointsOnARoute(instanceFile).size(), pointsVisitedOnAnOptimalRoute.size());
	EXPECT_EQ(specimen.getPointsOnARoute(instanceFile), pointsVisitedOnAnOptimalRoute);

}

TEST(BasicRouteCost, BasicRouteTest) {
	instanceFile instanceFile;

	instanceFile.loadInstanceData("C:/Users/Kornel/source/repos/Vehicle Routing Problem/Vehicle Routing Problem Test/fewPointsTestInstance.txt");

	specimen specimen;

	EXPECT_EQ(instanceFile.pointsVector.size(), 3);
	EXPECT_EQ(instanceFile.dimension, 3);

	instanceFile.calculateDistancesBetweenPoints();

	std::vector<int> pointOrder = { 0, 2, 6};
	specimen.setPointOrder(pointOrder);
	EXPECT_EQ(specimen.calculateTotalCost(instanceFile), 66);
}

TEST(PMXTest, PMXTest) {

}

TEST(CMXTest, CMXTest) {

}

TEST(OMXTest, OMXTest) {

}

TEST(RouletteWheelSelectionTest, RouletteWheelSelectionTest) {

}

TEST(TournamentSelectionTest, TournamentSelectionTest) {

}

TEST(StochasticUniversalSamplingSelection, StochasticUniversalSamplingSelection) {

}

TEST(RankSelectionTest, RankSelectionTest) {

}

TEST(SwapMutationTest, SwapMutationTest) {

}

TEST(InvertMutationTest, InvertMutationTest) {

}


int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}