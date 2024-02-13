#include <limits>
#include <vector>
#include "Centroids.h"

/**
 * @struct Solution
 *
 * Structure representing a solution to the k-means algorithm.
 */
struct Solution {
	double sse{ std::numeric_limits<double>::max() };  /* Sum of Squared Errors (default initialized to maximum value) */
	Centroids centroids{};                             /* Centroids associated with the solution */
	std::vector<int> clusterIds{};                     /* Cluster assignments for each data point in the solution */
};