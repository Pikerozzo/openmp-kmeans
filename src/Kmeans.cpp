#include <random>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdatomic.h>
#include <numeric>
#include <stdexcept>
#include "omp.h"

#include "Kmeans.h"
#include "Solution.h"

using namespace std;

Kmeans::Kmeans(shared_ptr<Points> p, shared_ptr<Centroids> c, double xCoordMax, double yCoordMax, int totalPoints, int totalCentroids,
	int maxIterations, bool stopAtMaxIterations, double centroidThreshold, int seed, int kmeansIterations) :
	p{ p }, c{ c }, xCoordMax{ xCoordMax }, yCoordMax{ yCoordMax }, totalPoints{ totalPoints },
	totalCentroids{ totalCentroids }, maxIterations{ maxIterations }, stopAtMaxIterations{ stopAtMaxIterations },
	centroidThreshold{ centroidThreshold }, seed{ seed }, kmeansIterations{ kmeansIterations } {
}

Kmeans::~Kmeans() = default;

void Kmeans::run(int numThreads) {

	// initialize current centroids and points
	Centroids currCentroids{};
	Points currPoints{ *p };

	// vector of point ids
	vector<int> indices(totalPoints);
	iota(indices.begin(), indices.end(), 0);

	// vector of solutions, one for each thread
	vector<Solution> solutions(numThreads);

	// start of parallel section
#pragma omp parallel firstprivate(currPoints, currCentroids, indices) shared(solutions) default(none) num_threads(numThreads)
	{
		// create thread-custom random number generator
		int threadId = omp_get_thread_num();
		mt19937_64 gen(threadId + seed);

		// initialize thread best solution
		solutions[threadId] = Solution{};

		// parallel for loop, each thread runs a "macro k-means iteration"
#pragma omp for
		for (int run = 0; run < kmeansIterations; ++run) {

			// initialize iteration centroids with random points
			currCentroids.clear();
			shuffle(indices.begin(), indices.end(), gen);
			for (int i = 0; i < totalCentroids; i++) {
				currCentroids.addCentroid(currPoints.posX[indices[i]], currPoints.posY[indices[i]]);
			}

			// run kmeans on the current initial centroids
			runKmeans(currCentroids, currPoints);

			// get SSE of current solution
			double currentSSE = calcSSE(currCentroids, currPoints);

			// update best solution found by the current thread 
			Solution threadBestSolution = solutions[threadId];
			if (currentSSE < threadBestSolution.sse) {
				solutions[threadId].sse = currentSSE;
				solutions[threadId].centroids = currCentroids;
				solutions[threadId].clusterIds = currPoints.centroidIds;
			}
		}
	}
	// end of parallel section


	// find best solution
	auto bestSolution = min_element(solutions.begin(), solutions.end(), [](const Solution& sol1, const Solution& sol2) {
		return sol1.sse < sol2.sse;
		});

	*c = bestSolution->centroids;
	p->centroidIds = bestSolution->clusterIds;
}


void Kmeans::runKmeans(Centroids& centroids, Points& points) const {

	// variables for convergence check
	int it = 0;
	bool centroidsMoved = false;
	double centroidThresholdSquared = centroidThreshold * centroidThreshold;
	bool notConverged = true;

	// run kmeans until convergence or max iterations reached
	while (true) {
		centroidsMoved = false;

		// assign points to closest centroid
		for (int i = 0; i < points.totalPoints; i++) {
			double p_x = points.posX[i];
			double p_y = points.posY[i];

			int closest_c_id = points.centroidIds[i];
			double closest_c_dist = -1;
			if (closest_c_id != -1)
			{
				closest_c_dist = sqrt(pow(centroids.posX[closest_c_id] - p_x, 2) + pow(centroids.posY[closest_c_id] - p_y, 2));
			}


			// find closest centroid to point
			double distance = 0;
			for (int j = 0; j < centroids.totalCentroids; j++) {

				distance = sqrt(pow(centroids.posX[j] - p_x, 2) + pow(centroids.posY[j] - p_y, 2));
				if (distance < closest_c_dist || closest_c_id == -1) {
					closest_c_dist = distance;
					closest_c_id = centroids.ids[j];

					points.centroidIds[i] = closest_c_id;
				}
			}

		}

		// update centroid positions
		for (int i = 0; i < centroids.totalCentroids; i++) {
			double tempX = 0;
			double tempY = 0;

			int currCentroidId = centroids.ids[i];
			double currCentroidPoints = 0;
			double newX = 0;
			double newY = 0;
			for (int j = 0; j < points.totalPoints; ++j) {

				// skip points not assigned to current centroid
				if (currCentroidId != points.centroidIds[j])
					continue;

				tempX += points.posX[j];
				tempY += points.posY[j];
				currCentroidPoints++;
			}

			if (currCentroidPoints > 0) {
				newX = tempX / currCentroidPoints;
				newY = tempY / currCentroidPoints;

				// update coordinates if centroid moved more than threshold
				if (pow(newX - centroids.posX[i], 2) > centroidThresholdSquared || pow(newY - centroids.posY[i], 2) > centroidThresholdSquared) {
					centroids.posX[i] = newX;
					centroids.posY[i] = newY;

					centroidsMoved = true;
				}

			}

		}

		// check for convergence: no centroids moved or max iterations reached
		if (!centroidsMoved || (stopAtMaxIterations && it == maxIterations)) {
			break;
		}

		// increment iteration counter
		it++;
	}
}


double Kmeans::calcSSE(Centroids& centroids, Points& points) const {
	double sse = 0;
	for (int i = 0; i < points.totalPoints; i++) {
		sse += sqrt(pow(centroids.posX[points.centroidIds[i]] - points.posX[i], 2) +
			pow(centroids.posY[points.centroidIds[i]] - points.posY[i], 2));
	}
	return sse;
}