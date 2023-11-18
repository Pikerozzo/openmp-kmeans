//
// Created by grego on 24/10/2023.
//

#include <random>
#include <iostream>
#include "Kmeans.h"
#include "omp.h"

Kmeans::Kmeans(Points *p, Centroids *c, float xCoordMax, float yCoordMax, int totalPoints, int totalCentroids,
               int maxIterations, bool stopAtMaxIterations, float centroidThreshold) :
        p{p}, c{c}, xCoordMax{xCoordMax}, yCoordMax{yCoordMax}, totalPoints{totalPoints},
        totalCentroids{totalCentroids}, maxIterations{maxIterations}, stopAtMaxIterations{stopAtMaxIterations},
        centroidThreshold{centroidThreshold} {

}

Kmeans::~Kmeans() {

}

//Kmeans::init(){
//    float x, y;
//#pragma omp for
//    for (int i = 0; i < TOTAL_POINTS; i++) {
//        x = static_cast <float> (random()) / (static_cast <float> (RAND_MAX / POINTS_X_MAX));;
//        y = static_cast <float> (random()) / (static_cast <float> (RAND_MAX / POINTS_Y_MAX));;
//        p.addPoint(x, y, POINTS_X_MAX);
//    }
//
//
//    // add centroids
//#pragma omp for
//    for (int i = 0; i < TOTAL_CENTROIDS; i++) {
//        x = static_cast <float> (random()) / (static_cast <float> (RAND_MAX / POINTS_X_MAX));;
//        y = static_cast <float> (random()) / (static_cast <float> (RAND_MAX / POINTS_Y_MAX));;
//        c.addCentroid(x, y);
//    }
//}


Centroids *Kmeans::run() {
    int it = 0;
    bool centroidsMoved = false;

/*    float x, y;
    for (int i = 0; i < totalPoints; i++) {
        x = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / xCoordMax));;
        y = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / yCoordMax));;
        p.addPoint(x, y, xCoordMax);
    }
    for (int i = 0; i < totalCentroids; i++) {
        x = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / xCoordMax));;
        y = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / yCoordMax));;
        c.addCentroid(x, y);
    }
*/
    for (int i = 0; i < c->totalCentroids; i++) {
        std::cout << "Centroid " << c->ids[i] << ": " << c->posX[i] << ", " << c->posY[i] << std::endl;
    }

/*
    for (int i = 0; i < p.totalPoints; i++) {
        std::cout << "Point " << p.ids[i] << ": " << p.posX[i] << ", " << p.posY[i] << std::endl;
    }
*/
//    std::cout << "fin qui tutto bene" << std::endl;

/*
    for (int i = 0; i < totalPoints; i++) {
        std::cout << "Point " << p->ids[i] << ": " << p->posX[i] << ", " << p->posY[i] << std::endl;
    }
    std::cout << "--------------------------" << std::endl;
    for (int i = 0; i < totalCentroids; i++) {
        std::cout << "Centroid " << c->ids[i] << ": " << c->posX[i] << ", " << c->posY[i] << std::endl;
    }
*/
/*
    for (int i = 0; i < c->totalCentroids; i++) {
        auto r = (random()) / (RAND_MAX / p->totalPoints);
        std::cout << "R" << i << " : " << r << std::endl;
        int x = floor(r);
        c->posX[i] = p->posX[x];
        c->posY[i] = p->posY[x];
    }
*/
    std::cout << "Running Kmeans" << std::endl;

    float centroidThresholdSquared = centroidThreshold * centroidThreshold;
#pragma omp parallel shared(xCoordMax, yCoordMax, totalPoints, totalCentroids, maxIterations, stopAtMaxIterations, it, centroidsMoved, p, c, centroidThresholdSquared) default(none) //num_threads(2)
    {


        printf("Thread %d starting...\n", omp_get_thread_num());

//#pragma omp single
//        printf("Creating points...\n");
//        // add points
//        float x, y;
//#pragma omp for
//#pragma omp master
//        for (int i = 0; i < totalPoints; i++) {
//            x = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / xCoordMax));;
//            y = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / yCoordMax));;
//            p.addPoint(x, y, xCoordMax);
//        }
//#pragma omp single
//        printf("%d points created\n", totalPoints);
//
//
//#pragma omp single
//        printf("Creating centroids...\n");
//        // add centroids
//#pragma omp for
//#pragma omp master
//        for (int i = 0; i < totalCentroids; i++) {
//            x = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / xCoordMax));;
//            y = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / yCoordMax));;
//            c.addCentroid(x, y);
//        }
//#pragma omp single
//        printf("%d centroids created\n", totalCentroids);

#pragma omp master
        {
            printf("Starting iterations...\n");
        };

        bool notConverged = true;
        while (notConverged) {


#pragma omp master
            {
                centroidsMoved = false;
//                printf("Iteration %d\n", it);

                // if max iterations reached or centroid did not move break
                if (stopAtMaxIterations && it == maxIterations) {
                    notConverged = false;
                }
            }


            // assign points to closest centroid
#pragma omp for
            for (int i = 0; i < totalPoints; i++) {
                float p_x = p->posX[i];
                float p_y = p->posY[i];

                int closest_c_id = p->centroidId[i];
                float closest_c_dist = p->centroidDistance[i];
//                if (closest_c_dist < 0)
//                    closest_c_dist = FLT_MAX;

                float distance = 0;
                for (int j = 0; j < totalCentroids; j++) {

                    distance = sqrt(pow(c->posX[j] - p_x, 2) + pow(c->posY[j] - p_y, 2));
                    if (distance < closest_c_dist) {
                        closest_c_dist = distance;
                        closest_c_id = c->ids[j];
                    }
                }

                p->centroidId[i] = closest_c_id;
            }


            // calculate new centroid position
#pragma omp for
            for (int i = 0; i < totalCentroids; i++) {
                float tempX = 0;
                float tempY = 0;

                int currCentroidId = c->ids[i];
                float currCentroidPoints = 0;
                float newX = 0;
                float newY = 0;
                for (int j = 0; j < totalPoints; ++j) {
                    if (currCentroidId != p->centroidId[j])
                        continue;

                    tempX += p->posX[j];
                    tempY += p->posY[j];
                    currCentroidPoints++;
                }

                if (currCentroidPoints > 0) {
                    newX = tempX / currCentroidPoints;
                    newY = tempY / currCentroidPoints;


                    if (pow(newX - c->posX[i], 2) > centroidThresholdSquared ||
                        pow(newY - c->posY[i], 2) > centroidThresholdSquared) {
                        c->posX[i] = newX;
                        c->posY[i] = newY;
                        centroidsMoved = true;
//                        printf("Centroid %d new coords : (%fd, %fd)\n", c->ids[i], c->posX[i], c->posY[i]);
                    }
//                    else {
//                        printf("Centroid %d: (%fd, %fd) \n            (%fd, %fd)\n", c->ids[i], c->posX[i], c->posY[i],
//                               newX, newY);
//                    }

                }

            }

#pragma omp barrier
#pragma omp master
            {
                if (!centroidsMoved) {
                    notConverged = false;
                    printf("Centroids did not move\n");
                }

                it++;
//                printf("- iteration = %d -\n", it);
            }

//#pragma omp barrier

        }
    }

    std::cout << "----- Kmeans finished after " << it << " iterations -----" << std::endl;

    return c;
}