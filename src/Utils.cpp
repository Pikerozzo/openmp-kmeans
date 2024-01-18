#include "Utils.h"

string Utils::formatMicroseconds(chrono::microseconds duration)
{
	auto seconds = chrono::duration_cast<chrono::seconds>(duration);
	auto millis = chrono::duration_cast<chrono::milliseconds>(duration % chrono::seconds(1));
	auto micros = duration % chrono::milliseconds(1);

	stringstream ss;
	ss << seconds.count();
	string secondsStr = ss.str();
	ss.str("");
	ss << setw(3) << setfill('0') << millis.count();
	string millisStr = ss.str();
	ss.str("");
	ss << setw(3) << micros.count();
	string microsStr = ss.str();

	return secondsStr + "'" + millisStr + "'" + microsStr;
}

vector<vector<double>> Utils::makeBlobs(int totalPoints, int totalCenters, double clusterStd, int xCoordMax, int yCoordMax, int seed) {
	vector<vector<double>> points;
	vector<vector<double>> centers;

	// generate random cluster centers
	mt19937 gen(seed);
	uniform_real_distribution<> xDis(0, xCoordMax);
	uniform_real_distribution<> yDis(0, yCoordMax);
	for (int i = 0; i < totalCenters; i++) {
		double x = xDis(gen);
		double y = yDis(gen);
		centers.push_back({ x, y });
	}

	// generate points around cluster centers
	uniform_int_distribution<> centersDis(0, totalCenters - 1);
	uniform_real_distribution<> clusterStdDis(0, clusterStd);
	uniform_real_distribution<> angleDis(0, 2 * _Pi);
	for (int i = 0; i < totalPoints; i++) {
		int center = centersDis(gen);
		double radius = clusterStdDis(gen);
		double angle = angleDis(gen);

		double x = centers[center][0] + radius * cos(angle);
		double y = centers[center][1] + radius * sin(angle);
		points.push_back({ x, y });
	}

	return points;
}