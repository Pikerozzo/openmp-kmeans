#include <limits>
#include <vector>
#include "Centroids.h"

/**
* @class Solution
* 
* Class that represents a solution to the k-means problem.
*/
struct Solution {
	double sse{ std::numeric_limits<double>::max() };
	Centroids centroids{};
	std::vector<int> clusterIds{};
};