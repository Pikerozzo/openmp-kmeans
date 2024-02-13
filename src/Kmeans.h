#include "Points.h"
#include "Centroids.h"

/**
 * @class Kmeans
 *
 * Class that represents and implements the K-means clustering algorithm.
 */
class Kmeans {
private:
	std::shared_ptr<Points> p;               /* Shared pointer to the Points object */
	std::shared_ptr<Centroids> c;            /* Shared pointer to the Centroids object */
	double xCoordMax;                        /* Maximum value for X-coordinates */
	double yCoordMax;                        /* Maximum value for Y-coordinates */
	int totalPoints;                         /* Total number of data points */
	int totalCentroids;                      /* Total number of centroids */
	int maxIterations;                       /* Maximum number of iterations for K-means */
	bool stopAtMaxIterations;                /* Flag to stop K-means execution at maximum iterations */
	double centroidThreshold;                /* Convergence threshold for centroid movement */
	int seed;                                /* Seed for random number generation */
	int kmeansExecutions;                    /* Number of K-means executions */

public:
	/**
	 * @brief Constructor for the Kmeans class.
	 *
	 * @param p Shared pointer to the Points object.
	 * @param c Shared pointer to the Centroids object.
	 * @param xCoordMax Maximum value for X-coordinates.
	 * @param yCoordMax Maximum value for Y-coordinates.
	 * @param totalPoints Total number of data points.
	 * @param totalCentroids Total number of centroids.
	 * @param maxIterations Maximum number of iterations for K-means.
	 * @param stopAtMaxIterations Flag to stop K-means execution at maximum iterations.
	 * @param centroidThreshold Convergence threshold for centroid movement.
	 * @param seed Seed for random number generation.
	 * @param kmeansExecutions Number of K-means executions (default is 20).
	 */
	Kmeans(std::shared_ptr<Points> p, std::shared_ptr<Centroids> c, double xCoordMax, double yCoordMax, int totalPoints, int totalCentroids, int maxIterations, bool stopAtMaxIterations, double centroidThreshold, int seed, int kmeansExecutions = 20);

	/**
	 * @brief Destructor.
	 */
	~Kmeans();

	/**
	 * @brief Run the K-means algorithm in parallel with a specified number of threads.
	 *
	 * @param numThreads Number of threads to use in parallel execution (default is 4).
	 */
	void runParallel(int numThreads = 4);

	/**
	 * @brief Run the K-means algorithm sequentially.
	 */
	void runSequential();
};