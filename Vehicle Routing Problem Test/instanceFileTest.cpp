#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/point.h"
#include "../Vehicle Routing Problem/specimen.h"
#include "../Vehicle Routing Problem/Filepath.h"

class InstanceFileTest : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
	}
	instanceFile instanceFile;
};

TEST_F(InstanceFileTest, LoadInstanceDataTest) {
	EXPECT_EQ(instanceFile.dimension, 16);
	EXPECT_EQ(instanceFile.getFileName(), "P-n16-k8.vrp.txt");
	EXPECT_EQ(instanceFile.getOptimalValue(), 450);
	EXPECT_EQ(instanceFile.getMaxCapacity(), 35);
}

TEST_F(InstanceFileTest, LoadPointsTest) {
	point point1, point16;

	point1.setPosition(30, 40);
	point1.setIndex(0);
	point1.setWeight(0);

	point16.setPosition(37, 69);
	point16.setIndex(15);
	point16.setWeight(11);

	EXPECT_EQ(instanceFile.pointsVector.size(), 16);

	EXPECT_EQ(instanceFile.pointsVector[0].getPositionX(), point1.getPositionX());
	EXPECT_EQ(instanceFile.pointsVector[0].getPositionY(), point1.getPositionY());
	EXPECT_EQ(instanceFile.pointsVector[0].getIndex(), point1.getIndex());
	EXPECT_EQ(instanceFile.pointsVector[0].getWeight(), point1.getWeight());

	EXPECT_EQ(instanceFile.pointsVector[15].getPositionX(), point16.getPositionX());
	EXPECT_EQ(instanceFile.pointsVector[15].getPositionY(), point16.getPositionY());
	EXPECT_EQ(instanceFile.pointsVector[15].getIndex(), point16.getIndex());
	EXPECT_EQ(instanceFile.pointsVector[15].getWeight(), point16.getWeight());
}

TEST_F(InstanceFileTest, ClearInstanceDataTest) {
	EXPECT_EQ(instanceFile.pointsVector.size(), 16);

	instanceFile.clearInstanceData();

	EXPECT_EQ(instanceFile.pointsVector.size(), 0);
	EXPECT_EQ(instanceFile.pointsVector.empty(), true);
}

TEST_F(InstanceFileTest, SymmetricalDistanceBetweenPointsTest) {
	instanceFile.calculateDistancesBetweenPoints();

	for (int i = 0; i < instanceFile.dimension; i++) {
		for (int j = 0; j < instanceFile.dimension; j++) {
			EXPECT_EQ(instanceFile.distanceBetweenPoints[i][j], instanceFile.distanceBetweenPoints[j][i]);
		}
	}
}

TEST_F(InstanceFileTest, ReallocateDistanceBetweenPointsTest) {
	instanceFile.calculateDistancesBetweenPoints();

	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][15], 30);

	instanceFile.dealocateDistancesBetweenPoints();
	instanceFile.calculateDistancesBetweenPoints();

	EXPECT_EQ(instanceFile.distanceBetweenPoints[0][15], 30);
}

class InstanceFileReadTest : public ::testing::Test {
protected:
	void SetUp() override {
		std::string instanceFileName = "P-n16-k8.vrp.txt";
		instanceFile.loadInstanceData(filepath.getInstancesPath(), instanceFileName);
	}

	instanceFile instanceFile;
	specimen specimen;
	filepath filepath;
};

TEST_F(InstanceFileReadTest, sampleRoute1Distances) {
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

TEST_F(InstanceFileReadTest, sampleRoute2Instances) {
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

	std::vector<int> pointOrder = { 0, 6, 8, 13, 2, 4, 7, 9, 5, 3, 14, 1, 10, 12, 15, 11 };
	specimen.setPointOrder(pointOrder);
	std::vector<int> pointsVisitedOnAnOptimalRoute = { 0, 6, 0, 8, 13, 0, 2, 0, 4, 0, 7, 9, 5, 0, 3, 14, 0, 1, 10, 0, 12, 15, 11, 0 };
	EXPECT_EQ(specimen.getPointsOnARoute(instanceFile).size(), pointsVisitedOnAnOptimalRoute.size());
	EXPECT_EQ(specimen.getPointsOnARoute(instanceFile), pointsVisitedOnAnOptimalRoute);
	EXPECT_EQ(specimen.calculateTotalCost(instanceFile), 466);
}

TEST_F(InstanceFileReadTest, pointOrderCostSizeRoute) {
	EXPECT_EQ(instanceFile.pointsVector.size(), 16);
	EXPECT_EQ(instanceFile.dimension, 16);

	instanceFile.calculateDistancesBetweenPoints();

	std::vector<int> pointOrder = { 0, 2, 6, 8, 15, 12, 10, 14, 5, 13, 9, 7, 11, 4, 3, 1 };
	specimen.setPointOrder(pointOrder);

	std::vector<int> pointsVisitedOnAnOptimalRoute = { 0, 2, 0, 6, 0, 8, 0, 15, 12, 10, 0, 14, 5, 0, 13, 9, 7, 0, 11, 4, 0, 3, 1, 0 }; //24 size

	EXPECT_EQ(specimen.calculateTotalCost(instanceFile), 450);

	EXPECT_EQ(specimen.getPointsOnARoute(instanceFile).size(), pointsVisitedOnAnOptimalRoute.size());
	EXPECT_EQ(specimen.getPointsOnARoute(instanceFile), pointsVisitedOnAnOptimalRoute);
}