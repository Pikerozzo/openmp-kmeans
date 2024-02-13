#ifndef KMEANS_POINTS_H
#define KMEANS_POINTS_H

#include <vector>
#include <cfloat>


/**
 * @class Points
 *
 * Class representing a collection of data points.
 */
class Points {
public:
	int totalPoints = 0;            /* Total number of data points */
	std::vector<double> posX;       /* X coordinates of data points */
	std::vector<double> posY;       /* Y coordinates of data points */
	std::vector<int> ids;           /* IDs associated with data points */
	std::vector<int> centroidIds;   /* IDs of the centroids to which data points are assigned */

	/**
	 * @brief Default constructor.
	 */
	Points();

	/**
	 * @brief Copy constructor.
	 *
	 * @param p Another Points object to copy from.
	 */
	Points(const Points& p);

	/**
	 * @brief Destructor.
	 */
	~Points();

	/**
	 * @brief Add a single data point to the collection.
	 *
	 * @param x X-coordinate of the data point.
	 * @param y Y-coordinate of the data point.
	 */
	void addPoint(double x, double y);

	/**
	 * @brief Add multiple data points to the collection.
	 *
	 * @param points Vector of vectors representing multiple data points.
	 */
	void addPoints(const std::vector<std::vector<double>>& points);
};

#endif //KMEANS_POINTS_H