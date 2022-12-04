#include "pch.h"
#include "instanceFile.h"
#include "specimen.h"
#include "point.h"

#include "instanceFile.cpp"
#include "specimen.cpp"
#include "point.cpp"

#include <vector>

TEST(TestCaseName, TestName) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(DistancesBetweenPointsTest, saffsa) {
	instanceFile instanceFile;
	instanceFile.loadInstanceData("P-n16-k8.vrp.txt");
	instanceFile.calculateDistancesBetweenPoints();

	specimen specimen;
	std::vector<int> pointOrder = { 0, 2, 6, 8, 15, 12, 10, 14, 5, 13, 9, 7, 11, 4, 3, 1 };
	specimen.setPointOrder(pointOrder);

	EXPECT_EQ(specimen.calculateTotalCost(instanceFile), 450);
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}