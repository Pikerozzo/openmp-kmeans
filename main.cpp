#include <iostream>
#include <vector>
#include <fstream>
#include "omp.h"
#include "random"
#include <chrono>

#include "Points.h"
#include "Centroids.h"
#include "Kmeans.h"

using namespace std;


double coords[][2] = {{-2.8917641,  -3.16344611},
                      {0.4912776,   10.85860642},
                      {-0.46055925, 10.30841339},
                      {-3.81213693, 8.94555036},
                      {-4.6893374,  0.67948962},
                      {-3.89285302, -3.64178132},
                      {5.30133342,  3.93008754},
                      {6.10772634,  4.04372491},
                      {5.95159992,  6.1136376},
                      {-3.7800966,  -0.36255457},
                      {-4.68682706, 8.70439497},
                      {-3.07880547, -2.55348423},
                      {-3.83127059, 7.19480461},
                      {0.37763607,  10.64435637},
                      {-1.76977776, 10.91406784},
                      {-3.1873165,  8.30525484},
                      {-2.5076661,  8.12935141},
                      {-2.36064331, 9.40523033},
                      {4.96071816,  3.50465973},
                      {4.75283056,  5.25005627},
                      {5.35247914,  2.68578358},
                      {-3.96050733, -4.93566333},
                      {-2.65320188, 8.45047925},
                      {-1.68966568, 9.51957051},
                      {5.09951359,  3.23290432},
                      {4.45612689,  3.77669837},
                      {-0.97483064, 9.97063732},
                      {-4.74187801, 2.04746963},
                      {-1.26458677, -3.07145708},
                      {-3.03549726, -1.93320114},
                      {5.12729852,  4.08286474},
                      {-4.54698355, 0.26448476},
                      {3.82143518,  4.26344517},
                      {4.11190513,  3.24673038},
                      {-4.61945415, 6.08551671},
                      {-0.44482549, 9.51004038},
                      {-3.23950584, -3.85500258},
                      {-5.07146363, 3.00275398},
                      {-0.25980859, 9.6349738},
                      {-3.65376967, 3.96505223},
                      {-5.32788473, 7.3341192},
                      {4.87946732,  3.3523865},
                      {-4.8853809,  1.43766893},
                      {-5.33449884, 3.08680416},
                      {-0.44866525, 8.6102598},
                      {-4.01236535, 1.61346224},
                      {-3.92600499, 3.12440958},
                      {5.31165901,  3.53916204},
                      {-3.11506769, -1.38823828},
                      {-2.40061539, 2.25917682},
                      {-4.84908383, 7.10607237},
                      {-2.60614579, -2.37746698},
                      {-1.41120509, 10.79106285},
                      {-4.88450273, 6.01446063},
                      {-2.84047395, -1.46285541},
                      {6.47907742,  3.37829861},
                      {-4.49242667, 0.99900272},
                      {-3.57756424, 9.00364308},
                      {-5.58268192, 7.7837077},
                      {-2.1219076,  -0.64269729},
                      {-5.26132918, 6.98381361},
                      {2.55690849,  3.6995352},
                      {-2.17278359, 8.2226435},
                      {-0.44268634, 8.76590164},
                      {-2.3890903,  7.44665788},
                      {-4.32639023, 7.29193737},
                      {-1.71197713, -2.90990706},
                      {-2.33073227, -2.95897589},
                      {-1.74289149, -2.20993386},
                      {-1.01340969, 11.0103262},
                      {1.12686605,  9.13504189},
                      {-5.66871869, 8.078039},
                      {-3.67074617, 0.34583902},
                      {-1.75353674, -2.74518139},
                      {0.33505136,  8.08148864},
                      {3.95906585,  3.63908696},
                      {-3.7627,     8.38703795},
                      {-3.05627476, 1.4358264},
                      {7.43830026,  5.13529012},
                      {-3.04495873, 7.91167569},
                      {5.06423596,  4.61552063},
                      {-2.03138428, -1.42128583},
                      {3.10601567,  3.46148969},
                      {-4.8117128,  3.48270121},
                      {-3.81088502, 2.07507523},
                      {6.09131395,  3.75841069},
                      {-4.99901063, -3.45493751},
                      {-3.11353005, -2.77163487},
                      {1.63379754,  9.39421799},
                      {6.80244137,  5.00747694},
                      {5.14308299,  4.08685998},
                      {-0.70213003, 9.00226114},
                      {-0.4477402,  10.99159928},
                      {-1.10734512, 10.10658214},
                      {3.91156191,  5.53634488},
                      {-2.23492536, -4.15845263},
                      {-3.65610822, -3.28606762},
                      {-5.21732586, 1.76499757},
                      {-4.07648231, 1.01269306},
                      {-3.91147076, 8.0870396},
                      {-3.59230569, 3.43730778},
                      {-4.3897226,  7.34570796},
                      {-3.64944687, -3.15008166},
                      {-4.94220563, 7.21846128},
                      {-4.23677269, 2.44912856},
                      {4.03487094,  2.13682473},
                      {-4.55923805, 2.45442736},
                      {-4.31188362, 2.67620734},
                      {-4.08949167, -2.01468597},
                      {-2.65170169, -1.93836137},
                      {6.03788056,  3.74440226},
                      {-2.52403091, 6.22185387},
                      {-4.11692768, 2.6033148},
                      {-1.3120057,  8.19508676},
                      {4.46866081,  4.13749339},
                      {-0.17201112, 11.0929654},
                      {-1.5714155,  9.72687918},
                      {4.56057867,  4.69196499},
                      {-5.30662224, 7.03727739},
                      {-2.82694697, 7.31795586},
                      {-5.29138115, 2.25303503},
                      {-1.78477701, -2.73957684},
                      {-4.59984578, 3.71828834},
                      {5.75569386,  4.43469205},
                      {-1.57999087, 10.30477946},
                      {-1.24096523, 9.63609091},
                      {-4.05004965, 6.62862182},
                      {-4.83290799, 8.15252979},
                      {-4.09516165, 6.78083174},
                      {-3.4723587,  6.79083348},
                      {-1.01844319, 8.46010686},
                      {4.40231993,  4.37746053},
                      {4.78905344,  3.80454424},
                      {-1.80058146, -2.02196522},
                      {-0.45823354, 10.81305683},
                      {-4.16658824, -1.98684495},
                      {-6.54486653, 1.80427039},
                      {-4.61209072, 1.85908931},
                      {-3.05842506, -2.84791692},
                      {-4.93948919, 3.69884974},
                      {-2.19243438, -3.06983506},
                      {-0.86212034, 8.54744835},
                      {-5.09177744, 3.94068314},
                      {4.96424145,  5.5141421},
                      {-0.03442753, 7.65777745},
                      {-4.86304752, 2.56938093},
                      {-3.21365693, -2.11623806},
                      {-0.07444635, 8.96034536},
                      {-1.40807157, 9.05801992},
                      {4.01869026,  4.05489827},
                      {-2.63095856, -3.39290842},
                      {-0.89931705, 9.21505778},
                      {-1.86468155, -1.60863491},
                      {5.83962681,  3.38884895},
                      {-2.57504318, -1.96085377},
                      {-5.39879186, 6.3136966},
                      {-4.22376286, 2.17091646},
                      {-3.76440223, 6.28103911},
                      {-5.06639861, 4.12016893},
                      {-3.20459353, 6.99228564},
                      {-3.59985619, 9.36477287},
                      {-0.02402677, 10.41202056},
                      {-3.44923614, 7.99676883},
                      {-4.77778872, 8.61042863},
                      {-1.31351453, 10.46580838},
                      {-2.74582676, -3.80090923},
                      {-1.44734157, 8.7484268},
                      {-4.32418542, 6.63849689},
                      {3.49850517,  2.93827485},
                      {0.07184768,  9.79135289},
                      {-5.16109151, 9.72483375},
                      {-4.79220617, 0.13437307},
                      {-5.80665242, 0.58960846},
                      {-7.42963006, 3.1390634},
                      {5.07943902,  4.59766197},
                      {4.36517859,  2.66521037},
                      {-4.06823596, 3.25840762},
                      {-3.46764352, 7.73559857},
                      {-4.97960541, -2.43309931},
                      {-0.67767456, -3.82008564},
                      {-4.13361589, 8.02166872},
                      {-4.19447617, 7.2251987},
                      {-5.71619525, 3.42456497},
                      {-3.19005947, -3.77042842},
                      {-5.49245025, -1.13963124},
                      {-3.40734098, 8.26814345},
                      {3.31418505,  5.06716722},
                      {-1.87286924, 9.12690692},
                      {-3.77244137, 2.80255781},
                      {5.05972574,  5.23220052},
                      {4.45527314,  4.00965436},
                      {-0.14336246, 10.41181581},
                      {-2.48039498, 2.47021794},
                      {5.58013245,  5.53342943},
                      {1.14869667,  10.07945801},
                      {-2.84380661, 3.14762156},
                      {-5.66919293, 2.65650794},
                      {-3.4922285,  7.96289882},
                      {-0.92959582, -3.85257398},
                      {-1.05696129, -2.58558606}};






