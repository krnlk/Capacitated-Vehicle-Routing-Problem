#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "geneticCVRP.h"

class correctSolutionValuesTest : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		geneticCVRP.setInstanceFile(instanceFile);
		instanceFile.calculateDistancesBetweenPoints();

		start = { 0 };
		route1 = { 2 };
		route2 = { 6 };
		route3 = { 8 };
		route4 = { 15, 12, 10 };
		route5 = { 14, 5 };
		route6 = { 13, 9, 7 };
		route7 = { 11, 4 };
		route8 = { 3, 1 };
		pointOrder.reserve(start.size() + route1.size() + route2.size() + route3.size() + route4.size() + route5.size() + route6.size() + route7.size() + route8.size());
	}

	instanceFile instanceFile;
	geneticCVRP geneticCVRP;
	specimen specimen;
	std::vector<int> start, route1, route2, route3, route4, route5, route6, route7, route8, pointOrder;
};

TEST_F(correctSolutionValuesTest, Variant1Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route5.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route7.begin(), route7.end());
	pointOrder.insert(pointOrder.end(), route8.begin(), route8.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	EXPECT_EQ(specimen.getTotalCost(), 450);
}

TEST_F(correctSolutionValuesTest, Variant2Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route5.end());
	pointOrder.insert(pointOrder.end(), route8.begin(), route8.end());
	pointOrder.insert(pointOrder.end(), route7.begin(), route7.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	EXPECT_EQ(specimen.getTotalCost(), 450);
}

TEST_F(correctSolutionValuesTest, Variant3Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route8.begin(), route8.end());
	pointOrder.insert(pointOrder.end(), route7.begin(), route7.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route5.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	EXPECT_EQ(specimen.getTotalCost(), 450);
}

TEST_F(correctSolutionValuesTest, Variant4Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route7.begin(), route7.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route8.begin(), route8.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route5.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	EXPECT_EQ(specimen.getTotalCost(), 450);
}

TEST_F(correctSolutionValuesTest, Variant5Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route7.begin(), route7.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route5.end());
	pointOrder.insert(pointOrder.end(), route8.begin(), route8.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	EXPECT_EQ(specimen.getTotalCost(), 450);
}

/*
#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "geneticCVRP.h"

class correctSolutionValuesTest : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		geneticCVRP.setInstanceFile(instanceFile);
		instanceFile.calculateDistancesBetweenPoints();

		start = { 0 };
		route1 = { 18, 17, 11, 40, 30, 19, 34, 44 };
		route2 = { 29, 43, 13, 7, 28, 23 };
		route3 = { 32, 20, 5, 21, 33, 41, 8, 10, 3 };
		route4 = { 12, 39, 36, 42, 4, 16, 22, 9 };
		route5 = { 26, 27, 37, 24, 6, 1 };
		route6 = { 15, 25, 2, 38, 31, 35, 14 };

		pointOrder.reserve(start.size() + route1.size() + route2.size() + route3.size() + route4.size() + route5.size() + route6.size());
	}

	instanceFile instanceFile;
	geneticCVRP geneticCVRP;
	specimen specimen;
	std::vector<int> start, route1, route2, route3, route4, route5, route6, route 7, pointOrder;
};

TEST_F(correctSolutionValuesTest, Variant1Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route5.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route6.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	ASSERT_EQ(specimen.getTotalCost(), 944);
}

TEST_F(correctSolutionValuesTest, Variant2Test {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route5.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	ASSERT_EQ(specimen.getTotalCost(), 944);
}

TEST_F(correctSolutionValuesTest, Variant3Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route5.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route6.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	ASSERT_EQ(specimen.getTotalCost(), 944);
}

TEST_F(correctSolutionValuesTest, Variant4Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route5.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route4.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	ASSERT_EQ(specimen.getTotalCost(), 944);
}

TEST_F(correctSolutionValuesTest, Variant5Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route5.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	ASSERT_EQ(specimen.getTotalCost(), 944);
}

TEST_F(correctSolutionValuesTest, Variant6Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route5.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route4.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	ASSERT_EQ(specimen.getTotalCost(), 944);
}

TEST_F(correctSolutionValuesTest, Variant7Test) {
	pointOrder.insert(pointOrder.end(), start.begin(), start.end());
	pointOrder.insert(pointOrder.end(), route1.begin(), route1.end());
	pointOrder.insert(pointOrder.end(), route2.begin(), route2.end());
	pointOrder.insert(pointOrder.end(), route3.begin(), route3.end());
	pointOrder.insert(pointOrder.end(), route4.begin(), route4.end());
	pointOrder.insert(pointOrder.end(), route6.begin(), route6.end());
	pointOrder.insert(pointOrder.end(), route5.begin(), route5.end());

	specimen.setPointOrder(pointOrder);
	specimen.calculateTotalCost(instanceFile);
	ASSERT_EQ(specimen.getTotalCost(), 944);
}

TEST_F(correctSolutionValuesTest, Aaaa__Test) {

}
*/