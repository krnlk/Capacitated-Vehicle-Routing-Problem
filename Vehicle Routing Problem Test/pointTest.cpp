#include "pch.h"

#include "../Vehicle Routing Problem/point.h"

class PointTest : public ::testing::Test {
protected:
	void SetUp() override {
		point1.setPosition(5, 25);
		point1.setIndex(0);
		point1.setWeight(37);

		point2.setPosition(27, 0);
		point2.setIndex(1);
		point2.setWeight(19);

		point3.setPosition(2, 13);
		point3.setIndex(2);
		point3.setWeight(10);

		point4.setPosition(58, 48);
		point4.setIndex(3);
		point4.setWeight(6);
	}
	point point1, point2, point3, point4;
};

TEST_F(PointTest, SetterGetterDataTest) {
	EXPECT_EQ(point1.getPositionX(), 5);
	EXPECT_EQ(point1.getPositionY(), 25);
	EXPECT_EQ(point1.getIndex(), 0);
	EXPECT_EQ(point1.getWeight(), 37);

	EXPECT_EQ(point2.getPositionX(), 27);
	EXPECT_EQ(point2.getPositionY(), 0);
	EXPECT_EQ(point2.getIndex(), 1);
	EXPECT_EQ(point2.getWeight(), 19);

	EXPECT_EQ(point3.getPositionX(), 2);
	EXPECT_EQ(point3.getPositionY(), 13);
	EXPECT_EQ(point3.getIndex(), 2);
	EXPECT_EQ(point3.getWeight(), 10);

	EXPECT_EQ(point4.getPositionX(), 58);
	EXPECT_EQ(point4.getPositionY(), 48);
	EXPECT_EQ(point4.getIndex(), 3);
	EXPECT_EQ(point4.getWeight(), 6);
};

TEST_F(PointTest, DistanceBetweenPointsTest) {
	EXPECT_EQ(point1.getDistanceToAnotherPoint(point1), 0);
	EXPECT_EQ(point2.getDistanceToAnotherPoint(point2), 0);
	EXPECT_EQ(point3.getDistanceToAnotherPoint(point3), 0);

	EXPECT_EQ(point1.getDistanceToAnotherPoint(point2), 33);
	EXPECT_EQ(point1.getDistanceToAnotherPoint(point3), 12);

	EXPECT_EQ(point2.getDistanceToAnotherPoint(point1), 33);
	EXPECT_EQ(point2.getDistanceToAnotherPoint(point3), 28);

	EXPECT_EQ(point3.getDistanceToAnotherPoint(point1), 12);
	EXPECT_EQ(point3.getDistanceToAnotherPoint(point2), 28);

	EXPECT_EQ(point4.getDistanceToAnotherPoint(point1), 58);
}