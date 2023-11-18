//
// Created by grego on 24/10/2023.
//

#ifndef KMEANS_KMEANS_H
#define KMEANS_KMEANS_H


#include "Points.h"
#include "Centroids.h"

class Kmeans {
public:
    Kmeans(Points* p, Centroids* c, float xCoordMax, float yCoordMax, int totalPoints, int totalCentroids, int maxIterations, bool stopAtMaxIterations, float centroidThreshold);
    ~Kmeans();
    Centroids* run();


private:
    Points *p;
    Centroids* c;
    float xCoordMax;
    float yCoordMax;
    int totalPoints;
    int totalCentroids;
    int maxIterations;
    bool stopAtMaxIterations;
    float centroidThreshold;
};


#endif //KMEANS_KMEANS_H
