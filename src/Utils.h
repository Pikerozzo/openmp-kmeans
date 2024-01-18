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
* Class containing utility functions.
*/
class Utils
{
public:

	// turn duration in microseconds to an easily comprehensible format string
	static string formatMicroseconds(chrono::microseconds duration);
	
	// generate random blobs of points, with random centers
	static vector<vector<double>> makeBlobs(int totalPoints, int totalCenters, double clusterStd, int xCoordMax, int yCoordMax, int seed);
};