/**
 *  KMEANS algorithm:
 *  1. Generate random points, chose random k as cluster centers
 *  2. Assign each point to the closest cluster center
 *  3. Update cluster centers as the mean of the points assigned to it
 *  4. Repeat 2-3 until convergence, or max iterations
 */

//const int TOTAL_POINTS = 50;
int TOTAL_POINTS = 100;
const int TOTAL_CENTROIDS = 5;

const float X_COORD_MAX = 500;
const float Y_COORD_MAX = 500;

const int MAX_ITERATIONS = 1'000;
const bool STOP_AT_MAX_ITERATIONS = true;

//const float EPSILON = 0.000001;
const float EPSILON = 0;


// 2D POINTS
//struct Point {
//    int totalPoints = 0;
//    vector<float> posX;
//    vector<float> posY;
//    vector<int> ids;
//
//    vector<int> centroidId;
//
//    void addPoint(float x, float y) {
//        posX.push_back(x);
//        posY.push_back(y);
//        ids.push_back(totalPoints++);
//        centroidId.push_back(-1);
//    }
//};

//struct Centroid {
//    int totalPoints = 0;
//    vector<float> posX;
//    vector<float> posY;
//    vector<int> ids;
//
//};


//float[] generate_random_points(int num_points) {
//    float[] points = new float[num_points * 2];
//    for (int i = 0; i < num_points; i++) {
//        points[i * 2] = (float) Math.random();
//        points[i * 2 + 1] = (float) Math.random();
//    }
//    return points;
//}

