#ifndef KMEANS_CENTROIDS_H
#define KMEANS_CENTROIDS_H

#include <vector>

/**
 * @class Centroids
 *
 * Class representing a collection of centroids with their coordinates.
 */
class Centroids {
public:
	int totalCentroids = 0;         /* Total number of centroids */
	std::vector<double> posX;       /* X coordinates of centroids */
	std::vector<double> posY;       /* Y coordinates of centroids */
	std::vector<int> ids;           /* IDs associated with centroids */

	/**
	 * @brief Default constructor.
	 */
	Centroids();

	/**
	 * @brief Constructor that initializes the number of centroids.
	 *
	 * @param size Number of centroids to initialize.
	 */
	Centroids(int size);

	/**
	 * @brief Copy constructor.
	 *
	 * @param c Another Centroids object to copy from.
	 */
	Centroids(const Centroids& c);

	/**
	 * @brief Destructor.
	 */
	~Centroids();

	/**
	 * @brief Assignment operator.
	 *
	 * @param c Another Centroids object to assign values from.
	 * @return Reference to the current Centroids object after assignment.
	 */
	Centroids& operator =(const Centroids& c);

	/**
	 * @brief Add a new centroid with specified coordinates.
	 *
	 * @param x X-coordinate of the centroid.
	 * @param y Y-coordinate of the centroid.
	 */
	void addCentroid(double x, double y);

	/**
	 * @brief Remove all centroids, clearing the data.
	 */
	void clear();
};

#endif //KMEANS_CENTROIDS_H