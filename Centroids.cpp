//
// Created by grego on 23/10/2023.
//

#include "Centroids.h"

void Centroids::addCentroid(float x, float y) {
    posX.push_back(x);
    posY.push_back(y);
    ids.push_back(totalCentroids++);
}

void Centroids::recenterCentroid(){

}

