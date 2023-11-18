//
// Created by grego on 23/10/2023.
//

#ifndef KMEANS_POINTS_H
#define KMEANS_POINTS_H

#include <vector>
#include <cfloat>

class Points {

public:
    int totalPoints = 0;
    std::vector<float> posX;
    std::vector<float> posY;
    std::vector<int> ids;
    std::vector<int> centroidId;

    std::vector<float> centroidDistance;


//    void addPoint(float x, float y, float defaultDistance = FLT_MAX);
//    void addPoints(const std::vector<std::vector<float>> &points, float defaultDistance = FLT_MAX);

    void addPoint(float x, float y, float defaultDistance = -1);
    void addPoints(const std::vector<std::vector<float>> &points, float defaultDistance = -1);

};

#endif //KMEANS_POINTS_H
