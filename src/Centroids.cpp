#include <iostream>

#include "Centroids.h"

Centroids::Centroids() = default;

Centroids::Centroids(const Centroids& c) {
    this->posX = c.posX;
    this->posY = c.posY;
    this->ids = c.ids;
    this->totalCentroids = c.totalCentroids;
}

Centroids::~Centroids() = default;

Centroids& Centroids::operator=(const Centroids& c) {
    if (this != &c) {
        this->posX = c.posX;
        this->posY = c.posY;
        this->ids = c.ids;
        this->totalCentroids = c.totalCentroids;
    }
    return *this;
}

void Centroids::addCentroid(double x, double y) {
    posX.push_back(x);
    posY.push_back(y);
    ids.push_back(totalCentroids++);
}

void Centroids::clear() {
    posX.clear();
    posY.clear();
    ids.clear();
    totalCentroids = 0;
}