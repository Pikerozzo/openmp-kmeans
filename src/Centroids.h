#ifndef KMEANS_CENTROIDS_H
#define KMEANS_CENTROIDS_H

#include <vector>

/**
* @class Centroids
* 
* Class representing the centroids.
*/
class Centroids {
public:
    int totalCentroids = 0;
    std::vector<double> posX;
    std::vector<double> posY;
    std::vector<int> ids;

	// constructors and destructor
    Centroids();
    Centroids(const Centroids& c);
    ~Centroids();

    // assignment operator
    Centroids& operator =(const Centroids& c);

	// add new centroid with coordinates
    void addCentroid(double, double);
    
	// remove all centroids
    void clear();
};


#endif //KMEANS_CENTROIDS_H