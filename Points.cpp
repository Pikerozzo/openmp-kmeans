//
// Created by grego on 23/10/2023.
//

#include "Points.h"

void Points::addPoint(float x, float y, float defaultDistance) {
    posX.push_back(x);
    posY.push_back(y);
    ids.push_back(totalPoints++);
    centroidId.push_back(-1);
    centroidDistance.push_back(defaultDistance);
}

void Points::addPoints(const std::vector<std::vector<float>> &points, float defaultDistance) {
    for (auto point: points) {
        posX.push_back(point[0]);
        posY.push_back(point[1]);
        ids.push_back(totalPoints++);
        centroidId.push_back(-1);
        centroidDistance.push_back(defaultDistance);
    }
}
