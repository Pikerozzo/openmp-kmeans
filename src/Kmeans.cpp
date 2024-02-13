#include <random>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "omp.h"

#include "Kmeans.h"
#include "Solution.h"

using namespace std;

Kmeans::Kmeans(shared_ptr<Points> p, shared_ptr<Centroids> c, double xCoordMax, double yCoordMax, int totalPoints, int totalCentroids, int maxIterations, bool stopAtMaxIterations, double centroidThreshold, int seed, int kmeansExecutions) :
	p{ p }, c{ c }, xCoordMax{ xCoordMax }, yCoordMax{ yCoordMax }, totalPoints{ totalPoints }, totalCentroids{ totalCentroids }, maxIterations{ maxIterations }, stopAtMaxIterations{ stopAtMaxIterations },
	centroidThreshold{ centroidThreshold }, seed{ seed }, kmeansExecutions{ kmeansExecutions } {
}

Kmeans::~Kmeans() = default;


void Kmeans::runParallel(int numThreads) {

	// initialize current centroids and points
	Centroids centroids{ totalCentroids };
	Points points = *p;

	// vector of kmeans solutions
	vector<Solution> solutions(kmeansExecutions);

	// variables for convergence check
	bool notConverged = true;
	int it = 0;
	int totCentroidsMoved = 0;
	double currSse = 0.0;
	double centroidThresholdSquared = centroidThreshold * centroidThreshold;

	// vectors to store centroid positions and counts for each thread
	vector<double> centroidPositionsX(totalCentroids * numThreads);
	vector<double> centroidPositionsY(totalCentroids * numThreads);
	vector<int> centroidCounts(totalCentroids * numThreads);

	// initialize centroids with random points	
	mt19937_64 gen(seed);
	vector<int> indices{ points.ids };
	vector<int> executionInitCentroids(kmeansExecutions * totalCentroids);
	for (int run = 0; run < kmeansExecutions; ++run)
	{
		shuffle(indices.begin(), indices.end(), gen);
		for (int i = 0; i < totalCentroids; i++) {
			executionInitCentroids[run * totalCentroids + i] = indices[i];
		}
	}
	
	// start of parallel section
#pragma omp parallel shared(centroids, points, solutions, executionInitCentroids, notConverged, it, currSse, totCentroidsMoved, centroidPositionsX, centroidPositionsY, centroidCounts) default(none) num_threads(numThreads)
	{
		int threadID = omp_get_thread_num();

		double pointX, pointY;
		int closestCentroidId;
		double closestCentroidDist;
		double deltaX, deltaY;
		double distance;
		bool assignToCentroid;

		int index;
		double x, y, count;
		double newX, newY;

		int centroidId;


		// run kmeans for each set of initial centroids
		for (int run = 0; run < kmeansExecutions; ++run)
		{
			// reset convergence variables for current run
#pragma omp single nowait
			{
				totCentroidsMoved = 0;
				currSse = 0;
				notConverged = true;
				it = 0;
			}

			// set initial centroids for current run 
#pragma omp for
			for (int i = 0; i < totalCentroids; i++)
			{
				index = executionInitCentroids[run * totalCentroids + i];
				centroids.posX[i] = points.posX[index];
				centroids.posY[i] = points.posY[index];
				centroids.ids[i] = i;
			}

			// kmeans run start
			while (notConverged)
			{
				// assign points to closest centroid
#pragma omp for
				for (int i = 0; i < totalPoints; i++)
				{
					pointX = points.posX[i];
					pointY = points.posY[i];

					closestCentroidId = 0;
					closestCentroidDist = -1;

					// find closest centroid to point
					for (int j = 0; j < totalCentroids; j++)
					{
						deltaX = centroids.posX[j] - pointX;
						deltaY = centroids.posY[j] - pointY;
						distance = deltaX * deltaX + deltaY * deltaY;
						assignToCentroid = distance < closestCentroidDist || closestCentroidDist < 0;

						// update closest centroid if distance is smaller
						if (assignToCentroid) {
							closestCentroidDist = distance;
							closestCentroidId = centroids.ids[j];

							points.centroidIds[i] = closestCentroidId;
						}
					}

					// sum the position of current point associated to its centroid, for each thread
					centroidPositionsX[closestCentroidId + totalCentroids * threadID] += pointX;
					centroidPositionsY[closestCentroidId + totalCentroids * threadID] += pointY;
					centroidCounts[closestCentroidId + totalCentroids * threadID]++;
				}

				// update centroid positions, reduction on total moved centroids
#pragma omp for reduction(+:totCentroidsMoved)
				for (int i = 0; i < totalCentroids; i++)
				{
					x = 0;
					y = 0;
					count = 0;
					// sum the coordinates of points associated to a centroid, for each thread
					for (int j = 0; j < numThreads; j++)
					{
						index = j * totalCentroids + i;
						x += centroidPositionsX[index];
						y += centroidPositionsY[index];
						count += centroidCounts[index];
					}

					// update centroid position if there are points associated to it
					if (count > 0) {
						newX = x / count;
						newY = y / count;

						deltaX = newX - centroids.posX[i];
						deltaY = newY - centroids.posY[i];
						distance = deltaX * deltaX + deltaY * deltaY;
						
						if (distance > centroidThresholdSquared) {
							centroids.posX[i] = newX;
							centroids.posY[i] = newY;

							totCentroidsMoved++;
						}
					}
				}

				// find SSE of current solution
#pragma omp for reduction(+:currSse)
				for (int i = 0; i < totalPoints; i++)
				{
					centroidId = points.centroidIds[i];
					deltaX = centroids.posX[centroidId] - points.posX[i];
					deltaY = centroids.posY[centroidId] - points.posY[i];
					currSse += sqrt(deltaX * deltaX + deltaY * deltaY);
				}

				// check stop conditions: no centroids moved or max iterations reached
#pragma omp single
				{
					// if no centroids moved or max iterations reached, save solution
					if (totCentroidsMoved == 0 || (stopAtMaxIterations && it == maxIterations)) {
						solutions[run] = Solution{ currSse, centroids, points.centroidIds };
						notConverged = false;
					}

					// update iteration counter, reset solution results and helper variables
					totCentroidsMoved = 0;
					currSse = 0;
					it++;

					std::fill(centroidPositionsX.begin(), centroidPositionsX.end(), 0);
					std::fill(centroidPositionsY.begin(), centroidPositionsY.end(), 0);
					std::fill(centroidCounts.begin(), centroidCounts.end(), 0);
				}
			}
			// kmeans run end

#pragma omp barrier
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


void Kmeans::runSequential() {

	// initialize current centroids and points
	Centroids centroids{ totalCentroids };
	Points points = *p;

	// vector of kmeans solutions
	vector<Solution> solutions(kmeansExecutions);

	// vector of point ids for centroids initialization
	mt19937_64 gen(seed);
	vector<int> indices{ points.ids };
	vector<int> executionInitCentroids(kmeansExecutions * totalCentroids);
	
	// variables for convergence check
	bool notConverged = true;
	int it = 0;
	int totCentroidsMoved = 0;
	double currSse = 0;
	double centroidThresholdSquared = centroidThreshold * centroidThreshold;

	vector<double> centroidPositionsX(totalCentroids);
	vector<double> centroidPositionsY(totalCentroids);
	vector<int> centroidCounts(totalCentroids);


	double pointX, pointY;
	int closestCentroidId;
	double closestCentroidDist;
	double deltaX, deltaY;
	double distance;
	bool assignToCentroid;

	int index;
	double newX, newY;

	int centroidId;


	// run kmeans for each set of initial centroids
	for (int run = 0; run < kmeansExecutions; ++run)
	{
		totCentroidsMoved = 0;
		currSse = 0;
		notConverged = true;
		it = 0;

		// set initial centroids for current run		
		shuffle(indices.begin(), indices.end(), gen);
		for (int i = 0; i < totalCentroids; i++)
		{
			index = indices[i];
			centroids.posX[i] = points.posX[index];
			centroids.posY[i] = points.posY[index];
			centroids.ids[i] = i;
		}

		// kmeans run start
		while (notConverged)
		{
			// assign points to closest centroid
			for (int i = 0; i < totalPoints; i++)
			{
				pointX = points.posX[i];
				pointY = points.posY[i];

				closestCentroidId = points.centroidIds[i];
				closestCentroidDist = -1;

				// find closest centroid to point
				distance = 0;
				for (int j = 0; j < totalCentroids; j++)
				{
					if (j == closestCentroidId)
						continue;

					deltaX = centroids.posX[j] - pointX;
					deltaY = centroids.posY[j] - pointY;
					distance = deltaX * deltaX + deltaY * deltaY;
					assignToCentroid = distance < closestCentroidDist || closestCentroidDist < 0;

					// update closest centroid if distance is smaller
					if (assignToCentroid) {
						closestCentroidDist = distance;
						closestCentroidId = centroids.ids[j];

						points.centroidIds[i] = closestCentroidId;
					}
				}

				centroidPositionsX[closestCentroidId] += pointX;
				centroidPositionsY[closestCentroidId] += pointY;
				centroidCounts[closestCentroidId]++;
			}

			// update centroid positions
			for (int i = 0; i < totalCentroids; i++)
			{
				// update centroid position if there are points associated to it
				if (centroidCounts[i] > 0) {
					newX = centroidPositionsX[i] / centroidCounts[i];
					newY = centroidPositionsY[i] / centroidCounts[i];

					deltaX = newX - centroids.posX[i];
					deltaY = newY - centroids.posY[i];
					distance = deltaX * deltaX + deltaY * deltaY;
					
					if (distance > centroidThresholdSquared) {
						centroids.posX[i] = newX;
						centroids.posY[i] = newY;

						totCentroidsMoved++;
					}
				}
			}


			// find SSE of current solution
			for (int i = 0; i < totalPoints; i++)
			{
				centroidId = points.centroidIds[i];
				deltaX = centroids.posX[centroidId] - points.posX[i];
				deltaY = centroids.posY[centroidId] - points.posY[i];
				currSse += sqrt(deltaX * deltaX + deltaY * deltaY);
			}

			// check stop conditions: no centroids moved or max iterations reached
			if (totCentroidsMoved == 0 || (stopAtMaxIterations && it == maxIterations)) {
				solutions[run] = Solution{ currSse, centroids, points.centroidIds };
				notConverged = false;
			}

			// reset solution results and update iteration counter
			totCentroidsMoved = 0;
			currSse = 0;
			it++;

			std::fill(centroidPositionsX.begin(), centroidPositionsX.end(), 0);
			std::fill(centroidPositionsY.begin(), centroidPositionsY.end(), 0);
			std::fill(centroidCounts.begin(), centroidCounts.end(), 0);
		}
		// kmeans run end
	}

	// find best solution
	auto bestSolution = min_element(solutions.begin(), solutions.end(), [](const Solution& sol1, const Solution& sol2) {
		return sol1.sse < sol2.sse;
		});

	*c = bestSolution->centroids;
	p->centroidIds = bestSolution->clusterIds;
}