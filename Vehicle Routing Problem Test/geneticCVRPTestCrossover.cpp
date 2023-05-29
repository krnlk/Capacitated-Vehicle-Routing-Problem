#include "pch.h"

#include "../Vehicle Routing Problem/instanceFile.h"
#include "../Vehicle Routing Problem/cVRPSolution.h"
#include "../Vehicle Routing Problem/specimen.h"
#include "geneticCVRP.h"

class geneticCVRPTestCrossover : public ::testing::Test {
protected:
	void SetUp() override {
		instanceFile.loadInstanceData("Instances/", "P-n16-k8.vrp.txt");
		instanceFile.calculateDistancesBetweenPoints();
		geneticCVRP.setInstanceFile(instanceFile);

		pointOrder1 = { 0, 4, 7, 11,  2, 14, 5, 1, 15,  3,  9, 8, 13, 10, 12, 6 };
		pointOrder2 = { 0, 5, 9,  6, 13,  2, 1, 7, 11, 15, 14, 3, 10,  4, 12, 8 };
	
		specimen1.setPointOrder(pointOrder1);
		specimen2.setPointOrder(pointOrder2);

		geneticCVRP.allCurrentSpecimen.push_back(specimen1);
		geneticCVRP.allCurrentSpecimen.push_back(specimen2);
	}

	instanceFile instanceFile;
	cVRPSolution cVRPSolution;
	geneticCVRP geneticCVRP;
	specimen specimen1, specimen2;
	std::vector<int> pointOrder1, pointOrder2;
};

TEST_F(geneticCVRPTestCrossover, CycleCrossoverTest) {
	std::vector<int> child1PointOrder = { 0, 4, 7, 6, 2, 14, 5, 1, 11, 15, 9, 3, 13, 10, 12, 8 };
	std::vector<int> child2PointOrder = { 0, 5, 9, 11, 13, 2, 1, 7, 15, 3, 14, 8, 10, 4, 12, 6 };
	geneticCVRP.cycleCrossover(0, 1);

	EXPECT_EQ(geneticCVRP.newGenerationOfSpecimen[0].pointOrder, child1PointOrder);
	EXPECT_EQ(geneticCVRP.newGenerationOfSpecimen[1].pointOrder, child2PointOrder);
}

TEST_F(geneticCVRPTestCrossover, OrderedCrossoverTest) {
	std::vector<int> child1PointOrder = { 0, 13, 2, 7, 11, 14, 5, 1, 15, 3, 9, 8, 10, 4, 12, 6 };
	EXPECT_EQ(geneticCVRP.orderedCrossover(0, 1, 5, 11).pointOrder, child1PointOrder);

	std::vector<int> child2PointOrder = { 0, 4, 5, 9, 8, 2, 1, 7, 11, 15, 14, 3, 13, 10, 12, 6 };
	EXPECT_EQ(geneticCVRP.orderedCrossover(1, 0, 5, 11).pointOrder, child2PointOrder);
}

TEST_F(geneticCVRPTestCrossover, PartiallyMappedCrossoverTest) {
	std::vector<int> child1PointOrder = { 0, 7, 2, 6, 13, 14, 5, 1, 15, 3, 9, 8, 10, 4, 12, 11 };
	EXPECT_EQ(geneticCVRP.partiallyMappedCrossover(0, 1, 5, 11).pointOrder, child1PointOrder);

	std::vector<int> child2PointOrder = { 0, 4, 5, 8, 9, 2, 1, 7, 11, 15, 14, 3, 13, 10, 12, 6 };
	EXPECT_EQ(geneticCVRP.partiallyMappedCrossover(1, 0, 5, 11).pointOrder, child2PointOrder);
}

/*
* 
*
parent1 = { 0, 4, 7, 11,  2, 14, 5, 1, 15,  3,  9, 8, 13, 10, 12, 6 }
parent2 = { 0, 5, 9,  6, 13,  2, 1, 7, 11, 15, 14, 3, 10,  4, 12, 8 }

cycle crossover

cycle1Indexes = { 1, 6, 7, 2, 10, 5, 4, 12, 13, 1 }
cycle2Indexes = { 3, 15, 11, 9, 8, 3 }
cycle3Indexes = { 14 }

child1 = { 0, 4, 7, 6, 2, 14, 5, 1, 11, 15, 9, 3, 13, 10, 12, 8 }
child2 = { 0, 5, 9, 11, 13, 2, 1, 7, 15, 3, 14, 8, 10, 4, 12, 6 }
--------------------------------------------------------------------
ordered crossover
leftCutoff = 5
rightCutoff = 11

child1 = { 0, 0, 0, 0, 0, 14, 5, 1, 15, 3, 9, 8, 0, 0, 0, 0 }
child1 = { 0, 13, 2, 7, 11, 14, 5, 1, 15, 3, 9, 8, 10, 4, 12, 6 }

child2 = { 0, 0, 0, 0, 0, 2, 1, 7, 11, 15, 14, 3, 0, 0, 0, 0 }
child2 = { 0, 4, 5, 9, 8, 2, 1, 7, 11, 15, 14, 3, 13, 10, 12, 6 }
--------------------------------------------------------------------
partially mapped crossover
leftCutoff = 5
rightCutoff = 11

parent1 = { 0, 4, 7, 11,  2, 14, 5, 1, 15,  3,  9, 8, 13, 10, 12, 6 }
parent2 = { 0, 5, 9,  6, 13,  2, 1, 7, 11, 15, 14, 3, 10,  4, 12, 8 }

child1 = { 0, 4, 7, 11,  2, |2, 1, 7, 11, 15, 14, 3|, 13, 10, 12, 6 }
child1 = { 0, 0, 0, 0, 0, 14, 5, 1, 15, 3, 9, 8, 0, 0, 0, 0 }

child1 = { 0, 4, 7, 11, 2, |2, 1, 7, 11, 15, 14, 3|, 13, 10, 12, 6 }
child2 = { 0, 5, 9, 6, 13, |14, 5, 1, 15, 3, 9, 8|, 10, 4, 12, 8 }

child1 = { 0, 4, 7, 11, 2, | 2, 1, 7, 11, 15, 14, 3|, 13, 10, 12, 6 }
child2 = { 0, 5, 9, 6, 13, |14, 5, 1, 15,  3,  9, 8|, 10, 4, 12, 8 }

2 = 14, 7 = 1, 11 = 15 fail
2 = 14 = 9, 1 = 5 = 7, 11 = 15 = 3 fail
2 = 14 = 9, 1 = 5 = 7, 11 = 15 = 3 = 8 

child1 = { 0, 4, 5, 8, 9, | 2, 1, 7, 11, 15, 14, 3|, 13, 10, 12,  6 }
child2 = { 0, 7, 2, 6, 13, |14, 5, 1, 15,  3,  9, 8|, 10, 4, 12, 11 }
*/