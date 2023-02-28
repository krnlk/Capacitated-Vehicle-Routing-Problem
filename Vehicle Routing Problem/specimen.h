#pragma once
#include <list>

#include "instanceFile.h"
#include "cVRPSolution.h"

// This class represents a single specimen in a generation in genetic algorithm.
class specimen: public cVRPSolution
{
public:
	bool obligatoryMutation = false; // Used to mark a specimen that was created by directly copying it from the previous generation.

	void mutateSwap(instanceFile instanceFile); // Mutation operator - swap.
	void mutateInvert(instanceFile instanceFile); // Mutation operator - invert.
};

