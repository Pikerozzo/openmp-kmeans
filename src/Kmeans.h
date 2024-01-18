#ifndef KMEANS_KMEANS_H
#define KMEANS_KMEANS_H

#include "Points.h"
#include "Centroids.h"

/**
* @Class Kmeans
* 
* @brief Class that implements the Kmeans algorithm
* 
* 
*/
class Kmeans {
    std::shared_ptr<Points> p;
    std::shared_ptr<Centroids> c;
    double xCoordMax;
    double yCoordMax;
    int totalPoints;
    int totalCentroids;
    int maxIterations;
    bool stopAtMaxIterations;
    double centroidThreshold;
    int seed;
    int kmeansIterations;

    void runKmeans(Centroids&, Points& points) const;
    double calcSSE(Centroids& centroids, Points& points) const;

public:
    Kmeans(std::shared_ptr<Points> p, std::shared_ptr<Centroids> c, double xCoordMax, double yCoordMax, int totalPoints, int totalCentroids, int maxIterations, bool stopAtMaxIterations, double centroidThreshold, int seed, int kmeansIterations = 30);
    ~Kmeans();
    
    void run(int numThreads = 1);
};


#endif //KMEANS_KMEANS_H