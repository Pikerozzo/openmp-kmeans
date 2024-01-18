#ifndef KMEANS_POINTS_H
#define KMEANS_POINTS_H

#include <vector>
#include <cfloat>

/**
* @class Points
*
* Class representing the data points.
*/
class Points {
public:
    int totalPoints = 0;
    std::vector<double> posX;
    std::vector<double> posY;
    std::vector<int> ids;
    std::vector<int> centroidIds;
    
	// constructors and destructor
    Points();
    Points(const Points&);
    ~Points();
    
	// add new points
    void addPoint(double x, double y, double defaultDistance = -1);
    void addPoints(const std::vector<std::vector<double>>& points, double defaultDistance = -1);

    void clear();
};

#endif //KMEANS_POINTS_H