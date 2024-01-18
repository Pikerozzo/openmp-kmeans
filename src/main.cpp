/**
* ##	K-Means Clustering Algorithm Parallelization with OpenMP	##
* 
* This project focused on implementing a parallel version of the K-Means clustering algorithm using the OpenMP API. 
*	The project was developed as part of the Parallel Programming for Machine Learning course at the University of Florence.
* 
* Gregorio Piqué
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "omp.h"

#include "Points.h"
#include "Centroids.h"
#include "Kmeans.h"
#include "Utils.h"

using namespace std;


bool READ_POINTS_FROM_FILE = false;
string POINTS_FILE_NAME = "points.csv";

int TOTAL_POINTS = 10000;
int TOTAL_POINT_CENTERS = 5;
int TOTAL_CENTROIDS = 5;

bool MAKE_POINT_BLOBS = true;
bool MAKE_RANDOM_BLOBS = false;

const double X_COORD_MAX = 500;
const double Y_COORD_MAX = 500;

const int MAX_ITERATIONS = 1'000;
const bool STOP_AT_MAX_ITERATIONS = true;
const double EPSILON = 0;

int NUM_THREADS = 4;

const int SEED = 1492;


/**
* Application entry point.
*
* @param argc number of command-line arguments passed
* @param argv array containing up to argc passed arguments
* @return error code (0 on success, failure otherwise)
*/
int main(int argc, char** argv) {
	
	if (argc == 4) {
		TOTAL_POINTS = atoi(argv[1]);
		TOTAL_CENTROIDS = atoi(argv[2]);
		NUM_THREADS = atoi(argv[3]);
	}
	else {
		cout << "No additional parameters specified: using default configuration." << endl;
	}
	
	auto start = chrono::high_resolution_clock::now();
	shared_ptr<Points> p{ new Points };
	shared_ptr<Centroids> c{ new Centroids };

	if (READ_POINTS_FROM_FILE) {

		ifstream file(POINTS_FILE_NAME);
		if (file.fail())
		{
			cerr << "Error opening file: " << POINTS_FILE_NAME << endl << "Program terminating." << endl;
			return 1;
		}

		double x, y;
		string line;
		bool firstLine = true;
		while (getline(file, line)) {

			if (firstLine) {
				firstLine = false;
				continue;
			}

			stringstream ss(line);
			string token;
			getline(ss, token, ',');
			x = stof(token);
			getline(ss, token, ',');
			y = stof(token);

			getline(ss, token, '\n');
			auto id = stof(token);

			p->addPoint(x, y, X_COORD_MAX);
		}
		file.close();
		TOTAL_POINTS = p->totalPoints;
	}
	else {
		random_device rd;
		if (MAKE_POINT_BLOBS)
		{
			auto blobs = Utils::makeBlobs(TOTAL_POINTS, TOTAL_POINT_CENTERS, X_COORD_MAX / 5.0, X_COORD_MAX, Y_COORD_MAX, MAKE_RANDOM_BLOBS ? rd() : SEED);
			p->addPoints(blobs, X_COORD_MAX);
		}
		else {
			double x, y;
			mt19937 gen(SEED);
			uniform_real_distribution<> xDis(0, X_COORD_MAX);
			uniform_real_distribution<> yDis(0, Y_COORD_MAX);
			for (int i = 0; i < TOTAL_POINTS; i++) {
				x = xDis(gen);
				y = yDis(gen);
				p->addPoint(x, y, X_COORD_MAX);
			}
		}
	}
	
	cout << "Config settings:" << endl;
	cout << "\tTotal points: " << TOTAL_POINTS << endl;
	cout << "\tTotal centroids: " << TOTAL_CENTROIDS << endl;
	cout << "\tNumber of threads: " << NUM_THREADS << endl;

	// initialize kmeans object
	Kmeans kmeans{ p, c, X_COORD_MAX, Y_COORD_MAX, TOTAL_POINTS, TOTAL_CENTROIDS, MAX_ITERATIONS, STOP_AT_MAX_ITERATIONS, EPSILON, SEED };

	auto initDuration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	cout << "\tInitialization time: " << Utils::formatMicroseconds(initDuration) << " microseconds" << endl;
	start = chrono::high_resolution_clock::now();

	// run kmeans algorithm
	kmeans.run(NUM_THREADS);

	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "\tKmeans execution time: " << Utils::formatMicroseconds(duration) << " microseconds" << endl;

	// save solution to file
	ofstream centroidsFile;
	centroidsFile.open("..\\solution\\centroids.csv");
	if (!centroidsFile.is_open()) {
		cerr << "Error opening file: ./solution/centroids.csv" << endl;
	}
	else {
		centroidsFile << "x,y,id\n";
		for (int i = 0; i < c->totalCentroids; i++) {
			centroidsFile << c->posX[i] << "," << c->posY[i] << "," << c->ids[i] << "\n";
		}
	}
	centroidsFile.close();

	ofstream pointsFile;
	pointsFile.open("..\\solution\\points.csv");
	if (!pointsFile.is_open()) {
		cerr << "Error opening file: ./solution/points.csv" << endl;
	}
	else {
		pointsFile << "x,y,centr_id\n";
		for (int i = 0; i < p->totalPoints; i++) {
			pointsFile << p->posX[i] << "," << p->posY[i] << "," << p->centroidIds[i] << "\n";
		}
	}
	pointsFile.close();


	// save execution time and configuration settings to file
	ostringstream oss;
	oss << chrono::current_zone()->to_local(chrono::system_clock::now());
	auto formattedTime = oss.str();
	auto resultsFileName = "..\\results\\" + formattedTime + ".csv";
	replace(resultsFileName.begin(), resultsFileName.end(), ' ', '_');
	replace(resultsFileName.begin(), resultsFileName.end(), ':', '-');
	ofstream execTimeFile;
	execTimeFile.open(resultsFileName);
	if (!execTimeFile.is_open()) {
		cerr << "Error opening file: " << resultsFileName << endl;
	}
	else {
		execTimeFile << "init_exec_time,kmeans_exec_time,points,centroids,threads\n";
		execTimeFile << initDuration.count() << "," << duration.count() << "," << TOTAL_POINTS << "," << TOTAL_CENTROIDS << "," << NUM_THREADS << "\n";
	}
	execTimeFile.close();


	return 0;
}