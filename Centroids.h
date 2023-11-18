//
// Created by grego on 23/10/2023.
//

#ifndef KMEANS_CENTROIDS_H
#define KMEANS_CENTROIDS_H
#include <vector>


class Centroids {
public:
    int totalCentroids = 0;
    std::vector<float> posX;
    std::vector<float> posY;
    std::vector<int> ids;

    void addCentroid(float, float);
    void recenterCentroid();

};


#endif //KMEANS_CENTROIDS_H
