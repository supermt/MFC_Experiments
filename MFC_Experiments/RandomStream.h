/* The following 3 declarations are for use of the random-number generator
   lcgrand and the associated functions lcgrandst and lcgrandgt for seed
   management.  This file (named lcgrand.h) should be included in any program
   using these functions by executing
       #include "lcgrand.h"
   before referencing the functions. */
#pragma once
#include <vector>

class RandomGenerator{
public:
	static float lcgrand(int stream);
	static void lcgrandst(long zset, int stream);
	static long lcgrandgt(int stream);
	static std::vector<long> zrng;
};