vector<vector<float>> makeBlobs(int totalPoints, int totalCenters, float clusterStd) {
    vector<vector<float>> points{};
    vector<vector<float>> centers{};


    cout << "CREATION STARTING" << endl;


    for (int i = 0; i < totalCenters; i++) {
        float x = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / X_COORD_MAX));
        float y = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / Y_COORD_MAX));
        centers.push_back({x, y});
//        centers[i][0] = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / X_COORD_MAX));
//        centers[i][1] = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / Y_COORD_MAX));

cout << "Center: " << centers[i][0] << ", " << centers[i][1] << endl;
    }
    cout << "CENTERS CREATION COMPLETE" << endl;

    for (int i = 0; i < totalPoints; i++) {
        int center = (int) ((random() / RAND_MAX) * totalCenters);


        float x = centers[center][0] + static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / clusterStd));
        float y = centers[center][1] + static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / clusterStd));
        points.push_back({x, y});
        points.push_back({centers[center][0] + (float) random() / ((float) RAND_MAX / clusterStd),
                          centers[center][1] + (float) random() / ((float) RAND_MAX / clusterStd)});
//        points[i][0] = centers[center][0] + static_cast <float>((float) random() / (float) RAND_MAX) * clusterStd;
//        points[i][1] = centers[center][1] + static_cast <float>((float) random() / (float) RAND_MAX) * clusterStd;
    }

    cout << "POINTS CREATION COMPLETE" << endl;

    return points;
}


