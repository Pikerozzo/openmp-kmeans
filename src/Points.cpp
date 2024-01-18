#include <utility>
#include <limits>

#include "Points.h"

Points::Points() = default;

Points::Points(const Points& p)
    : centroidIds(p.centroidIds),
    ids(p.ids),
    posX(p.posX),
    posY(p.posY),
    totalPoints(p.totalPoints)
{ }

Points::~Points() = default;

void Points::addPoint(double x, double y, double defaultDistance) {
    posX.push_back(x);
    posY.push_back(y);
    ids.push_back(totalPoints++);
    centroidIds.push_back(-1);
}

void Points::addPoints(const std::vector<std::vector<double>>& points, double defaultDistance) {
    for (auto point : points) {
        posX.push_back(point[0]);
        posY.push_back(point[1]);
        ids.push_back(totalPoints++);
        centroidIds.push_back(-1);
    }
}

void Points::clear() {
    posX.clear();
    posY.clear();
    ids.clear();
    centroidIds.clear();
    totalPoints = 0;
}