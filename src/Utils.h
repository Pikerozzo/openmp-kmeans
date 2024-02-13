#include <string>
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <random>
using namespace std;

/**
 * @class Utils
 *
 * Utility class providing helper functions for common tasks.
 */
class Utils {
public:

	/**
	 * @brief Convert duration in microseconds to a formatted string.
	 *
	 * @param duration Duration in microseconds to be formatted.
	 * @return Formatted string representing the duration.
	 */
	static string formatMicroseconds(chrono::microseconds duration);

	/**
	 * @brief Generate random blobs of points with random centers.
	 *
	 * @param totalPoints Total number of points to generate.
	 * @param totalCenters Total number of centers (clusters).
	 * @param clusterStd Standard deviation of the clusters.
	 * @param xCoordMax Maximum value for X-coordinates.
	 * @param yCoordMax Maximum value for Y-coordinates.
	 * @param seed Seed for random number generation.
	 * @return Vector of vectors representing the generated points.
	 */
	static vector<vector<double>> makeBlobs(int totalPoints, int totalCenters, double clusterStd, double xCoordMax, double yCoordMax, int seed);
};