int main() {

    Points p{};
    Centroids c{};

    srandom(time(0));
    float x, y;

//    std::random_device dev;
//    std::mt19937 rng(dev());
//    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,X_COORD_MAX);
//    for (int i = 0; i < 10; i++)
//        std::cout << "RANDOM " << i << ": " << dist6(rng) << std::endl;


    bool printResults = false;
    bool randomCentroids = false;
    bool createRandomPoints = true;

    if (!createRandomPoints)
        TOTAL_POINTS = sizeof(coords) / sizeof(coords[0]);

    auto start = std::chrono::high_resolution_clock::now();


    for (int i = 0; i < TOTAL_POINTS; i++) {
        if (createRandomPoints) {
            x = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / X_COORD_MAX));
            y = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / Y_COORD_MAX));
        } else {
            x = coords[i][0];
            y = coords[i][1];
        }
        p.addPoint(x, y, X_COORD_MAX);
    }

/*
    if (createRandomPoints) {
//        vector<vector<double>> points = makeBlobs(TOTAL_POINTS, TOTAL_CENTROIDS, X_COORD_MAX / 25.0);
        p.addPoints(makeBlobs(TOTAL_POINTS, TOTAL_CENTROIDS, 10.0));

    } else {
        for (int i = 0; i < TOTAL_POINTS; i++) {
            x = coords[i][0];
            y = coords[i][1];
            p.addPoint(x, y, X_COORD_MAX);
        }
    }
*/
    for (int i = 0; i < TOTAL_CENTROIDS; i++) {

        if (randomCentroids) {
            x = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / X_COORD_MAX));
            y = static_cast <float> (random()) / (static_cast <float> ((float) RAND_MAX / Y_COORD_MAX));
        } else {
            int r = floor(random() / (RAND_MAX / TOTAL_POINTS));
            x = p.posX[r];
            y = p.posY[r];
        }
        c.addCentroid(x, y);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
//    std::cout << "Execution Time: " << duration.count() << " microseconds" << std::endl;


    Kmeans kmeans{&p, &c, X_COORD_MAX, Y_COORD_MAX, TOTAL_POINTS, TOTAL_CENTROIDS, MAX_ITERATIONS,
                  STOP_AT_MAX_ITERATIONS, EPSILON};

    start = std::chrono::high_resolution_clock::now();

    Centroids *result = kmeans.run();

    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    std::cout << "KMEANS Execution Time: " << duration.count() << " microseconds" << std::endl;


    if (printResults)
        for (float i = -1; i < X_COORD_MAX + 1; i++) {
            for (float j = -1; j < Y_COORD_MAX + 1; j++) {

                if (i == -1 || i == X_COORD_MAX || j == -1 || j == Y_COORD_MAX) {
                    cout << "#";
                    continue;
                }
                bool point = false;
                bool centroid = false;
                int s;
                int c_id;
                for (int k = 0; k < TOTAL_POINTS; k++) {
                    if (round(p.posX[k]) == i && round(p.posY[k]) == j) {
                        point = true;
                        s = p.centroidId[k];
                    }
                }
                for (int k = 0; k < TOTAL_CENTROIDS; k++) {
                    if (round(c.posX[k]) == i && round(c.posY[k]) == j) {
                        centroid = true;
                        c_id = c.ids[k];
                    }
                }

                if (point)
//                cout << s;
//                    cout << " ";
                    cout << "⋅";
                else if (centroid)
//                cout << "X";
                    cout << c_id;
                else
                    cout << " ";
            }
            cout << endl;
        }

    cout << "Finished with " << result->totalCentroids << " centroids!" << endl;

    for (int i = 0; i < result->totalCentroids; i++) {
        cout << result->posX[i] << "," << result->posY[i] << "," << result->ids[i] << "\n";
    }

// write to file csv

    ofstream myfile;
    myfile.open("centroids.csv");
    myfile << "x,y,id\n";
    for (int i = 0; i < result->totalCentroids; i++) {
        myfile << result->posX[i] << "," << result->posY[i] << "," << result->ids[i] << "\n";
    }
    myfile.close();

    ofstream myfile2;
    myfile2.open("points.csv");
    myfile2 << "x,y,centr_id\n";
    for (int i = 0; i < p.totalPoints; i++) {
        myfile2 << p.posX[i] << "," << p.posY[i] << "," << p.centroidId[i] << "\n";
    }
    myfile2.close();


    return 0;
}
